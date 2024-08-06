# help command documentation

This directory contains material for generating user-facing gdb4hpc command documentation.

It's used to generate:

- The text seen in gdb4hpc when the `help` command is used
- A command reference markdown file included in the gdb4hpc rpm
- A command reference web page included in the cpedocs website.

## `help_input.md`

All output is generated from the single `help_input.md` file. `help_input.md`
is a mostly vanilla markdown file with some quirks for input/output purposes.

### Specification

The input file has the following format:

```
# global header
...global header text...

## command header 1 `cpp_name_1`
...command help text...

## command header 2 `cpp_name_2`
...command help text...

... and so on ...
```

### Build System Substitutions

We support substituting special values known at build time into the generated documentation.
Any value in meson's generated `config.h` can be used. Surround substitutions with `@` and
they will be replaced in the output. For example:

```
gdb4hpc supports the following python versions: @SUPPORTED_PYTHON_VERSIONS_STR@
```

### Help Command Text in gdb4hpc

Each command header/text gets a corresponding entry in the generated .h/.cpp files.
The entry in the .h file will have the cpp_name in the header, which enclosed in backticks.
The entry in the .cpp file will be generated in the following way:

```
For each command:
    1. Copy the given help text verbatim
    2. Apply build system substitutions (see build system substitutions section)
    3. Trim trailing backslashes from each line (see markdown formatting notes section)
    4. Trim trailing whitespace from each line
    5. Trim empty lines from the end of the help text
```

Note that the global header is completely ignored.

### Command Reference Markdown

The command reference markdown is copied mostly verbatim from `help_input.md`,
but also has build system substitutions applied. The generated markdown file is
included in the gdb4hpc rpm.

1. Copy `help_input.md` to `help.md`
2. Apply build system substitutions (see build system substitutions section)

### Command Reference Webpage

The command reference webpage is generated from the command reference markdown. The
only difference is some html related escape sequences (escaping `<` with `&lt;`, for example).

1. Copy `help.md` to `help-web.md`
2. Escape dangerous html characters

Note that while `help-web.md` is an auto generated file, **it needs to be
committed to the git repo when updated**. This is to support cpedocs, which is
what takes the file and ultimately puts it on to a webpage. The webpage depends
on build system substitutions, which require meson. The cpedocs pipelines
neither have meson installed or desire to do a full gdb4hpc build when
generating documentation. So while committing a generated file is normally bad
practice, we compromise with `help-web.md` to make supporting cpedocs simple.

### Markdown Formatting Notes

The majority of `help_input.md` was ported from a legacy help system that
simply hard coded everything in a .cpp file. This means that the majority of
the help text was written as plaintext, not markdown. This has some subtle
implications which are described in the following sections.

See the commonmark specification for more info. https://spec.commonmark.org/

#### Line breaks

Markdown does not map every input (plaintext) `\n` to an output (html/pdf/etc)
line break. This makes sense - it allows a markdown author to write a markdown
document that is nicely formatted in plaintext as well as in its final output.
In fact, that is exactly we want to achieve with the gdb4hpc help - it's
intended to be viewed as plaintext (as the output of the gdb4hpc `help` command
in the terminal) and html (cpedocs).

<!--
If you are viewing this document in plaintext, some of the following examples
won't make a lot of sense. For best results, view in a markdown renderer (like github)
-->

Take the following blurb as an example:

```
The app_handle argument is a process set handle used to refer to this
application in future commands.  The number of ranks is taken from the
application, so app_handle should be scalar variable such as $a.
After attaching, $a{0} refers to the first rank exactly like the
launch case.
```

In the terminal, we want it to look exactly like it looks in the code block.
But in html, we want:

```
<p>
The app_handle argument is a process set handle used to refer to this
application in future commands.  The number of ranks is taken from the
application, so app_handle should be scalar variable such as $a.
After attaching, $a{0} refers to the first rank exactly like the
launch case.
</p>
```

Instead of

```
<p>The app_handle argument is a process set handle used to refer to this<br/></p>
<p>application in future commands.  The number of ranks is taken from the<br/></p>
<p>application, so app_handle should be scalar variable such as $a.<br/></p>
<p>After attaching, $a{0} refers to the first rank exactly like the<br/></p>
<p>launch case.<br/></p>
```

Luckily for us, this is exactly how all markdown renderers work.

Unfortunately, it makes it hard to express text with multiple line alignment.

For example, this looks great on the terminal:

```
Summary: Evaluate an expression and assign the result to a variable.
Usage: set variable <name> = <expression>
       set <name> = <expression>
Example commands: set program_variable_foo = program_variables_bar
                  set $client_value = program_variable
```

But in github's markdown renderer, it looks like this:

---

Summary: Evaluate an expression and assign the result to a variable.
Usage: set variable <name> = <expression>
       set <name> = <expression>
Example commands: set program_variable_foo = program_variables_bar
                  set $client_value = program_variable

---

Most markdown renderers (including the ones we use) allow the author to express line breaks
by adding a trailing backslash to the line:

---

```
Summary: Evaluate an expression and assign the result to a variable.\
Usage: set variable <name> = <expression>\
       set <name> = <expression>\
Example commands: set program_variable_foo = program_variables_bar\
                  set $client_value = program_variable
```

Summary: Evaluate an expression and assign the result to a variable.\
Usage: set variable <name> = <expression>\
       set <name> = <expression>\
Example commands: set program_variable_foo = program_variables_bar\
                  set $client_value = program_variable

---

This output, honestly, isn't *great* (leading whitespace is still gone...), but
it's not *disastrous*. It's good enough for porting the legacy help text.
Future help text should be written with this restriction in mind.

Note that you *can* express line breaks in markdown by adding an empty line
between paragraphs.

---

```
Description:\
The set variable command evaluates the given expression and assigns the result to a variable.

<name> may be a program variable or the name of a client side value. To assign to a
client side value, begin the <name> with $. <expression> may be any expression valid
in the current language.

The set command is an alias for set variable if the given argument doesn't correspond
to a debugger setting.
```

Description:\
The set variable command evaluates the given expression and assigns the result to a variable.

<name> may be a program variable or the name of a client side value. To assign to a
client side value, begin the <name> with $. <expression> may be any expression valid
in the current language.

The set command is an alias for set variable if the given argument doesn't correspond
to a debugger setting.

---

#### Asterisks

A few gdb4hpc commands use asterisks. In markdown, asterisks are used for
emphasis, usually italics. We have to be careful when using asterisks in the help
text or they will disappear in the html output and cause everything to be italicized:

---

```
The location of the breakpoint can be defined as a line number, a function
name, or * and an address. If an address is specified, it must begin
with an asterisk (*). For example, break *0x0000000000400693 would break
```

The location of the breakpoint can be defined as a line number, a function
name, or * and an address. If an address is specified, it must begin
with an asterisk (*). For example, break *0x0000000000400693 would break

---

The github markdown renderer actually renders the above intelligently - sphinx
doesn't though.

Use backticks to surround anything with asterisks. This doesn't look very
pretty in plaintext, but it is still perfectly legible.

---

```
The location of the breakpoint can be defined as a line number, a function
name, or `*` and an address. If an address is specified, it must begin
with an asterisk (`*`). For example, `break *0x0000000000400693` would break
```

The location of the breakpoint can be defined as a line number, a function
name, or `*` and an address. If an address is specified, it must begin
with an asterisk (`*`). For example, `break *0x0000000000400693` would break

---
