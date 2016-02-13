# PDBX support for GDB, the GNU debugger
#
# Copyright (C) 2012 Free Software Foundation, Inc.
#
# Contributed by Intel Corporation, <markus.t.metzger@intel.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#
# Assumes stop-all mode!
# Assumes a single inferior.
#
# This is still a bit hacky in some places.

"""Support for the PDBX data race detector."""

import gdb
import struct
import re
import traceback

class pdbx_feature:
    tsm = 1
    tsm_ignore_read = 1 << 30
    tsm_ignore_atomic = 1 << 29
    tsm_ignore_task_filter = 1 << 28
    relaxed_call_guards = 1 << 27

    @staticmethod
    def visualize(feat):
        if feat == pdbx_feature.tsm: return "tsm"
        if feat == pdbx_feature.tsm_ignore_read: return "tsm: ignore reads"
        if feat == pdbx_feature.tsm_ignore_atomic: return "tsm: return atomics"
        if feat == pdbx_feature.tsm_ignore_task_filter: return "tsm: do not filter tasks"
        if feat == pdbx_feature.relaxed_call_guards: return "relaxed call guards"

class pdbx_action:
    reset_tsm = pdbx_feature.tsm
    hello = 1 << 29
    update_filters = 1 << 30
    alloc_filters = 1 << 31

    @staticmethod
    def visualize(action):
        if action == pdbx_action.reset_tsm: return "reset"
        if action == pdbx_action.hello: return "hello"
        if action == pdbx_action.update_filters: return "update filters"
        if action == pdbx_action.alloc_filters: return "alloc filters"

class pdbx_fset_mode:
    focus = 3
    suppress = 5

    @staticmethod
    def visualize(mode):
        if mode == pdbx_fset_mode.focus: return "focus"
        if mode == pdbx_fset_mode.suppress: return "suppress"

class pdbx_access_type:
    read = 0
    write = 1
    update = 2
    atomic = 3
    nonatomic_read = 4

    @staticmethod
    def visualize(acc):
        if acc == pdbx_access_type.read: return "read"
        if acc == pdbx_access_type.write: return "write"
        if acc == pdbx_access_type.update: return "update"
        if acc == pdbx_access_type.atomic: return "atomic"
        if acc == pdbx_access_type.nonatomic_read: return "read"

class pdbx_filter_type:
    code = 1
    data = 2
    task = 4
    mode = 8

    @staticmethod
    def visualize(ftype):
        if ftype == pdbx_filter_type.code: return "code"
        if ftype == pdbx_filter_type.data: return "data"
        if ftype == pdbx_filter_type.task: return "task"
        if ftype == pdbx_filter_type.mode: return "mode"

class pdbx_filter_op:
    clear = 1
    add = 2
    focus = 3
    remove = 4
    suppress = 5

    @staticmethod
    def visualize(fop):
        if fop == pdbx_filter_op.clear: return "clear"
        if fop == pdbx_filter_op.add: return "add"
        if fop == pdbx_filter_op.focus: return "focus"
        if fop == pdbx_filter_op.remove: return "remove"
        if fop == pdbx_filter_op.suppress: return "suppress"


def hack_strip_address(addr_value):
    """Addresses of symbols print as "<addr> <symbol>" and thus cannot be
    converted to int directly.

    Strip any symbol string and convert the remaining thing into an int."""

    addr_string = str(addr_value)

    end = addr_string.find(" ")
    if end != -1:
        addr_string = addr_string[0:end]

    if addr_string.startswith("0x"):
        addr_string = addr_string[2:]

    return int(addr_string, 16)

# Mappings from size to struct format specifier
#
size_to_signed_format = {
    1: "=b",
    2: "=h",
    4: "=i",
    8: "=q"
}
size_to_unsigned_format = {
    1: "=B",
    2: "=H",
    4: "=I",
    8: "=Q"
}
size_to_format = {
    True:  size_to_signed_format,
    False: size_to_unsigned_format
}

def read_memory(address, size, is_signed = False):
    """Read one integer from the selected inferior's memory."""

    inferior = gdb.selected_inferior()
    memory   = inferior.read_memory(address, size)
    format   = size_to_format[is_signed][size]

    return struct.unpack_from(format, memory)[0]

def write_memory(address, size, value, is_signed = False):
    """Write one integer into the selected inferior's memory."""

    inferior = gdb.selected_inferior()
    memory   = inferior.read_memory(address, size)
    format   = size_to_format[is_signed][size]

    struct.pack_into(format, memory, 0, value)
    inferior.write_memory(address, memory, size)

def ptid_to_threadid(ptid):
    """Extract the thread id from a gdb ptid tuple."""

    (pid, lwpid, tid) = ptid
    if tid == 0: tid = lwpid
    if tid == 0: tid = None

    return tid

def lookup_thread(threadid):
    """Find the gdb thread object in the selected inferior by the OS
    thread id."""

    inferior = gdb.selected_inferior()
    threads  = inferior.threads()
    for thread in threads:
        ptid = thread.ptid
        tid  = ptid_to_threadid(ptid)
        if tid == threadid:
            return thread

    return None

def read_thread(address, version):
    """Read a pdbx thread object from the given address in the selected
    inferior."""

    threadid = None
    if version < 3:
        threadid = read_memory(address + 8, 8)
    if version < 5:
        kind = read_memory(address, 4)
        if kind in (1, 3):
            threadid = read_memory(address + 8, 8)

    return threadid

re_addr_range = re.compile("^ *0x([0-9a-f]+) *- *0x([0-9a-f]+) *$")
re_addr = re.compile("^ *0x([0-9a-f]+) *$")

def parse_address_range(arg):
    """Parse a range of hex addresses separated by -

    Returns a tuple (base, size)."""

    if not arg:
        raise gdb.GdbError("ERROR: missing range argument.")

    match = re_addr_range.match(arg)
    if match:
        begin = int(match.group(1), 16)
        end   = int(match.group(2), 16)
        if end <= begin:
            raise gdb.GdbError("ERROR: range empty.")
        return (begin, end - begin)

    match = re_addr.match(arg)
    if match:
        begin = int(match.group(1), 16)
        return (begin, 1)

    raise gdb.GdbError("ERROR: bad range argument: %s." % arg)

re_index_range = re.compile("^ *([0-9]+) *- *([0-9]+) *$")
re_index = re.compile("^ *([0-9]+) *$")

def parse_index_range(arg):
    """Parse a range of integer indices.

    Returns a tuple (begin, end) or None if the argument is not an index range."""

    if not arg:
        return None

    match = re_index_range.match(arg)
    if match:
        begin = int(match.group(1))
        end   = int(match.group(2))
        if end < begin:
            raise gdb.GdbError("ERROR: bad range: %d < %d." % (end, begin))
        return (begin, end)

    match = re_index.match(arg)
    if match:
        begin = int(match.group(1))
        return (begin, begin)

    return None

