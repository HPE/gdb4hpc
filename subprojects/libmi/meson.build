project('libmi', 'c')

libmi_sources = [
  './CLICommand.c',
  './CLIOutput.c',
  './MIArg.c',
  './MIBreakpoint.c',
  './MIBreakCommand.c',
  './MICommand.c',
  './MIEnvironmentCommand.c',
  './MIEnvironment.c',
  './MIError.c',
  './MIEvent.c',
  './MIExecCommand.c',
  './MIFile.c',
  './MIFileCommand.c',
  './MIFrame.c',
  './MIGDBCommand.c',
  './MIList.c',
  './MIMemory.c',
  './MIMemoryCommand.c',
  './MIOOBRecord.c',
  './MIOutput.c',
  './MIParser.c',
  './MIPty.c',
  './MIResult.c',
  './MIResultRecord.c',
  './MISession.c',
  './MISignalInfo.c',
  './MIStackCommand.c',
  './MISymbolCommand.c',
  './MISymbol.c',
  './MIString.c',
  './MITargetCommand.c',
  './MIThread.c',
  './MIThreadCommand.c',
  './MIDisassembly.c',
  './MIValue.c',
  './MIVar.c',
  './MIVarCommand.c',
]

libmi_a = static_library(
  'mi',
  libmi_sources,
  c_args : [
    '-D_XOPEN_SOURCE=500', # for grantpt/unlockpt/ptsname
    '-D_GNU_SOURCE', # for asprintf/vasprintf
  ]
)

libmi_dep = declare_dependency(
  include_directories : '.',
  link_with : libmi_a
)
