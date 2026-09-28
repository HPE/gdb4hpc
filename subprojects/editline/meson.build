project('editline', 'c')

editline_sources = [
  './src/chared.c',
  './src/common.c',
  './src/el.c',
  './src/eln.c',
  './src/emacs.c',
  './src/hist.c',
  './src/keymacro.c',
  './src/map.c',
  './src/chartype.c',
  './src/parse.c',
  './src/prompt.c',
  './src/read.c',
  './src/refresh.c',
  './src/search.c',
  './src/sig.c',
  './src/terminal.c',
  './src/tty.c',
  './src/vi.c',
  './src/wcsdup.c',
  './src/tokenizer.c',
  './src/tokenizern.c',
  './src/history.c',
  './src/historyn.c',
  './src/filecomplete.c',
  './src/readline.c',
  './src/literal.c',
  './src/vis.c',
  './src/unvis.c',
  './src/strlcpy.c',
]

editline_a = static_library(
  'editline',
  editline_sources,
  include_directories : [ './src' ],
  c_args : [
    '-D_GNU_SOURCE', # for asprintf/vasprintf
  ]
)

editline_dep = declare_dependency(
  include_directories : [ './src' ],
  link_with : editline_a,
)