def require_index_range(arg):
    """Parse a range of integer indices or raise a gdb error."""

    rng = parse_index_range(arg)

    if not rng:
        raise gdb.GdbError("ERROR: bad range argument: %s." % arg)

    return rng

def parse_line_range(arg):
    """Parse a string followed by an index range.

    Both string and index range are optional. If both exist, they are separated
    by ':', e.g.

    8
    8-19
    file
    file:8
    file:8-19

    Returns a tuple (file, begin, end)."""

    if not arg:
        return (None, None, None)

    parts = arg.split(":")
    if len(parts) == 1:
        rng = parse_index_range(parts[0])
        if not rng:
            return (parts[0], None, None)
        else:
            return (None, rng[0], rng[1])

    if len(parts) > 2:
        raise gdb.GdbError("ERROR: trailing junk: %s." % join(parts[2:]))

    (begin, end) = require_index_range(parts[1])

    return (parts[0], begin, end)

def parse_filter_range(arg):
    """Parse a filter range argument.

    Both filter set name and index range are optional.
    If both exist, they are separated by ':', e.g.
    If no filter set name is given, the current filter set is used.

    8
    8-19
    fset
    fset:8
    fset:8-19

    Returns a tuple (fset, begin, end)."""

    global pdbx

    (name, begin, end) = parse_line_range(arg)
    fset = pdbx.get_fset(name)

    if begin is None:
        begin = 0
    if end is None:
        end = len(fset.filters) - 1

    return (fset, begin, end)

def parse_modifiers(arg, allowed):
    """Strip a string of modifiers from an argument string.

    Returns (modifiers, remaining argument)."""

    if not arg:
        return ("", None)

    modifiers = ""

    while arg and arg[0] == "/":
        end = arg.find(" ")

        if end == -1:
            modifiers += arg[1:]
            arg = ""
        else:
            modifiers += arg[1:end]
            arg = arg[end+1:]

    for m in modifiers:
        if not m in allowed:
            raise gdb.GdbError("ERROR: bad modifier: %s." % m)

    return (modifiers, arg)

def parse_junk(arg):
    """Raise a trailing junk error if the argument is not empty."""

    if arg:
        raise gdb.GdbError("ERROR: trailing junk: %s." % arg)

# A map from pdbx version to the size of an access description in bytes.
#
pdbx_access_size = {
    1: 40,
    2: 40,
    3: 40,
    4: 88
}

class PdbxAccess(object):
    """An internal class describing a thread's memory access.

    The constructor reads the access description from a pdbx race report
    at the given address in the selected inferior."""

    def __init__(self, address, version):
        # The raw data.
        #
        self.version  = version
        self.space    = pdbx_access_size[version]
        self.kind     = read_memory(address, 4)
        self.size     = read_memory(address + 4, 4)
        self.base     = read_memory(address + 8, 8)
        self.threadid = read_thread(address + 16, version)
        self.location = read_memory(address + 32, 8)

        # Source information.
        #
        self.variable = "0x%x" % self.base
        self.source = "0x%x" % self.location

        # Try to find the gdb thread object.
        self.thread = lookup_thread(self.threadid)

        # Try to map the raw addresses back to symbols.
        #
        info = gdb.execute("info symbol %s" % self.base, False, True)
        end  = info.find(" in section")
        if end != -1:
            self.variable = info[:end]

        # In case we get more than one symbol, we randomly take one - the last,
        # in this case.
        #
        (unparsed, symln) = gdb.decode_line("*" + str(self.location))
        if not unparsed:
            for sl in symln:
                self.line = sl.line
                if sl.symtab is not None:
                    self.filename = sl.symtab.filename

        if self.line and self.filename:
            self.source = str(self.filename) + ":" + str(self.line)

    def __contained(self, begin, end):
        """Check whether this access is contained in [begin; end[."""

        return  (begin <= self.base) and ((self.base + self.size) <= end)

    def __overlaps(self, begin, end):
        """Check whether this access overlaps with [begin; end["""

        if end <= self.base:
            return False

        if (self.base + self.size) <= begin:
            return False

        return True

    def filtered(self, flt, mode):
        """Check whether this access would have been hit by a given filter."""

        if not flt.enabled:
            return False

        for (begin, size) in flt.code:
            if begin <= self.location < (begin + size):
                return True

        if mode == pdbx_fset_mode.suppress:
            for (begin, size) in flt.data:
                if self.__contained(begin, begin + size):
                    return True

        if mode == pdbx_fset_mode.focus:
            for (begin, size) in flt.data:
                if self.__overlaps(begin, begin + size):
                    return True

        return False

    def __str__(self):
        if self.thread is None:
            thread = "<unknown>"
        elif self.thread.is_valid():
            thread = self.thread.num
        else:
            thread = "<exited>"

        kind = pdbx_access_type.visualize(self.kind)
        return "%s: %s %s, %d bytes from %s" % (thread,
                                                kind,
                                                self.variable,
                                                self.size,
                                                self.source)

class PdbxRaceReport(object):
    """An internal class describing a pdbx data race report.

    The constructor reads the report from the given address in the selected
    inferior's memory."""

    def __init__(self, address, version):
        self.version = version
        self.entry   = read_memory(address, 8)

        access   = PdbxAccess(address + 8, version)
        address += access.space

        self.accesses = [ access ]
        self.size     = read_memory(address + 8, 8)

        address = read_memory(address + 16, 8)
        for i in range(0, self.size):
            access   = PdbxAccess(address, version)
            address += access.space
            self.accesses.append(access)

            # Sorting the accesses makes it easier for testing.
            #
            self.accesses.sort(lambda l,r: cmp(l.location, r.location))

    def filter(self, fset):
        """Remove accesses that are filtered out by the given filter set."""

        remaining = []
        for access in self.accesses:
            hit = False

            for flt in fset.filters:
                hit = hit or access.filtered(flt, fset.mode)

            if fset.mode == pdbx_fset_mode.focus:
                hit = not hit

            if not hit:
                remaining.append(access)

        self.accesses = remaining

    def racy(self):
        """Check whether the report contains a data race."""

        writers = 0
        total   = 0
        for access in self.accesses:
            total += 1
            if access.kind == 1: writers += 1
            if access.kind == 2: writers += 1
            if access.kind == 3: writers += 1

        return writers > 0 and total > 1

    def stat(self):
        """Return statistic information about the reported data race.

        Returns ([threads], #reads, #writes, #updates)."""

        reads   = 0
        writes  = 0
        updates = 0
        threads = []
        for access in self.accesses:
            threads.append(access.thread)
            if access.kind == 0: reads   += 1
            if access.kind == 1: writes  += 1
            if access.kind == 2: updates += 1
            if access.kind == 3: updates += 1
            if access.kind == 4: reads   += 1

        return (threads, reads, writes, updates)

    def __str__(self):
        report = ""
        for access in self.accesses:
            report += "%s\n" % str(access)
        return report


