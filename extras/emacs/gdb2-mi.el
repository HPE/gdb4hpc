;;; gdb2-mi.el --- User Interface for running GDB2  -*- lexical-binding: t -*-

;; Copyright (C) 2007-2020 Free Software Foundation, Inc.

;; Author: Nick Roberts <nickrob@gnu.org>
;; Maintainer: emacs-devel@gnu.org
;; Keywords: unix, tools

;; This file is part of GNU Emacs.

;; Homepage: https://www.emacswiki.org/emacs/GDB2-MI

;; GNU Emacs is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.

;; GNU Emacs is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.

;;; Credits:

;; This file was written by Nick Roberts following the general design
;; used in gdb2-ui.el for Emacs 22.1 - 23.1.  It was further developed
;; by Dmitry Dzhus <dima@sphinx.net.ru> as part of the Google Summer
;; of Code 2009 Project "Emacs GDB2/MI migration".

;;; Commentary:

;; This mode acts as a graphical user interface to GDB2.  You can interact with
;; GDB2 through the GUD buffer in the usual way, but there are also further
;; buffers which control the execution and describe the state of your program.
;; It separates the input/output of your program from that of GDB2 and displays
;; expressions and their current values in their own buffers.  It also uses
;; features such as the fringe/display margin for breakpoints, and
;; the toolbar (see the GDB2 Graphical Interface section in the Emacs info
;; manual).

;; M-x gdb2 will start the debugger.

;; This file uses GDB2/MI as the primary interface to GDB2.  It runs gdb2 with
;; GDB2/MI (-interp=mi) and access CLI using "-interpreter-exec console
;; cli-command".  This code replaces gdb2-ui.el and uses MI tokens instead
;; of queues.  Eventually MI should be asynchronous.

;; Windows Platforms:

;; If you are using Emacs and GDB2 on Windows you will need to flush the buffer
;; explicitly in your program if you want timely display of I/O in Emacs.
;; Alternatively you can make the output stream unbuffered, for example, by
;; using a macro:

;;           #ifdef UNBUFFERED
;;	     setvbuf (stdout, (char *) NULL, _IONBF, 0);
;;	     #endif

;; and compiling with -DUNBUFFERED while debugging.

;; If you are using Cygwin GDB2 and find that the source is not being displayed
;; in Emacs when you step through it, possible solutions are to:

;;   1) Use Cygwin X Windows and Cygwin Emacs.
;;        (Since 22.1 Emacs builds under Cygwin.)
;;   2) Use MinGW GDB2 instead.
;;   3) Use cygwin-mount.el

;;; macOS:

;; GDB2 in Emacs on macOS works best with FSF GDB2 as Apple have made
;; some changes to the version that they include as part of macOS.
;; This requires GDB2 version 7.0 or later as earlier versions do not
;; compile on macOS.

;;; Known Bugs:

;; 1) Stack buffer doesn't parse MI output if you stop in a routine without
;;    line information, e.g., a routine in libc (just a TODO item).

;; TODO:
;; 2) Watch windows to work with threads.
;; 3) Use treebuffer.el instead of the speedbar for watch-expressions?
;; 4) Mark breakpoint locations on scroll-bar of source buffer?

;;; Code:

