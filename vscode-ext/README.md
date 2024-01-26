Running GDB4HPC debugger in VSCode:

Copy vsix file from the repository under cdst-gdb4hpc/vscode-ext to the desired machine.
Log into the desired machine in vscode.

Open the extensions panel on the left.
Click on "..." located next to the Extensions header and choose "install from VSIX..."
The VSCode window might need to be reloaded once the extension is installed.

Open the Run and Debug panel.
A launch.json file is needed. If you do not have one already you will be prompted to create one.
A sample launch.json file can be found in the repository under cdst-gdb4hpc/vscode-ext/SampleWorkspace/.vscode/launch.json you can copy and modify.