class PdbxHistory(object):
    """An internal class providing a history of race detection reports."""

    def __init__(self):
        super(PdbxHistory, self).__init__()

        self.reports = []

    def get(self, index):
        try:
            return self.reports[index]
        except IndexError:
            raise gdb.GdbError("ERROR: no such element: %d." % index)

    def add(self, report):
        self.reports.append(report)

    def remove(self, index):
        try:
            self.reports.pop(index)
        except IndexError:
            raise gdb.GdbError("ERROR: no such element: %d." % index)

    def clear(self):
        self.reports = []


    def stat(self):
        """Collect some statistics on the history.

        Returns (#reports, #threads, #reads, #writes, #updates)."""

        reads   = 0
        writes  = 0
        updates = 0
        threads = []
        entries = 0
        for report in self.reports:
            (thr, rds, wrs, ups) = report.stat()

            threads += thr
            reads   += rds
            writes  += wrs
            updates += ups
            entries += 1

        return (entries, len(set(threads)), reads, writes, updates)

    def __str__(self):
        return "%u races in %u threads; %u reads, %u writes, %u updates" % self.stat()


class PdbxReporter(gdb.Breakpoint):
    """An internal breakpoint class used for reporting data races in the
    correct context."""

    def __init__(self, address, thread, report, pdbx):
        super(PdbxReporter, self).__init__("*" + str(address), internal=True)
        self.thread  = int(thread.num)
        self.report  = report
        self.pdbx    = pdbx

    def stop(self):
        # FIXME: delete temporary breakpoint
        #
        # The reporting breakpoint is supposed to be temporary. When I try to
        # self.delete() it, though, gdb dies.
        #
        # Working around this by just disabling the breakpoint. This will leak
        # breakpoints! We do not expect too many detections, though, so this
        # should normally not be a huge problem.
        #
        self.enabled = False

        # Discard any in-flight reports when pdbx is not enabled.
        #
        if not self.pdbx.enabled:
            return False

        # Filters are applied in libpdbx to boost detection performance.
        #
        # If the filter set is changed, though, libpdbx may report races based
        # on the old filter set. We filter once again to avoid what would appear
        # to the user as false positives.
        #
        self.report.filter(self.pdbx.filterset)

        if not self.report.racy():
            return False

        gdb.write("data race detected\n%s" % self.report)
        self.pdbx.history.add(self.report)
        return True


class PdbxCommunicator(gdb.Breakpoint):
    """An internal breakpoint on the PDBX magic communication symbol forwarding
    PDBX events to the respective PDBX class."""

    def __init__(self, pdbx):
        super(PdbxCommunicator, self).__init__("__pdbx_switch_to_debugger",
                                               internal=True)
        self.pdbx = pdbx

    @staticmethod
    def read_event():
        try:
            # Since we're handling library load events, we cannot rely on PDBX
            # to be fully initialized, yet.
            #
            # We need to do our own lookup of the event reason magic symbol.
            #
            symbol = PdbxSymbols.lookup("__pdbx_event_reason")
            event  = read_memory(symbol, 4, is_signed = True)
        except:
            event = None

        return event

    @staticmethod
    def handler_prefix(event):
        code = int(event)
        if code < -30000: return "user"
        elif code < -10000: return "warn"
        elif code < 0: return "error"
        else: return "event"

    def find_handler(self, kind, event):
        # Errors are negative, their handlers use the positive number.
        #
        if event < 0: event = - event

        handler = "%s_%u" % (kind, event)
        return getattr(self.pdbx, handler, None)

    def stop(self):
        event   = self.read_event()
        if event is None:
            raise gdb.GdbError("Warning: pdbx not working properly. May miss races.")

        prefix  = self.handler_prefix(event)
        handler = self.find_handler(prefix, event)
        try:
            if handler is not None:
                handler()
            else:
                getattr(self.pdbx, prefix)(event)
        except Exception as error:
            print "internal error: %s" % error
            traceback.print_exc()
            raise gdb.GdbError("Warning: pdbx got out of sync. May miss races.")
        return False


class PdbxFilter(object):
    """An internal class describing a PDBX filter."""

    def __init__(self):
        self.code = []
        self.data = []
        self.tasks = False
        self.reads = False
        self.enabled = True
        self.evaluated = False

    def enable(self):
        self.enabled = True

    def disable(self):
        self.enabled = False

    def status(self):
        if not self.enabled:
            return " (disabled)"
        if not self.evaluated:
            return " (pending)"
        return ""


class PdbxCodeFilter(PdbxFilter):
    """An internal class describing a PDBX filter on a range of code addresses."""

    def __init__(self, base, size):
        super(PdbxCodeFilter, self).__init__()
        self.base = base
        self.size = size

    def evaluate(self):
        self.code = [ (self.base, self.size) ]
        self.data = []
        self.evaluated = True

    def __str__(self):
        return "code: 0x%x, %u bytes" % (self.base, self.size)


class PdbxDataFilter(PdbxFilter):
    """An internal class describing a PDBX filter on a range of data addresses."""

    def __init__(self, base, size):
        super(PdbxDataFilter, self).__init__()
        self.base = base
        self.size = size

    def evaluate(self):
        self.code = []
        self.data = [ (self.base, self.size) ]
        self.evaluated = True

    def __str__(self):
        return "data: 0x%x, %u bytes" % (self.base, self.size)


class PdbxVarFilter(PdbxFilter):
    """An internal class describing a PDBX filter on a source variable expression."""

    def __init__(self, expr):
        super(PdbxVarFilter, self).__init__()
        self.expr = expr

    def evaluate(self):
        self.evaluated = False
        self.code = []
        self.data = []

        addr = gdb.parse_and_eval("&(%s)" % self.expr)
        size = gdb.parse_and_eval("sizeof(%s)" % self.expr)
        base = hack_strip_address(addr)

        self.data.append((base, size))
        self.evaluated = True

    def __str__(self):
        return "var: %s" % self.expr


re_info_at_addr = re.compile("is at address 0x([0-9a-f]+)")
re_info_addr_range = re.compile("starts at address 0x([0-9a-f]+).*ends at 0x([0-9a-f]+)")