(require 'gud)
(require 'json)
(require 'bindat)
(require 'cl-lib)
(require 'cl-seq)
(eval-when-compile (require 'pcase))

(declare-function speedbar-change-initial-expansion-list
                  "speedbar" (new-default))
(declare-function speedbar-timer-fn "speedbar" ())
(declare-function speedbar-line-text "speedbar" (&optional p))
(declare-function speedbar-change-expand-button-char "speedbar" (char))
(declare-function speedbar-delete-subblock "speedbar" (indent))
(declare-function speedbar-center-buffer-smartly "speedbar" ())

(defvar tool-bar-map)
(defvar speedbar-initial-expansion-list-name)
(defvar speedbar-frame)

(defvar-local gdb2-memory-address-expression "main"
  "This expression is passed to gdb2.
Possible value: main, $rsp, x+3.")
(defvar-local gdb2-memory-address nil
  "Address of memory display.")
(defvar-local gdb2-memory-last-address nil
  "Last successfully accessed memory address.")
(defvar	gdb2-memory-next-page nil
  "Address of next memory page for program memory buffer.")
(defvar	gdb2-memory-prev-page nil
  "Address of previous memory page for program memory buffer.")
(defvar-local gdb2--memory-display-warning nil
  "Display warning on memory header if t.
When error occurs when retrieving memory, gdb2-mi displays the
last successful page.  In that case the expression might not
match the memory displayed.  We want to let the user be aware of
that, so display a warning exclamation mark in the header line.")

(defvar gdb2-thread-number nil
  "Main current thread.
Invalidation triggers use this variable to query GDB2 for
information on the specified thread by wrapping GDB2/MI commands
in `gdb2-current-context-command'.
This variable may be updated implicitly by GDB2 via `gdb2-stopped'
or explicitly by `gdb2-select-thread'.
Only `gdb2-setq-thread-number' should be used to change this
value.")

(defvar gdb2-frame-number nil
  "Selected frame level for main current thread.
Updated according to the following rules:
When a thread is selected or current thread stops, set to \"0\".
When current thread goes running (and possibly exits eventually),
set to nil.
May be manually changed by user with `gdb2-select-frame'.")

(defvar gdb2-frame-address nil "Identity of frame for watch expression.")

;; Used to show overlay arrow in source buffer. All set in
;; gdb2-get-main-selected-frame. Disassembly buffer should not use
;; these but rely on buffer-local thread information instead.
(defvar gdb2-selected-frame nil
  "Name of selected function for main current thread.")
(defvar gdb2-selected-file nil
  "Name of selected file for main current thread.")
(defvar gdb2-selected-line nil
  "Number of selected line for main current thread.")

(defvar gdb2-threads-list nil
  "Associative list of threads provided by \"-thread-info\" MI command.
Keys are thread numbers (in strings) and values are structures as
returned from -thread-info by `gdb2-json-partial-output'.  Updated in
`gdb2-thread-list-handler-custom'.")

(defvar gdb2-running-threads-count nil
  "Number of currently running threads.
If nil, no information is available.
Updated in `gdb2-thread-list-handler-custom'.")

(defvar gdb2-stopped-threads-count nil
  "Number of currently stopped threads.
See also `gdb2-running-threads-count'.")

(defvar gdb2-breakpoints-list nil
  "Associative list of breakpoints provided by \"-break-list\" MI command.
Keys are breakpoint numbers (in string) and values are structures
as returned from \"-break-list\" by `gdb2-json-partial-output'
\(\"body\" field is used). Updated in
`gdb2-breakpoints-list-handler-custom'.")

(defvar gdb2-current-language nil)
(defvar gdb2-var-list nil
  "List of variables in watch window.
Each element has the form
  (VARNUM EXPRESSION NUMCHILD TYPE VALUE STATUS HAS_MORE FP)
where STATUS is nil (`unchanged'), `changed' or `out-of-scope', FP the frame
address for root variables.")
(defvar gdb2-main-file nil "Source file from which program execution begins.")

;; Overlay arrow markers
(defvar gdb2-stack-position nil)
(defvar gdb2-thread-position nil)
(defvar gdb2-disassembly-position nil)

(defvar gdb2-location-alist nil
  "Alist of breakpoint numbers and full filenames.
Only used for files that Emacs can't find.")
(defvar gdb2-active-process nil
  "GUD tooltips display variable values when t, and macro definitions otherwise.")
(defvar gdb2-error "Non-nil when GDB2 is reporting an error.")
(defvar gdb2-macro-info nil
  "Non-nil if GDB2 knows that the inferior includes preprocessor macro info.")
(defvar gdb2-register-names nil "List of register names.")
(defvar gdb2-changed-registers nil
  "List of changed register numbers (strings).")
(defvar gdb2-buffer-fringe-width nil)
(defvar gdb2-last-command nil)
(defvar gdb2-prompt-name nil)
(defvar gdb2-token-number 0)
(defvar gdb2-handler-list '()
  "List of gdb2-handler keeping track of all pending GDB2 commands.")
(defvar gdb2-source-file-list nil
  "List of source files for the current executable.")
(defvar gdb2-first-done-or-error t)
(defvar gdb2-source-window-list nil
  "List of windows used for displaying source files.
Sorted in most-recently-visited-first order.")
(defvar gdb2-inferior-status nil)
(defvar gdb2-continuation nil)
(defvar gdb2-supports-non-stop nil)
(defvar gdb2-filter-output nil
  "Message to be shown in GUD console.
This variable is updated in `gdb2-done-or-error' and returned by
`gud-gdb2mi-marker-filter'.")

(defvar gdb2-non-stop nil
  "Indicates whether current GDB2 session is using non-stop mode.
It is initialized to `gdb2-non-stop-setting' at the beginning of
every GDB2 session.")

(defvar-local gdb2-buffer-type nil
  "One of the symbols bound in `gdb2-buffer-rules'.")

(defvar gdb2-output-sink 'nil
  "The disposition of the output of the current gdb2 command.
Possible values are these symbols:
    `user' -- gdb2 output should be copied to the GUD buffer
              for the user to see.
    `emacs' -- output should be collected in the partial-output-buffer
	       for subsequent processing by a command.  This is the
	       disposition of output generated by commands that
	       gdb2 mode sends to gdb2 on its own behalf.")

(defvar gdb2--window-configuration-before nil
  "Stores the window configuration before starting GDB2.")

(defcustom gdb2-restore-window-configuration-after-quit nil
  "If non-nil, restore window configuration as of before GDB2 started.
Possible values are:
    t -- Always restore.
    nil -- Don't restore.
    `if-gdb2-show-main' -- Restore only if variable `gdb2-show-main'
                          is non-nil
    `if-gdb2-many-windows' -- Restore only if variable `gdb2-many-windows'
                             is non-nil."
  :type '(choice
          (const :tag "Always restore" t)
          (const :tag "Don't restore" nil)
          (const :tag "Depends on `gdb2-show-main'" 'if-gdb2-show-main)
          (const :tag "Depends on `gdb2-many-windows'" 'if-gdb2-many-windows))
  :group 'gdb2
  :version "28.1")

(defcustom gdb2-discard-unordered-replies t
  "Non-nil means discard any out-of-order GDB2 replies.
This protects against lost GDB2 replies, assuming that GDB2 always
replies in the same order as Emacs sends commands.  When receiving a
reply with a given token-number, assume any pending messages with a
lower token-number are out-of-order."
  :type 'boolean
  :group 'gud
  :version "24.4")

(cl-defstruct gdb2-handler
  "Data required to handle the reply of a command sent to GDB2."
  ;; Prefix of the command sent to GDB2.  The GDB2 reply for this command
  ;; will be prefixed with this same TOKEN-NUMBER
  (token-number nil :read-only t)
  ;; Callback to invoke when the reply is received from GDB2
  (function nil :read-only t)
  ;; PENDING-TRIGGER is used to prevent congestion: Emacs won't send
  ;; two requests with the same PENDING-TRIGGER until a reply is received
  ;; for the first one."
  (pending-trigger nil))

(defun gdb2-add-handler (token-number handler-function &optional pending-trigger)
  "Insert a new GDB2 command handler in `gdb2-handler-list'.
Handlers are used to keep track of the commands sent to GDB2
and to handle the replies received.
Upon reception of a reply prefixed with TOKEN-NUMBER,
invoke the callback HANDLER-FUNCTION.
If PENDING-TRIGGER is specified, no new GDB2 commands will be
sent with this same PENDING-TRIGGER until a reply is received
for this handler."

  (push (make-gdb2-handler :token-number token-number
                          :function handler-function
                          :pending-trigger pending-trigger)
        gdb2-handler-list))

(defun gdb2-delete-handler (token-number)
  "Remove the handler TOKEN-NUMBER from `gdb2-handler-list'.
Additionally, if `gdb2-discard-unordered-replies' is non-nil,
discard all handlers having a token number less than TOKEN-NUMBER."
  (if gdb2-discard-unordered-replies

      (setq gdb2-handler-list
            (cl-delete-if
             (lambda (handler)
               "Discard any HANDLER with a token number `<=' than TOKEN-NUMBER."
               (when (< (gdb2-handler-token-number handler) token-number)
                 (message "WARNING! Discarding GDB2 handler with token #%d\n"
			  (gdb2-handler-token-number handler)))
               (<= (gdb2-handler-token-number handler) token-number))
             gdb2-handler-list))

    (setq gdb2-handler-list
          (cl-delete-if
           (lambda (handler)
             "Discard any HANDLER with a token number `eq' to TOKEN-NUMBER."
             (eq (gdb2-handler-token-number handler) token-number))
           gdb2-handler-list))))

(defun gdb2-get-handler-function (token-number)
  "Return the function callback registered with the handler TOKEN-NUMBER."
  (gdb2-handler-function
   (cl-find-if (lambda (handler) (eq (gdb2-handler-token-number handler)
                                      token-number))
                gdb2-handler-list)))


(defun gdb2-pending-handler-p (pending-trigger)
  "Return non-nil if a command handler is pending with trigger PENDING-TRIGGER."
  (cl-find-if (lambda (handler) (eq (gdb2-handler-pending-trigger handler)
                                     pending-trigger))
               gdb2-handler-list))


(defun gdb2-handle-reply (token-number)
  "Handle the GDB2 reply TOKEN-NUMBER.
This invokes the handler registered with this token number
in `gdb2-handler-list' and clears all pending handlers invalidated
by the reception of this reply."
  (let ((handler-function (gdb2-get-handler-function token-number)))
    (when handler-function
      ;; Protect against errors in handler-function.
      (condition-case err
          (funcall handler-function)
        (error (message (error-message-string err))))
      (gdb2-delete-handler token-number))))

(defun gdb2-remove-all-pending-triggers ()
  "Remove all pending triggers from gdb2-handler-list.
The handlers are left in gdb2-handler-list so that replies received
from GDB2 could still be handled.  However, removing the pending triggers
allows Emacs to send new commands even if replies of previous commands
were not yet received."
  (dolist (handler gdb2-handler-list)
    (setf (gdb2-handler-pending-trigger handler) nil)))

(defmacro gdb2-wait-for-pending (&rest body)
  "Wait for all pending GDB2 commands to finish and evaluate BODY.
This function checks every 0.5 seconds if there are any pending
triggers in `gdb2-handler-list'."
  `(run-with-timer
    0.5 nil
    '(lambda ()
       (if (not (cl-find-if (lambda (handler)
                               (gdb2-handler-pending-trigger handler))
                             gdb2-handler-list))
	   (progn ,@body)
	 (gdb2-wait-for-pending ,@body)))))

;; Publish-subscribe

(defmacro gdb2-add-subscriber (publisher subscriber)
  "Register new PUBLISHER's SUBSCRIBER.
SUBSCRIBER must be a pair, where cdr is a function of one
argument (see `gdb2-emit-signal')."
  `(add-to-list ',publisher ,subscriber t))

(defmacro gdb2-delete-subscriber (publisher subscriber)
  "Unregister SUBSCRIBER from PUBLISHER."
  `(setq ,publisher (delete ,subscriber
                            ,publisher)))

(defun gdb2-get-subscribers (publisher)
  publisher)

(defun gdb2-emit-signal (publisher &optional signal)
  "Call cdr for each subscriber of PUBLISHER with SIGNAL as argument."
  (dolist (subscriber (gdb2-get-subscribers publisher))
    (funcall (cdr subscriber) signal)))

(defvar gdb2-buf-publisher '()
  "Used to invalidate GDB2 buffers by emitting a signal in `gdb2-update'.
Must be a list of pairs with cars being buffers and cdr's being
valid signal handlers.")

(defgroup gdb2 nil
  "GDB2 graphical interface."
  :group 'tools
  :link '(info-link "(emacs)GDB2 Graphical Interface")
  :version "23.2")

(defgroup gdb2-non-stop nil
  "GDB2 non-stop debugging settings."
  :group 'gdb2
  :version "23.2")

(defgroup gdb2-buffers nil
  "GDB2 buffers."
  :group 'gdb2
  :version "23.2")

(defcustom gdb2-debug-log-max 128
  "Maximum size of `gdb2-debug-log'.  If nil, size is unlimited."
  :group 'gdb2
  :type '(choice (integer :tag "Number of elements")
          (const   :tag "Unlimited" nil))
  :version "22.1")

(defcustom gdb2-non-stop-setting (not (eq system-type 'windows-nt))
  "If non-nil, GDB2 sessions are expected to support the non-stop mode.
When in the non-stop mode, stopped threads can be examined while
other threads continue to execute.
If this is non-nil, GDB2 will be sent the \"set non-stop 1\" command,
and if that results in an error, the non-stop setting will be
turned off automatically.
On MS-Windows, this is off by default, because MS-Windows targets
don't support the non-stop mode.
GDB2 session needs to be restarted for this setting to take effect."
  :type 'boolean
  :group 'gdb2-non-stop
  :version "26.1")

;; TODO Some commands can't be called with --all (give a notice about
;; it in setting doc)
(defcustom gdb2-gud-control-all-threads t
  "When non-nil, GUD execution commands affect all threads when
in non-stop mode.  Otherwise, only current thread is affected."
  :type 'boolean
  :group 'gdb2-non-stop
  :version "23.2")

(defcustom gdb2-switch-reasons t
  "List of stop reasons for which Emacs should switch thread.
When t, switch to stopped thread no matter what the reason was.
When nil, never switch to stopped thread automatically.
This setting is used in non-stop mode only.  In all-stop mode,
Emacs always switches to the thread which caused the stop."
  ;; exited, exited-normally and exited-signaled are not
  ;; thread-specific stop reasons and therefore are not included in
  ;; this list
  :type '(choice
          (const :tag "All reasons" t)
          (set :tag "Selection of reasons..."
               (const :tag "A breakpoint was reached." "breakpoint-hit")
               (const :tag "A watchpoint was triggered." "watchpoint-trigger")
               (const :tag "A read watchpoint was triggered."
                      "read-watchpoint-trigger")
               (const :tag "An access watchpoint was triggered."
                      "access-watchpoint-trigger")
               (const :tag "Function finished execution." "function-finished")
               (const :tag "Location reached." "location-reached")
               (const :tag "Watchpoint has gone out of scope"
                      "watchpoint-scope")
               (const :tag "End of stepping range reached."
                      "end-stepping-range")
               (const :tag "Signal received (like interruption)."
                      "signal-received"))
          (const :tag "None" nil))
  :group 'gdb2-non-stop
  :version "23.2"
  :link '(info-link "(gdb2)GDB2/MI Async Records"))

(defcustom gdb2-stopped-functions nil
  "List of functions called whenever GDB2 stops.
Each function takes one argument, a parsed MI response, which
contains fields of corresponding MI *stopped async record:
    ((stopped-threads . \"all\")
     (thread-id . \"1\")
     (frame (line . \"38\")
            (fullname . \"/home/sphinx/projects/gsoc/server.c\")
            (file . \"server.c\")
            (args ((value . \"0x804b038\")
                   (name . \"arg\")))
            (func . \"hello\")
            (addr . \"0x0804869e\"))
     (reason . \"end-stepping-range\"))
Note that \"reason\" is only present in non-stop debugging mode.
`bindat-get-field' may be used to access the fields of response.
Each function is called after the new current thread was selected
and GDB2 buffers were updated in `gdb2-stopped'."
  :type '(repeat function)
  :group 'gdb2
  :version "23.2"
  :link '(info-link "(gdb2)GDB2/MI Async Records"))

(defcustom gdb2-switch-when-another-stopped t
  "When nil, don't switch to stopped thread if some other
stopped thread is already selected."
  :type 'boolean
  :group 'gdb2-non-stop
  :version "23.2")

(defcustom gdb2-stack-buffer-locations t
  "Show file information or library names in stack buffers."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defcustom gdb2-stack-buffer-addresses nil
  "Show frame addresses in stack buffers."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defcustom gdb2-thread-buffer-verbose-names t
  "Show long thread names in threads buffer."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defcustom gdb2-thread-buffer-arguments t
  "Show function arguments in threads buffer."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defcustom gdb2-thread-buffer-locations t
  "Show file information or library names in threads buffer."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defcustom gdb2-thread-buffer-addresses nil
  "Show addresses for thread frames in threads buffer."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defcustom gdb2-show-threads-by-default nil
  "Show threads list buffer instead of breakpoints list by default."
  :type 'boolean
  :group 'gdb2-buffers
  :version "23.2")

(defvar gdb2-debug-log nil
  "List of commands sent to and replies received from GDB2.
Most recent commands are listed first.  This list stores only the last
`gdb2-debug-log-max' values.  This variable is used to debug GDB2-MI.")

;;;###autoload
(define-minor-mode gdb2-enable-debug
  "Toggle logging of transaction between Emacs and Gdb2.
The log is stored in `gdb2-debug-log' as an alist with elements
whose cons is send, send-item or recv and whose cdr is the string
being transferred.  This list may grow up to a size of
`gdb2-debug-log-max' after which the oldest element (at the end of
the list) is deleted every time a new one is added (at the front)."
  :global t
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-cpp-define-alist-program "gcc -E -dM -"
  "Shell command for generating a list of defined macros in a source file.
This list is used to display the #define directive associated
with an identifier as a tooltip.  It works in a debug session with
GDB2, when `gud-tooltip-mode' is t.
Set `gdb2-cpp-define-alist-flags' for any include paths or
predefined macros."
  :type 'string
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-cpp-define-alist-flags ""
  "Preprocessor flags for `gdb2-cpp-define-alist-program'."
  :type 'string
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-create-source-file-list t
  "Non-nil means create a list of files from which the executable was built.
Set this to nil if the GUD buffer displays \"initializing...\" in the mode
line for a long time when starting, possibly because your executable was
built from a large number of files.  This allows quicker initialization
but means that these files are not automatically enabled for debugging,
e.g., you won't be able to click in the fringe to set a breakpoint until
execution has already stopped there."
  :type 'boolean
  :group 'gdb2
  :version "23.1")

(defcustom gdb2-show-main nil
  "Non-nil means display source file containing the main routine at startup.
Also display the main routine in the disassembly buffer if present."
  :type 'boolean
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-window-configuration-directory user-emacs-directory
  "Directory where GDB2 window configuration files are stored.
If nil, use `default-directory'."
  :type 'string
  :group 'gdb2
  :version "28.1")

(defcustom gdb2-default-window-configuration-file nil
  "If non-nil, load this window configuration (layout) on startup.
This should be the full name of the window configuration file.
If this is not an absolute path, GDB2 treats it as a relative path
and looks under `gdb2-window-configuration-directory'.
Note that this variable only takes effect when variable
`gdb2-many-windows' is t."
  :type '(choice (const :tag "None" nil)
                 string)
  :group 'gdb2
  :version "28.1")

(defcustom gdb2-display-source-buffer-action '(nil . ((inhibit-same-window . t)))
  "`display-buffer' action used when GDB2 displays a source buffer."
  :type 'sexp
  :group 'gdb2
  :version "28.1")

(defcustom gdb2-max-source-window-count 1
  "Maximum number of source windows to use.
Until there are such number of source windows on screen, GDB2
tries to open a new window when visiting a new source file; after
that GDB2 starts to reuse existing source windows."
  :type 'number
  :group 'gdb2
  :version "28.1")

(defvar gdb2mi-debug-mode nil
  "When non-nil, print the messages sent/received from GDB2/MI in *Messages*.")

(defun gdb2-force-mode-line-update (status)
  (let ((buffer gud-comint-buffer))
    (if (and buffer (buffer-name buffer))
	(with-current-buffer buffer
	  (setq mode-line-process
		(format ":%s [%s]"
			(process-status (get-buffer-process buffer)) status))
	  ;; Force mode line redisplay soon.
	  (force-mode-line-update)))))

;; These two are used for menu and toolbar
(defun gdb2-control-all-threads ()
  "Switch to non-stop/A mode."
  (interactive)
  (setq gdb2-gud-control-all-threads t)
  ;; Actually forcing the tool-bar to update.
  (force-mode-line-update)
  (message "Now in non-stop/A mode."))

(defun gdb2-control-current-thread ()
  "Switch to non-stop/T mode."
  (interactive)
  (setq gdb2-gud-control-all-threads nil)
  ;; Actually forcing the tool-bar to update.
  (force-mode-line-update)
  (message "Now in non-stop/T mode."))

(defun gdb2-find-watch-expression ()
  (let* ((var (nth (- (line-number-at-pos (point)) 2) gdb2-var-list))
	 (varnum (car var)) expr)
    (string-match "\\(var[0-9]+\\)\\.\\(.*\\)" varnum)
    (let ((var1 (assoc (match-string 1 varnum) gdb2-var-list)) var2 varnumlet
	  (component-list (split-string (match-string 2 varnum) "\\." t)))
      (setq expr (nth 1 var1))
      (setq varnumlet (car var1))
      (dolist (component component-list)
	(setq var2 (assoc varnumlet gdb2-var-list))
	(setq expr (concat expr
			   (if (string-match ".*\\[[0-9]+\\]$" (nth 3 var2))
			       (concat "[" component "]")
			     (concat "." component))))
	(setq varnumlet (concat varnumlet "." component)))
      expr)))

;; noall is used for commands which don't take --all, but only
;; --thread.
(defun gdb2-gud-context-command (command &optional noall)
  "When `gdb2-non-stop' is t, add --thread option to COMMAND if
`gdb2-gud-control-all-threads' is nil and --all option otherwise.
If NOALL is t, always add --thread option no matter what
`gdb2-gud-control-all-threads' value is.
When `gdb2-non-stop' is nil, return COMMAND unchanged."
  (if gdb2-non-stop
      (if (and gdb2-gud-control-all-threads
               (not noall)
	       gdb2-supports-non-stop)
          (concat command " --all ")
        (gdb2-current-context-command command))
    command))

(defmacro gdb2-gud-context-call (cmd1 &optional cmd2 noall noarg)
  "`gud-call' wrapper which adds --thread/--all options between
CMD1 and CMD2.  NOALL is the same as in `gdb2-gud-context-command'.
NOARG must be t when this macro is used outside `gud-def'."
  `(gud-call
    (concat (gdb2-gud-context-command ,cmd1 ,noall) " " ,cmd2)
    ,(when (not noarg) 'arg)))

(defun gdb2--check-interpreter (filter proc string)
  (unless (zerop (length string))
    (remove-function (process-filter proc) #'gdb2--check-interpreter)
    (unless (memq (aref string 0) '(?^ ?~ ?@ ?& ?* ?= ?1))
      ;; Apparently we're not running with -i=mi.
      (let ((msg (concat "Error: you did not specify -i=mi on GDB2's command line!"  string)))
        (message msg)
        (setq string (concat (propertize msg 'font-lock-face 'error)
                             "\n" string)))
      ;; Use the old gud-gbd filter, not because it works, but because it
      ;; will properly display GDB2's answers rather than hanging waiting for
      ;; answers that aren't coming.
      (set (make-local-variable 'gud-marker-filter) #'gud-gdb-marker-filter))
    (funcall filter proc string)))

(defvar gdb2-control-level 0)

;;;###autoload
(defun gdb2 (command-line)
  "Run gdb2 passing it COMMAND-LINE as arguments.
If COMMAND-LINE names a program FILE to debug, gdb2 will run in
a buffer named *gud-FILE*, and the directory containing FILE
becomes the initial working directory and source-file directory
for your debugger.
If COMMAND-LINE requests that gdb2 attaches to a process PID, gdb2
will run in *gud-PID*, otherwise it will run in *gud*; in these
cases the initial working directory is the default-directory of
the buffer in which this command was invoked.
COMMAND-LINE should include \"-i=mi\" to use gdb2's MI text interface.
Note that the old \"--annotate\" option is no longer supported.
If option `gdb2-many-windows' is nil (the default value) then gdb2 just
pops up the GUD buffer unless `gdb2-show-main' is t.  In this case
it starts with two windows: one displaying the GUD buffer and the
other with the source file with the main routine of the inferior.
If option `gdb2-many-windows' is t, regardless of the value of
`gdb2-show-main', the layout below will appear.  Keybindings are
shown in some of the buffers.
Watch expressions appear in the speedbar/slowbar.
The following commands help control operation :
`gdb2-many-windows'    - Toggle the number of windows gdb2 uses.
`gdb2-restore-windows' - To restore the window layout.
See Info node `(emacs)GDB2 Graphical Interface' for a more
detailed description of this mode.
+----------------------------------------------------------------------+
|                               GDB2 Toolbar                            |
+-----------------------------------+----------------------------------+
| GUD buffer (I/O of GDB2)           | Locals buffer                    |
|                                   |                                  |
|                                   |                                  |
|                                   |                                  |
+-----------------------------------+----------------------------------+
| Source buffer                     | I/O buffer (of debugged program) |
|                                   | (comint-mode)                    |
|                                   |                                  |
|                                   |                                  |
|                                   |                                  |
|                                   |                                  |
|                                   |                                  |
|                                   |                                  |
+-----------------------------------+----------------------------------+
| Stack buffer                      | Breakpoints buffer               |
| RET      gdb2-select-frame         | SPC    gdb2-toggle-breakpoint     |
|                                   | RET    gdb2-goto-breakpoint       |
|                                   | D      gdb2-delete-breakpoint     |
+-----------------------------------+----------------------------------+"
  ;;
  (interactive (list (gud-query-cmdline 'gdb2)))

  (when (and gud-comint-buffer
             (buffer-name gud-comint-buffer)
             (get-buffer-process gud-comint-buffer)
             (with-current-buffer gud-comint-buffer (eq gud-minor-mode 'gdb2a)))
    (gdb2-restore-windows)
    (error
     "Multiple debugging requires restarting in text command mode"))

  ;; Save window configuration before starting gdb2 so we can restore
  ;; it after gdb2 quits. Save it regardless of the value of
  ;; `gdb2-restore-window-configuration-after-quit'.
  (setq gdb2--window-configuration-before (window-state-get))

  ;;
  (gud-common-init command-line nil 'gud-gdb2mi-marker-filter)

  ;; Setup a temporary process filter to warn when GDB2 was not started
  ;; with -i=mi.
  (let ((proc (get-buffer-process gud-comint-buffer)))
    (add-function :around (process-filter proc) #'gdb2--check-interpreter))

  (set (make-local-variable 'gud-minor-mode) 'gdb2mi)
  (set (make-local-variable 'gdb2-control-level) 0)
  (setq comint-input-sender 'gdb2-send)
  (when (ring-empty-p comint-input-ring) ; cf shell-mode
    (let ((hfile (expand-file-name (or (getenv "GDB2HISTFILE")
				       (if (eq system-type 'ms-dos)
					   "_gdb2_history"
					 ".gdb2_history"))))
	  ;; gdb2 defaults to 256, but we'll default to comint-input-ring-size.
	  (hsize (getenv "HISTSIZE")))
      (dolist (file (append '("~/.gdb2init")
			    (unless (string-equal (expand-file-name ".")
                                                  (expand-file-name "~"))
			      '(".gdb2init"))))
	(if (file-readable-p (setq file (expand-file-name file)))
	    (with-temp-buffer
	      (insert-file-contents file)
	      ;; TODO? check for "set history save\\(  *on\\)?" and do
	      ;; not use history otherwise?
	      (while (re-search-forward
		      "^ *set history \\(filename\\|size\\)  *\\(.*\\)" nil t)
		(cond ((string-equal (match-string 1) "filename")
		       (setq hfile (expand-file-name
				    (match-string 2)
				    (file-name-directory file))))
		      ((string-equal (match-string 1) "size")
		       (setq hsize (match-string 2))))))))
      (and (stringp hsize)
	   (integerp (setq hsize (string-to-number hsize)))
	   (> hsize 0)
	   (set (make-local-variable 'comint-input-ring-size) hsize))
      (if (stringp hfile)
	  (set (make-local-variable 'comint-input-ring-file-name) hfile))
      (comint-read-input-ring t)))
  (gud-def gud-tbreak "tbreak %f:%l" "\C-t"
	   "Set temporary breakpoint at current line.")
  (gud-def gud-jump
	   (progn (gud-call "tbreak %f:%l" arg) (gud-call "jump %f:%l"))
	   "\C-j" "Set execution address to current line.")

  (gud-def gud-up     "up %p"     "<" "Up N stack frames (numeric arg).")
  (gud-def gud-down   "down %p"   ">" "Down N stack frames (numeric arg).")
  (gud-def gud-print  "print %e"  "\C-p" "Evaluate C expression at point.")
  (gud-def gud-pstar  "print* %e" nil
	   "Evaluate C dereferenced pointer expression at point.")

  (gud-def gud-step   (gdb2-gud-context-call "-exec-step" "%p" t)
           "\C-s"
	   "Step one source line with display.")
  (gud-def gud-stepi  (gdb2-gud-context-call "-exec-step-instruction" "%p" t)
           "\C-i"
	   "Step one instruction with display.")
  (gud-def gud-next   (gdb2-gud-context-call "-exec-next" "%p" t)
           "\C-n"
	   "Step one line (skip functions).")
  (gud-def gud-nexti  (gdb2-gud-context-call "-exec-next-instruction" "%p" t)
           nil
	   "Step one instruction (skip functions).")
  (gud-def gud-cont   (gdb2-gud-context-call "-exec-continue")
           "\C-r"
	   "Continue with display.")
  (gud-def gud-finish (gdb2-gud-context-call "-exec-finish" nil t)
           "\C-f"
	   "Finish executing current function.")
  (gud-def gud-run    "-exec-run"
           nil
           "Run the program.")

  (gud-def gud-break (if (not (string-match "Disassembly" mode-name))
			 (gud-call "break %f:%l" arg)
		       (save-excursion
			 (beginning-of-line)
			 (forward-char 2)
			 (gud-call "break *%a" arg)))
	   "\C-b" "Set breakpoint at current line or address.")

  (gud-def gud-remove (if (not (string-match "Disassembly" mode-name))
			  (gud-call "clear %f:%l" arg)
			(save-excursion
			  (beginning-of-line)
			  (forward-char 2)
			  (gud-call "clear *%a" arg)))
	   "\C-d" "Remove breakpoint at current line or address.")

  ;; -exec-until doesn't support --all yet
  (gud-def gud-until  (if (not (string-match "Disassembly" mode-name))
			  (gud-call "-exec-until %f:%l" arg)
			(save-excursion
			  (beginning-of-line)
			  (forward-char 2)
			  (gud-call "-exec-until *%a" arg)))
	   "\C-u" "Continue to current line or address.")
  ;; TODO Why arg here?
  (gud-def
   gud-go (gud-call (if gdb2-active-process
                        (gdb2-gud-context-command "-exec-continue")
                      "-exec-run") arg)
   nil "Start or continue execution.")

  ;; For debugging Emacs only.
  (gud-def gud-pp
	   (gud-call
	    (concat
	     "pp " (if (eq (buffer-local-value
			    'major-mode (window-buffer)) 'speedbar-mode)
		       (gdb2-find-watch-expression) "%e")) arg)
	   nil   "Print the Emacs s-expression.")

  (define-key gud-minor-mode-map [left-margin mouse-1]
    'gdb2-mouse-set-clear-breakpoint)
  (define-key gud-minor-mode-map [left-fringe mouse-1]
    'gdb2-mouse-set-clear-breakpoint)
  (define-key gud-minor-mode-map [left-margin C-mouse-1]
    'gdb2-mouse-toggle-breakpoint-margin)
  (define-key gud-minor-mode-map [left-fringe C-mouse-1]
    'gdb2-mouse-toggle-breakpoint-fringe)

  (define-key gud-minor-mode-map [left-margin drag-mouse-1]
    'gdb2-mouse-until)
  (define-key gud-minor-mode-map [left-fringe drag-mouse-1]
    'gdb2-mouse-until)
  (define-key gud-minor-mode-map [left-margin mouse-3]
    'gdb2-mouse-until)
  (define-key gud-minor-mode-map [left-fringe mouse-3]
    'gdb2-mouse-until)

  (define-key gud-minor-mode-map [left-margin C-drag-mouse-1]
    'gdb2-mouse-jump)
  (define-key gud-minor-mode-map [left-fringe C-drag-mouse-1]
    'gdb2-mouse-jump)
  (define-key gud-minor-mode-map [left-fringe C-mouse-3]
    'gdb2-mouse-jump)
  (define-key gud-minor-mode-map [left-margin C-mouse-3]
    'gdb2-mouse-jump)

  (set (make-local-variable 'gud-gdb2-completion-function)
       'gud-gdb2mi-completions)

  (add-hook 'completion-at-point-functions #'gud-gdb2-completion-at-point
            nil 'local)
  (local-set-key "\C-i" 'completion-at-point)

  (local-set-key [remap comint-delchar-or-maybe-eof] 'gdb2-delchar-or-quit)

  (setq gdb2-first-prompt t)
  (setq gud-running nil)

  (gdb2-update)

  (run-hooks 'gdb2-mode-hook))

(defun gdb2-init-1 ()
  ;; (Re-)initialize.
  (setq gdb2-selected-frame nil
	gdb2-frame-number nil
        gdb2-thread-number nil
	gdb2-var-list nil
	gdb2-output-sink 'user
	gdb2-location-alist nil
	gdb2-source-file-list nil
	gdb2-last-command nil
	gdb2-token-number 0
	gdb2-handler-list '()
	gdb2-prompt-name nil
	gdb2-first-done-or-error t
	gdb2-buffer-fringe-width (car (window-fringes))
	gdb2-debug-log nil
	gdb2-source-window-list nil
	gdb2-inferior-status nil
	gdb2-continuation nil
        gdb2-buf-publisher '()
        gdb2-threads-list '()
        gdb2-breakpoints-list '()
        gdb2-register-names '()
        gdb2-non-stop gdb2-non-stop-setting)
  ;;
  (gdb2mi-bnf-init)
  ;;
  (setq gdb2-buffer-type 'gdb2mi)
  ;;
  (gdb2-force-mode-line-update
   (propertize "initializing..." 'face font-lock-variable-name-face))

  (gdb2-get-buffer-create 'gdb2-inferior-io)
  (gdb2-clear-inferior-io)
  (gdb2-inferior-io--init-proc (get-process "gdb4hpc"))

  (when (eq system-type 'windows-nt)
    ;; Don't create a separate console window for the debuggee.
    (gdb2-input "-gdb-set new-console off" 'ignore)
    ;; Force GDB2 to behave as if its input and output stream were
    ;; connected to a TTY device (since on Windows we use pipes for
    ;; communicating with GDB2).
    (gdb2-input "-gdb-set interactive-mode on" 'ignore))
  (gdb2-input "-gdb-set height 0" 'ignore)

  (when gdb2-non-stop
    (gdb2-input "-gdb-set non-stop 1" 'gdb2-non-stop-handler))

  (gdb2-input "-enable-pretty-printing" 'ignore)
  (gdb2-input "-enable-frame-filters" 'ignore)

  ;; Find source file and compilation directory here.
  (if gdb2-create-source-file-list
      ;; Needs GDB2 6.2 onwards.
      (gdb2-input "-file-list-exec-source-files" 'gdb2-get-source-file-list))
  ;; Needs GDB2 6.0 onwards.
  (gdb2-input "-file-list-exec-source-file" 'gdb2-get-source-file)
  (gdb2-input "-gdb-show prompt" 'gdb2-get-prompt))

(defun gdb2-non-stop-handler ()
  (goto-char (point-min))
  (if (re-search-forward "No symbol" nil t)
      (progn
	(message
         "This version of GDB2 doesn't support non-stop mode.  Turning it off.")
	(setq gdb2-non-stop nil)
	(setq gdb2-supports-non-stop nil))
    (setq gdb2-supports-non-stop t)
    (gdb2-input "-gdb-set target-async 1" 'ignore)
    (gdb2-input "-list-target-features" 'gdb2-check-target-async)))

(defun gdb2-check-target-async ()
  (goto-char (point-min))
  (unless (re-search-forward "async" nil t)
    (message
     "Target doesn't support non-stop mode.  Turning it off.")
    (setq gdb2-non-stop nil)
    (gdb2-input "-gdb-set non-stop 0" 'ignore)))

(defun gdb2-delchar-or-quit (arg)
  "Delete ARG characters or send a quit command to GDB2.
Send a quit only if point is at the end of the buffer, there is
no input, and GDB2 is waiting for input."
  (interactive "p")
  (unless (and (eq (current-buffer) gud-comint-buffer)
	       (eq gud-minor-mode 'gdb2mi))
    (error "Not in a GDB2-MI buffer"))
  (let ((proc (get-buffer-process gud-comint-buffer)))
    (if (and (eobp)
             (process-live-p proc)
	     (not gud-running)
	     (= (point) (marker-position (process-mark proc))))
	;; Sending an EOF does not work with GDB2-MI; submit an
	;; explicit quit command.
	(progn
          (if (> gdb2-control-level 0)
              (process-send-eof proc)
            (insert "quit")
            (comint-send-input t t)))
      (delete-char arg))))

(defvar gdb2-define-alist nil "Alist of #define directives for GUD tooltips.")

(defun gdb2-create-define-alist ()
  "Create an alist of #define directives for GUD tooltips."
  (let* ((file (buffer-file-name))
	 (output
	  (with-output-to-string
	    (with-current-buffer standard-output
 	      (and file
		   (file-exists-p file)
 		   ;; call-process doesn't work with remote file names.
		   (not (file-remote-p default-directory))
 		   (call-process shell-file-name file
				 (list t nil) nil "-c"
				 (concat gdb2-cpp-define-alist-program " "
					 gdb2-cpp-define-alist-flags))))))
         (define-list (split-string output "\n" t))
         (name))
    (setq gdb2-define-alist nil)
    (dolist (define define-list)
      (setq name (nth 1 (split-string define "[( ]")))
      (push (cons name define) gdb2-define-alist))))

(declare-function tooltip-show "tooltip" (text &optional use-echo-area))

(defconst gdb2--string-regexp (rx "\""
                                 (* (or (seq "\\" nonl)
                                        (not (any "\"\\"))))
                                 "\""))

(defun gdb2-tooltip-print (expr)
  (with-current-buffer (gdb2-get-buffer 'gdb2-partial-output-buffer)
    (goto-char (point-min))
    (cond
     ((re-search-forward (concat ".*value=\\(" gdb2--string-regexp
                                 "\\)")
                         nil t)
      (tooltip-show
       (concat expr " = " (read (match-string 1)))
       (or gud-tooltip-echo-area
	   (not (display-graphic-p)))))
     ((re-search-forward  "msg=\\(\".+\"\\)$" nil t)
      (tooltip-show (read (match-string 1))
       (or gud-tooltip-echo-area
	   (not (display-graphic-p))))))))

;; If expr is a macro for a function don't print because of possible dangerous
;; side-effects. Also printing a function within a tooltip generates an
;; unexpected starting annotation (phase error).
(defun gdb2-tooltip-print-1 (expr)
  (with-current-buffer (gdb2-get-buffer 'gdb2-partial-output-buffer)
    (goto-char (point-min))
    (if (search-forward "expands to: " nil t)
	(unless (looking-at "\\S-+.*(.*).*")
	  (gdb2-input (concat "-data-evaluate-expression \"" expr "\"")
		     `(lambda () (gdb2-tooltip-print ,expr)))))))

(defun gdb2-init-buffer ()
  (set (make-local-variable 'gud-minor-mode) 'gdb2mi)
  (set (make-local-variable 'tool-bar-map) gud-tool-bar-map)
  (when gud-tooltip-mode
    (make-local-variable 'gdb2-define-alist)
    (gdb2-create-define-alist)
    (add-hook 'after-save-hook 'gdb2-create-define-alist nil t)))

(defmacro gdb2--if-arrow (arrow-position start-posn end-posn &rest body)
  (declare (indent 3))
  (let ((buffer (make-symbol "buffer")))
    `(if ,arrow-position
         (let ((,buffer (marker-buffer ,arrow-position)))
           (if (equal ,buffer (window-buffer (posn-window ,end-posn)))
               (with-current-buffer ,buffer
                 (when (or (equal ,start-posn ,end-posn)
                           (equal (posn-point ,start-posn)
                                  (marker-position ,arrow-position)))
                   ,@body)))))))

(defun gdb2-mouse-until (event)
  "Continue running until a source line past the current line.
The destination source line can be selected either by clicking
with mouse-3 on the fringe/margin or dragging the arrow
with mouse-1 (default bindings)."
  (interactive "e")
  (let ((start (event-start event))
	(end (event-end event)))
    (gdb2--if-arrow gud-overlay-arrow-position start end
      (let ((line (line-number-at-pos (posn-point end))))
        (gud-call (concat "until " (number-to-string line)))))
    (gdb2--if-arrow gdb2-disassembly-position start end
      (save-excursion
        (goto-char (point-min))
        (forward-line (1- (line-number-at-pos (posn-point end))))
        (forward-char 2)
        (gud-call (concat "until *%a"))))))

(defun gdb2-mouse-jump (event)
  "Set execution address/line.
The destination source line can be selected either by clicking with C-mouse-3
on the fringe/margin or dragging the arrow with C-mouse-1 (default bindings).
Unlike `gdb2-mouse-until' the destination address can be before the current
line, and no execution takes place."
  (interactive "e")
  (let ((start (event-start event))
	(end (event-end event)))
    (gdb2--if-arrow gud-overlay-arrow-position start end
      (let ((line (line-number-at-pos (posn-point end))))
        (gud-call (concat "tbreak " (number-to-string line)))
        (gud-call (concat "jump " (number-to-string line)))))
    (gdb2--if-arrow gdb2-disassembly-position start end
      (save-excursion
        (goto-char (point-min))
        (forward-line (1- (line-number-at-pos (posn-point end))))
        (forward-char 2)
        (gud-call (concat "tbreak *%a"))
        (gud-call (concat "jump *%a"))))))

(defcustom gdb2-show-changed-values t
  "If non-nil change the face of out of scope variables and changed values.
Out of scope variables are suppressed with `shadow' face.
Changed values are highlighted with the face `font-lock-warning-face'.
Used by Speedbar."
  :type 'boolean
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-max-children 40
  "Maximum number of children before expansion requires confirmation.
Used by Speedbar."
  :type 'integer
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-delete-out-of-scope t
  "If non-nil delete watch expressions automatically when they go out of scope."
  :type 'boolean
  :group 'gdb2
  :version "22.2")

(define-minor-mode gdb2-speedbar-auto-raise
  "Minor mode to automatically raise the speedbar for watch expressions."
  :global t
  :group 'gdb2
  :version "22.1")

(defcustom gdb2-use-colon-colon-notation nil
  "If non-nil use FUN::VAR format to display variables in the speedbar."
  :type 'boolean
  :group 'gdb2
  :version "22.1")

(define-key gud-minor-mode-map "\C-c\C-w" 'gud-watch)
(define-key global-map (vconcat gud-key-prefix "\C-w") 'gud-watch)

(declare-function tooltip-identifier-from-point "tooltip" (point))

(defun gud-watch (&optional arg event)
  "Watch expression at point.
With arg, enter name of variable to be watched in the minibuffer."
  (interactive (list current-prefix-arg last-input-event))
  (let ((minor-mode (buffer-local-value 'gud-minor-mode gud-comint-buffer)))
    (if (eq minor-mode 'gdb2mi)
	(progn
	  (if event (posn-set-point (event-end event)))
	  (require 'tooltip)
	  (save-selected-window
	    (let ((expr
		   (if arg
		       (completing-read "Name of variable: "
					'gud-gdb2-complete-command)
		     (if (and transient-mark-mode mark-active)
			 (buffer-substring (region-beginning) (region-end))
		       (concat (if (derived-mode-p 'gdb2-registers-mode) "$")
			       (tooltip-identifier-from-point (point)))))))
	      (set-text-properties 0 (length expr) nil expr)
	      (gdb2-input (concat "-var-create - * "  expr "")
			 `(lambda () (gdb2-var-create-handler ,expr))))))
      (message "gud-watch is a no-op in this mode."))))

(defun gdb2-var-create-handler (expr)
  (let* ((result (gdb2-json-partial-output)))
    (if (not (bindat-get-field result 'msg))
        (let ((var
	       (list (bindat-get-field result 'name)
		     (if (and (string-equal gdb2-current-language "c")
			      gdb2-use-colon-colon-notation gdb2-selected-frame)
			 (setq expr (concat gdb2-selected-frame "::" expr))
		       expr)
		     (bindat-get-field result 'numchild)
		     (bindat-get-field result 'type)
		     (bindat-get-field result 'value)
		     nil
		     (bindat-get-field result 'has_more)
                     gdb2-frame-address)))
	  (push var gdb2-var-list)
	  (speedbar 1)
	  (unless (string-equal
		   speedbar-initial-expansion-list-name "GUD")
	    (speedbar-change-initial-expansion-list "GUD")))
      (message-box "No symbol \"%s\" in current context." expr))))

(defun gdb2-speedbar-update ()
  (when (and (boundp 'speedbar-frame) (frame-live-p speedbar-frame))
    ;; Dummy command to update speedbar even when idle.
    (gdb2-input "-environment-pwd"
               'gdb2-speedbar-timer-fn
               'gdb2-speedbar-update)))

(defun gdb2-speedbar-timer-fn ()
  (if gdb2-speedbar-auto-raise
      (raise-frame speedbar-frame))
  (speedbar-timer-fn))

(defun gdb2-var-evaluate-expression-handler (varnum changed)
  (goto-char (point-min))
  (re-search-forward (concat ".*value=\\(" gdb2--string-regexp "\\)")
                     nil t)
  (let ((var (assoc varnum gdb2-var-list)))
    (when var
      (if changed (setcar (nthcdr 5 var) 'changed))
      (setcar (nthcdr 4 var) (read (match-string 1)))))
  (gdb2-speedbar-update))

                                        ; Uses "-var-list-children --all-values".  Needs GDB2 6.1 onwards.
(defun gdb2-var-list-children (varnum)
  (gdb2-input (concat "-var-update " varnum) 'ignore)
  (gdb2-input (concat "-var-list-children --all-values " varnum)
	     `(lambda () (gdb2-var-list-children-handler ,varnum))))

(defun gdb2-var-list-children-handler (varnum)
  (let* ((var-list nil)
	 (output (gdb2-json-partial-output "child"))
	 (children (bindat-get-field output 'children)))
    (catch 'child-already-watched
      (dolist (var gdb2-var-list)
	(if (string-equal varnum (car var))
	    (progn
	      ;; With dynamic varobjs numchild may have increased.
	      (setcar (nthcdr 2 var) (bindat-get-field output 'numchild))
	      (push var var-list)
	      (dolist (child children)
		(let ((varchild (list (bindat-get-field child 'name)
				      (bindat-get-field child 'exp)
				      (bindat-get-field child 'numchild)
				      (bindat-get-field child 'type)
				      (bindat-get-field child 'value)
				      nil
				      (bindat-get-field child 'has_more))))
		  (if (assoc (car varchild) gdb2-var-list)
		      (throw 'child-already-watched nil))
		  (push varchild var-list))))
	  (push var var-list)))
      (setq gdb2-var-list (nreverse var-list))))
  (gdb2-speedbar-update))

(defun gdb2-var-set-format (format)
  "Set the output format for a variable displayed in the speedbar."
  (let* ((var (nth (- (count-lines (point-min) (point)) 2) gdb2-var-list))
	 (varnum (car var)))
    (gdb2-input (concat "-var-set-format " varnum " " format) 'ignore)
    (gdb2-var-update)))

(defun gdb2-var-delete-1 (var varnum)
  (gdb2-input (concat "-var-delete " varnum) 'ignore)
  (setq gdb2-var-list (delq var gdb2-var-list))
  (dolist (varchild gdb2-var-list)
    (if (string-match (concat (car var) "\\.") (car varchild))
	(setq gdb2-var-list (delq varchild gdb2-var-list)))))

(defun gdb2-var-delete ()
  "Delete watch expression at point from the speedbar."
  (interactive)
  (let ((text (speedbar-line-text)))
    (string-match "\\(\\S-+\\)" text)
    (let* ((var (nth (- (count-lines (point-min) (point)) 2) gdb2-var-list))
           (varnum (car var)))
      (if (string-match "\\." (car var))
          (message-box "Can only delete a root expression")
        (gdb2-var-delete-1 var varnum)))))

(defun gdb2-var-delete-children (varnum)
  "Delete children of variable object at point from the speedbar."
  (gdb2-input (concat "-var-delete -c " varnum) 'ignore))

(defun gdb2-edit-value (_text _token _indent)
  "Assign a value to a variable displayed in the speedbar."
  (let* ((var (nth (- (count-lines (point-min) (point)) 2) gdb2-var-list))
	 (varnum (car var))
         (value (read-string "New value: ")))
    (gdb2-input (concat "-var-assign " varnum " " value)
	       `(lambda () (gdb2-edit-value-handler ,value)))))

(defconst gdb2-error-regexp "\\^error,msg=\\(\".+\"\\)")

(defun gdb2-edit-value-handler (value)
  (goto-char (point-min))
  (if (re-search-forward gdb2-error-regexp nil t)
      (message-box "Invalid number or expression (%s)" value)))

                                        ; Uses "-var-update --all-values".  Needs GDB2 6.4 onwards.
(defun gdb2-var-update ()
  (gdb2-input "-var-update --all-values *"
             'gdb2-var-update-handler
             'gdb2-var-update))

(defun gdb2-var-update-handler ()
  (let ((changelist (bindat-get-field (gdb2-json-partial-output) 'changelist)))
    (dolist (var gdb2-var-list)
      (setcar (nthcdr 5 var) nil))
    (let ((temp-var-list gdb2-var-list))
      (dolist (change changelist)
	(let* ((varnum (bindat-get-field change 'name))
	       (var (assoc varnum gdb2-var-list))
	       (new-num (bindat-get-field change 'new_num_children)))
	  (when var
	    (let ((scope (bindat-get-field change 'in_scope))
		  (has-more (bindat-get-field change 'has_more)))
	      (cond ((string-equal scope "false")
		     (if gdb2-delete-out-of-scope
			 (gdb2-var-delete-1 var varnum)
		       (setcar (nthcdr 5 var) 'out-of-scope)))
		    ((string-equal scope "true")
		     (setcar (nthcdr 6 var) has-more)
		     (when (and (or (not has-more)
				    (string-equal has-more "0"))
				(not new-num)
				(string-equal (nth 2 var) "0"))
		       (setcar (nthcdr 4 var)
			       (bindat-get-field change 'value))
		       (setcar (nthcdr 5 var) 'changed)))
		    ((string-equal scope "invalid")
		     (gdb2-var-delete-1 var varnum)))))
	  (let ((var-list nil) var1
		(children (bindat-get-field change 'new_children)))
	    (when new-num
              (setq var1 (pop temp-var-list))
              (while var1
                (if (string-equal varnum (car var1))
                    (let ((new (string-to-number new-num))
                          (previous (string-to-number (nth 2 var1))))
                      (setcar (nthcdr 2 var1) new-num)
                      (push var1 var-list)
                      (cond
                       ((> new previous)
                        ;; Add new children to list.
                        (dotimes (_ previous)
                          (push (pop temp-var-list) var-list))
                        (dolist (child children)
                          (let ((varchild
                                 (list (bindat-get-field child 'name)
                                       (bindat-get-field child 'exp)
                                       (bindat-get-field child 'numchild)
                                       (bindat-get-field child 'type)
                                       (bindat-get-field child 'value)
                                       'changed
                                       (bindat-get-field child 'has_more))))
                            (push varchild var-list))))
                       ;; Remove deleted children from list.
                       ((< new previous)
                        (dotimes (_ new)
                          (push (pop temp-var-list) var-list))
                        (dotimes (_ (- previous new))
                          (pop temp-var-list)))))
                  (push var1 var-list))
                (setq var1 (pop temp-var-list)))
              (setq gdb2-var-list (nreverse var-list))))))))
  (gdb2-speedbar-update))

(defun gdb2-speedbar-expand-node (text token indent)
  "Expand the node the user clicked on.
TEXT is the text of the button we clicked on, a + or - item.
TOKEN is data related to this node.
INDENT is the current indentation depth."
  (cond ((string-match "\\+" text)        ;expand this node
	 (let* ((var (assoc token gdb2-var-list))
		(expr (nth 1 var)) (children (nth 2 var)))
	   (if (or (<= (string-to-number children) gdb2-max-children)
		   (y-or-n-p
		    (format "%s has %s children. Continue? " expr children)))
	       (gdb2-var-list-children token))))
	((string-match "-" text)	;contract this node
	 (dolist (var gdb2-var-list)
	   (if (string-match (concat token "\\.") (car var))
	       (setq gdb2-var-list (delq var gdb2-var-list))))
	 (gdb2-var-delete-children token)
	 (speedbar-change-expand-button-char ?+)
	 (speedbar-delete-subblock indent))
	(t (error "Ooops...  not sure what to do")))
  (speedbar-center-buffer-smartly))

(defun gdb2-get-target-string ()
  (with-current-buffer gud-comint-buffer
    gud-target-name))


;;
;; gdb2 buffers.
;;
;; Each buffer has a TYPE -- a symbol that identifies the function
;; of that particular buffer.
;;
;; The usual gdb2 interaction buffer is given the type `gdb2mi' and
;; is constructed specially.
;;
;; Others are constructed by gdb2-get-buffer-create and
;; named according to the rules set forth in the gdb2-buffer-rules

(defvar gdb2-buffer-rules '())

(defun gdb2-rules-name-maker (rules-entry)
  (cadr rules-entry))
(defun gdb2-rules-buffer-mode (rules-entry)
  (nth 2 rules-entry))
(defun gdb2-rules-update-trigger (rules-entry)
  (nth 3 rules-entry))

(defun gdb2-update-buffer-name ()
  "Rename current buffer according to name-maker associated with
it in `gdb2-buffer-rules'."
  (let ((f (gdb2-rules-name-maker (assoc gdb2-buffer-type
                                        gdb2-buffer-rules))))
    (when f (rename-buffer (funcall f)))))

(defun gdb2-current-buffer-rules ()
  "Get `gdb2-buffer-rules' entry for current buffer type."
  (assoc gdb2-buffer-type gdb2-buffer-rules))

(defun gdb2-current-buffer-thread ()
  "Get thread object of current buffer from `gdb2-threads-list'.
When current buffer is not bound to any thread, return main
thread."
  (cdr (assoc gdb2-thread-number gdb2-threads-list)))

(defun gdb2-current-buffer-frame ()
  "Get current stack frame object for thread of current buffer."
  (bindat-get-field (gdb2-current-buffer-thread) 'frame))

(defun gdb2-buffer-type (buffer)
  "Get value of `gdb2-buffer-type' for BUFFER."
  (with-current-buffer buffer
    gdb2-buffer-type))

(defun gdb2-buffer-shows-main-thread-p ()
  "Return t if current GDB2 buffer shows main selected thread and
is not bound to it."
  (current-buffer)
  (not (local-variable-p 'gdb2-thread-number)))

(defun gdb2-get-buffer (buffer-type &optional thread)
  "Get a specific GDB2 buffer.
In that buffer, `gdb2-buffer-type' must be equal to BUFFER-TYPE
and `gdb2-thread-number' (if provided) must be equal to THREAD."
  (catch 'found
    (dolist (buffer (buffer-list) nil)
      (with-current-buffer buffer
        (when (and (eq gdb2-buffer-type buffer-type)
                   (or (not thread)
                       (equal gdb2-thread-number thread)))
          (throw 'found buffer))))))

(defun gdb2-get-buffer-create (buffer-type &optional thread)
  "Create a new GDB2 buffer of the type specified by BUFFER-TYPE.
The buffer-type should be one of the cars in `gdb2-buffer-rules'.
If THREAD is non-nil, it is assigned to `gdb2-thread-number'
buffer-local variable of the new buffer.
Buffer mode and name are selected according to buffer type.
If buffer has trigger associated with it in `gdb2-buffer-rules',
this trigger is subscribed to `gdb2-buf-publisher' and called with
'update argument."
  (or (gdb2-get-buffer buffer-type thread)
      (let ((rules (assoc buffer-type gdb2-buffer-rules))
            (new (generate-new-buffer "limbo")))
	(with-current-buffer new
	  (let ((mode (gdb2-rules-buffer-mode rules))
                (trigger (gdb2-rules-update-trigger rules)))
	    (when mode (funcall mode))
	    (setq gdb2-buffer-type buffer-type)
            (when thread
              (set (make-local-variable 'gdb2-thread-number) thread))
	    (set (make-local-variable 'gud-minor-mode)
		 (buffer-local-value 'gud-minor-mode gud-comint-buffer))
	    (set (make-local-variable 'tool-bar-map) gud-tool-bar-map)
            (rename-buffer (funcall (gdb2-rules-name-maker rules)))
	    (when trigger
              (gdb2-add-subscriber gdb2-buf-publisher
                                  (cons (current-buffer)
                                        (gdb2-bind-function-to-buffer
                                         trigger (current-buffer))))
              (funcall trigger 'start))
            (current-buffer))))))

(defun gdb2-bind-function-to-buffer (expr buffer)
  "Return a function which will evaluate EXPR in BUFFER."
  `(lambda (&rest args)
     (with-current-buffer ,buffer
       (apply ',expr args))))

;; Used to display windows with thread-bound buffers
(defmacro def-gdb2-preempt-display-buffer (name buffer &optional doc
					       split-horizontal)
  `(defun ,name (&optional thread)
     ,(when doc doc)
     (message "%s" thread)
     (gdb2-preempt-existing-or-display-buffer
      (gdb2-get-buffer-create ,buffer thread)
      ,split-horizontal)))

;; This assoc maps buffer type symbols to rules.  Each rule is a list of
;; at least one and possible more functions.  The functions have these
;; roles in defining a buffer type:
;;
;;     NAME - Return a name for this  buffer type.
;;
;; The remaining function(s) are optional:
;;
;;     MODE - called in a new buffer with no arguments, should establish
;;	      the proper mode for the buffer.
;;

(defun gdb2-set-buffer-rules (buffer-type &rest rules)
  (let ((binding (assoc buffer-type gdb2-buffer-rules)))
    (if binding
	(setcdr binding rules)
      (push (cons buffer-type rules)
	    gdb2-buffer-rules))))

(defun gdb2-parent-mode ()
  "Generic mode to derive all other GDB2 buffer modes from."
  (kill-all-local-variables)
  (setq buffer-read-only t)
  (buffer-disable-undo)
  ;; Delete buffer from gdb2-buf-publisher when it's killed
  ;; (if it has an associated update trigger)
  (add-hook
   'kill-buffer-hook
   (function
    (lambda ()
      (let ((trigger (gdb2-rules-update-trigger
                      (gdb2-current-buffer-rules))))
        (when trigger
          (gdb2-delete-subscriber
           gdb2-buf-publisher
           ;; This should match gdb2-add-subscriber done in
           ;; gdb2-get-buffer-create
           (cons (current-buffer)
                 (gdb2-bind-function-to-buffer trigger (current-buffer))))))))
   nil t))

;; Partial-output buffer : This accumulates output from a command executed on
;; behalf of emacs (rather than the user).
;;
(gdb2-set-buffer-rules 'gdb2-partial-output-buffer
		      'gdb2-partial-output-name)

(defun gdb2-partial-output-name ()
  (concat " *partial-output-"
	  (gdb2-get-target-string)
	  "*"))


(gdb2-set-buffer-rules 'gdb2-inferior-io
		      'gdb2-inferior-io-name
		      'gdb2-inferior-io-mode)

(defun gdb2-inferior-io-name ()
  (concat "*input/output of "
	  (gdb2-get-target-string)
	  "*"))

(defun gdb2-display-io-buffer ()
  "Display IO of debugged program in a separate window."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-inferior-io)))

(defun gdb2-inferior-io--init-proc (proc)
  ;; Set up inferior I/O.  Needs GDB2 6.4 onwards.
  (set-process-filter proc 'gdb2-inferior-filter)
  (set-process-sentinel proc 'gdb2-inferior-io-sentinel)
  ;; The process can run on a remote host.
  (let ((tty (or (process-get proc 'remote-tty)
		 (process-tty-name proc))))
    (unless (or (null tty)
		(string= tty ""))
      (gdb2-input
       (concat "-inferior-tty-set " tty) 'ignore))))

(defun gdb2-inferior-io-sentinel (proc _str)
  (when (eq (process-status proc) 'failed)
    ;; When the debugged process exits, Emacs gets an EIO error on
    ;; read from the pty, and stops listening to it.  If the gdb2
    ;; process is still running, remove the pty, make a new one, and
    ;; pass it to gdb2.
    (let ((io-buffer (process-buffer proc)))
      (when (and (process-live-p (get-buffer-process gud-comint-buffer))
		 (buffer-live-p io-buffer))
	;; `comint-exec' deletes the original process as a side effect.
	(comint-exec io-buffer "gdb4hpc" nil nil nil)
	(gdb2-inferior-io--init-proc (get-buffer-process io-buffer))))))

(defcustom gdb2-display-buffer-other-frame-action
  '((display-buffer-reuse-window display-buffer-pop-up-frame)
    (reusable-frames . visible)
    (inhibit-same-window . t)
    (pop-up-frame-parameters (height . 14)
			     (width . 80)
			     (unsplittable . t)
			     (tool-bar-lines . nil)
			     (menu-bar-lines . nil)
			     (minibuffer . nil)))
  "`display-buffer' action for displaying GDB2 utility frames."
  :group 'gdb2
  :type display-buffer--action-custom-type
  :risky t
  :version "24.3")

(defun gdb2-frame-io-buffer ()
  "Display IO of debugged program in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-inferior-io)
		  gdb2-display-buffer-other-frame-action))

(defvar gdb2-inferior-io-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map "\C-c\C-c" 'gdb2-io-interrupt)
    (define-key map "\C-c\C-z" 'gdb2-io-stop)
    (define-key map "\C-c\C-\\" 'gdb2-io-quit)
    (define-key map "\C-c\C-d" 'gdb2-io-eof)
    (define-key map "\C-d" 'gdb2-io-eof)
    map))

;; We want to use comint because it has various nifty and familiar features.
(define-derived-mode gdb2-inferior-io-mode comint-mode "Inferior I/O"
  "Major mode for gdb2 inferior-io."
  :syntax-table nil :abbrev-table nil
  (make-comint-in-buffer "gdb4hpc" (current-buffer) nil))

(defcustom gdb2-display-io-nopopup nil
  "When non-nil, and the `gdb2-inferior-io' buffer is buried, don't pop it up."
  :type 'boolean
  :group 'gdb2
  :version "25.1")

(defun gdb2-inferior-filter (proc string)
  (unless (string-equal string "")
    (let (buf)
      (unless (and gdb2-display-io-nopopup
                   (setq buf (gdb2-get-buffer 'gdb2-inferior-io))
                   (null (get-buffer-window buf)))
        (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-inferior-io)))))
  (with-current-buffer (gdb2-get-buffer-create 'gdb2-inferior-io)
    (comint-output-filter proc string)))

(defun gdb2-io-interrupt ()
  "Interrupt the program being debugged."
  (interactive)
  (interrupt-process
   (get-buffer-process (gdb2-get-buffer-create 'gdb2-inferior-io)) comint-ptyp))

(defun gdb2-io-quit ()
  "Send quit signal to the program being debugged."
  (interactive)
  (quit-process
   (get-buffer-process (gdb2-get-buffer-create 'gdb2-inferior-io)) comint-ptyp))

(defun gdb2-io-stop ()
  "Stop the program being debugged."
  (interactive)
  (stop-process
   (get-buffer-process (gdb2-get-buffer-create 'gdb2-inferior-io)) comint-ptyp))

(defun gdb2-io-eof ()
  "Send end-of-file to the program being debugged."
  (interactive)
  (process-send-eof
   (get-buffer-process (gdb2-get-buffer-create 'gdb2-inferior-io))))

(defun gdb2-clear-inferior-io ()
  (with-current-buffer (gdb2-get-buffer-create 'gdb2-inferior-io)
    (erase-buffer)))


(defconst breakpoint-xpm-data
  "/* XPM */
static char *magick[] = {
/* columns rows colors chars-per-pixel */
\"10 10 2 1\",
\"  c red\",
\"+ c None\",
/* pixels */
\"+++    +++\",
\"++      ++\",
\"+        +\",
\"          \",
\"          \",
\"          \",
\"          \",
\"+        +\",
\"++      ++\",
\"+++    +++\",
};"
  "XPM data used for breakpoint icon.")

(defconst breakpoint-enabled-pbm-data
  "P1
10 10\",
0 0 0 0 1 1 1 1 0 0 0 0
0 0 0 1 1 1 1 1 1 0 0 0
0 0 1 1 1 1 1 1 1 1 0 0
0 1 1 1 1 1 1 1 1 1 1 0
0 1 1 1 1 1 1 1 1 1 1 0
0 1 1 1 1 1 1 1 1 1 1 0
0 1 1 1 1 1 1 1 1 1 1 0
0 0 1 1 1 1 1 1 1 1 0 0
0 0 0 1 1 1 1 1 1 0 0 0
0 0 0 0 1 1 1 1 0 0 0 0"
  "PBM data used for enabled breakpoint icon.")

(defconst breakpoint-disabled-pbm-data
  "P1
10 10\",
0 0 1 0 1 0 1 0 0 0
0 1 0 1 0 1 0 1 0 0
1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1
1 0 1 0 1 0 1 0 1 0
0 1 0 1 0 1 0 1 0 1
0 0 1 0 1 0 1 0 1 0
0 0 0 1 0 1 0 1 0 0"
  "PBM data used for disabled breakpoint icon.")

(defvar breakpoint-enabled-icon nil
  "Icon for enabled breakpoint in display margin.")

(defvar breakpoint-disabled-icon nil
  "Icon for disabled breakpoint in display margin.")

;; Bitmap for breakpoint in fringe
(define-fringe-bitmap 'breakpoint
  "\x3c\x7e\xff\xff\xff\xff\x7e\x3c")
;; Bitmap for gud-overlay-arrow in fringe
(define-fringe-bitmap 'hollow-right-triangle
  "\xe0\x90\x88\x84\x84\x88\x90\xe0")

(defface breakpoint-enabled
  '((t
     :foreground "red1"
     :weight bold))
  "Face for enabled breakpoint icon in fringe."
  :group 'gdb2)

(defface breakpoint-disabled
  '((((class color) (min-colors 88)) :foreground "grey70")
    ;; Ensure that on low-color displays that we end up something visible.
    (((class color) (min-colors 8) (background light))
     :foreground "black")
    (((class color) (min-colors 8) (background dark))
     :foreground "white")
    (((type tty) (class mono))
     :inverse-video t)
    (t :background "gray"))
  "Face for disabled breakpoint icon in fringe."
  :group 'gdb2)


(defvar gdb2-python-guile-commands-regexp
  "python\\|python-interactive\\|pi\\|guile\\|guile-repl\\|gr"
  "Regexp that matches Python and Guile commands supported by GDB2.")

(defvar gdb2-control-commands-regexp
  (concat
   "^\\("
   "comm\\(a\\(n\\(ds?\\)?\\)?\\)?\\|if\\|while"
   "\\|def\\(i\\(ne?\\)?\\)?\\|doc\\(u\\(m\\(e\\(nt?\\)?\\)?\\)?\\)?\\|"
   gdb2-python-guile-commands-regexp
   "\\|while-stepping\\|stepp\\(i\\(ng?\\)?\\)?\\|ws\\|actions"
   "\\|expl\\(o\\(re?\\)?\\)?"
   "\\)\\([[:blank:]]+\\([^[:blank:]]*\\)\\)*$")
  "Regexp matching GDB2 commands that enter a recursive reading loop.
As long as GDB2 is in the recursive reading loop, it does not expect
commands to be prefixed by \"-interpreter-exec console\".")

(defun gdb2-strip-string-backslash (string)
  (replace-regexp-in-string "\\\\$" "" string))

(defun gdb2-send (proc string)
  "A comint send filter for gdb2."
  (with-current-buffer gud-comint-buffer
    (let ((inhibit-read-only t))
      (remove-text-properties (point-min) (point-max) '(face nil))))
  ;; mimic <RET> key to repeat previous command in GDB2
  (when (= gdb2-control-level 0)
    (if (not (string= "" string))
        (if gdb2-continuation
            (setq gdb2-last-command (concat gdb2-continuation
                                           (gdb2-strip-string-backslash string)
                                           " "))
          (setq gdb2-last-command (gdb2-strip-string-backslash string)))
      (if gdb2-last-command (setq string gdb2-last-command))
      (setq gdb2-continuation nil)))
  (if (and (not gdb2-continuation)
           (or (string-match "^-" string)
               (> gdb2-control-level 0)))
      ;; Either MI command or we are feeding GDB2's recursive reading loop.
      (progn
	(setq gdb2-first-done-or-error t)
	(process-send-string proc (concat string "\n"))
	(if (and (string-match
                  (concat "^\\("
                          (if (eq system-type 'windows-nt) "\026" "\004")
                          "\\|,q\\|,quit\\|end\\)$")
                  string)
		 (> gdb2-control-level 0))
	    (setq gdb2-control-level (1- gdb2-control-level))))
    ;; CLI command
    (if (string-match "\\\\$" string)
	(setq gdb2-continuation
	      (concat gdb2-continuation (gdb2-strip-string-backslash
					string)
		      " "))
      (setq gdb2-first-done-or-error t)
      (let ((to-send (concat "-interpreter-exec console "
                             (gdb2-mi-quote (concat gdb2-continuation string))
                             "\n")))
        (if gdb2-enable-debug
            (push (cons 'mi-send to-send) gdb2-debug-log))
        (process-send-string proc to-send))
      (if (and (string-match
                  (concat "^\\("
                          (if (eq system-type 'windows-nt) "\026" "\004")
                          "\\|,q\\|,quit\\|end\\)$")
                  string)
	       (> gdb2-control-level 0))
	  (setq gdb2-control-level (1- gdb2-control-level)))
      (setq gdb2-continuation nil)))
  ;; Python and Guile commands that have an argument don't enter the
  ;; recursive reading loop.
  (let* ((control-command-p (string-match gdb2-control-commands-regexp string))
         (command-arg (and control-command-p (match-string 3 string)))
         (python-or-guile-p (string-match gdb2-python-guile-commands-regexp
                                          string)))
    (if (and control-command-p
             (or (not python-or-guile-p)
                 (null command-arg)
                 (zerop (length command-arg))))
        (setq gdb2-control-level (1+ gdb2-control-level)))))

(defun gdb2-mi-quote (string)
  "Return STRING quoted properly as an MI argument.
The string is enclosed in double quotes.
All embedded quotes, newlines, and backslashes are preceded with a backslash."
  (setq string (replace-regexp-in-string "\\([\"\\]\\)" "\\\\\\&" string))
  (setq string (replace-regexp-in-string "\n" "\\n" string t t))
  (concat "\"" string "\""))

(defun gdb2-input (command handler-function &optional trigger-name)
  "Send COMMAND to GDB2 via the MI interface.
Run the function HANDLER-FUNCTION, with no arguments, once the command is
complete.  Do not send COMMAND to GDB2 if TRIGGER-NAME is non-nil and
Emacs is still waiting for a reply from another command previously
sent with the same TRIGGER-NAME."
  (when (or (not trigger-name)
            (not (gdb2-pending-handler-p trigger-name)))
    (setq gdb2-token-number (1+ gdb2-token-number))
    (setq command (concat (number-to-string gdb2-token-number) command))

    (if gdb2-enable-debug (push (list 'send-item command handler-function)
                               gdb2-debug-log))

    (gdb2-add-handler gdb2-token-number handler-function trigger-name)

    (if gdb2mi-debug-mode (message "gdb2-input: %s" command))
    (process-send-string (get-buffer-process gud-comint-buffer)
                         (concat command "\n"))))

;; NOFRAME is used for gud execution control commands
(defun gdb2-current-context-command (command)
  "Add --thread to gdb2 COMMAND when needed."
  (if (and gdb2-thread-number
	   gdb2-supports-non-stop)
      (concat command " --thread " gdb2-thread-number)
    command))

(defun gdb2-current-context-buffer-name (name)
  "Add thread information and asterisks to string NAME.
If `gdb2-thread-number' is nil, just wrap NAME in asterisks."
  (concat "*" name
          (if (local-variable-p 'gdb2-thread-number)
              (format " (bound to thread %s)" gdb2-thread-number)
            "")
          "*"))

(defun gdb2-current-context-mode-name (mode)
  "Add thread information to MODE which is to be used as `mode-name'."
  (concat mode
          (if gdb2-thread-number
              (format " [thread %s]" gdb2-thread-number)
            "")))


(defcustom gud-gdb2-command-name "gdb4hpc -i=mi"
  "Default command to execute an executable under the GDB2 debugger."
  :type 'string
  :group 'gdb2)

(defun gdb2-resync()
  (setq gud-running nil)
  (setq gdb2-output-sink 'user)
  (gdb2-remove-all-pending-triggers))

(defun gdb2-update (&optional no-proc)
  "Update buffers showing status of debug session.
If NO-PROC is non-nil, do not try to contact the GDB2 process."
  (when gdb2-first-prompt
    (gdb2-force-mode-line-update
     (propertize "initializing..." 'face font-lock-variable-name-face))
    (gdb2-init-1)
    (setq gdb2-first-prompt nil))

  (unless no-proc
    (gdb2-get-main-selected-frame))

  ;; We may need to update gdb2-threads-list so we can use
  (gdb2-get-buffer-create 'gdb2-threads-buffer)
  ;; gdb2-break-list is maintained in breakpoints handler
  (gdb2-get-buffer-create 'gdb2-breakpoints-buffer)

  (gdb2-get-changed-registers)
  (unless no-proc
    (gdb2-emit-signal gdb2-buf-publisher 'update))

  (when (and (boundp 'speedbar-frame) (frame-live-p speedbar-frame))
    (dolist (var gdb2-var-list)
      (setcar (nthcdr 5 var) nil))
    (gdb2-var-update)))

;; gdb2-setq-thread-number and gdb2-update-gud-running are decoupled
;; because we may need to update current gud-running value without
;; changing current thread (see gdb2-running)
(defun gdb2-setq-thread-number (number)
  "Set `gdb2-thread-number' to NUMBER.
Only this function must be used to change `gdb2-thread-number'
value to NUMBER, because `gud-running' and `gdb2-frame-number'
need to be updated appropriately when current thread changes."
  ;; GDB2 6.8 and earlier always output thread-id="0" when stopping.
  (unless (string-equal number "0") (setq gdb2-thread-number number))
  (setq gdb2-frame-number "0")
  (gdb2-update-gud-running))

(defun gdb2-update-gud-running ()
  "Set `gud-running' according to the state of current thread.
`gdb2-frame-number' is set to 0 if current thread is now stopped.
Note that when `gdb2-gud-control-all-threads' is t, `gud-running'
cannot be reliably used to determine whether or not execution
control buttons should be shown in menu or toolbar.  Use
`gdb2-running-threads-count' and `gdb2-stopped-threads-count'
instead.
For all-stop mode, thread information is unavailable while target
is running."
  (let ((old-value gud-running))
    (setq gud-running
          (string= (bindat-get-field (gdb2-current-buffer-thread) 'state)
                   "running"))
    ;; Set frame number to "0" when _current_ threads stops.
    (when (and (gdb2-current-buffer-thread)
               (not (eq gud-running old-value)))
      (setq gdb2-frame-number "0"))))

(defun gdb2-show-run-p ()
  "Return t if \"Run/continue\" should be shown on the toolbar."
  (or (not gdb2-active-process)
      (and (or
            (not gdb2-gud-control-all-threads)
            (not gdb2-non-stop))
           (not gud-running))
      (and gdb2-gud-control-all-threads
           (> gdb2-stopped-threads-count 0))))

(defun gdb2-show-stop-p ()
  "Return t if \"Stop\" should be shown on the toolbar."
  (or (and (or
            (not gdb2-gud-control-all-threads)
            (not gdb2-non-stop))
           gud-running)
      (and gdb2-gud-control-all-threads
           (not (null gdb2-running-threads-count))
           (> gdb2-running-threads-count 0))))

;; GUD displays the selected GDB2 frame.  This might might not be the current
;; GDB2 frame (after up, down etc).  If no GDB2 frame is visible but the last
;; visited breakpoint is, use that window.
(defun gdb2-display-source-buffer (buffer)
  "Find a window to display BUFFER.
Always find a window to display buffer, and return it."
  ;; This function doesn't take care of setting up source window(s) at startup,
  ;; that's handled by `gdb2-setup-windows' (if `gdb2-many-windows' is non-nil).
  ;; If `buffer' is already shown in a window, use that window.
  (or (get-buffer-window buffer)
      (progn
        ;; First, update the window list.
        (setq gdb2-source-window-list
              (cl-remove-duplicates
               (cl-remove-if-not
                (lambda (win)
                  (and (window-live-p win)
                       (eq (window-frame win)
                           (selected-frame))))
                gdb2-source-window-list)))
        ;; Should we create a new window or reuse one?
        (if (> gdb2-max-source-window-count
               (length gdb2-source-window-list))
            ;; Create a new window, push it to window list and return it.
            (car (push (display-buffer buffer gdb2-display-source-buffer-action)
                       gdb2-source-window-list))
          ;; Reuse a window, we use the oldest window and put that to
          ;; the front of the window list.
          (let ((last-win (car (last gdb2-source-window-list)))
                (rest (butlast gdb2-source-window-list)))
            (set-window-buffer last-win buffer)
            (setq gdb2-source-window-list
                  (cons last-win rest))
            last-win)))))


(defun gdb2mi-start-with (str offset match)
  "Return non-nil if string STR starts with MATCH, else returns nil.
OFFSET is the position in STR at which the comparison takes place."
  (let ((match-length (length match))
	(str-length (- (length str) offset)))
    (when (>= str-length match-length)
      (string-equal match (substring str offset (+ offset match-length))))))

(defun gdb2mi-same-start (str offset match)
  "Return non-nil if STR and MATCH are equal up to the end of either strings.
OFFSET is the position in STR at which the comparison takes place."
  (let* ((str-length (- (length str) offset))
	 (match-length (length match))
	 (compare-length (min str-length match-length)))
    (when (> compare-length 0)
      (string-equal (substring str offset (+ offset compare-length))
		    (substring match 0 compare-length)))))

(defun gdb2mi-is-number (character)
  "Return non-nil if CHARACTER is a numerical character between 0 and 9."
  (and (>= character ?0)
       (<= character ?9)))


(defvar-local gdb2mi-bnf-state 'gdb2mi-bnf-output
  "Current GDB2/MI output parser state.
The parser is placed in a different state when an incomplete data steam is
received from GDB2.
This variable will preserve the state required to resume the parsing
when more data arrives.")

(defvar-local gdb2mi-bnf-offset 0
  "Offset in `gud-marker-acc' at which the parser is reading.
This offset is used to be able to parse the GDB2/MI message
in-place, without the need of copying the string in a temporary buffer
or discarding parsed tokens by substringing the message.")

(defun gdb2mi-bnf-init ()
  "Initialize the GDB2/MI message parser."
  (setq gdb2mi-bnf-state 'gdb2mi-bnf-output)
  (setq gdb2mi-bnf-offset 0)
  (setq gud-marker-acc ""))


(defun gdb2mi-bnf-output ()
  "Implementation of the following GDB2/MI output grammar rule:
  output ==>
       ( out-of-band-record )* [ result-record ] gdb2-prompt"

  (gdb2mi-bnf-skip-unrecognized)
  (while (gdb2mi-bnf-out-of-band-record))
  (gdb2mi-bnf-result-record)
  (gdb2mi-bnf-gdb2-prompt))


(defun gdb2mi-bnf-skip-unrecognized ()
  "Skip characters until is encounters the beginning of a valid record.
Used as a protection mechanism in case something goes wrong when parsing
a GDB2/MI reply message."
  (let ((acc-length (length gud-marker-acc))
	(prefix-offset gdb2mi-bnf-offset)
	(prompt "(gdb2) \n"))

    (while (and (< prefix-offset acc-length)
                (gdb2mi-is-number (aref gud-marker-acc prefix-offset)))
      (setq prefix-offset (1+ prefix-offset)))

    (if (and (< prefix-offset acc-length)
             (not (memq (aref gud-marker-acc prefix-offset)
                        '(?^ ?* ?+ ?= ?~ ?@ ?&)))
             (not (gdb2mi-same-start gud-marker-acc gdb2mi-bnf-offset prompt))
             (string-match "\\([^^*+=~@&]+\\)" gud-marker-acc
                           gdb2mi-bnf-offset))
        (let ((unrecognized-str (match-string 0 gud-marker-acc)))
          (setq gdb2mi-bnf-offset (match-end 0))
	  (if gdb2mi-debug-mode
              (message "gdb2mi-bnf-skip-unrecognized: %s" unrecognized-str))
          (gdb2-shell unrecognized-str)
	  t))))


(defun gdb2mi-bnf-gdb2-prompt ()
  "Implementation of the following GDB2/MI output grammar rule:
  gdb2-prompt ==>
       `(gdb2)' nl
  nl ==>
       CR | CR-LF"

  (let ((prompt "(gdb2) \n"))
    (when (gdb2mi-start-with gud-marker-acc gdb2mi-bnf-offset prompt)
      (if gdb2mi-debug-mode (message "gdb2mi-bnf-gdb2-prompt: %s" prompt))
      (gdb2-gdb2 prompt)
      (setq gdb2mi-bnf-offset (+ gdb2mi-bnf-offset (length prompt)))

      ;; Returns non-nil to tell gud-gdb2mi-marker-filter we've reached
      ;; the end of a GDB2 reply message.
      t)))


(defun gdb2mi-bnf-result-record ()
  "Implementation of the following GDB2/MI output grammar rule:
  result-record ==>
       [ token ] `^' result-class ( `,' result )* nl
  token ==>
       any sequence of digits."

  (gdb2mi-bnf-result-and-async-record-impl))


(defun gdb2mi-bnf-out-of-band-record ()
  "Implementation of the following GDB2/MI output grammar rule:
  out-of-band-record ==>
       async-record | stream-record"

  (or (gdb2mi-bnf-async-record)
      (gdb2mi-bnf-stream-record)))


(defun gdb2mi-bnf-async-record ()
  "Implementation of the following GDB2/MI output grammar rules:
  async-record ==>
       exec-async-output | status-async-output | notify-async-output
  exec-async-output ==>
       [ token ] `*' async-output
  status-async-output ==>
       [ token ] `+' async-output
  notify-async-output ==>
       [ token ] `=' async-output
  async-output ==>
       async-class ( `,' result )* nl"

  (gdb2mi-bnf-result-and-async-record-impl))


(defun gdb2mi-bnf-stream-record ()
  "Implement the following GDB2/MI output grammar rule:
  stream-record ==>
       console-stream-output | target-stream-output | log-stream-output
  console-stream-output ==>
       `~' c-string
  target-stream-output ==>
       `@' c-string
  log-stream-output ==>
       `&' c-string"
  (when (< gdb2mi-bnf-offset (length gud-marker-acc))
    (if (and (member (aref gud-marker-acc gdb2mi-bnf-offset) '(?~ ?@ ?&))
             (string-match (concat "\\([~@&]\\)\\(" gdb2--string-regexp "\\)\n")
                           gud-marker-acc
                           gdb2mi-bnf-offset))
        (let ((prefix (match-string 1 gud-marker-acc))
              (c-string (match-string 2 gud-marker-acc)))

          (setq gdb2mi-bnf-offset (match-end 0))
          (if gdb2mi-debug-mode (message "gdb2mi-bnf-stream-record: %s"
                                        (match-string 0 gud-marker-acc)))

          (cond ((string-equal prefix "~")
                 (gdb2mi-bnf-console-stream-output c-string))
                ((string-equal prefix "@")
                 (gdb2mi-bnf-target-stream-output c-string))
                ((string-equal prefix "&")
                 (gdb2mi-bnf-log-stream-output c-string)))
	  t))))

(defun gdb2mi-bnf-console-stream-output (c-string)
  "Handler for the console-stream-output GDB2/MI output grammar rule."
  (gdb2-console c-string)
  ;; We've written to the GUD console, so we should print the prompt
  ;; after the next result-class or async-class.
  (setq gdb2-first-done-or-error t))

(defun gdb2mi-bnf-target-stream-output (_c-string)
  "Handler for the target-stream-output GDB2/MI output grammar rule."
  ;; Not currently used.
  )

(defun gdb2mi-bnf-log-stream-output (c-string)
  "Handler for the log-stream-output GDB2/MI output grammar rule."
  ;; Suppress "No registers."  GDB2 6.8 and earlier
  ;; duplicates MI error message on internal stream.
  ;; Don't print to GUD buffer.
  (if (not (string-equal (read c-string) "No registers.\n"))
      (gdb2-internals c-string)))


(defconst gdb2mi-bnf-result-state-configs
  '(("^" . (("done" . (gdb2-done . progressive))
            ("error" . (gdb2-error . progressive))
            ("running" . (gdb2-starting . atomic))))
    ("*" . (("stopped" . (gdb2-stopped . atomic))
            ("running" . (gdb2-running . atomic))))
    ("+" . ())
    ("=" . (("thread-created" . (gdb2-thread-created . atomic))
            ("thread-selected" . (gdb2-thread-selected . atomic))
            ("thread-existed" . (gdb2-ignored-notification . atomic))
            ('default . (gdb2-ignored-notification . atomic)))))
  "Alist of alists, mapping the type and class of message to a handler function.
Handler functions are all flagged as either `progressive' or `atomic'.
`progressive' handlers are capable of parsing incomplete messages.
They can be called several time with new data chunk as they arrive from GDB2.
`progressive' handlers must have an extra argument that is set to a non-nil
value when the message is complete.
Implement the following GDB2/MI output grammar rule:
  result-class ==>
       `done' | `running' | `connected' | `error' | `exit'
  async-class ==>
       `stopped' | others (where others will be added depending on the needs
                           --this is still in development).")

(defun gdb2mi-bnf-result-and-async-record-impl ()
  "Common implementation of the result-record and async-record rule.
Both rules share the same syntax.  Those records may be very large in size.
For that reason, the \"result\" part of the  record is parsed by
`gdb2mi-bnf-incomplete-record-result', which will keep
receiving characters as they arrive from GDB2 until the record is complete."
  (let ((acc-length (length gud-marker-acc))
	(prefix-offset gdb2mi-bnf-offset))

    (while (and (< prefix-offset acc-length)
                (gdb2mi-is-number (aref gud-marker-acc prefix-offset)))
      (setq prefix-offset (1+ prefix-offset)))

    (if (and (< prefix-offset acc-length)
             (member (aref gud-marker-acc prefix-offset) '(?* ?+ ?= ?^))
             (string-match "\\([0-9]*\\)\\([*+=^]\\)\\(.+?\\)\\([,\n]\\)"
                           gud-marker-acc gdb2mi-bnf-offset))

        (let ((token (match-string 1 gud-marker-acc))
	      (prefix (match-string 2 gud-marker-acc))
	      (class (match-string 3 gud-marker-acc))
	      (complete (string-equal (match-string 4 gud-marker-acc) "\n"))
	      class-alist
	      class-command)

          (setq gdb2mi-bnf-offset (match-end 0))
	  (if gdb2mi-debug-mode (message "gdb2mi-bnf-result-record: %s"
                                        (match-string 0 gud-marker-acc)))

          (setq class-alist
                (cdr (assoc prefix gdb2mi-bnf-result-state-configs)))
          (setq class-command (cdr (assoc class class-alist)))
          (if (null class-command)
              (setq class-command (cdr (assoc 'default class-alist))))

          (if complete
              (if class-command
                  (if (equal (cdr class-command) 'progressive)
                      (funcall (car class-command) token "" complete)
                    (funcall (car class-command) token "")))
            (setq gdb2mi-bnf-state
                  (lambda ()
                    (gdb2mi-bnf-incomplete-record-result token class-command)))
            (funcall gdb2mi-bnf-state))
	  t))))

(defun gdb2mi-bnf-incomplete-record-result (token class-command)
  "State of the parser used to progressively parse a result-record or async-record
rule from an incomplete data stream.  The parser will stay in this state until
the end of the current result or async record is reached."
  (when (< gdb2mi-bnf-offset (length gud-marker-acc))
    ;; Search the data stream for the end of the current record:
    (let* ((newline-pos (string-match "\n" gud-marker-acc gdb2mi-bnf-offset))
	   (is-progressive (equal (cdr class-command) 'progressive))
       (is-complete (not (null newline-pos)))
       result-str)

      (when gdb2mi-debug-mode
        (message "gdb2mi-bnf-incomplete-record-result: %s"
                 (substring gud-marker-acc gdb2mi-bnf-offset newline-pos)))

      ;; Update the gdb2mi-bnf-offset only if the current chunk of data can
      ;; be processed by the class-command handler:
      (when (or is-complete is-progressive)
        (setq result-str
              (substring gud-marker-acc gdb2mi-bnf-offset newline-pos))

        ;; Move gdb2mi-bnf-offset past the end of the chunk.
        (setq gdb2mi-bnf-offset (+ gdb2mi-bnf-offset (length result-str)))
        (when newline-pos
          (setq gdb2mi-bnf-offset (1+ gdb2mi-bnf-offset))))

      ;; Update the parsing state before invoking the handler in class-command
      ;; to make sure it's not left in an invalid state if the handler was
      ;; to generate an error.
      (if is-complete
	  (setq gdb2mi-bnf-state 'gdb2mi-bnf-output))

      (if class-command
	  (if is-progressive
	      (funcall (car class-command) token result-str is-complete)
	    (if is-complete
		(funcall (car class-command) token result-str))))

      (unless is-complete
        ;; Incomplete gdb2 response: abort parsing until we receive more data.
        (if gdb2mi-debug-mode (message "gdb2mi-bnf-incomplete-record-result, aborting: incomplete stream"))
        (throw 'gdb2mi-incomplete-stream nil))

      is-complete)))


; The following grammar rules are not yet implemented by this GDB2MI-BNF parser.
; The handling of those rules is currently done by the handlers registered
; in gdb2mi-bnf-result-state-configs
;
; result ==>
;      variable "=" value
;
; variable ==>
;      string
;
; value ==>
;      const | tuple | list
;
; const ==>
;      c-string
;
; tuple ==>
;      "{}" | "{" result ( "," result )* "}"
;
; list ==>
;      "[]" | "[" value ( "," value )* "]" | "[" result ( "," result )* "]"

(defcustom gdb2-mi-decode-strings nil
  "When non-nil, decode octal escapes in GDB2 output into non-ASCII text.
If the value is a coding-system, use that coding-system to decode
the bytes reconstructed from octal escapes.  Any other non-nil value
means to decode using the coding-system set for the GDB2 process.
Warning: setting this non-nil might mangle strings reported by GDB2
that have literal substrings which match the \\nnn octal escape
patterns, where nnn is an octal number between 200 and 377.  So
we only recommend to set this variable non-nil if the program you
are debugging really reports non-ASCII text, or some of its source
file names include non-ASCII characters."
  :type '(choice
          (const :tag "Don't decode" nil)
          (const :tag "Decode using default coding-system" t)
          (coding-system :tag "Decode using this coding-system"))
  :group 'gdb2
  :version "25.1")

;; The idea of the following function was suggested
;; by Kenichi Handa <handa@gnu.org>.
;;
;; FIXME: This is fragile: it relies on the assumption that all the
;; non-ASCII strings output by GDB2, including names of the source
;; files, values of string variables in the inferior, etc., are all
;; encoded in the same encoding.  It also assumes that the \nnn
;; sequences are not split between chunks of output of the GDB2 process
;; due to buffering, and arrive together.  Finally, if some string
;; included literal \nnn strings (as opposed to non-ASCII characters
;; converted by GDB2/MI to octal escapes), this decoding will mangle
;; those strings.  When/if GDB2 acquires the ability to not
;; escape-protect non-ASCII characters in its MI output, this kludge
;; should be removed.
(defun gdb2-mi-decode (string)
  "Decode octal escapes in MI output STRING into multibyte text."
  (let ((coding
         (if (coding-system-p gdb2-mi-decode-strings)
             gdb2-mi-decode-strings
           (with-current-buffer
               (gdb2-get-buffer-create 'gdb2-partial-output-buffer)
             buffer-file-coding-system))))
    (with-temp-buffer
      (set-buffer-multibyte nil)
      (prin1 string (current-buffer))
      (goto-char (point-min))
      ;; prin1 quotes the octal escapes as well, which interferes with
      ;; their interpretation by 'read' below.  Remove the extra
      ;; backslashes to countermand that.
      (while (re-search-forward "\\\\\\(\\\\[2-3][0-7][0-7]\\)" nil t)
        (replace-match "\\1" nil nil))
      (goto-char (point-min))
      (decode-coding-string (read (current-buffer)) coding))))

(defun gud-gdb2mi-marker-filter (string)
  "Filter GDB2/MI output."

  ;; If required, decode non-ASCII text encoded with octal escapes.
  (or (null gdb2-mi-decode-strings)
      (setq string (gdb2-mi-decode string)))

  ;; Record transactions if logging is enabled.
  (when gdb2-enable-debug
    (push (cons 'recv string) gdb2-debug-log)
    (if (and gdb2-debug-log-max
	     (> (length gdb2-debug-log) gdb2-debug-log-max))
	(setcdr (nthcdr (1- gdb2-debug-log-max) gdb2-debug-log) nil)))

  ;; Recall the left over gud-marker-acc from last time.
  (setq gud-marker-acc (concat gud-marker-acc string))

  ;; Start accumulating output for the GUD buffer.
  (setq gdb2-filter-output "")

  (let ((acc-length (length gud-marker-acc)))
    (catch 'gdb2mi-incomplete-stream
      (while (and (< gdb2mi-bnf-offset acc-length)
		  (funcall gdb2mi-bnf-state)))))

  (when (/= gdb2mi-bnf-offset 0)
    (setq gud-marker-acc (substring gud-marker-acc gdb2mi-bnf-offset))
    (setq gdb2mi-bnf-offset 0))

  (when (and gdb2mi-debug-mode (> (length gud-marker-acc) 0))
    (message "gud-gdb2mi-marker-filter, unparsed string: %s" gud-marker-acc))

  gdb2-filter-output)

(defun gdb2-gdb2 (_output-field)
  ;; This is needed because the "explore" command is not ended by the
  ;; likes of "end" or "quit", but instead by a RET at the appropriate
  ;; place, and we know we have exited "explore" when we get the
  ;; "(gdb2)" prompt.
  (and (> gdb2-control-level 0)
       (setq gdb2-control-level (1- gdb2-control-level))))

(defun gdb2-shell (output-field)
  (setq gdb2-filter-output
        (concat output-field gdb2-filter-output)))

(defun gdb2-ignored-notification (_token _output-field))

;; gdb2-invalidate-threads is defined to accept 'update-threads signal
(defun gdb2-thread-created (_token _output-field))
(defun gdb2-thread-exited (_token output-field)
  "Handle =thread-exited async record.
Unset `gdb2-thread-number' if current thread exited and update threads list."
  (let* ((thread-id (bindat-get-field (gdb2-json-string output-field) 'id)))
    (if (string= gdb2-thread-number thread-id)
        (gdb2-setq-thread-number nil))
    ;; When we continue current thread and it quickly exits,
    ;; the pending triggers in gdb2-handler-list left after gdb2-running
    ;; disallow us to properly call -thread-info without --thread option.
    ;; Thus we need to use gdb2-wait-for-pending.
    (gdb2-wait-for-pending
     (gdb2-emit-signal gdb2-buf-publisher 'update-threads))))

(defun gdb2-thread-selected (_token output-field)
  "Handler for =thread-selected MI output record.
Sets `gdb2-thread-number' to new id."
  (let* ((result (gdb2-json-string output-field))
         (thread-id (bindat-get-field result 'id)))
    (gdb2-setq-thread-number thread-id)
    ;; Typing `thread N' in GUD buffer makes GDB2 emit `^done' followed
    ;; by `=thread-selected' notification. `^done' causes `gdb2-update'
    ;; as usually. Things happen too fast and second call (from
    ;; gdb2-thread-selected handler) gets cut off by our beloved
    ;; pending triggers.
    ;; Solution is `gdb2-wait-for-pending' macro: it guarantees that its
    ;; body will get executed when `gdb2-handler-list' if free of
    ;; pending triggers.
    (gdb2-wait-for-pending
     (gdb2-update))))

(defun gdb2-running (_token output-field)
  (let* ((thread-id
          (bindat-get-field (gdb2-json-string output-field) 'thread-id)))
    ;; We reset gdb2-frame-number to nil if current thread has gone
    ;; running. This can't be done in gdb2-thread-list-handler-custom
    ;; because we need correct gdb2-frame-number by the time
    ;; -thread-info command is sent.
    (when (or (string-equal thread-id "all")
              (string-equal thread-id gdb2-thread-number))
      (setq gdb2-frame-number nil)))
  (setq gdb2-inferior-status "running")
  (gdb2-force-mode-line-update
   (propertize gdb2-inferior-status 'face font-lock-type-face))
  (when (not gdb2-non-stop)
    (setq gud-running t))
  (setq gdb2-active-process t))

(defun gdb2-starting (_output-field _result)
  ;; CLI commands don't emit ^running at the moment so use gdb2-running too.
  (setq gdb2-inferior-status "running")
  (gdb2-force-mode-line-update
   (propertize gdb2-inferior-status 'face font-lock-type-face))
  (setq gdb2-active-process t)
  (setq gud-running t))

;; -break-insert -t didn't give a reason before gdb2 6.9

(defun gdb2-stopped (_token output-field)
  "Given the contents of *stopped MI async record, select new
current thread and update GDB2 buffers."
  ;; Reason is available with target-async only
  (let* ((result (gdb2-json-string output-field))
         (reason (bindat-get-field result 'reason))
         (thread-id (bindat-get-field result 'thread-id))
         (retval (bindat-get-field result 'return-value))
         (varnum (bindat-get-field result 'gdb2-result-var)))

    ;; -data-list-register-names needs to be issued for any stopped
    ;; thread
    (when (not gdb2-register-names)
      (gdb2-input (concat "-data-list-register-names"
			 (if gdb2-supports-non-stop
			     (concat " --thread " thread-id)))
		 'gdb2-register-names-handler))

    ;; Don't set gud-last-frame here as it's currently done in
    ;; gdb2-frame-handler because synchronous GDB2 doesn't give these fields
    ;; with CLI.
    ;;(when file
    ;;  (setq
    ;;   ;; Extract the frame position from the marker.
    ;;   gud-last-frame (cons file
    ;;    		    (string-to-number
    ;;    		     (match-string 6 gud-marker-acc)))))

    (setq gdb2-inferior-status (or reason "unknown"))
    (gdb2-force-mode-line-update
     (propertize gdb2-inferior-status 'face font-lock-warning-face))
    (if (string-equal reason "exited-normally")
	(setq gdb2-active-process nil))

    (when (and retval varnum
               ;; When the user typed CLI commands, GDB2/MI helpfully
               ;; includes the "Value returned" response in the "~"
               ;; record; here we avoid displaying it twice.
               (not (string-match "^Value returned is " gdb2-filter-output)))
      (setq gdb2-filter-output
            (concat gdb2-filter-output
                    (format "Value returned is %s = %s\n" varnum retval))))

    ;; Select new current thread.

    ;; Don't switch if we have no reasons selected
    (when gdb2-switch-reasons
      ;; Switch from another stopped thread only if we have
      ;; gdb2-switch-when-another-stopped:
      (when (or gdb2-switch-when-another-stopped
                (not (string= "stopped"
                              (bindat-get-field (gdb2-current-buffer-thread) 'state))))
        ;; Switch if current reason has been selected or we have no
        ;; reasons
        (if (or (eq gdb2-switch-reasons t)
                (member reason gdb2-switch-reasons))
            (when (not (string-equal gdb2-thread-number thread-id))
              (message "Switched to thread %s" thread-id)
              (gdb2-setq-thread-number thread-id))
          (message "Thread %s stopped" thread-id))))

    ;; Print "(gdb2)" to GUD console
    (when gdb2-first-done-or-error
      (setq gdb2-filter-output (concat gdb2-filter-output gdb2-prompt-name)))

    ;; In non-stop, we update information as soon as another thread gets
    ;; stopped
    (when (or gdb2-first-done-or-error
              gdb2-non-stop)
      ;; In all-stop this updates gud-running properly as well.
      (gdb2-update)
      (setq gdb2-first-done-or-error nil))
    (run-hook-with-args 'gdb2-stopped-functions result)))

;; Remove the trimmings from log stream containing debugging messages
;; being produced by GDB2's internals, use warning face and send to GUD
;; buffer.
(defun gdb2-internals (output-field)
  (setq gdb2-filter-output
	(gdb2-concat-output
	 gdb2-filter-output
	 (if (string= output-field "\"\\n\"")
	     ""
	   (let ((error-message
		  (read output-field)))
	     (put-text-property
	      0 (length error-message)
	      'face font-lock-warning-face
	      error-message)
	     error-message)))))

;; Remove the trimmings from the console stream and send to GUD buffer
;; (frontend MI commands should not print to this stream)
(defun gdb2-console (output-field)
  (setq gdb2-filter-output
	(gdb2-concat-output gdb2-filter-output (read output-field))))

(defun gdb2-done (token-number output-field is-complete)
  (gdb2-done-or-error token-number 'done output-field is-complete))

(defun gdb2-error (token-number output-field is-complete)
  (gdb2-done-or-error token-number 'error output-field is-complete))

(defun gdb2-done-or-error (token-number type output-field is-complete)
  (if (string-equal token-number "")
      ;; Output from command entered by user
      (progn
	(setq gdb2-output-sink 'user)
	(setq token-number nil)
	;; MI error - send to minibuffer
	(when (eq type 'error)
          ;; Skip "msg=" from `output-field'
          (message "%s" (read (substring output-field 4)))
          ;; Don't send to the console twice.  (If it is a console error
          ;; it is also in the console stream.)
          (setq output-field nil)))
    ;; Output from command from frontend.
    (setq gdb2-output-sink 'emacs))

  ;; The process may already be dead (e.g. C-d at the gdb2 prompt).
  (let* ((proc (get-buffer-process gud-comint-buffer))
	 (no-proc (or (null proc)
		      (memq (process-status proc) '(exit signal)))))

    (when (and is-complete gdb2-first-done-or-error)
      (unless (or token-number gud-running no-proc)
	(setq gdb2-filter-output (concat gdb2-filter-output gdb2-prompt-name)))
      (gdb2-update no-proc)
      (setq gdb2-first-done-or-error nil))

    (setq gdb2-filter-output
	  (gdb2-concat-output gdb2-filter-output output-field))

    ;; We are done concatenating to the output sink.  Restore it to user sink:
    (setq gdb2-output-sink 'user)

    (when (and token-number is-complete)
      (with-current-buffer
	  (gdb2-get-buffer-create 'gdb2-partial-output-buffer)
	(gdb2-handle-reply (string-to-number token-number))))

  (when is-complete
    (gdb2-clear-partial-output))))

(defun gdb2-concat-output (so-far new)
  (cond
   ((eq gdb2-output-sink 'user) (concat so-far new))
   ((eq gdb2-output-sink 'emacs)
    (gdb2-append-to-partial-output new)
    so-far)))

(defun gdb2-append-to-partial-output (string)
  (with-current-buffer (gdb2-get-buffer-create 'gdb2-partial-output-buffer)
    (goto-char (point-max))
    (insert string)))

(defun gdb2-clear-partial-output ()
  (with-current-buffer (gdb2-get-buffer-create 'gdb2-partial-output-buffer)
    (erase-buffer)))

(defun gdb2-jsonify-buffer (&optional fix-key fix-list)
  "Prepare GDB2/MI output in current buffer for parsing with `json-read'.
Field names are wrapped in double quotes and equal signs are
replaced with semicolons.
If FIX-KEY is non-nil, strip all \"FIX-KEY=\" occurrences from
partial output.  This is used to get rid of useless keys in lists
in MI messages, e.g.: [key=.., key=..].  -stack-list-frames and
-break-info are examples of MI commands which issue such
responses.
If FIX-LIST is non-nil, \"FIX-LIST={..}\" is replaced with
\"FIX-LIST=[..]\" prior to parsing.  This is used to fix broken
-break-info output when it contains breakpoint script field
incompatible with GDB2/MI output syntax.
If `default-directory' is remote, full file names are adapted accordingly."
  (save-excursion
    (let ((remote (file-remote-p default-directory)))
      (when remote
        (goto-char (point-min))
        (while (re-search-forward "[\\[,]fullname=\"\\(.+\\)\"" nil t)
          (replace-match (concat remote "\\1") nil nil nil 1))))
    (goto-char (point-min))
    (when fix-key
      (save-excursion
        (while (re-search-forward (concat "[\\[,]\\(" fix-key "=\\)") nil t)
          (replace-match "" nil nil nil 1))))
    (when fix-list
      (save-excursion
        ;; Find positions of braces which enclose broken list
        (while (re-search-forward (concat fix-list "={\"") nil t)
          (let ((p1 (goto-char (- (point) 2)))
                (p2 (progn (forward-sexp)
                           (1- (point)))))
            ;; Replace braces with brackets
            (save-excursion
              (goto-char p1)
              (delete-char 1)
              (insert "[")
              (goto-char p2)
              (delete-char 1)
              (insert "]"))))))
    (goto-char (point-min))
    (insert "{")
    (let ((re (concat "\\([[:alnum:]_-]+\\)=")))
      (while (re-search-forward re nil t)
        (replace-match "\"\\1\":" nil nil)
        (if (eq (char-after) ?\") (forward-sexp) (forward-char))))
    (goto-char (point-max))
    (insert "}")))

(defun gdb2-json-read-buffer (&optional fix-key fix-list)
  "Prepare and parse GDB2/MI output in current buffer with `json-read'.
FIX-KEY and FIX-LIST work as in `gdb2-jsonify-buffer'."
  (gdb2-jsonify-buffer fix-key fix-list)
  (save-excursion
    (goto-char (point-min))
    (let ((json-array-type 'list))
      (json-read))))

(defun gdb2-json-string (string &optional fix-key fix-list)
  "Prepare and parse STRING containing GDB2/MI output with `json-read'.
FIX-KEY and FIX-LIST work as in `gdb2-jsonify-buffer'."
  (with-temp-buffer
    (insert string)
    (gdb2-json-read-buffer fix-key fix-list)))

(defun gdb2-json-partial-output (&optional fix-key fix-list)
  "Prepare and parse gdb2-partial-output-buffer with `json-read'.
FIX-KEY and FIX-KEY work as in `gdb2-jsonify-buffer'."
  (with-current-buffer (gdb2-get-buffer-create 'gdb2-partial-output-buffer)
    (gdb2-json-read-buffer fix-key fix-list)))

(defun gdb2-line-posns (line)
  "Return a pair of LINE beginning and end positions."
  (let ((offset (1+ (- line (line-number-at-pos)))))
    (cons
     (line-beginning-position offset)
     (line-end-position offset))))

(defmacro gdb2-mark-line (line variable)
  "Set VARIABLE marker to point at beginning of LINE.
If current window has no fringes, inverse colors on LINE.
Return position where LINE begins."
  `(save-excursion
     (let* ((posns (gdb2-line-posns ,line))
            (start-posn (car posns))
            (end-posn (cdr posns)))
       (set-marker ,variable (copy-marker start-posn))
       (when (not (> (car (window-fringes)) 0))
         (put-text-property start-posn end-posn
                            'font-lock-face '(:inverse-video t)))
       start-posn)))

(defun gdb2-pad-string (string padding)
  (format (concat "%" (number-to-string padding) "s") string))

;; gdb2-table struct is a way to programmatically construct simple
;; tables. It help to reliably align columns of data in GDB2 buffers
;; and provides
(cl-defstruct gdb2-table
  (column-sizes nil)
  (rows nil)
  (row-properties nil)
  (right-align nil))

(defun gdb2-table-add-row (table row &optional properties)
  "Add ROW of string to TABLE and recalculate column sizes.
When non-nil, PROPERTIES will be added to the whole row when
calling `gdb2-table-string'."
  (let ((rows (gdb2-table-rows table))
        (row-properties (gdb2-table-row-properties table))
        (column-sizes (gdb2-table-column-sizes table))
        (right-align (gdb2-table-right-align table)))
    (when (not column-sizes)
      (setf (gdb2-table-column-sizes table)
            (make-list (length row) 0)))
    (setf (gdb2-table-rows table)
          (append rows (list row)))
    (setf (gdb2-table-row-properties table)
          (append row-properties (list properties)))
    (setf (gdb2-table-column-sizes table)
          (cl-mapcar (lambda (x s)
                         (let ((new-x
                                (max (abs x) (string-width (or s "")))))
                           (if right-align new-x (- new-x))))
                       (gdb2-table-column-sizes table)
                       row))
    ;; Avoid trailing whitespace at eol
    (if (not (gdb2-table-right-align table))
        (setcar (last (gdb2-table-column-sizes table)) 0))))

(defun gdb2-table-string (table &optional sep)
  "Return TABLE as a string with columns separated with SEP."
  (let ((column-sizes (gdb2-table-column-sizes table)))
    (mapconcat
     'identity
     (cl-mapcar
      (lambda (row properties)
        (apply 'propertize
               (mapconcat 'identity
                          (cl-mapcar (lambda (s x) (gdb2-pad-string s x))
                                       row column-sizes)
                          sep)
               properties))
      (gdb2-table-rows table)
      (gdb2-table-row-properties table))
     "\n")))

;; bindat-get-field goes deep, gdb2-get-many-fields goes wide
(defun gdb2-get-many-fields (struct &rest fields)
  "Return a list of FIELDS values from STRUCT."
  (let ((values))
    (dolist (field fields)
      (push (bindat-get-field struct field) values))
    (nreverse values)))

(defmacro def-gdb2-auto-update-trigger (trigger-name gdb2-command
                                                    handler-name
                                                    &optional signal-list)
  "Define a trigger TRIGGER-NAME which sends GDB2-COMMAND and sets
HANDLER-NAME as its handler.  HANDLER-NAME is bound to current
buffer with `gdb2-bind-function-to-buffer'.
If SIGNAL-LIST is non-nil, GDB2-COMMAND is sent only when the
defined trigger is called with an argument from SIGNAL-LIST.  It's
not recommended to define triggers with empty SIGNAL-LIST.
Normally triggers should respond at least to the `update' signal.
Normally the trigger defined by this command must be called from
the buffer where HANDLER-NAME must work.  This should be done so
that buffer-local thread number may be used in GDB2-COMMAND (by
calling `gdb2-current-context-command').
`gdb2-bind-function-to-buffer' is used to achieve this, see
`gdb2-get-buffer-create'.
Triggers defined by this command are meant to be used as a
trigger argument when describing buffer types with
`gdb2-set-buffer-rules'."
  `(defun ,trigger-name (&optional signal)
     (when
         (or (not ,signal-list)
             (memq signal ,signal-list))
       (gdb2-input ,gdb2-command
                  (gdb2-bind-function-to-buffer ',handler-name (current-buffer))
                  (cons (current-buffer) ',trigger-name)))))

;; Used by disassembly buffer only, the rest use
;; def-gdb2-trigger-and-handler
(defmacro def-gdb2-auto-update-handler (handler-name custom-defun
                                                    &optional nopreserve)
  "Define a handler HANDLER-NAME calling CUSTOM-DEFUN.
Handlers are normally called from the buffers they put output in.
Erase current buffer and evaluate CUSTOM-DEFUN.
Then call `gdb2-update-buffer-name'.
If NOPRESERVE is non-nil, window point is not restored after CUSTOM-DEFUN."
  `(defun ,handler-name ()
     (let* ((inhibit-read-only t)
            ,@(unless nopreserve
                '((window (get-buffer-window (current-buffer) 0))
                  (start (window-start window))
                  (p (window-point window)))))
       (erase-buffer)
       (,custom-defun)
       (gdb2-update-buffer-name)
       ,@(when (not nopreserve)
          '((set-window-start window start)
            (set-window-point window p))))))

(defmacro def-gdb2-trigger-and-handler (trigger-name gdb2-command
                                                    handler-name custom-defun
                                                    &optional signal-list)
  "Define trigger and handler.
TRIGGER-NAME trigger is defined to send GDB2-COMMAND.
See `def-gdb2-auto-update-trigger'.
HANDLER-NAME handler uses customization of CUSTOM-DEFUN.
See `def-gdb2-auto-update-handler'."
  `(progn
     (def-gdb2-auto-update-trigger ,trigger-name
       ,gdb2-command
       ,handler-name ,signal-list)
     (def-gdb2-auto-update-handler ,handler-name
       ,custom-defun)))



;; Breakpoint buffer : This displays the output of `-break-list'.
(def-gdb2-trigger-and-handler
  gdb2-invalidate-breakpoints "-break-list"
  gdb2-breakpoints-list-handler gdb2-breakpoints-list-handler-custom
  '(start update))

(gdb2-set-buffer-rules
 'gdb2-breakpoints-buffer
 'gdb2-breakpoints-buffer-name
 'gdb2-breakpoints-mode
 'gdb2-invalidate-breakpoints)

(defun gdb2-breakpoints-list-handler-custom ()
  (let ((breakpoints-list (bindat-get-field
                           (gdb2-json-partial-output "bkpt" "script")
                           'BreakpointTable 'body))
        (table (make-gdb2-table)))
    (setq gdb2-breakpoints-list nil)
    (gdb2-table-add-row table '("Num" "Type" "Disp" "Enb" "Addr" "Hits" "What"))
    (dolist (breakpoint breakpoints-list)
      (add-to-list 'gdb2-breakpoints-list
                   (cons (bindat-get-field breakpoint 'number)
                         breakpoint))
      (let ((at (bindat-get-field breakpoint 'at))
            (pending (bindat-get-field breakpoint 'pending))
            (func (bindat-get-field breakpoint 'func))
	    (type (bindat-get-field breakpoint 'type)))
        (gdb2-table-add-row table
                           (list
                            (bindat-get-field breakpoint 'number)
                            (or type "")
                            (or (bindat-get-field breakpoint 'disp) "")
                            (let ((flag (bindat-get-field breakpoint 'enabled)))
                              (if (string-equal flag "y")
                                  (eval-when-compile
                                    (propertize "y" 'font-lock-face
                                                font-lock-warning-face))
                                (eval-when-compile
                                  (propertize "n" 'font-lock-face
                                              font-lock-comment-face))))
                            (bindat-get-field breakpoint 'addr)
                            (or (bindat-get-field breakpoint 'times) "")
                            (if (and type (string-match ".*watchpoint" type))
                                (bindat-get-field breakpoint 'what)
                              (or pending at
                                  (concat "in "
                                          (propertize (or func "unknown")
                                                      'font-lock-face font-lock-function-name-face)
                                          (gdb2-frame-location breakpoint)))))
                           ;; Add clickable properties only for breakpoints with file:line
                           ;; information
                           (append (list 'gdb2-breakpoint breakpoint)
                                   (when func '(help-echo "mouse-2, RET: visit breakpoint"
                                                mouse-face highlight))))))
    (insert (gdb2-table-string table " "))
    (gdb2-place-breakpoints)))

;; Put breakpoint icons in relevant margins (even those set in the GUD buffer).
(defun gdb2-place-breakpoints ()
  ;; Remove all breakpoint-icons in source buffers but not assembler buffer.
  (dolist (buffer (buffer-list))
    (with-current-buffer buffer
      (if (and (eq gud-minor-mode 'gdb2mi)
               (not (string-match "\\` ?\\*.+\\*\\'" (buffer-name))))
          (gdb2-remove-breakpoint-icons (point-min) (point-max)))))
  (dolist (breakpoint gdb2-breakpoints-list)
    (let* ((breakpoint (cdr breakpoint)) ; gdb2-breakpoints-list is
                                        ; an associative list
           (line (bindat-get-field breakpoint 'line)))
      (when line
        (let ((file (bindat-get-field breakpoint 'fullname))
              (flag (bindat-get-field breakpoint 'enabled))
              (bptno (bindat-get-field breakpoint 'number)))
          (unless (and file (file-exists-p file))
            (setq file (cdr (assoc bptno gdb2-location-alist))))
	  (if (or (null file)
		  (string-equal file "File not found"))
	      ;; If the full filename is not recorded in the
	      ;; breakpoint structure or in `gdb2-location-alist', use
	      ;; -file-list-exec-source-file to extract it.
	      (when (setq file (bindat-get-field breakpoint 'file))
		(gdb2-input (concat "list " file ":1") 'ignore)
		(gdb2-input "-file-list-exec-source-file"
			   `(lambda () (gdb2-get-location
					,bptno ,line ,flag))))
	    (with-current-buffer (find-file-noselect file 'nowarn)
	      (gdb2-init-buffer)
	      ;; Only want one breakpoint icon at each location.
	      (gdb2-put-breakpoint-icon (string-equal flag "y") bptno
				       (string-to-number line)))))))))

(defconst gdb2-source-file-regexp
  (concat "fullname=\\(" gdb2--string-regexp "\\)"))

(defun gdb2-get-location (bptno line flag)
  "Find the directory containing the relevant source file.
Put in buffer and place breakpoint icon."
  (goto-char (point-min))
  (catch 'file-not-found
    (if (re-search-forward gdb2-source-file-regexp nil t)
	(delete (cons bptno "File not found") gdb2-location-alist)
      ;; FIXME: Why/how do we use (match-string 1) when the search failed?
      (push (cons bptno (match-string 1)) gdb2-location-alist)
      (gdb2-resync)
      (unless (assoc bptno gdb2-location-alist)
	(push (cons bptno "File not found") gdb2-location-alist)
	(message-box "Cannot find source file for breakpoint location.
Add directory to search path for source files using the GDB2 command, dir."))
      (throw 'file-not-found nil))
    (with-current-buffer (find-file-noselect (match-string 1))
      (gdb2-init-buffer)
      ;; only want one breakpoint icon at each location
      (gdb2-put-breakpoint-icon (eq flag ?y) bptno (string-to-number line)))))

(add-hook 'find-file-hook 'gdb2-find-file-hook)

(defun gdb2-find-file-hook ()
  "Set up buffer for debugging if file is part of the source code
of the current session."
  (if (and (buffer-name gud-comint-buffer)
	   ;; in case gud or gdb2-ui is just loaded
	   gud-comint-buffer
	   (eq (buffer-local-value 'gud-minor-mode gud-comint-buffer)
	       'gdb2mi))
      (if (member buffer-file-name gdb2-source-file-list)
	  (with-current-buffer (find-buffer-visiting buffer-file-name)
	    (gdb2-init-buffer)))))

(declare-function gud-remove "gdb2-mi" t t) ; gud-def
(declare-function gud-break  "gdb2-mi" t t) ; gud-def
(declare-function fringe-bitmaps-at-pos "fringe.c" (&optional pos window))

(defun gdb2-mouse-set-clear-breakpoint (event)
  "Set/clear breakpoint in left fringe/margin at mouse click.
If not in a source or disassembly buffer just set point."
  (interactive "e")
  (mouse-minibuffer-check event)
  (let ((posn (event-end event)))
    (with-selected-window (posn-window posn)
      (if (or (buffer-file-name) (derived-mode-p 'gdb2-disassembly-mode))
	  (if (numberp (posn-point posn))
	      (save-excursion
		(goto-char (posn-point posn))
		(if (or (posn-object posn)
			(eq (car (fringe-bitmaps-at-pos (posn-point posn)))
			    'breakpoint))
		    (gud-remove nil)
		  (gud-break nil)))))
      (posn-set-point posn))))

(defun gdb2-mouse-toggle-breakpoint-margin (event)
  "Enable/disable breakpoint in left margin with mouse click."
  (interactive "e")
  (mouse-minibuffer-check event)
  (let ((posn (event-end event)))
    (if (numberp (posn-point posn))
	(with-selected-window (posn-window posn)
	  (save-excursion
	    (goto-char (posn-point posn))
	    (if	(posn-object posn)
		(gud-basic-call
		 (let ((bptno (get-text-property
			       0 'gdb2-bptno (car (posn-string posn)))))
		   (concat
		    (if (get-text-property
			 0 'gdb2-enabled (car (posn-string posn)))
			"-break-disable "
		      "-break-enable ")
		    bptno)))))))))

(defun gdb2-mouse-toggle-breakpoint-fringe (event)
  "Enable/disable breakpoint in left fringe with mouse click."
  (interactive "e")
  (mouse-minibuffer-check event)
  (let* ((posn (event-end event))
	 (pos (posn-point posn))
	 obj)
    (when (numberp pos)
      (with-selected-window (posn-window posn)
	(with-current-buffer (window-buffer)
	  (goto-char pos)
	  (dolist (overlay (overlays-in pos pos))
	    (when (overlay-get overlay 'put-break)
	      (setq obj (overlay-get overlay 'before-string))))
	  (when (stringp obj)
	    (gud-basic-call
	     (concat
	      (if (get-text-property 0 'gdb2-enabled obj)
		  "-break-disable "
		"-break-enable ")
              (get-text-property 0 'gdb2-bptno obj)))))))))

(defun gdb2-breakpoints-buffer-name ()
  (concat "*breakpoints of " (gdb2-get-target-string) "*"))

(defun gdb2-display-breakpoints-buffer (&optional thread)
  "Display GDB2 breakpoints."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-breakpoints-buffer thread)))

(defun gdb2-frame-breakpoints-buffer (&optional thread)
  "Display GDB2 breakpoints in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-breakpoints-buffer thread)
		  gdb2-display-buffer-other-frame-action))

(defvar gdb2-breakpoints-mode-map
  (let ((map (make-sparse-keymap))
	(menu (make-sparse-keymap "Breakpoints")))
    (define-key menu [quit] '("Quit"   . gdb2-delete-frame-or-window))
    (define-key menu [goto] '("Goto"   . gdb2-goto-breakpoint))
    (define-key menu [delete] '("Delete" . gdb2-delete-breakpoint))
    (define-key menu [toggle] '("Toggle" . gdb2-toggle-breakpoint))
    (suppress-keymap map)
    (define-key map [menu-bar breakpoints] (cons "Breakpoints" menu))
    (define-key map " " 'gdb2-toggle-breakpoint)
    (define-key map "D" 'gdb2-delete-breakpoint)
    ;; Don't bind "q" to kill-this-buffer as we need it for breakpoint icons.
    (define-key map "q" 'gdb2-delete-frame-or-window)
    (define-key map "\r" 'gdb2-goto-breakpoint)
    (define-key map "\t" (lambda ()
                           (interactive)
                           (gdb2-set-window-buffer
                            (gdb2-get-buffer-create 'gdb2-threads-buffer) t)))
    (define-key map [mouse-2] 'gdb2-goto-breakpoint)
    (define-key map [follow-link] 'mouse-face)
    map))

(defun gdb2-delete-frame-or-window ()
  "Delete frame if there is only one window.  Otherwise delete the window."
  (interactive)
  (if (one-window-p) (delete-frame)
    (delete-window)))

;;from make-mode-line-mouse-map
(defun gdb2-make-header-line-mouse-map (mouse function) "\
Return a keymap with single entry for mouse key MOUSE on the header line.
MOUSE is defined to run function FUNCTION with no args in the buffer
corresponding to the mode line clicked."
  (let ((map (make-sparse-keymap)))
    (define-key map (vector 'header-line mouse) function)
    (define-key map (vector 'header-line 'down-mouse-1) 'ignore)
    map))

(defmacro gdb2-propertize-header (name buffer help-echo mouse-face face)
  `(propertize ,name
	       'help-echo ,help-echo
	       'mouse-face ',mouse-face
	       'face ',face
	       'local-map
	       (gdb2-make-header-line-mouse-map
		'mouse-1
		(lambda (event) (interactive "e")
		  (save-selected-window
		    (select-window (posn-window (event-start event)))
                    (gdb2-set-window-buffer
                     (gdb2-get-buffer-create ',buffer) t) )))))


;; uses "-thread-info". Needs GDB2 7.0 onwards.
;;; Threads view

(defun gdb2-threads-buffer-name ()
  (concat "*threads of " (gdb2-get-target-string) "*"))

(defun gdb2-display-threads-buffer (&optional thread)
  "Display GDB2 threads."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-threads-buffer thread)))

(defun gdb2-frame-threads-buffer (&optional thread)
  "Display GDB2 threads in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-threads-buffer thread)
		  gdb2-display-buffer-other-frame-action))

(def-gdb2-trigger-and-handler
  gdb2-invalidate-threads (gdb2-current-context-command "-thread-info")
  gdb2-thread-list-handler gdb2-thread-list-handler-custom
  '(start update update-threads))

(gdb2-set-buffer-rules
 'gdb2-threads-buffer
 'gdb2-threads-buffer-name
 'gdb2-threads-mode
 'gdb2-invalidate-threads)

(defvar gdb2-threads-font-lock-keywords
  '(("in \\([^ ]+\\)"  (1 font-lock-function-name-face))
    (" \\(stopped\\)"  (1 font-lock-warning-face))
    (" \\(running\\)"  (1 font-lock-string-face))
    ("\\(\\(\\sw\\|[_.]\\)+\\)="  (1 font-lock-variable-name-face)))
  "Font lock keywords used in `gdb2-threads-mode'.")

(defvar gdb2-threads-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map "\r" 'gdb2-select-thread)
    (define-key map "f" 'gdb2-display-stack-for-thread)
    (define-key map "F" 'gdb2-frame-stack-for-thread)
    (define-key map "l" 'gdb2-display-locals-for-thread)
    (define-key map "L" 'gdb2-frame-locals-for-thread)
    (define-key map "r" 'gdb2-display-registers-for-thread)
    (define-key map "R" 'gdb2-frame-registers-for-thread)
    (define-key map "d" 'gdb2-display-disassembly-for-thread)
    (define-key map "D" 'gdb2-frame-disassembly-for-thread)
    (define-key map "i" 'gdb2-interrupt-thread)
    (define-key map "c" 'gdb2-continue-thread)
    (define-key map "s" 'gdb2-step-thread)
    (define-key map "\t"
      (lambda ()
        (interactive)
        (gdb2-set-window-buffer
         (gdb2-get-buffer-create 'gdb2-breakpoints-buffer) t)))
    (define-key map [mouse-2] 'gdb2-select-thread)
    (define-key map [follow-link] 'mouse-face)
    map))

(defvar gdb2-threads-header
  (list
   (gdb2-propertize-header
    "Breakpoints" gdb2-breakpoints-buffer
    "mouse-1: select" mode-line-highlight mode-line-inactive)
   " "
   (gdb2-propertize-header "Threads" gdb2-threads-buffer
			  nil nil mode-line)))

(define-derived-mode gdb2-threads-mode gdb2-parent-mode "Threads"
  "Major mode for GDB2 threads."
  (setq gdb2-thread-position (make-marker))
  (add-to-list 'overlay-arrow-variable-list 'gdb2-thread-position)
  (setq header-line-format gdb2-threads-header)
  (set (make-local-variable 'font-lock-defaults)
       '(gdb2-threads-font-lock-keywords))
  'gdb2-invalidate-threads)

(defun gdb2-thread-list-handler-custom ()
  (let ((threads-list (bindat-get-field (gdb2-json-partial-output) 'threads))
        (table (make-gdb2-table))
        (marked-line nil))
    (setq gdb2-threads-list nil)
    (setq gdb2-running-threads-count 0)
    (setq gdb2-stopped-threads-count 0)
    (set-marker gdb2-thread-position nil)

    (dolist (thread (reverse threads-list))
      (let ((running (equal (bindat-get-field thread 'state) "running")))
        (add-to-list 'gdb2-threads-list
                     (cons (bindat-get-field thread 'id)
                           thread))
        (cl-incf (if running
                     gdb2-running-threads-count
                   gdb2-stopped-threads-count))

        (gdb2-table-add-row
         table
         (list
          (bindat-get-field thread 'id)
          (concat
           (if gdb2-thread-buffer-verbose-names
               (concat (bindat-get-field thread 'target-id) " ") "")
           (bindat-get-field thread 'state)
           ;; Include frame information for stopped threads
           (if (not running)
               (concat
                " in " (bindat-get-field thread 'frame 'func)
                (if gdb2-thread-buffer-arguments
                    (concat
                     " ("
                     (let ((args (bindat-get-field thread 'frame 'args)))
                       (mapconcat
                        (lambda (arg)
                          (apply #'format "%s=%s"
                                 (gdb2-get-many-fields arg 'name 'value)))
                        args ","))
                     ")")
                  "")
                (if gdb2-thread-buffer-locations
                    (gdb2-frame-location (bindat-get-field thread 'frame)) "")
                (if gdb2-thread-buffer-addresses
                    (concat " at " (bindat-get-field thread 'frame 'addr)) ""))
             "")))
         (list
          'gdb2-thread thread
          'mouse-face 'highlight
          'help-echo "mouse-2, RET: select thread")))
      (when (string-equal gdb2-thread-number
                          (bindat-get-field thread 'id))
        (setq marked-line (length gdb2-threads-list))))
    (insert (gdb2-table-string table " "))
    (when marked-line
      (gdb2-mark-line marked-line gdb2-thread-position)))
  ;; We update gud-running here because we need to make sure that
  ;; gdb2-threads-list is up-to-date
  (gdb2-update-gud-running)
  (gdb2-emit-signal gdb2-buf-publisher 'update-disassembly))

(defmacro def-gdb2-thread-buffer-command (name custom-defun &optional doc)
  "Define a NAME command which will act upon thread on the current line.
CUSTOM-DEFUN may use locally bound `thread' variable, which will
be the value of `gdb2-thread' property of the current line.
If `gdb2-thread' is nil, error is signaled."
  `(defun ,name (&optional event)
     ,(when doc doc)
     (interactive (list last-input-event))
     (if event (posn-set-point (event-end event)))
     (save-excursion
       (beginning-of-line)
       (let ((thread (get-text-property (point) 'gdb2-thread)))
         (if thread
             ,custom-defun
           (error "Not recognized as thread line"))))))

(defmacro def-gdb2-thread-buffer-simple-command (name buffer-command
                                                     &optional doc)
  "Define a NAME which will call BUFFER-COMMAND with id of thread
on the current line."
  `(def-gdb2-thread-buffer-command ,name
     (,buffer-command (bindat-get-field thread 'id))
     ,doc))

(def-gdb2-thread-buffer-command gdb2-select-thread
  (let ((new-id (bindat-get-field thread 'id)))
    (gdb2-setq-thread-number new-id)
    (gdb2-input (concat "-thread-select " new-id) 'ignore)
    (gdb2-update))
  "Select the thread at current line of threads buffer.")

(def-gdb2-thread-buffer-simple-command
  gdb2-display-stack-for-thread
  gdb2-preemptively-display-stack-buffer
  "Display stack buffer for the thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-display-locals-for-thread
  gdb2-preemptively-display-locals-buffer
  "Display locals buffer for the thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-display-registers-for-thread
  gdb2-preemptively-display-registers-buffer
  "Display registers buffer for the thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-display-disassembly-for-thread
  gdb2-preemptively-display-disassembly-buffer
  "Display disassembly buffer for the thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-frame-stack-for-thread
  gdb2-frame-stack-buffer
  "Display another frame with stack buffer for thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-frame-locals-for-thread
  gdb2-frame-locals-buffer
  "Display another frame with locals buffer for thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-frame-registers-for-thread
  gdb2-frame-registers-buffer
  "Display another frame with registers buffer for the thread at current line.")

(def-gdb2-thread-buffer-simple-command
  gdb2-frame-disassembly-for-thread
  gdb2-frame-disassembly-buffer
  "Display another frame with disassembly buffer for the thread at current line.")

(defmacro def-gdb2-thread-buffer-gud-command (name gud-command &optional doc)
  "Define a NAME which will execute GUD-COMMAND with
`gdb2-thread-number' locally bound to id of thread on the current
line."
  `(def-gdb2-thread-buffer-command ,name
     (if gdb2-non-stop
         (let ((gdb2-thread-number (bindat-get-field thread 'id))
               (gdb2-gud-control-all-threads nil))
           (call-interactively #',gud-command))
       (error "Available in non-stop mode only, customize `gdb2-non-stop-setting'"))
     ,doc))

(def-gdb2-thread-buffer-gud-command
  gdb2-interrupt-thread
  gud-stop-subjob
  "Interrupt thread at current line.")

;; Defined opaquely in M-x gdb2 via gud-def.
(declare-function gud-cont "gdb2-mi" (arg) t)

(def-gdb2-thread-buffer-gud-command
  gdb2-continue-thread
  gud-cont
  "Continue thread at current line.")

(declare-function gud-step "gdb2-mi" (arg) t)

(def-gdb2-thread-buffer-gud-command
  gdb2-step-thread
  gud-step
  "Step thread at current line.")


;;; Memory view

(defcustom gdb2-memory-rows 8
  "Number of data rows in memory window."
  :type 'integer
  :group 'gud
  :version "23.2")

(defcustom gdb2-memory-columns 4
  "Number of data columns in memory window."
  :type 'integer
  :group 'gud
  :version "23.2")

(defcustom gdb2-memory-format "x"
  "Display format of data items in memory window."
  :type '(choice (const :tag "Hexadecimal" "x")
          (const :tag "Signed decimal" "d")
          (const :tag "Unsigned decimal" "u")
          (const :tag "Octal" "o")
          (const :tag "Binary" "t"))
  :group 'gud
  :version "22.1")

(defcustom gdb2-memory-unit 4
  "Unit size of data items in memory window."
  :type '(choice (const :tag "Byte" 1)
          (const :tag "Halfword" 2)
          (const :tag "Word" 4)
          (const :tag "Giant word" 8))
  :group 'gud
  :version "23.2")

(def-gdb2-trigger-and-handler
  gdb2-invalidate-memory
  (format "-data-read-memory %s %s %d %d %d"
          (gdb2-mi-quote gdb2-memory-address-expression)
          gdb2-memory-format
          gdb2-memory-unit
          gdb2-memory-rows
          gdb2-memory-columns)
  gdb2-read-memory-handler
  gdb2-read-memory-custom
  '(start update))

(gdb2-set-buffer-rules
 'gdb2-memory-buffer
 'gdb2-memory-buffer-name
 'gdb2-memory-mode
 'gdb2-invalidate-memory)

(defun gdb2-memory-column-width (size format)
  "Return length of string with memory unit of SIZE in FORMAT.
SIZE is in bytes, as in `gdb2-memory-unit'.  FORMAT is a string as
in `gdb2-memory-format'."
  (let ((format-base (cdr (assoc format
                                 '(("x" . 16)
                                   ("d" . 10) ("u" . 10)
                                   ("o" . 8)
                                   ("t" . 2))))))
    (if format-base
        (let ((res (ceiling (log (expt 2.0 (* size 8)) format-base))))
          (cond ((string-equal format "x")
                 (+ 2 res)) ; hexadecimal numbers have 0x in front
                ((or (string-equal format "d")
                     (string-equal format "o"))
                 (1+ res))
                (t res)))
      (error "Unknown format"))))

(defun gdb2-read-memory-custom ()
  (let* ((res (gdb2-json-partial-output))
         (err-msg (bindat-get-field res 'msg)))
    (if (not err-msg)
        (let ((memory (bindat-get-field res 'memory)))
          (when gdb2-memory-last-address
            ;; Nil means last retrieve emits error or just started the session.
            (setq gdb2--memory-display-warning nil))
          (setq gdb2-memory-address (bindat-get-field res 'addr))
          (setq gdb2-memory-next-page (bindat-get-field res 'next-page))
          (setq gdb2-memory-prev-page (bindat-get-field res 'prev-page))
          (setq gdb2-memory-last-address gdb2-memory-address)
          (dolist (row memory)
            (insert (concat (bindat-get-field row 'addr) ":"))
            (dolist (column (bindat-get-field row 'data))
              (insert (gdb2-pad-string column
                                      (+ 2 (gdb2-memory-column-width
                                            gdb2-memory-unit
                                            gdb2-memory-format)))))
            (newline)))
      ;; Show last page instead of empty buffer when out of bounds
      (when gdb2-memory-last-address
        (let ((gdb2-memory-address-expression gdb2-memory-last-address))
          ;; If we don't set `gdb2-memory-last-address' to nil,
          ;; `gdb2-invalidate-memory' eventually calls
          ;; `gdb2-read-memory-custom', making an infinite loop.
          (setq gdb2-memory-last-address nil
                gdb2--memory-display-warning t)
          (gdb2-invalidate-memory 'update)
          (user-error "Error when retrieving memory: %s Displaying last successful page" err-msg))))))

(defvar gdb2-memory-mode-map
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map t)
    (define-key map "q" 'kill-current-buffer)
    (define-key map "n" 'gdb2-memory-show-next-page)
    (define-key map "p" 'gdb2-memory-show-previous-page)
    (define-key map "a" 'gdb2-memory-set-address)
    (define-key map "t" 'gdb2-memory-format-binary)
    (define-key map "o" 'gdb2-memory-format-octal)
    (define-key map "u" 'gdb2-memory-format-unsigned)
    (define-key map "d" 'gdb2-memory-format-signed)
    (define-key map "x" 'gdb2-memory-format-hexadecimal)
    (define-key map "b" 'gdb2-memory-unit-byte)
    (define-key map "h" 'gdb2-memory-unit-halfword)
    (define-key map "w" 'gdb2-memory-unit-word)
    (define-key map "g" 'gdb2-memory-unit-giant)
    (define-key map "R" 'gdb2-memory-set-rows)
    (define-key map "C" 'gdb2-memory-set-columns)
    map))

(defun gdb2-memory-set-address-event (event)
  "Handle a click on address field in memory buffer header."
  (interactive "e")
  (save-selected-window
    (select-window (posn-window (event-start event)))
    (gdb2-memory-set-address)))

;; Non-event version for use within keymap
(defun gdb2-memory-set-address ()
  "Set the start memory address."
  (interactive)
  (let ((arg (read-from-minibuffer "Memory address: ")))
    (setq gdb2-memory-address-expression arg))
  (gdb2-invalidate-memory 'update))

(defmacro def-gdb2-set-positive-number (name variable echo-string &optional doc)
  "Define a function NAME which reads new VAR value from minibuffer."
  `(defun ,name (event)
     ,(when doc doc)
     (interactive "e")
     (save-selected-window
       (select-window (posn-window (event-start event)))
       (let* ((arg (read-from-minibuffer ,echo-string))
              (count (string-to-number arg)))
         (if (<= count 0)
             (error "Positive number only")
           (customize-set-variable ',variable count)
           (gdb2-invalidate-memory 'update))))))

(def-gdb2-set-positive-number
  gdb2-memory-set-rows
  gdb2-memory-rows
  "Rows: "
  "Set the number of data rows in memory window.")

(def-gdb2-set-positive-number
  gdb2-memory-set-columns
  gdb2-memory-columns
  "Columns: "
  "Set the number of data columns in memory window.")

(defmacro def-gdb2-memory-format (name format doc)
  "Define a function NAME to switch memory buffer to use FORMAT.
DOC is an optional documentation string."
  `(defun ,name () ,(when doc doc)
     (interactive)
     (customize-set-variable 'gdb2-memory-format ,format)
     (gdb2-invalidate-memory 'update)))

(def-gdb2-memory-format
  gdb2-memory-format-binary "t"
  "Set the display format to binary.")

(def-gdb2-memory-format
  gdb2-memory-format-octal "o"
  "Set the display format to octal.")

(def-gdb2-memory-format
  gdb2-memory-format-unsigned "u"
  "Set the display format to unsigned decimal.")

(def-gdb2-memory-format
  gdb2-memory-format-signed "d"
  "Set the display format to decimal.")

(def-gdb2-memory-format
  gdb2-memory-format-hexadecimal "x"
  "Set the display format to hexadecimal.")

(defvar gdb2-memory-format-map
  (let ((map (make-sparse-keymap)))
    (define-key map [header-line down-mouse-3] 'gdb2-memory-format-menu-1)
    map)
  "Keymap to select format in the header line.")

(defvar gdb2-memory-format-menu
  (let ((map (make-sparse-keymap "Format")))

    (define-key map [binary]
      '(menu-item "Binary" gdb2-memory-format-binary
        :button (:radio . (equal gdb2-memory-format "t"))))
    (define-key map [octal]
      '(menu-item "Octal" gdb2-memory-format-octal
        :button (:radio . (equal gdb2-memory-format "o"))))
    (define-key map [unsigned]
      '(menu-item "Unsigned Decimal" gdb2-memory-format-unsigned
        :button (:radio . (equal gdb2-memory-format "u"))))
    (define-key map [signed]
      '(menu-item "Signed Decimal" gdb2-memory-format-signed
        :button (:radio . (equal gdb2-memory-format "d"))))
    (define-key map [hexadecimal]
      '(menu-item "Hexadecimal" gdb2-memory-format-hexadecimal
        :button (:radio . (equal gdb2-memory-format "x"))))
    map)
  "Menu of display formats in the header line.")

(defun gdb2-memory-format-menu (event)
  (interactive "@e")
  (x-popup-menu event gdb2-memory-format-menu))

(defun gdb2-memory-format-menu-1 (event)
  (interactive "e")
  (save-selected-window
    (select-window (posn-window (event-start event)))
    (let* ((selection (gdb2-memory-format-menu event))
	   (binding (and selection (lookup-key gdb2-memory-format-menu
					       (vector (car selection))))))
      (if binding (call-interactively binding)))))

(defmacro def-gdb2-memory-unit (name unit-size doc)
  "Define a function NAME to switch memory unit size to UNIT-SIZE.
DOC is an optional documentation string."
  `(defun ,name () ,(when doc doc)
     (interactive)
     (customize-set-variable 'gdb2-memory-unit ,unit-size)
     (gdb2-invalidate-memory 'update)))

(def-gdb2-memory-unit gdb2-memory-unit-giant 8
  "Set the unit size to giant words (eight bytes).")

(def-gdb2-memory-unit gdb2-memory-unit-word 4
  "Set the unit size to words (four bytes).")

(def-gdb2-memory-unit gdb2-memory-unit-halfword 2
  "Set the unit size to halfwords (two bytes).")

(def-gdb2-memory-unit gdb2-memory-unit-byte 1
  "Set the unit size to bytes.")

(defmacro def-gdb2-memory-show-page (name address-var &optional doc)
  "Define a function NAME which show new address in memory buffer.
The defined function switches Memory buffer to show address
stored in ADDRESS-VAR variable.
DOC is an optional documentation string."
  `(defun ,name
     ,(when doc doc)
     (interactive)
     (let ((gdb2-memory-address ,address-var))
       (gdb2-invalidate-memory))))

(def-gdb2-memory-show-page gdb2-memory-show-previous-page
  gdb2-memory-prev-page)

(def-gdb2-memory-show-page gdb2-memory-show-next-page
  gdb2-memory-next-page)

(defvar gdb2-memory-unit-map
  (let ((map (make-sparse-keymap)))
    (define-key map [header-line down-mouse-3] 'gdb2-memory-unit-menu-1)
    map)
  "Keymap to select units in the header line.")

(defvar gdb2-memory-unit-menu
  (let ((map (make-sparse-keymap "Unit")))
    (define-key map [giantwords]
      '(menu-item "Giant words" gdb2-memory-unit-giant
        :button (:radio . (equal gdb2-memory-unit 8))))
    (define-key map [words]
      '(menu-item "Words" gdb2-memory-unit-word
        :button (:radio . (equal gdb2-memory-unit 4))))
    (define-key map [halfwords]
      '(menu-item "Halfwords" gdb2-memory-unit-halfword
        :button (:radio . (equal gdb2-memory-unit 2))))
    (define-key map [bytes]
      '(menu-item "Bytes" gdb2-memory-unit-byte
        :button (:radio . (equal gdb2-memory-unit 1))))
    map)
  "Menu of units in the header line.")

(defun gdb2-memory-unit-menu (event)
  (interactive "@e")
  (x-popup-menu event gdb2-memory-unit-menu))

(defun gdb2-memory-unit-menu-1 (event)
  (interactive "e")
  (save-selected-window
    (select-window (posn-window (event-start event)))
    (let* ((selection (gdb2-memory-unit-menu event))
	   (binding (and selection (lookup-key gdb2-memory-unit-menu
					       (vector (car selection))))))
      (if binding (call-interactively binding)))))

(defvar gdb2-memory-font-lock-keywords
  '(;; <__function.name+n>
    ("<\\(\\(\\sw\\|[_.]\\)+\\)\\(\\+[0-9]+\\)?>"
     (1 font-lock-function-name-face)))
  "Font lock keywords used in `gdb2-memory-mode'.")

(defvar gdb2-memory-header
  '(:eval
    (concat
     "Start address "
     ;; If `gdb2-memory-address-expression' is nil, `propertize' would error.
     (propertize (or gdb2-memory-address-expression "N/A")
                 'face font-lock-warning-face
                 'help-echo "mouse-1: set start address"
                 'mouse-face 'mode-line-highlight
                 'local-map (gdb2-make-header-line-mouse-map
                             'mouse-1
                             #'gdb2-memory-set-address-event))
     (if gdb2--memory-display-warning
         (propertize " !" 'face '(:inherit error :weight bold))
       "")
     " ["
     (propertize "-"
                 'face font-lock-warning-face
                 'help-echo "mouse-1: decrement address"
                 'mouse-face 'mode-line-highlight
                 'local-map (gdb2-make-header-line-mouse-map
                             'mouse-1
                             #'gdb2-memory-show-previous-page))
     "|"
     (propertize "+"
                 'face font-lock-warning-face
                 'help-echo "mouse-1: increment address"
                 'mouse-face 'mode-line-highlight
                 'local-map (gdb2-make-header-line-mouse-map
                             'mouse-1
                             #'gdb2-memory-show-next-page))
     "]: "
     ;; If `gdb2-memory-address' is nil, `propertize' would error.
     (propertize (or gdb2-memory-address "N/A")
                 'face font-lock-warning-face)
     "  Rows: "
     (propertize (number-to-string gdb2-memory-rows)
                 'face font-lock-warning-face
                 'help-echo "mouse-1: set number of columns"
                 'mouse-face 'mode-line-highlight
                 'local-map (gdb2-make-header-line-mouse-map
                             'mouse-1
                             #'gdb2-memory-set-rows))
     "  Columns: "
     (propertize (number-to-string gdb2-memory-columns)
                 'face font-lock-warning-face
                 'help-echo "mouse-1: set number of columns"
                 'mouse-face 'mode-line-highlight
                 'local-map (gdb2-make-header-line-mouse-map
                             'mouse-1
                             #'gdb2-memory-set-columns))
     "  Display Format: "
     (propertize gdb2-memory-format
                 'face font-lock-warning-face
                 'help-echo "mouse-3: select display format"
                 'mouse-face 'mode-line-highlight
                 'local-map gdb2-memory-format-map)
     "  Unit Size: "
     (propertize (number-to-string gdb2-memory-unit)
                 'face font-lock-warning-face
                 'help-echo "mouse-3: select unit size"
                 'mouse-face 'mode-line-highlight
                 'local-map gdb2-memory-unit-map)))
  "Header line used in `gdb2-memory-mode'.")

(define-derived-mode gdb2-memory-mode gdb2-parent-mode "Memory"
  "Major mode for examining memory."
  (setq header-line-format gdb2-memory-header)
  (set (make-local-variable 'font-lock-defaults)
       '(gdb2-memory-font-lock-keywords))
  'gdb2-invalidate-memory)

(defun gdb2-memory-buffer-name ()
  (concat "*memory of " (gdb2-get-target-string) "*"))

(defun gdb2-display-memory-buffer (&optional thread)
  "Display GDB2 memory contents."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-memory-buffer thread)))

(defun gdb2-frame-memory-buffer ()
  "Display memory contents in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-memory-buffer)
		  gdb2-display-buffer-other-frame-action))


;;; Disassembly view

(defun gdb2-disassembly-buffer-name ()
  (gdb2-current-context-buffer-name
   (concat "disassembly of " (gdb2-get-target-string))))

(defun gdb2-display-disassembly-buffer (&optional thread)
  "Display GDB2 disassembly information."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-disassembly-buffer thread)))

(def-gdb2-preempt-display-buffer
  gdb2-preemptively-display-disassembly-buffer
  'gdb2-disassembly-buffer)

(defun gdb2-frame-disassembly-buffer (&optional thread)
  "Display GDB2 disassembly information in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-disassembly-buffer thread)
		  gdb2-display-buffer-other-frame-action))

(def-gdb2-auto-update-trigger gdb2-invalidate-disassembly
  (let* ((frame (gdb2-current-buffer-frame))
         (file (bindat-get-field frame 'fullname))
         (line (bindat-get-field frame 'line)))
    (if file
      (format "-data-disassemble -f %s -l %s -n -1 -- 0" file line)
    ;; If we're unable to get a file name / line for $PC, simply
    ;; follow $PC, disassembling the next 10 (x ~15 (on IA) ==
    ;; 150 bytes) instructions.
    "-data-disassemble -s $pc -e \"$pc + 150\" -- 0"))
  gdb2-disassembly-handler
  ;; We update disassembly only after we have actual frame information
  ;; about all threads, so no there's `update' signal in this list
  '(start update-disassembly))

(def-gdb2-auto-update-handler
  gdb2-disassembly-handler
  gdb2-disassembly-handler-custom
  t)

(gdb2-set-buffer-rules
 'gdb2-disassembly-buffer
 'gdb2-disassembly-buffer-name
 'gdb2-disassembly-mode
 'gdb2-invalidate-disassembly)

(defvar gdb2-disassembly-font-lock-keywords
  '(;; <__function.name+n>
    ("<\\(\\(\\sw\\|[_.]\\)+\\)\\(\\+[0-9]+\\)?>"
     (1 font-lock-function-name-face))
    ;; 0xNNNNNNNN <__function.name+n>: opcode
    ("^0x[0-9a-f]+ \\(<\\(\\(\\sw\\|[_.]\\)+\\)\\+[0-9]+>\\)?:[ \t]+\\(\\sw+\\)"
     (4 font-lock-keyword-face))
    ;; %register(at least i386)
    ("%\\sw+" . font-lock-variable-name-face)
    ("^\\(Dump of assembler code for function\\) \\(.+\\):"
     (1 font-lock-comment-face)
     (2 font-lock-function-name-face))
    ("^\\(End of assembler dump\\.\\)" . font-lock-comment-face))
  "Font lock keywords used in `gdb2-disassembly-mode'.")

(defvar gdb2-disassembly-mode-map
  ;; TODO
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map)
    (define-key map "q" 'kill-current-buffer)
    map))

(define-derived-mode gdb2-disassembly-mode gdb2-parent-mode "Disassembly"
  "Major mode for GDB2 disassembly information."
  ;; TODO Rename overlay variable for disassembly mode
  (add-to-list 'overlay-arrow-variable-list 'gdb2-disassembly-position)
  (setq fringes-outside-margins t)
  (set (make-local-variable 'gdb2-disassembly-position) (make-marker))
  (set (make-local-variable 'font-lock-defaults)
       '(gdb2-disassembly-font-lock-keywords))
  'gdb2-invalidate-disassembly)

(defun gdb2-disassembly-handler-custom ()
  (let* ((instructions (bindat-get-field (gdb2-json-partial-output) 'asm_insns))
         (address (bindat-get-field (gdb2-current-buffer-frame) 'addr))
         (table (make-gdb2-table))
         (marked-line nil))
    (dolist (instr instructions)
      (gdb2-table-add-row table
                         (list
                          (bindat-get-field instr 'address)
                          (let
                              ((func-name (bindat-get-field instr 'func-name))
                               (offset (bindat-get-field instr 'offset)))
                            (if func-name
                                (format "<%s+%s>:" func-name offset)
                              ""))
                          (bindat-get-field instr 'inst)))
      (when (string-equal (bindat-get-field instr 'address)
                          address)
        (progn
          (setq marked-line (length (gdb2-table-rows table)))
          (setq fringe-indicator-alist
                (if (string-equal gdb2-frame-number "0")
                    nil
                  '((overlay-arrow . hollow-right-triangle)))))))
    (insert (gdb2-table-string table " "))
    (gdb2-disassembly-place-breakpoints)
    ;; Mark current position with overlay arrow and scroll window to
    ;; that point
    (when marked-line
      (let ((window (get-buffer-window (current-buffer) 0)))
        (set-window-point window (gdb2-mark-line marked-line
                                                gdb2-disassembly-position))))
    (setq mode-name
          (gdb2-current-context-mode-name
           (concat "Disassembly: "
                   (bindat-get-field (gdb2-current-buffer-frame) 'func))))))

(defun gdb2-disassembly-place-breakpoints ()
  (gdb2-remove-breakpoint-icons (point-min) (point-max))
  (dolist (breakpoint gdb2-breakpoints-list)
    (let* ((breakpoint (cdr breakpoint))
           (bptno (bindat-get-field breakpoint 'number))
           (flag (bindat-get-field breakpoint 'enabled))
           (address (bindat-get-field breakpoint 'addr)))
      (save-excursion
        (goto-char (point-min))
        (if (re-search-forward (concat "^" address) nil t)
            (gdb2-put-breakpoint-icon (string-equal flag "y") bptno))))))


(defvar gdb2-breakpoints-header
  (list
   (gdb2-propertize-header "Breakpoints" gdb2-breakpoints-buffer
			  nil nil mode-line)
   " "
   (gdb2-propertize-header "Threads" gdb2-threads-buffer
			  "mouse-1: select" mode-line-highlight
                          mode-line-inactive)))

;;; Breakpoints view
(define-derived-mode gdb2-breakpoints-mode gdb2-parent-mode "Breakpoints"
  "Major mode for gdb2 breakpoints."
  (setq header-line-format gdb2-breakpoints-header)
  'gdb2-invalidate-breakpoints)

(defun gdb2-toggle-breakpoint ()
  "Enable/disable breakpoint at current line of breakpoints buffer."
  (interactive)
  (save-excursion
    (beginning-of-line)
    (let ((breakpoint (get-text-property (point) 'gdb2-breakpoint)))
      (if breakpoint
          (gud-basic-call
           (concat (if (equal "y" (bindat-get-field breakpoint 'enabled))
                       "-break-disable "
                     "-break-enable ")
                   (bindat-get-field breakpoint 'number)))
        (error "Not recognized as break/watchpoint line")))))

(defun gdb2-delete-breakpoint ()
  "Delete the breakpoint at current line of breakpoints buffer."
  (interactive)
  (save-excursion
    (beginning-of-line)
    (let ((breakpoint (get-text-property (point) 'gdb2-breakpoint)))
      (if breakpoint
          (gud-basic-call (concat "-break-delete "
                                  (bindat-get-field breakpoint 'number)))
        (error "Not recognized as break/watchpoint line")))))

(defun gdb2-goto-breakpoint (&optional event)
  "Go to the location of breakpoint at current line of breakpoints buffer."
  (interactive (list last-input-event))
  (if event (posn-set-point (event-end event)))
  ;; Hack to stop gdb2-goto-breakpoint displaying in GUD buffer.
  (let ((window (get-buffer-window gud-comint-buffer)))
    (if window (save-selected-window  (select-window window))))
  (save-excursion
    (beginning-of-line)
    (let ((breakpoint (get-text-property (point) 'gdb2-breakpoint)))
      (if breakpoint
          (let ((bptno (bindat-get-field breakpoint 'number))
                (file  (bindat-get-field breakpoint 'fullname))
                (line  (bindat-get-field breakpoint 'line)))
            (save-selected-window
              (let* ((buffer (find-file-noselect
                              (if (file-exists-p file) file
                                (cdr (assoc bptno gdb2-location-alist)))))
                     (window (gdb2-display-source-buffer buffer)))
                (with-current-buffer buffer
                  (goto-char (point-min))
                  (forward-line (1- (string-to-number line)))
                  (set-window-point window (point))))))
        (error "Not recognized as break/watchpoint line")))))


;; Frames buffer.  This displays a perpetually correct backtrack trace.
;;
(def-gdb2-trigger-and-handler
  gdb2-invalidate-frames (gdb2-current-context-command "-stack-list-frames")
  gdb2-stack-list-frames-handler gdb2-stack-list-frames-custom
  '(start update))

(gdb2-set-buffer-rules
 'gdb2-stack-buffer
 'gdb2-stack-buffer-name
 'gdb2-frames-mode
 'gdb2-invalidate-frames)

(defun gdb2-frame-location (frame)
  "Return \" of file:line\" or \" of library\" for structure FRAME.
FRAME must have either \"file\" and \"line\" members or \"from\"
member."
  (let ((file (bindat-get-field frame 'file))
        (line (bindat-get-field frame 'line))
        (from (bindat-get-field frame 'from)))
    (let ((res (or (and file line (concat file ":" line))
                   from)))
      (if res (concat " of " res) ""))))

(defun gdb2-stack-list-frames-custom ()
  (let ((stack (bindat-get-field (gdb2-json-partial-output "frame") 'stack))
        (table (make-gdb2-table)))
    (set-marker gdb2-stack-position nil)
    (dolist (frame stack)
      (gdb2-table-add-row table
                         (list
                          (bindat-get-field frame 'level)
                          "in"
                          (concat
                           (bindat-get-field frame 'func)
                           (if gdb2-stack-buffer-locations
                               (gdb2-frame-location frame) "")
                           (if gdb2-stack-buffer-addresses
                               (concat " at " (bindat-get-field frame 'addr)) "")))
                         `(mouse-face highlight
                                      help-echo "mouse-2, RET: Select frame"
                                      gdb2-frame ,frame)))
    (insert (gdb2-table-string table " ")))
  (when (and gdb2-frame-number
             (gdb2-buffer-shows-main-thread-p))
    (gdb2-mark-line (1+ (string-to-number gdb2-frame-number))
                   gdb2-stack-position))
  (setq mode-name
        (gdb2-current-context-mode-name "Frames")))

(defun gdb2-stack-buffer-name ()
  (gdb2-current-context-buffer-name
   (concat "stack frames of " (gdb2-get-target-string))))

(defun gdb2-display-stack-buffer (&optional thread)
  "Display GDB2 backtrace for current stack."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-stack-buffer thread)))

(def-gdb2-preempt-display-buffer
  gdb2-preemptively-display-stack-buffer
  'gdb2-stack-buffer nil t)

(defun gdb2-frame-stack-buffer (&optional thread)
  "Display GDB2 backtrace for current stack in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-stack-buffer thread)
		  gdb2-display-buffer-other-frame-action))

(defvar gdb2-frames-mode-map
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map)
    (define-key map "q" 'kill-current-buffer)
    (define-key map "\r" 'gdb2-select-frame)
    (define-key map [mouse-2] 'gdb2-select-frame)
    (define-key map [follow-link] 'mouse-face)
    map))

(defvar gdb2-frames-font-lock-keywords
  '(("in \\([^ ]+\\)"  (1 font-lock-function-name-face)))
  "Font lock keywords used in `gdb2-frames-mode'.")

(define-derived-mode gdb2-frames-mode gdb2-parent-mode "Frames"
  "Major mode for gdb2 call stack."
  (setq gdb2-stack-position (make-marker))
  (add-to-list 'overlay-arrow-variable-list 'gdb2-stack-position)
  (setq truncate-lines t)  ;; Make it easier to see overlay arrow.
  (set (make-local-variable 'font-lock-defaults)
       '(gdb2-frames-font-lock-keywords))
  'gdb2-invalidate-frames)

(defun gdb2-select-frame (&optional event)
  "Select the frame and display the relevant source."
  (interactive (list last-input-event))
  (if event (posn-set-point (event-end event)))
  (let ((frame (get-text-property (point) 'gdb2-frame)))
    (if frame
        (if (gdb2-buffer-shows-main-thread-p)
            (let ((new-level (bindat-get-field frame 'level)))
              (setq gdb2-frame-number new-level)
              (gdb2-input (concat "-stack-select-frame " new-level)
			 'ignore)
              (gdb2-update))
          (error "Could not select frame for non-current thread"))
      (error "Not recognized as frame line"))))


;; Locals buffer.
;; uses "-stack-list-locals --simple-values". Needs GDB2 6.1 onwards.
(def-gdb2-trigger-and-handler
  gdb2-invalidate-locals
  (concat (gdb2-current-context-command "-stack-list-locals")
          " --simple-values")
  gdb2-locals-handler gdb2-locals-handler-custom
  '(start update))

(gdb2-set-buffer-rules
 'gdb2-locals-buffer
 'gdb2-locals-buffer-name
 'gdb2-locals-mode
 'gdb2-invalidate-locals)

(defvar gdb2-locals-watch-map
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map)
    (define-key map "\r" 'gud-watch)
    (define-key map [mouse-2] 'gud-watch)
    map)
  "Keymap to create watch expression of a complex data type local variable.")

(defvar gdb2-edit-locals-map-1
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map)
    (define-key map "\r" 'gdb2-edit-locals-value)
    (define-key map [mouse-2] 'gdb2-edit-locals-value)
    map)
  "Keymap to edit value of a simple data type local variable.")

(defun gdb2-edit-locals-value (&optional event)
  "Assign a value to a variable displayed in the locals buffer."
  (interactive (list last-input-event))
  (save-excursion
    (if event (posn-set-point (event-end event)))
    (beginning-of-line)
    (let* ((var (bindat-get-field
                 (get-text-property (point) 'gdb2-local-variable) 'name))
	   (value (read-string (format "New value (%s): " var))))
      (gud-basic-call
       (concat  "-gdb-set variable " var " = " value)))))

;; Don't display values of arrays or structures.
;; These can be expanded using gud-watch.
(defun gdb2-locals-handler-custom ()
  (let ((locals-list (bindat-get-field (gdb2-json-partial-output) 'locals))
        (table (make-gdb2-table)))
    (dolist (local locals-list)
      (let ((name (bindat-get-field local 'name))
            (value (bindat-get-field local 'value))
            (type (bindat-get-field local 'type)))
        (when (not value)
          (setq value "<complex data type>"))
        (if (or (not value)
                (string-match "0x" value))
            (add-text-properties 0 (length name)
                                 `(mouse-face highlight
                                              help-echo "mouse-2: create watch expression"
                                              local-map ,gdb2-locals-watch-map)
                                 name)
          (add-text-properties 0 (length value)
                               `(mouse-face highlight
                                            help-echo "mouse-2: edit value"
                                            local-map ,gdb2-edit-locals-map-1)
                               value))
        (gdb2-table-add-row
         table
         (list
          (propertize type 'font-lock-face font-lock-type-face)
          (propertize name 'font-lock-face font-lock-variable-name-face)
          value)
         `(gdb2-local-variable ,local))))
    (insert (gdb2-table-string table " "))
    (setq mode-name
          (gdb2-current-context-mode-name
           (concat "Locals: "
                   (bindat-get-field (gdb2-current-buffer-frame) 'func))))))

(defvar gdb2-locals-header
  (list
   (gdb2-propertize-header "Locals" gdb2-locals-buffer
			  nil nil mode-line)
   " "
   (gdb2-propertize-header "Registers" gdb2-registers-buffer
			  "mouse-1: select" mode-line-highlight
                          mode-line-inactive)))

(defvar gdb2-locals-mode-map
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map)
    (define-key map "q" 'kill-current-buffer)
    (define-key map "\t" (lambda ()
                           (interactive)
                           (gdb2-set-window-buffer
                            (gdb2-get-buffer-create
                             'gdb2-registers-buffer
                             gdb2-thread-number) t)))
    map))

(define-derived-mode gdb2-locals-mode gdb2-parent-mode "Locals"
  "Major mode for gdb2 locals."
  (setq header-line-format gdb2-locals-header)
  'gdb2-invalidate-locals)

(defun gdb2-locals-buffer-name ()
  (gdb2-current-context-buffer-name
   (concat "locals of " (gdb2-get-target-string))))

(defun gdb2-display-locals-buffer (&optional thread)
  "Display the local variables of current GDB2 stack."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-locals-buffer thread)))

(def-gdb2-preempt-display-buffer
  gdb2-preemptively-display-locals-buffer
  'gdb2-locals-buffer nil t)

(defun gdb2-frame-locals-buffer (&optional thread)
  "Display the local variables of the current GDB2 stack in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-locals-buffer thread)
		  gdb2-display-buffer-other-frame-action))


;; Registers buffer.

(def-gdb2-trigger-and-handler
  gdb2-invalidate-registers
  (concat (gdb2-current-context-command "-data-list-register-values") " x")
  gdb2-registers-handler
  gdb2-registers-handler-custom
  '(start update))

(gdb2-set-buffer-rules
 'gdb2-registers-buffer
 'gdb2-registers-buffer-name
 'gdb2-registers-mode
 'gdb2-invalidate-registers)

(defun gdb2-registers-handler-custom ()
  (when gdb2-register-names
    (let ((register-values
           (bindat-get-field (gdb2-json-partial-output) 'register-values))
          (table (make-gdb2-table)))
      (dolist (register register-values)
        (let* ((register-number (bindat-get-field register 'number))
               (value (bindat-get-field register 'value))
               (register-name (nth (string-to-number register-number)
                                   gdb2-register-names)))
          (gdb2-table-add-row
           table
           (list
            (propertize register-name
                        'font-lock-face font-lock-variable-name-face)
            (if (member register-number gdb2-changed-registers)
                (propertize value 'font-lock-face font-lock-warning-face)
              value))
           `(mouse-face highlight
                        help-echo "mouse-2: edit value"
                        gdb2-register-name ,register-name))))
      (insert (gdb2-table-string table " ")))
    (setq mode-name
          (gdb2-current-context-mode-name "Registers"))))

(defun gdb2-edit-register-value (&optional event)
  "Assign a value to a register displayed in the registers buffer."
  (interactive (list last-input-event))
  (save-excursion
    (if event (posn-set-point (event-end event)))
    (beginning-of-line)
    (let* ((var (get-text-property (point) 'gdb2-register-name))
	   (value (read-string (format "New value (%s): " var))))
      (gud-basic-call
       (concat  "-gdb-set variable $" var " = " value)))))

(defvar gdb2-registers-mode-map
  (let ((map (make-sparse-keymap)))
    (suppress-keymap map)
    (define-key map "\r" 'gdb2-edit-register-value)
    (define-key map [mouse-2] 'gdb2-edit-register-value)
    (define-key map "q" 'kill-current-buffer)
    (define-key map "\t" (lambda ()
                           (interactive)
                           (gdb2-set-window-buffer
                            (gdb2-get-buffer-create
                             'gdb2-locals-buffer
                             gdb2-thread-number) t)))
    map))

(defvar gdb2-registers-header
  (list
   (gdb2-propertize-header "Locals" gdb2-locals-buffer
			  "mouse-1: select" mode-line-highlight
                          mode-line-inactive)
   " "
   (gdb2-propertize-header "Registers" gdb2-registers-buffer
			  nil nil mode-line)))

(define-derived-mode gdb2-registers-mode gdb2-parent-mode "Registers"
  "Major mode for gdb2 registers."
  (setq header-line-format gdb2-registers-header)
  'gdb2-invalidate-registers)

(defun gdb2-registers-buffer-name ()
  (gdb2-current-context-buffer-name
   (concat "registers of " (gdb2-get-target-string))))

(defun gdb2-display-registers-buffer (&optional thread)
  "Display GDB2 register contents."
  (interactive)
  (gdb2-display-buffer (gdb2-get-buffer-create 'gdb2-registers-buffer thread)))

(def-gdb2-preempt-display-buffer
  gdb2-preemptively-display-registers-buffer
  'gdb2-registers-buffer nil t)

(defun gdb2-frame-registers-buffer (&optional thread)
  "Display GDB2 register contents in another frame."
  (interactive)
  (display-buffer (gdb2-get-buffer-create 'gdb2-registers-buffer thread)
		  gdb2-display-buffer-other-frame-action))

;; Needs GDB2 6.4 onwards (used to fail with no stack).
(defun gdb2-get-changed-registers ()
  (when (gdb2-get-buffer 'gdb2-registers-buffer)
    (gdb2-input "-data-list-changed-registers"
               'gdb2-changed-registers-handler
               'gdb2-get-changed-registers)))

(defun gdb2-changed-registers-handler ()
  (setq gdb2-changed-registers nil)
  (dolist (register-number
           (bindat-get-field (gdb2-json-partial-output) 'changed-registers))
    (push register-number gdb2-changed-registers)))

(defun gdb2-register-names-handler ()
  ;; Don't use pending triggers because this handler is called
  ;; only once (in gdb2-init-1)
  (setq gdb2-register-names nil)
  (dolist (register-name
           (bindat-get-field (gdb2-json-partial-output) 'register-names))
    (push register-name gdb2-register-names))
  (setq gdb2-register-names (reverse gdb2-register-names)))


(defun gdb2-get-source-file-list ()
  "Create list of source files for current GDB2 session.
If buffers already exist for any of these files, `gud-minor-mode'
is set in them."
  (goto-char (point-min))
  (while (re-search-forward gdb2-source-file-regexp nil t)
    (push (read (match-string 1)) gdb2-source-file-list))
  (dolist (buffer (buffer-list))
    (with-current-buffer buffer
      (when (member buffer-file-name gdb2-source-file-list)
	(gdb2-init-buffer)))))

(defun gdb2-get-main-selected-frame ()
  "Trigger for `gdb2-frame-handler' which uses main current thread.
Called from `gdb2-update'."
  (gdb2-input (gdb2-current-context-command "-stack-info-frame")
             'gdb2-frame-handler
             'gdb2-get-main-selected-frame))

(defun gdb2-frame-handler ()
  "Set `gdb2-selected-frame' and `gdb2-selected-file' to show
overlay arrow in source buffer."
  (let ((frame (bindat-get-field (gdb2-json-partial-output) 'frame)))
    (when frame
      (setq gdb2-selected-frame (bindat-get-field frame 'func))
      (setq gdb2-selected-file (bindat-get-field frame 'fullname))
      (setq gdb2-frame-number (bindat-get-field frame 'level))
      (setq gdb2-frame-address (bindat-get-field frame 'addr))
      (let ((line (bindat-get-field frame 'line)))
        (setq gdb2-selected-line (and line (string-to-number line)))
        (when (and gdb2-selected-file gdb2-selected-line)
          (setq gud-last-frame (cons gdb2-selected-file gdb2-selected-line))
          (gud-display-frame)))
      (if gud-overlay-arrow-position
          (let ((buffer (marker-buffer gud-overlay-arrow-position))
                (position (marker-position gud-overlay-arrow-position)))
            (when buffer
              (with-current-buffer buffer
                (setq fringe-indicator-alist
                      (if (string-equal gdb2-frame-number "0")
                          nil
                        '((overlay-arrow . hollow-right-triangle))))
                (setq gud-overlay-arrow-position (make-marker))
                (set-marker gud-overlay-arrow-position position))))))))

(defconst gdb2-prompt-name-regexp
  (concat "value=\\(" gdb2--string-regexp "\\)"))

(defun gdb2-get-prompt ()
  "Find prompt for GDB2 session."
  (goto-char (point-min))
  (setq gdb2-prompt-name nil)
  (re-search-forward gdb2-prompt-name-regexp nil t)
  (setq gdb2-prompt-name (read (match-string 1)))
  ;; Insert first prompt.
  (setq gdb2-filter-output (concat gdb2-filter-output gdb2-prompt-name)))

;;;; Window management
(defun gdb2-display-buffer (buf)
  "Show buffer BUF, and make that window dedicated."
  (let ((window (display-buffer buf)))
    (set-window-dedicated-p window t)
    window))

  ;; (let ((answer (get-buffer-window buf 0)))
  ;;   (if answer
  ;; 	(display-buffer buf nil 0) ;Deiconify frame if necessary.
  ;;     (let ((window (get-lru-window)))
  ;; 	(if (eq (buffer-local-value 'gud-minor-mode (window-buffer window))
  ;;               'gdb2mi)
  ;; 	    (let ((largest (get-largest-window)))
  ;; 	      (setq answer (split-window largest))
  ;; 	      (set-window-buffer answer buf)
  ;; 	      (set-window-dedicated-p answer t)
  ;; 	      answer)
  ;; 	  (set-window-buffer window buf)
  ;; 	  window)))))


(defun gdb2-preempt-existing-or-display-buffer (buf &optional split-horizontal)
  "Find window displaying a buffer with the same
`gdb2-buffer-type' as BUF and show BUF there.  If no such window
exists, just call `gdb2-display-buffer' for BUF.  If the window
found is already dedicated, split window according to
SPLIT-HORIZONTAL and show BUF in the new window."
  (if buf
      (when (not (get-buffer-window buf))
        (let* ((buf-type (gdb2-buffer-type buf))
               (existing-window
                (get-window-with-predicate
                 #'(lambda (w)
                     (and (eq buf-type
                              (gdb2-buffer-type (window-buffer w)))
                          (not (window-dedicated-p w)))))))
          (if existing-window
              (set-window-buffer existing-window buf)
            (let ((dedicated-window
                   (get-window-with-predicate
                    #'(lambda (w)
                        (eq buf-type
                            (gdb2-buffer-type (window-buffer w)))))))
              (if dedicated-window
                  (set-window-buffer
                   (split-window dedicated-window nil split-horizontal) buf)
                (gdb2-display-buffer buf))))))
    (error "Null buffer")))

;;; Shared keymap initialization:

(let ((menu (make-sparse-keymap "GDB2-Windows")))
  (define-key gud-menu-map [displays]
    `(menu-item "GDB2-Windows" ,menu
		:visible (eq gud-minor-mode 'gdb2mi)))
  (define-key menu [gdb2-restore-windows]
    '(menu-item "Restore Initial Layout" gdb2-restore-windows
      :help "Restore the initial GDB2 window layout."))
  ;; Window layout vs window configuration: We use "window layout" in
  ;; GDB2 UI.  Internally we refer to "window configuration" because
  ;; that's the data structure used to store window layouts.  Though
  ;; bare in mind that there is a small difference between what we
  ;; store and what normal window configuration functions
  ;; output. Because GDB2 buffers (source, local, breakpoint, etc) are
  ;; different between each debugging sessions, simply save/load
  ;; window configurations doesn't
  ;; work. `gdb2-save-window-configuration' and
  ;; `gdb2-load-window-configuration' do some tricks to store and
  ;; recreate each buffer in the layout.
  (define-key menu [load-layout] '("Load Layout" "Load GDB2 window configuration (layout) from a file" . gdb2-load-window-configuration))
  (define-key menu [save-layout] '("Save Layout" "Save current GDB2 window configuration (layout) to a file" . gdb2-save-window-configuration))
  (define-key menu [restore-layout-after-quit]
    '(menu-item "Restore Layout After Quit" gdb2-toggle-restore-window-configuration
       :button (:toggle . gdb2-restore-window-configuration-after-quit)
       :help "Toggle between always restore the window configuration (layout) after GDB2 quits and never restore.\n You can also change this setting in Customize to conditionally restore."))
  (define-key menu [gdb2] '("Gdb2" . gdb2-display-gdb2-buffer))
  (define-key menu [threads] '("Threads" . gdb2-display-threads-buffer))
  (define-key menu [memory] '("Memory" . gdb2-display-memory-buffer))
  (define-key menu [disassembly]
    '("Disassembly" . gdb2-display-disassembly-buffer))
  (define-key menu [registers] '("Registers" . gdb2-display-registers-buffer))
  (define-key menu [inferior]
    '("IO" . gdb2-display-io-buffer))
  (define-key menu [locals] '("Locals" . gdb2-display-locals-buffer))
  (define-key menu [frames] '("Stack" . gdb2-display-stack-buffer))
  (define-key menu [breakpoints]
    '("Breakpoints" . gdb2-display-breakpoints-buffer)))

(let ((menu (make-sparse-keymap "GDB2-Frames")))
  (define-key gud-menu-map [frames]
    `(menu-item "GDB2-Frames" ,menu
		:visible (eq gud-minor-mode 'gdb2mi)))
  (define-key menu [gdb2] '("Gdb2" . gdb2-frame-gdb2-buffer))
  (define-key menu [threads] '("Threads" . gdb2-frame-threads-buffer))
  (define-key menu [memory] '("Memory" . gdb2-frame-memory-buffer))
  (define-key menu [disassembly]
    '("Disassembly" . gdb2-frame-disassembly-buffer))
  (define-key menu [registers] '("Registers" . gdb2-frame-registers-buffer))
  (define-key menu [inferior]
    '("IO" . gdb2-frame-io-buffer))
  (define-key menu [locals] '("Locals" . gdb2-frame-locals-buffer))
  (define-key menu [frames] '("Stack" . gdb2-frame-stack-buffer))
  (define-key menu [breakpoints]
    '("Breakpoints" . gdb2-frame-breakpoints-buffer)))

(let ((menu (make-sparse-keymap "GDB2-MI")))
  (define-key menu [gdb2-customize]
    '(menu-item "Customize" (lambda () (interactive) (customize-group 'gdb2))
      :help "Customize Gdb2 Graphical Mode options."))
  (define-key menu [gdb2-many-windows]
    '(menu-item "Display Other Windows" gdb2-many-windows
      :help "Toggle display of locals, stack and breakpoint information"
      :button (:toggle . gdb2-many-windows)))
  (define-key menu [sep1]
    '(menu-item "--"))
  (define-key menu [all-threads]
    '(menu-item "GUD controls all threads"
      (lambda ()
        (interactive)
        (setq gdb2-gud-control-all-threads t))
      :help "GUD start/stop commands apply to all threads"
      :button (:radio . gdb2-gud-control-all-threads)))
  (define-key menu [current-thread]
    '(menu-item "GUD controls current thread"
      (lambda ()
        (interactive)
        (setq gdb2-gud-control-all-threads nil))
      :help "GUD start/stop commands apply to current thread only"
      :button (:radio . (not gdb2-gud-control-all-threads))))
  (define-key menu [sep2]
    '(menu-item "--"))
  (define-key menu [gdb2-customize-reasons]
    '(menu-item "Customize switching..."
      (lambda ()
        (interactive)
        (customize-option 'gdb2-switch-reasons))))
  (define-key gud-menu-map [mi]
    `(menu-item "GDB2-MI" ,menu :visible (eq gud-minor-mode 'gdb2mi))))

;; TODO Fit these into tool-bar-local-item-from-menu call in gud.el.
;; GDB2-MI menu will need to be moved to gud.el. We can't use
;; tool-bar-local-item-from-menu here because it appends new buttons
;; to toolbar from right to left while we want our A/T throttle to
;; show up right before Run button.
(define-key-after gud-tool-bar-map [all-threads]
  '(menu-item "Switch to non-stop/A mode" gdb2-control-all-threads
    :image (find-image '((:type xpm :file "gud/thread.xpm")))
    :visible (and (eq gud-minor-mode 'gdb2mi)
                  gdb2-non-stop
                  (not gdb2-gud-control-all-threads)))
  'run)

(define-key-after gud-tool-bar-map [current-thread]
  '(menu-item "Switch to non-stop/T mode" gdb2-control-current-thread
    :image (find-image '((:type xpm :file "gud/all.xpm")))
    :visible (and (eq gud-minor-mode 'gdb2mi)
                  gdb2-non-stop
                  gdb2-gud-control-all-threads))
  'all-threads)

(defun gdb2-frame-gdb2-buffer ()
  "Display GUD buffer in another frame."
  (interactive)
  (display-buffer-other-frame gud-comint-buffer))

(defun gdb2-display-gdb2-buffer ()
  "Display GUD buffer."
  (interactive)
  (pop-to-buffer gud-comint-buffer nil 0))

(defun gdb2-set-window-buffer (name &optional ignore-dedicated window)
  "Set buffer of selected window to NAME and dedicate window.
When IGNORE-DEDICATED is non-nil, buffer is set even if selected
window is dedicated."
  (unless window (setq window (selected-window)))
  (when ignore-dedicated
    (set-window-dedicated-p window nil))
  (set-window-buffer window (get-buffer name))
  (set-window-dedicated-p window t))

(defun gdb2-toggle-restore-window-configuration ()
  "Toggle whether to restore window configuration when GDB2 quits."
  (interactive)
  (setq gdb2-restore-window-configuration-after-quit
        (not gdb2-restore-window-configuration-after-quit)))

(defun gdb2-get-source-buffer ()
  "Return a buffer displaying source file or nil if we can't find one.
The source file is the file that contains the source location
where GDB2 stops.  There could be multiple source files during a
debugging session, we get the most recently showed one.  If
program hasn't started running yet, the source file is the \"main
file\" where the GDB2 session starts (see `gdb2-main-file')."
  (if gud-last-last-frame
      (gud-find-file (car gud-last-last-frame))
    (when gdb2-main-file
      (gud-find-file gdb2-main-file))))

(defun gdb2-setup-windows ()
  "Lay out the window pattern for option `gdb2-many-windows'."
  (if gdb2-default-window-configuration-file
      (gdb2-load-window-configuration
       (if (file-name-absolute-p gdb2-default-window-configuration-file)
           gdb2-default-window-configuration-file
         (expand-file-name gdb2-default-window-configuration-file
                           gdb2-window-configuration-directory)))
    ;; Create default layout as before.
    (gdb2-get-buffer-create 'gdb2-locals-buffer)
    (gdb2-get-buffer-create 'gdb2-stack-buffer)
    (gdb2-get-buffer-create 'gdb2-breakpoints-buffer)
    (set-window-dedicated-p (selected-window) nil)
    (switch-to-buffer gud-comint-buffer)
    (delete-other-windows)
    (let ((win0 (selected-window))
          (win1 (split-window nil ( / ( * (window-height) 3) 4)))
          (win2 (split-window nil ( / (window-height) 3)))
          (win3 (split-window-right)))
      (gdb2-set-window-buffer (gdb2-locals-buffer-name) nil win3)
      (select-window win2)
      (set-window-buffer win2 (or (gdb2-get-source-buffer)
                                  (list-buffers-noselect)))
      (setq gdb2-source-window-list (list (selected-window)))
      (let ((win4 (split-window-right)))
        (gdb2-set-window-buffer
         (gdb2-get-buffer-create 'gdb2-inferior-io) nil win4))
      (select-window win1)
      (gdb2-set-window-buffer (gdb2-stack-buffer-name))
      (let ((win5 (split-window-right)))
        (gdb2-set-window-buffer (if gdb2-show-threads-by-default
                                   (gdb2-threads-buffer-name)
                                 (gdb2-breakpoints-buffer-name))
                               nil win5))
      (select-window win0))))

(defun gdb2-buffer-p (buffer)
  "Return t if BUFFER is GDB2-related."
  (with-current-buffer buffer
    (eq gud-minor-mode 'gdb2mi)))

(defun gdb2-function-buffer-p (buffer)
  "Return t if BUFFER is a GDB2 function buffer.
Function buffers are locals buffer, registers buffer, etc, but
not including main command buffer (the one where you type GDB2
commands) or source buffers (that display program source code)."
  (with-current-buffer buffer
    (derived-mode-p 'gdb2-parent-mode 'gdb2-inferior-io-mode)))

(defun gdb2--buffer-type (buffer)
  "Return the type of BUFFER if it is a function buffer.
Buffer type is like `gdb2-registers-type', `gdb2-stack-buffer'.
These symbols are used by `gdb2-get-buffer-create'.
Return nil if BUFFER is not a GDB2 function buffer."
  (with-current-buffer buffer
    (cl-loop for rule in gdb2-buffer-rules
             for mode-name = (gdb2-rules-buffer-mode rule)
             for type = (car rule)
             if (eq mode-name major-mode)
             return type
             finally return nil)))

(defun gdb2-save-window-configuration (file)
  "Save current window configuration (layout) to FILE.
You can later restore this configuration from that file by
`gdb2-load-window-configuration'."
  (interactive (list (read-file-name
                      "Save window configuration to file: "
                      (or gdb2-window-configuration-directory
                          default-directory))))
  ;; We replace the buffer in each window with a placeholder, store
  ;; the buffer type (register, breakpoint, etc) in window parameters,
  ;; and write the window configuration to the file.
  (save-window-excursion
    (let ((placeholder (get-buffer-create " *gdb2-placeholder*"))
          (window-persistent-parameters
           (cons '(gdb2-buffer-type . writable) window-persistent-parameters)))
      (unwind-protect
          (dolist (win (window-list nil 'no-minibuffer))
            (select-window win)
            (when (gdb2-buffer-p (current-buffer))
              (set-window-parameter
               nil 'gdb2-buffer-type
               (cond ((gdb2-function-buffer-p (current-buffer))
                      ;; 1) If a user arranged the window
                      ;; configuration herself and saves it, windows
                      ;; are probably not dedicated.  2) We use the
                      ;; same dedication flag as in
                      ;; `gdb2-display-buffer'.
                      (set-window-dedicated-p nil t)
                      ;; We save this gdb2-buffer-type symbol so
                      ;; we can later pass it to `gdb2-get-buffer-create';
                      ;; one example: `gdb2-registers-buffer'.
                      (or (gdb2--buffer-type (current-buffer))
                          (error "Unrecognized gdb2 buffer mode: %s" major-mode)))
                     ;; Command buffer.
                     ((derived-mode-p 'gud-mode) 'command)
                     ;; Consider everything else as source buffer.
                     (t 'source)))
              (with-window-non-dedicated nil
                (set-window-buffer nil placeholder)
                (set-window-prev-buffers (selected-window) nil)
                (set-window-next-buffers (selected-window) nil))))
        ;; Save the window configuration to FILE.
        (let ((window-config (window-state-get nil t)))
          (with-temp-buffer
            (prin1 window-config (current-buffer))
            (write-file file t)))
        (kill-buffer placeholder)))))

(defun gdb2-load-window-configuration (file)
  "Restore window configuration (layout) from FILE.
FILE should be a window configuration file saved by
`gdb2-save-window-configuration'."
  (interactive (list (read-file-name
                      "Restore window configuration from file: "
                      (or gdb2-window-configuration-directory
                          default-directory))))
  ;; Basically, we restore window configuration and go through each
  ;; window and restore the function buffers.
  (let* ((placeholder (get-buffer-create " *gdb2-placeholder*")))
    (unwind-protect ; Don't leak buffer.
        (let ((window-config (with-temp-buffer
                               (insert-file-contents file)
                               ;; We need to go to point-min because
                               ;; `read' reads from point
                               (goto-char (point-min))
                               (read (current-buffer))))
              (source-buffer (or (gdb2-get-source-buffer)
                                 ;; Do the same thing as in
                                 ;; `gdb2-setup-windows' if no source
                                 ;; buffer is found.
                                 (list-buffers-noselect)))
              buffer-type)
          (window-state-put window-config (frame-root-window))
          (dolist (window (window-list nil 'no-minibuffer))
            (with-selected-window window
              (setq buffer-type (window-parameter nil 'gdb2-buffer-type))
              (pcase buffer-type
                ('source (when source-buffer
                           (set-window-buffer nil source-buffer)
                           (push (selected-window) gdb2-source-window-list)))
                ('command (switch-to-buffer gud-comint-buffer))
                (_ (let ((buffer (gdb2-get-buffer-create buffer-type)))
                     (with-window-non-dedicated nil
                       (set-window-buffer nil buffer))))))))
      (kill-buffer placeholder))))

(define-minor-mode gdb2-many-windows
  "If nil just pop up the GUD buffer unless `gdb2-show-main' is t.
In this case it starts with two windows: one displaying the GUD
buffer and the other with the source file with the main routine
of the debugged program.  Non-nil means display the layout shown for
`gdb2'."
  :global t
  :group 'gdb2
  :version "22.1"
  (if (and gud-comint-buffer
           (buffer-name gud-comint-buffer))
      (ignore-errors
        (gdb2-restore-windows))))

(defun gdb2-restore-windows ()
  "Restore the basic arrangement of windows used by gdb2.
This arrangement depends on the values of variable
`gdb2-many-windows' and `gdb2-default-window-configuration-file'."
  ;; This function is used when the user messed up window
  ;; configuration and wants to "reset to default".  The function that
  ;; sets up window configuration on start up is
  ;; `gdb2-get-source-file'.
  (interactive)
  (switch-to-buffer gud-comint-buffer) ;Select the right window and frame.
  (delete-other-windows)
  (if gdb2-many-windows
      (gdb2-setup-windows)
    (when (or gud-last-last-frame gdb2-show-main)
      (let ((win (split-window)))
        (set-window-buffer
         win
         (if gud-last-last-frame
             (gud-find-file (car gud-last-last-frame))
           (gud-find-file gdb2-main-file)))
        (setq gdb2-source-window-list (list win))))))

;; Called from `gud-sentinel' in gud.el:
(defun gdb2-reset ()
  "Exit a debugging session cleanly.
Kills the gdb2 buffers, and resets variables and the source buffers."
  ;; The gdb2-inferior buffer has a pty hooked up to the main gdb2
  ;; process.  This pty must be deleted explicitly.
  (let ((pty (get-process "gdb4hpc")))
    (if pty (delete-process pty)))
  ;; Find gdb2-mi buffers and kill them.
  (dolist (buffer (buffer-list))
    (unless (eq buffer gud-comint-buffer)
      (with-current-buffer buffer
        (if (eq gud-minor-mode 'gdb2mi)
            (if (string-match "\\` ?\\*.+\\*\\'" (buffer-name))
                (kill-buffer nil)
              (gdb2-remove-breakpoint-icons (point-min) (point-max) t)
              (setq gud-minor-mode nil)
              (kill-local-variable 'tool-bar-map)
              (kill-local-variable 'gdb2-define-alist))))))
  (setq gdb2-disassembly-position nil)
  (setq overlay-arrow-variable-list
        (delq 'gdb2-disassembly-position overlay-arrow-variable-list))
  (setq fringe-indicator-alist '((overlay-arrow . right-triangle)))
  (setq gdb2-stack-position nil)
  (setq overlay-arrow-variable-list
        (delq 'gdb2-stack-position overlay-arrow-variable-list))
  (setq gdb2-thread-position nil)
  (setq overlay-arrow-variable-list
        (delq 'gdb2-thread-position overlay-arrow-variable-list))
  (if (boundp 'speedbar-frame) (speedbar-timer-fn))
  (setq gud-running nil)
  (setq gdb2-active-process nil)
  (remove-hook 'after-save-hook 'gdb2-create-define-alist t)
  ;; Recover window configuration.
  (when (or (eq gdb2-restore-window-configuration-after-quit t)
            (and (eq gdb2-restore-window-configuration-after-quit
                     'if-gdb2-show-main)
                 gdb2-show-main)
            (and (eq gdb2-restore-window-configuration-after-quit
                     'if-gdb2-many-windows)
                 gdb2-many-windows))
    (when gdb2--window-configuration-before
      (window-state-put gdb2--window-configuration-before)
      ;; This way we don't accidentally restore an outdated window
      ;; configuration.
      (setq gdb2--window-configuration-before nil))))

(defun gdb2-get-source-file ()
  "Find the source file where the program starts and display it with related
buffers, if required."
  ;; This function is called only once on startup.
  (goto-char (point-min))
  (if (re-search-forward gdb2-source-file-regexp nil t)
      (setq gdb2-main-file (read (match-string 1))))
  (if gdb2-many-windows
      (gdb2-setup-windows)
    (gdb2-get-buffer-create 'gdb2-breakpoints-buffer)
    (and gdb2-show-main
	 gdb2-main-file
	 (display-buffer (gud-find-file gdb2-main-file))))
  (gdb2-force-mode-line-update
   (propertize "ready" 'face font-lock-variable-name-face)))

;;from put-image
(defun gdb2-put-string (putstring pos &optional dprop &rest sprops)
  "Put string PUTSTRING in front of POS in the current buffer.
PUTSTRING is displayed by putting an overlay into the current buffer with a
`before-string' string that has a `display' property whose value is
PUTSTRING."
  (let ((string (make-string 1 ?x))
        (buffer (current-buffer)))
    (setq putstring (copy-sequence putstring))
    (let ((overlay (make-overlay pos pos buffer))
          (prop (or dprop
                    (list (list 'margin 'left-margin) putstring))))
      (put-text-property 0 1 'display prop string)
      (if sprops
          (add-text-properties 0 1 sprops string))
      (overlay-put overlay 'put-break t)
      (overlay-put overlay 'before-string string))))

;;from remove-images
(defun gdb2-remove-strings (start end &optional buffer)
  "Remove strings between START and END in BUFFER.
Remove only strings that were put in BUFFER with calls to `gdb2-put-string'.
BUFFER nil or omitted means use the current buffer."
  (unless buffer
    (setq buffer (current-buffer)))
  (dolist (overlay (overlays-in start end))
    (when (overlay-get overlay 'put-break)
      (delete-overlay overlay))))

(defun gdb2-put-breakpoint-icon (enabled bptno &optional line)
  (let* ((posns (gdb2-line-posns (or line (line-number-at-pos))))
         (start (- (car posns) 1))
         (end (+ (cdr posns) 1))
         (putstring (if enabled "B" "b"))
         (source-window (get-buffer-window (current-buffer) 0)))
    (add-text-properties
     0 1 '(help-echo "mouse-1: clear bkpt, mouse-3: enable/disable bkpt")
     putstring)
    (if enabled
        (add-text-properties
         0 1 `(gdb2-bptno ,bptno gdb2-enabled t) putstring)
      (add-text-properties
       0 1 `(gdb2-bptno ,bptno gdb2-enabled nil) putstring))
    (gdb2-remove-breakpoint-icons start end)
    (if (display-images-p)
        (if (>= (or left-fringe-width
                    (if source-window (car (window-fringes source-window)))
                    gdb2-buffer-fringe-width) 8)
            (gdb2-put-string
             nil (1+ start)
             `(left-fringe breakpoint
                           ,(if enabled
                                'breakpoint-enabled
                              'breakpoint-disabled))
             'gdb2-bptno bptno
             'gdb2-enabled enabled)
          (when (< left-margin-width 2)
            (save-current-buffer
              (setq left-margin-width 2)
              (if source-window
                  (set-window-margins
                   source-window
                   left-margin-width right-margin-width))))
          (put-image
           (if enabled
               (or breakpoint-enabled-icon
                   (setq breakpoint-enabled-icon
                         (find-image `((:type xpm :data
                                        ,breakpoint-xpm-data
                                        :ascent 100 :pointer hand)
                                       (:type pbm :data
                                        ,breakpoint-enabled-pbm-data
                                        :ascent 100 :pointer hand)))))
             (or breakpoint-disabled-icon
                 (setq breakpoint-disabled-icon
                       (find-image `((:type xpm :data
                                      ,breakpoint-xpm-data
                                      :conversion disabled
                                      :ascent 100 :pointer hand)
                                     (:type pbm :data
                                      ,breakpoint-disabled-pbm-data
                                      :ascent 100 :pointer hand))))))
           (+ start 1)
           putstring
           'left-margin))
      (when (< left-margin-width 2)
        (save-current-buffer
          (setq left-margin-width 2)
          (let ((window (get-buffer-window (current-buffer) 0)))
            (if window
                (set-window-margins
                 window left-margin-width right-margin-width)))))
      (gdb2-put-string
       (propertize putstring
                   'face (if enabled
                             'breakpoint-enabled 'breakpoint-disabled))
       (1+ start)))))

(defun gdb2-remove-breakpoint-icons (start end &optional remove-margin)
  (gdb2-remove-strings start end)
  (if (display-images-p)
      (remove-images start end))
  (when remove-margin
    (setq left-margin-width 0)
    (let ((window (get-buffer-window (current-buffer) 0)))
      (if window
          (set-window-margins
           window left-margin-width right-margin-width)))))


;;; Functions for inline completion.

(defvar gud-gdb2-fetch-lines-in-progress)
(defvar gud-gdb2-fetch-lines-string)
(defvar gud-gdb2-fetch-lines-break)
(defvar gud-gdb2-fetched-lines)

(defun gud-gdb2mi-completions (context command)
  "Completion table for GDB2/MI commands.
COMMAND is the prefix for which we seek completion.
CONTEXT is the text before COMMAND on the line."
  (let ((gud-gdb2-fetch-lines-in-progress t)
	(gud-gdb2-fetch-lines-string nil)
	(gud-gdb2-fetch-lines-break (length context))
	(gud-gdb2-fetched-lines nil)
	;; This filter dumps output lines to `gud-gdb2-fetched-lines'.
	(gud-marker-filter #'gud-gdb2mi-fetch-lines-filter))
    (with-current-buffer (gdb2-get-buffer 'gdb2-partial-output-buffer)
      (gdb2-input (concat "complete " context command)
		 (lambda () (setq gud-gdb2-fetch-lines-in-progress nil)))
      (while gud-gdb2-fetch-lines-in-progress
	(accept-process-output (get-buffer-process gud-comint-buffer))))
    (gud-gdb2-completions-1 gud-gdb2-fetched-lines)))

(defun gud-gdb2mi-fetch-lines-filter (string)
  "Custom filter function for `gud-gdb2mi-completions'."
  (setq string (concat gud-gdb2-fetch-lines-string
		       (gud-gdb2mi-marker-filter string)))
  (while (string-match "\n" string)
    (push (substring string gud-gdb2-fetch-lines-break (match-beginning 0))
	  gud-gdb2-fetched-lines)
    (setq string (substring string (match-end 0))))
  "")

(provide 'gdb2-mi)

;;; gdb2-mi.el ends here
