project('pythoncapi-compat', 'cpp')

# pythoncapi-compat is header-only; there is nothing to build or install here.
# the header lives at the repo root, not under an 'include' subdirectory.
python_compat_include_dir = include_directories('.')

python_compat_dep = declare_dependency(
  include_directories: python_compat_include_dir,
)