class PdbxLineFilter(PdbxFilter):
    """An internal class describing a PDBX filter on a source line."""

    def __init__(self, expr):
        super(PdbxLineFilter, self).__init__()
        self.expr = expr

    def evaluate(self):
        self.evaluated = False
        self.code = []
        self.data = []

        info = gdb.execute("info line %s" % self.expr, False, True)
        if "contains no code" in info:
            raise gdb.GdbError("warning: no code at line %s. Filter not active." % self.expr)

        begin = 0
        end   = 0
        match = re_info_at_addr.search(info)
        if match:
            begin = int(match.group(1), 16)
            self.code.append((begin, 1))

        match = re_info_addr_range.search(info)
        if match:
            begin = int(match.group(1), 16)
            end   = int(match.group(2), 16)

            if end < begin:
                (begin, end) = (end, begin)

            self.code.append((begin, end - begin))

        if len(self.code) == 0:
            raise gdb.GdbError("warning: cannot find code for line %s. Filter not active." % self.expr)

        self.evaluated = True

    def __str__(self):
        return "line: %s" % self.expr


class PdbxReadsFilter(PdbxFilter):
    """An internal class describing a global PDBX filter on reads."""

    def __init__(self):
        super(PdbxReadsFilter, self).__init__()

    def evaluate(self):
        self.reads = True
        self.evaluated = True

    def __str__(self):
        return "global: reads"


class PdbxFilterCmd(object):
    """An internal class describing a PDBX filter command.

    Such commands are used to communicate filters to the pdbx run-time."""

    def __init__(self, cmd = 0, param1 = 0, param2 = 0):
        self.cmd    = cmd
        self.flags  = 0
        self.param1 = param1
        self.param2 = param2

    @staticmethod
    def size():
        return 24

    @staticmethod
    def encode(ftype, fop):
        return (((ftype & 0xff) << 16) | ((fop & 0xff) << 24) | pdbx_feature.tsm)

    @staticmethod
    def decode(cmd):
        return ((cmd >> 16) & 0xff, (cmd >> 24) & 0xff)

    def write(self, address):
        """Write the filter command to the given address in the selected
        inferior's memory.
        Returns the number of bytes written."""

        write_memory(address,      4, self.cmd)
        write_memory(address +  4, 4, self.flags)
        write_memory(address +  8, 8, self.param1)
        write_memory(address + 16, 8, self.param2)

        return self.size()

    def __str__(self):
        (ftype, fop) = self.decode(self.cmd)
        return "%s %s filter (%s, %s)" % (pdbx_filter_op.visualize(fop),
                                          pdbx_filter_type.visualize(ftype),
                                          self.param1, self.param2)


class PdbxFilterSet(object):
    """An internal class representing a collection of filters."""

    def __init__(self, name, mode = pdbx_fset_mode.suppress):
        self.name    = name
        self.mode    = mode
        self.tasks   = False
        self.reads   = False
        self.filters = []

    def get(self, index):
        try:
            return self.filters[index]
        except IndexError:
            raise gdb.GdbError("ERROR: no such filter: %d." % index)

    def add(self, flt):
        self.filters.append(flt)

    def rename(self, name):
        self.name = name

    def remove(self, index):
        try:
            self.filters.pop(index)
        except IndexError:
            raise gdb.GdbError("ERROR: no such element: %d." % index)

    def clear(self):
        self.filters = []

    def reset(self):
        for flt in self.filters:
            flt.evaluated = False

    @staticmethod
    def extends(lhs, rhs):
        """Check whether the two parameter ranges may extend one another, i.e.
        if they are adjacent or if they overlap."""

        (lbegin, lend) = lhs
        (rbegin, rend) = rhs

        if lbegin == rbegin: return True
        if lbegin <  rbegin: return rbegin <= (lend + 1)
        if lbegin >  rbegin: return lbegin <= (rend + 1)

    def extend(self, ranges, rng):
        """Extend a list of ranges by a single range such that existing
        overlapping or adjacent ranges are extended.
        A new range is added only if no original range can be extended."""

        extended = []
        for old in ranges:
            if self.extends(old, rng):
                (lbegin, lend) = old
                (rbegin, rend) = rng

                rng = (min(lbegin, rbegin), max(lend, rend))
            else:
                extended.append(old)

        extended.append(rng)
        return extended

    @staticmethod
    def compile(ranges, ftype):
        """Compile a given list of ranges into a sequence of pdbx filter
        commands of the given type."""

        clear = PdbxFilterCmd.encode(ftype, pdbx_filter_op.clear)
        cmds  = [ PdbxFilterCmd(clear) ]

        for rng in ranges:
            (begin, end) = rng
            add = PdbxFilterCmd.encode(ftype, pdbx_filter_op.add)
            cmds.append(PdbxFilterCmd(add, begin, end))

        return cmds

    @staticmethod
    def evaluate(flt):
        try:
            flt.evaluate()
        except:
            pass

    def any_pending(self):
        """Return true if there is at least one filter enabled but not evaluated."""

        for flt in self.filters:
            if flt.enabled and not flt.evaluated:
                return True
        return False

    def commands(self):
        """Return a list of PDBX filter commands to install this filter set.

        As a side effect, sets the filter set's tasks and reads fields.
        This is somewhat ugly, but it allows us to present those global settings
        as normal filters to the user."""

        code  = []
        data  = []
        tasks = False
        reads = False
        for flt in self.filters:
            if flt.enabled:
                if not flt.evaluated:
                    self.evaluate(flt)
                for rng in flt.code:
                    code = self.extend(code, rng)
                for rng in flt.data:
                    data = self.extend(data, rng)
                if flt.tasks: tasks = True
                if flt.reads: reads = True

        mode = PdbxFilterCmd.encode(pdbx_filter_type.mode, self.mode)
        commands = [ PdbxFilterCmd(mode) ]
        commands += self.compile(code, pdbx_filter_type.code)
        commands += self.compile(data, pdbx_filter_type.data)

        self.tasks = tasks
        self.reads = reads

        return commands

    def __str__(self):
        fop = pdbx_filter_op.visualize(self.mode)
        return "%s filter set %s, %u filters" % (fop,
                                                 self.name,
                                                 len(self.filters))

class PdbxSymbols(object):
    """Internal class holding magic symbols of the PDBX run-time."""

    def __init__(self):
        inferior = gdb.selected_inferior()
        if inferior is None or not inferior.is_valid():
            raise gdb.GdbError("ERROR: no inferior.")

        try:
            self.config    = self.lookup("__pdbx_config")
            self.event     = self.lookup("__pdbx_event_reason")
            self.detection = self.lookup("__pdbx_current_detection")
            self.filters   = self.lookup("__pdbx_filter_space")
            self.guards    = self.lookup("__PDBX_isGuardedCall")
        except:
            raise gdb.GdbError("ERROR: pdbx not available.")

    @staticmethod
    def lookup(name):
        # We can't use gdb.lookup_global_symbol() since we typically do not
        # have debug information for the symbols we're looking for.
        #
        return gdb.parse_and_eval("(char*) &" + name)


