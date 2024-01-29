Running GDB4HPC debugger in VSCode:

Log into the desired machine in vscode.
Open the extensions panel on the left.
Click on "..." located next to the Extensions header and choose "install from VSIX..."
Use the vsix file located in the rpm's share/vscode directory.
The VSCode window might need to be reloaded once the extension is installed.

Open the Run and Debug panel.
A launch.json file is needed. If you do not have one already use the sample below

Create a launch.json file and copy the template below.

{
    "version": "1.1.1",
    "configurations": [
        {
            "type": "gdb4hpc",
            "request": "launch",
            "name": "GDB4HPC",
            "dbgversion": "",   
            "apps": [
                {
                    "procset": "",
                    "program": "",
                    "args": ""
                }
            ],
            "cwd": "", 
            "env": {

            },
        }
    ]
}


dbgversion = path to gdb4hpc
procset = name and ranks for launch command i.e. "App0{10}"
program = path to program to debug
args = arg flags for launch command i.e "--gpu --non-mpi"
cwd = current working directory
env = list of any env varibales to apply. List them like this:
{
    "GDB4HPC_DEBUG_INTERPRETER": "1",
    "CRAY_DBG_LOG_DIR": "."
}