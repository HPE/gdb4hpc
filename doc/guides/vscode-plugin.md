---
title: VSCode Plugin
---

# Running GDB4HPC debugger in VSCode

1. Log into the desired machine in vscode.
2. Open the extensions panel on the left.
3. Click on "..." located next to the Extensions header and choose "install from VSIX..."
4. Use the vsix file located in the rpm's share/vscode directory.
5. The VSCode window might need to be reloaded once the extension is installed.

6. Open the Run and Debug panel.
A launch.json file is needed.If you do not have one, create a launch.json file and copy the template below and modify as necessary.
```
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

```

- dbgversion = path to gdb4hpc
- procset = name and ranks for launch command i.e. "App0{10}"
- program = path of program to debug
- args = arg flags for launch command i.e "--gpu --non-mpi"
- cwd = current working directory
- env = list of any env varibales to apply. List them in this format:
```
{
    "GDB4HPC_DEBUG_INTERPRETER": "1",
    "CRAY_DBG_LOG_DIR": "."
}
```