class Pdbx(object):
    """An internal class holding the pdbx configuration and providing functions
    for configuring the pdbx race detector run-time."""

    def __init__(self):
        self.communication = PdbxCommunicator(self)
        self.history = PdbxHistory()
        self.enabled = False
        self.symbols = None
        self.version = None
        self.reporters  = []
        self.filterset  = PdbxFilterSet("default")
        self.filtersets = [ self.filterset ]
        self.filtercmds = []
        self.filtereval = "load"

        gdb.events.exited.connect(self.on_inferior_exit)

    def enable(self):
        """Enable data race detection for the selected inferior."""

        self.enabled = True
        self.sync_enabled()

        # Handle the attach case where we do not see the pdbx load.
        #
        if self.symbols is None:
            try:
                self.event_1()
            except gdb.GdbError:
                pass

    def disable(self):
        """Disable data race detection for the selected inferior."""

        self.enabled = False
        self.sync_enabled()

    def reset(self):
        if self.symbols is not None:
            self.action(pdbx_action.reset_tsm)
        #
        # Reset is pointless if PDBX is not loaded.

    def find_fset(self, name):
        for fset in self.filtersets:
            if fset.name == name:
                return fset
        return None

    def get_fset(self, name):
        if not name:
            return self.filterset

        fset = self.find_fset(name)
        if fset is None:
            raise gdb.GdbError("ERROR: no such filter set: %s." % name)

        return fset

    def sync_enabled(self):
        if self.symbols is None:
            return

        bits  = pdbx_feature.tsm
        bits |= pdbx_feature.tsm_ignore_atomic
        bits |= pdbx_feature.relaxed_call_guards
        #
        # For OpenMP, we might miss the first few memory accesses of the second
        # thread in a parallel region. We would need the parent thread to notify
        # us about the number of threads to expect as we do with pthread.

        if self.enabled:
            self.enable_feature(bits)
        else:
            self.disable_feature(bits)

    def sync_filters(self):
        self.filtercmds = self.filterset.commands()

        if self.filterset.any_pending():
            gdb.write("Warning: not all filters could be evaluated.\n")

        # Initiate the communication
        #
        if self.symbols is not None:
            self.action(pdbx_action.hello)
            self.action(pdbx_action.update_filters)
            #
            # This works around the problem that the actions become all clear
            # between hello and the actual filter update, thus allowing other
            # threads to enter pdbx while we're reconfiguring it.
            #
            # This is not a problem in itself, but it delays the reconfiguration
            # with respect to those other threads.

            task_feat = pdbx_feature.tsm_ignore_task_filter
            if self.filterset.tasks: self.disable_feature(task_feat)
            else: self.enable_feature(task_feat)

            read_feat = pdbx_feature.tsm_ignore_read
            if self.filterset.reads: self.enable_feature(read_feat)
            else: self.disable_feature(read_feat)
        #
        # The actual filter update is done by the configuration update event
        # handler below.

    def action(self, bits):
        if self.symbols is None:
            raise gdb.GdbError("ERROR: no PDBX run-time.")

        value = read_memory(self.symbols.config + 4, 4)
        value |= bits
        write_memory(self.symbols.config + 4, 4, value)

    def enable_feature(self, bits):
        if self.symbols is None:
            raise gdb.GdbError("ERROR: no PDBX run-time.")

        value = read_memory(self.symbols.config + 8, 4)
        value |= bits
        write_memory(self.symbols.config + 8, 4, value)

    def disable_feature(self, bits):
        if self.symbols is None:
            raise gdb.GdbError("ERROR: no PDBX run-time.")

        value = read_memory(self.symbols.config + 8, 4)
        value &= ~bits
        write_memory(self.symbols.config + 8, 4, value)

    def filterconf(self):
        size = read_memory(self.symbols.config + 12, 4)
        area = read_memory(self.symbols.filters, 8)

        return (area, size)

    def on_inferior_exit(self, event):
        for bp in self.reporters:
            bp.delete()

        self.reporters = []

    def event(self, code):
        gdb.write("PDBX event: %s unhandled.\n" % code)

    def event_1(self):
        """Handle pdbx run-time loaded events."""

        self.symbols = PdbxSymbols()
        self.version = int(read_memory(self.symbols.config, 4))

        if self.version > 4:
            raise gdb.GdbError("ERROR: unsupported pdbx version: %d." % self.version)

        if self.filtereval == "load":
            self.filterset.reset()

        self.sync_enabled()
        self.sync_filters()

    def event_2(self):
        """Handle pdbx run-time unloaded events."""

        self.symbols = None
        self.version = None

    def event_3(self):
        """Handle configuration updated events."""

        ncmds = len(self.filtercmds)

        # Let's see if we have any filter commands to write.
        #
        if ncmds > 0:
            (area, size) = self.filterconf()

            # Write the filter command size. We will need it to indicate either:
            # - how much space we need, or
            # - how much space we used
            #
            write_memory(self.symbols.config + 12, 4, ncmds)

            if  ncmds <= size:
                for cmd in self.filtercmds:
                    area += cmd.write(area)

                # We wrote all our filter commands.
                #
                self.filtercmds = []
                self.action(pdbx_action.update_filters)
            else:
                self.action(pdbx_action.alloc_filters)

    def event_4(self):
        """Handle communication initiated events."""

        self.event_3()

    def event_256(self):
        """Handle data race detected events."""

        # Discard any queued up reports when pdbx is not enabled.
        #
        if not self.enabled:
            return

        report = PdbxRaceReport(self.symbols.detection, self.version)
        thread = gdb.selected_thread()
        entry  = report.entry

        reporter = PdbxReporter(entry, thread, report, self)
        self.reporters.append(reporter)

    def user(self, code):
        gdb.write("PDBX warning: %s unhandled.\n" % code)

    def warn(self, code):
        gdb.write("PDBX internal warning: %s unhandled.\n" % code)

    def warn_30000(self):
        gdb.write("PDBX internal warning: invalid parameter.\n")

    def warn_29999(self):
        gdb.write("PDBX internal warning: unknown feature.\n")

    def warn_29998(self):
        gdb.write("PDBX internal warning: bad filter operation.\n")

    def warn_29997(self):
        gdb.write("PDBX internal warning: cannot apply filter.\n")

    def warn_29996(self):
        gdb.write("PDBX internal warning: task handle overflow.\n")

    def warn_29995(self):
        gdb.write("PDBX internal warning: bad task nesting.\n")

    def warn_29994(self):
        gdb.write("PDBX internal warning: memory access too big.\n")

    def warn_29993(self):
        gdb.write("PDBX internal warning: synchronization clock overflow.\n")

    def warn_29992(self):
        gdb.write("PDBX internal warning: no such task.\n")

    def warn_29991(self):
        gdb.write("PDBX internal warning: bad task type.\n")

    def warn_29990(self):
        gdb.write("PDBX internal warning: bad synchronization type.\n")

    def warn_29989(self):
        gdb.write("PDBX internal warning: bad memory order.\n")

    def warn_29988(self):
        gdb.write("PDBX internal warning: syncid overflow.\n")

    def warn_29987(self):
        gdb.write("PDBX internal warning: syncid mismatch.\n")

    def warn_29986(self):
        gdb.write("PDBX internal warning: bad openmp taskid.\n")

    def error(self, code):
        gdb.write("PDBX error: %s unhandled.\n" % code)


