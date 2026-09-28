Release libedit-20191211-3.1

http://thrysoee.dk/editline/libedit-20191211-3.1.tar.gz

-- Aug 17, 2026 John Vogt john.leroy.vogt@hpe

gdb4hpc has not updated from the upstream version of editline since December 2019,
and has had some local modification added since with no plans for upstreaming. This
includes small bug fixes and adding utility functions rl_replace_line and
rl_copy_text.  To simplify packaging, I am dropping the current version of this
code directly into the gdb4hpc repository which loses the history of our fork but the
loss is not great; there hasn't been a real code change since September 2021.

Should there be a desire to improve the test I/O experience, modifying this code not
the best approach.   As a first step, the motivation for using this particular
varaiant of editline was driven largely by it being set up for an autotools build.
Since gdb4hpc has since been changed to use meson, you could start by betting source
directly from the bsd version, or even just relying on the libedit packages available
from the OS.  The editline dependency in gdb4hpc is limited to exactly one file, so
updating to a current editline or replacing it with something like linenoise would not
be too difficult even if you would be working on a little visited corner of gdb4hpc.