# The pdbx instance.
#
pdbx = Pdbx()


class PdbxPrefixCommand(gdb.Command):
    """PDBX data race detection."""

    def __init__(self):
        super(PdbxPrefixCommand,
              self).__init__(name="pdbx",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = True)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        if pdbx.enabled:
            status = "enabled"
        else:
            status = "disabled"

        if pdbx.version is None:
            gdb.write("%s, run-time not loaded\n" % status)
        else:
            gdb.write("%s, run-time version %d\n" % (status, pdbx.version))


class PdbxCheckCommand(gdb.Command):
    """Diagnose potential setup problems.

    This searches for common setup problems that prevent PDBX from functioning
    properly."""

    def __init__(self):
        super(PdbxCheckCommand,
              self).__init__(name="pdbx check",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    @staticmethod
    def checkenv(var, val):
        env = gdb.execute("show env %s" % var, False, True)
        return ("%s = %s" % (var, val)) in env


    def invoke(self, arg, from_tty):
        parse_junk(arg)

        gdb.write("checking inferior...")
        thread = gdb.selected_thread()
        if not thread or not thread.is_valid():
            gdb.write("failed.\n\n")
            gdb.write("Please repeat the command with an inferior.\n")
            return

        gdb.write("passed.\n")
        gdb.write("checking libpdbx...")
        libpdbx = gdb.execute("info sharedlibrary libpdbx.so", False, True)
        if not "libpdbx.so" in libpdbx:
            gdb.write("failed.\n\n")
            gdb.write("Please compile with \"-debug parallel\".\n")
            return

        try:
            PdbxSymbols()
        except:
            gdb.write("failed.\n\n")
            gdb.write("Please make sure that gdb finds libpdbx.so.\n")
            gdb.write("Use \"set solib-search-path\" or \"set sysroot\".\n")
            return

        gdb.write("passed.\n")

        libiomp = gdb.execute("info sharedlibrary libiomp", False, True)
        if "libiomp" in libiomp:
            gdb.write("checking environment...")
            groups = self.checkenv("INTEL_ITTNOTIFY_GROUPS", "sync")
            lib64 = self.checkenv("INTEL_LIBITTNOTIFY64", "")
            lib32 = self.checkenv("INTEL_LIBITTNOTIFY32", "")
            if not (groups and lib64 and lib32):
                gdb.write("failed.\n\n")
                gdb.write("Please add the following environment variables.\n\n")
                gdb.write("  INTEL_ITTNOTIFY_GROUPS = sync\n")
                gdb.write("  INTEL_LIBITTNOTIFY64 = \"\"\n")
                gdb.write("  INTEL_LIBITTNOTIFY32 = \"\"\n")
                return

            gdb.write("passed.\n")

        gdb.write("\nThere are no obvious setup problems.\n")


class PdbxEnableCommand(gdb.Command):
    """Enable data race detection.

    This starts or resumes a suspended data race analysis."""

    def __init__(self):
        super(PdbxEnableCommand,
              self).__init__(name="pdbx enable",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        pdbx.enable()


class PdbxDisableCommand(gdb.Command):
    """Disable data race detection.

    This suspends data race analysis.
    The analysis may later be resumed.

    Use this to skip regions of the code as an alternative to filters."""

    def __init__(self):
        super(PdbxDisableCommand,
              self).__init__(name="pdbx disable",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        pdbx.disable()


class PdbxResetCommand(gdb.Command):
    """Reset data race detection.

    This clears the race analyzer's access logs making it forget what happened."""

    def __init__(self):
        super(PdbxResetCommand,
              self).__init__(name="pdbx reset",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        pdbx.reset()


class PdbxFilterEvalCommand(gdb.Command):
    """Show/set automatic filter evaluation settings.

    off  ... filters are not re-evaluated automatically.
    load ... filters are automatically re-evaluated on libpdbx load.

    Without an argument, this shows the current settings.
    With 'off' as an argument, filters are not automatically re-evaluated.
    With 'load' as an argument, filters are automatically re-evaluated when the
    pdbx run-time library is loaded."""

    def __init__(self):
        super(PdbxFilterEvalCommand,
              self).__init__(name="pdbx eval-filters",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        if not arg:
            gdb.write("%s\n" % pdbx.filtereval)
        elif arg in ("off", "load"):
            pdbx.filtereval = arg
        else:
            raise gdb.GdbError("ERROR: bad argument: %s." % arg)


class PdbxHistoryCommand(gdb.Command):
    """Show the history of reported data races.

    Without arguments, this shows a summary of reported races:
    - the number of individual reports
    - the number of threads involved
    - the number of reads
    - the number of writes
    - the number of updates
    """

    def __init__(self):
        super(PdbxHistoryCommand,
              self).__init__(name="pdbx history",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = True)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        gdb.write("%s\n" % pdbx.history)


class PdbxHistoryListCommand(gdb.Command):
    """List reported data races.

    Without arguments, this lists the entire history.
    With a single integer argument, this lists an individual report.
    With a range argument, this lists a range of reports."""

    def __init__(self):
        super(PdbxHistoryListCommand,
              self).__init__(name="pdbx history list",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)
    @staticmethod
    def write(index, report):
        (threads, reads, writes, updates) = report.stat()

        # We must have at least one access in the report.
        #
        var = report.accesses[0].variable

        gdb.write("%u: %s - %u threads, %u reads, %u writes, %u updates\n" %
                  (index, var, len(set(threads)), reads, writes, updates))

    def invoke(self, arg, from_tty):
        global pdbx

        if not arg:
            index = 0
            for report in pdbx.history.reports:
                self.write(index, report)
                index += 1
        else:
            (begin, end) = require_index_range(arg)

            for index in range(begin, end + 1):
                self.write(index, pdbx.history.get(index))


class PdbxHistoryShowCommand(gdb.Command):
    """Show data race report details.

    With a /r modifier, shows the raw data.

    Without an argument, this shows the full history.
    With a single integer argument, this shows an individual report.
    With a range argument, this shows a range of reports."""

    def __init__(self):
        super(PdbxHistoryShowCommand,
              self).__init__(name="pdbx history show",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    @staticmethod
    def write(report):
        gdb.write("%s" % report)

    @staticmethod
    def write_r(report):
        for access in report.accesses:
            desc = (access.threadid,
                    pdbx_access_type.visualize(access.kind),
                    access.base, access.size,
                    access.location)
            gdb.write("%u: %s 0x%x, %d bytes from 0x%x\n" % desc)

    def invoke(self, arg, from_tty):
        global pdbx

        (modifiers, arg) = parse_modifiers(arg, "r")

        if "r" in modifiers:
            writer = self.write_r
        else:
            writer = self.write

        if not arg:
            for report in pdbx.history.reports:
                writer(report)
        else:
            (begin, end) = require_index_range(arg)

            for index in range(begin, end + 1):
                writer(pdbx.history.get(index))


class PdbxHistoryRemoveCommand(gdb.Command):
    """Remove reported data races.

    Without arguments, this removes all reports.
    With a single integer argument, this removes an individual report.
    With a range argument, this removes a range of reports."""

    def __init__(self):
        super(PdbxHistoryRemoveCommand,
              self).__init__(name="pdbx history remove",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        rng = parse_index_range(arg)
        if not rng:
            pdbx.history.clear()
        else:
            (begin, end) = rng
            size = end - begin

            # When we remove elements, the indices will change. In our case,
            # we will end up removing the same index again and again.
            #
            for repeat in range(0, size + 1):
                pdbx.history.remove(begin)


class PdbxFiltersetCommand(gdb.Command):
    """Show or modify the selected filter set.

    Without arguments, this shows the currently selected filter set"""

    def __init__(self):
        super(PdbxFiltersetCommand,
              self).__init__(name="pdbx fset",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = True)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        gdb.write("%s\n" % pdbx.filterset)


class PdbxFiltersetListCommand(gdb.Command):
    """List the available filter sets."""

    def __init__(self):
        super(PdbxFiltersetListCommand,
              self).__init__(name="pdbx fset list",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        for fset in pdbx.filtersets:
            gdb.write("%s\n" % fset)


class PdbxFiltersetShowCommand(gdb.Command):
    """Show filters in a filter set.

    With a /r modifier, shows the raw data.

    Expects a filter set name and an index range as arguments.
    Both are optional. If both are present, they need to be separated by ':'.
    If no filter set name is given, the currently selected filter set is shown.
    If no index range is given, all filters are shown.

    Examples:
      pdbx fset show
      pdbx fset show 4
      pdbx fset show 1-6
      pdbx fset show other
      pdbx fset show other:4
      pdbx fset show other:1-6"""

    def __init__(self):
        super(PdbxFiltersetShowCommand,
              self).__init__(name="pdbx fset show",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    @staticmethod
    def raw(ranges, heading):
        if not ranges:
            return ""

        disp = heading
        sep  = ":"
        for (begin, size) in ranges:
            disp += "%s 0x%x..0x%x" % (sep, begin, begin + size)
            sep = ","

        return disp

    def write(self, index, flt):
        gdb.write("%d: %s%s\n" % (index, flt, flt.status()))

    def write_r(self, index, flt):
        gdb.write("%d:%s%s%s\n" % (index,
                                   self.raw(flt.code, " code"),
                                   self.raw(flt.data, " data"),
                                   flt.status()))

    def invoke(self, arg, from_tty):
        global pdbx

        (modifiers, arg) = parse_modifiers(arg, "r")
        if "r" in modifiers:
            writer = self.write_r
        else:
            writer = self.write

        (name, begin, end) = parse_line_range(arg)
        fset = pdbx.get_fset(name)

        if begin is None:
            begin = 0
        if end is None:
            end = len(fset.filters) - 1

        for index in range(begin, end + 1):
            writer(index, fset.get(index))


class PdbxFiltersetFocusCommand(gdb.Command):
    """Change the selected filter set to focus.

    Filters in a focus filter set specify the scope of the race analysis."""

    def __init__(self):
        super(PdbxFiltersetFocusCommand,
              self).__init__(name="pdbx fset focus",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        pdbx.filterset.mode = pdbx_fset_mode.focus
        pdbx.sync_filters()


class PdbxFiltersetSuppressCommand(gdb.Command):
    """Change the selected filter set to suppress.

    Filters in a suppress filter set specify what to ignore in the race analysis."""

    def __init__(self):
        super(PdbxFiltersetSuppressCommand,
              self).__init__(name="pdbx fset suppress",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        pdbx.filterset.mode = pdbx_fset_mode.suppress
        pdbx.sync_filters()


class PdbxFiltersetDeleteCommand(gdb.Command):
    """Delete a filter set.

    Takes as single argument the name of the filter set to delete.
    This command cannot delete the selected filter set.
    Use clear to delete filters in the selected filter set."""

    def __init__(self):
        super(PdbxFiltersetDeleteCommand,
              self).__init__(name="pdbx fset delete",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        fset = pdbx.get_fset(arg)

        if fset == pdbx.filterset:
            raise gdb.GdbError("ERROR: cannot delete selected filter set.")

        pdbx.filtersets.remove(fset)


class PdbxFiltersetRemoveCommand(gdb.Command):
    """Remove filters in the selected filter set.

    Expects a filter set name and an index range as arguments.
    Both are optional. If both are present, they need to be separated by ':'.
    If no filter set name is given, the currently selected filter set is used.
    If no index range is given, all filters are removed.

    Examples:
      pdbx fset remove
      pdbx fset remove 4
      pdbx fset remove 1-6
      pdbx fset remove other
      pdbx fset remove other:4
      pdbx fset remove other:1-6"""

    def __init__(self):
        super(PdbxFiltersetRemoveCommand,
              self).__init__(name="pdbx fset remove",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (fset, begin, end) = parse_filter_range(arg)

        # When we remove elements, the indices will change. In our case,
        # we will end up removing the same index again and again.
        #
        for repeat in range(begin, end + 1):
            fset.remove(begin)

        pdbx.sync_filters()


class PdbxFiltersetEnableCommand(gdb.Command):
    """Enable filters in the selected filter set.

    Expects a filter set name and an index range as arguments.
    Both are optional. If both are present, they need to be separated by ':'.
    If no filter set name is given, the currently selected filter set is used.
    If no index range is given, all filters are enabled.

    Examples:
      pdbx fset enable
      pdbx fset enable 4
      pdbx fset enable 1-6
      pdbx fset enable other
      pdbx fset enable other:4
      pdbx fset enable other:1-6"""

    def __init__(self):
        super(PdbxFiltersetEnableCommand,
              self).__init__(name="pdbx fset enable",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (fset, begin, end) = parse_filter_range(arg)

        for index in range(begin, end + 1):
            fset.get(index).enabled = True

        pdbx.sync_filters()


class PdbxFiltersetDisableCommand(gdb.Command):
    """Disable filters in the selected filter set.

    Expects a filter set name and an index range as arguments.
    Both are optional. If both are present, they need to be separated by ':'.
    If no filter set name is given, the currently selected filter set is used.
    If no index range is given, all filters are disabled.

    Examples:
      pdbx fset disable
      pdbx fset disable 4
      pdbx fset disable 1-6
      pdbx fset disable other
      pdbx fset disable other:4
      pdbx fset disable other:1-6"""

    def __init__(self):
        super(PdbxFiltersetDisableCommand,
              self).__init__(name="pdbx fset disable",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (fset, begin, end) = parse_filter_range(arg)

        for index in range(begin, end + 1):
            fset.get(index).enabled = False

        pdbx.sync_filters()


class PdbxFiltersetEvaluateCommand(gdb.Command):
    """Evaluate filters in the selected filter set.

    Expects a filter set name and an index range as arguments.
    Both are optional. If both are present, they need to be separated by ':'.
    If no filter set name is given, the currently selected filter set is used.
    If no index range is given, all filters are evaluated.

    Examples:
      pdbx fset evaluate
      pdbx fset evaluate 4
      pdbx fset evaluate 1-6
      pdbx fset evaluate other
      pdbx fset evaluate other:4
      pdbx fset evaluate other:1-6"""

    def __init__(self):
        super(PdbxFiltersetEvaluateCommand,
              self).__init__(name="pdbx fset evaluate",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (fset, begin, end) = parse_filter_range(arg)

        for index in range(begin, end + 1):
            fset.get(index).evaluate()

        pdbx.sync_filters()


class PdbxFiltersetSelectCommand(gdb.Command):
    """Select an existing filter set.

    Filters in the selected set become active.
    New filters will be added to the selected set."""

    def __init__(self):
        super(PdbxFiltersetSelectCommand,
              self).__init__(name="pdbx fset select",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        pdbx.filterset = pdbx.get_fset(arg)
        pdbx.sync_filters()


class PdbxFiltersetNewCommand(gdb.Command):
    """Add a new filter set.

    This command expects a single argument giving the name of the new filter set.
    The new filter set is automatically selected."""

    def __init__(self):
        super(PdbxFiltersetNewCommand,
              self).__init__(name="pdbx fset new",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        if not arg:
            raise gdb.GdbError("ERROR: expected filter set name.")

        if not arg[0].isalpha():
            raise gdb.GdbError("ERROR: name must start with a letter.")

        fset = pdbx.find_fset(arg)
        if fset is not None:
            raise gdb.GdbError("ERROR: filter set %s already exists." % arg)

        pdbx.filterset = PdbxFilterSet(arg)
        pdbx.filtersets.append(pdbx.filterset)
        pdbx.sync_filters()


class PdbxFiltersetImportCommand(gdb.Command):
    """Import filters from another filter set.

    With a single argument, this adds all filters from the specified filter set
    to the selected filter set.
    With an index range following the filter set name, adds only the specified
    filters.
    Filter set name and index range are separated by ':'.

    Examples:
      pdbx fset import other
      pdbx fset import other:4
      pdbx fset import other:1-6"""

    def __init__(self):
        super(PdbxFiltersetImportCommand,
              self).__init__(name="pdbx fset import",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (fset, begin, end) = parse_filter_range(arg)

        if fset == pdbx.filterset:
            raise gdb.GdbError("ERROR: cannot import selected filter set.")

        for index in range(begin, end + 1):
            pdbx.filterset.add(fset.get(index))

        pdbx.sync_filters()


class PdbxFilterPrefixCommand(gdb.Command):
    """Add a PDBX filter to the selected filter set."""

    def __init__(self):
        super(PdbxFilterPrefixCommand,
              self).__init__(name="pdbx filter",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = True)


class PdbxFilterCodeCommand(gdb.Command):
    """Add a PDBX filter on a code address range."""

    def __init__(self):
        super(PdbxFilterCodeCommand,
              self).__init__(name="pdbx filter code",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (base, size) = parse_address_range(arg)
        code_filter  = PdbxCodeFilter(base, size)
        pdbx.filterset.add(code_filter)
        pdbx.sync_filters()


class PdbxFilterDataCommand(gdb.Command):
    """Add a PDBX filter on a data address range."""

    def __init__(self):
        super(PdbxFilterDataCommand,
              self).__init__(name="pdbx filter data",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        (base, size) = parse_address_range(arg)
        data_filter  = PdbxDataFilter(base, size)
        pdbx.filterset.add(data_filter)
        pdbx.sync_filters()


class PdbxFilterVarCommand(gdb.Command):
    """Add a PDBX filter on a variable."""

    def __init__(self):
        super(PdbxFilterVarCommand,
              self).__init__(name="pdbx filter variable",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        var_filter  = PdbxVarFilter(arg)
        pdbx.filterset.add(var_filter)
        pdbx.sync_filters()


class PdbxFilterLineCommand(gdb.Command):
    """Add a PDBX filter on a source line."""

    def __init__(self):
        super(PdbxFilterLineCommand,
              self).__init__(name="pdbx filter line",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        line_filter  = PdbxLineFilter(arg)
        pdbx.filterset.add(line_filter)
        pdbx.sync_filters()


class PdbxFilterReadsCommand(gdb.Command):
    """Add a global PDBX filter on all reads."""

    def __init__(self):
        super(PdbxFilterReadsCommand,
              self).__init__(name="pdbx filter reads",
                             command_class = gdb.COMMAND_BREAKPOINTS,
                             prefix = False)

    def invoke(self, arg, from_tty):
        global pdbx

        parse_junk(arg)

        reads_filter  = PdbxReadsFilter()
        pdbx.filterset.add(reads_filter)
        pdbx.sync_filters()


PdbxPrefixCommand()
PdbxCheckCommand()
PdbxEnableCommand()
PdbxDisableCommand()
PdbxResetCommand()
PdbxFilterEvalCommand()

PdbxHistoryCommand()
PdbxHistoryListCommand()
PdbxHistoryShowCommand()
PdbxHistoryRemoveCommand()

PdbxFiltersetCommand()
PdbxFiltersetListCommand()
PdbxFiltersetShowCommand()
PdbxFiltersetFocusCommand()
PdbxFiltersetSuppressCommand()
PdbxFiltersetDeleteCommand()
PdbxFiltersetRemoveCommand()
PdbxFiltersetEnableCommand()
PdbxFiltersetDisableCommand()
PdbxFiltersetEvaluateCommand()
PdbxFiltersetSelectCommand()
PdbxFiltersetNewCommand()
PdbxFiltersetImportCommand()

PdbxFilterPrefixCommand()
PdbxFilterCodeCommand()
PdbxFilterDataCommand()
PdbxFilterVarCommand()
PdbxFilterLineCommand()
PdbxFilterReadsCommand()
