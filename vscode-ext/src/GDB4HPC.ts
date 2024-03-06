// Copyright 2024 Hewlett Packard Enterprise Development LP.

import {EventEmitter} from 'events';
import {ILaunchRequestArguments} from './DebugSession';
import {Breakpoint, Source, StackFrame, Thread} from '@vscode/debugadapter';
import {DebugProtocol} from '@vscode/debugprotocol';
import {Record, MIParser} from './MIParser';
import * as vscode from 'vscode';
import * as pty from 'node-pty';
import {clearInterval} from 'timers';
import { Procset} from './FocusProvider';
import { compare_list } from './CompareProvider';

export var pe_list: Procset[] = [];

export class DebuggerVariable {
  public name: string;  //name of variable
  public debuggerName: string;  //name assigned by MI
  public numberOfChildren: number;   //number of children
  public referenceID: number;  //scope tracking id
  public type: string;   //variable type
  public values: any[]=[];
}

export class GDB4HPC extends EventEmitter {
  protected cwd: string;
  protected apps: any;
  protected environmentVariables: string[];
  protected program: string;
  protected dbgversion: string;
  protected gdb4hpcPty: any;
  private output_panel: vscode.OutputChannel;
  private mi_log: vscode.OutputChannel;
  private error_log: vscode.OutputChannel;
  protected gdb4hpcReady = false;
  protected inferiorRunning = false;
  protected type = 'launch';
  private parser: MIParser = new MIParser();
  private token = 0;
  private buffer ='';
  private handlers: {[token: number]: (record: Record) => void} = [];
  private breakpoints = new Map<string, number[]>();
  private variables = new Map<string, DebuggerVariable>();

  public spawn(args: ILaunchRequestArguments): void {
    this.cwd = args.cwd || '';
    this.environmentVariables = args.env || [];
    this.apps = args.apps;
    this.dbgversion = args.dbgversion;
    this.type = args.request;
    this.output_panel = vscode.window.createOutputChannel("Program Output")
    this.mi_log = vscode.window.createOutputChannel("MI Log");
    this.error_log = vscode.window.createOutputChannel("Error Log");
    this.createPty();
    this.output_panel.show();
  }

  private createPty(): void {
    this.gdb4hpcPty = pty.spawn(this.dbgversion, ["--interpreter=mi"], {
      name: 'xterm-color',
      cols: 80,
      rows: 30,
      cwd: this.cwd,
      env: Object.assign(this.environmentVariables, process.env),
    });
    
    this.gdb4hpcPty.onData(data => {
      data = data.replace("dbg all> ","");
      //remove color codes
      data = data.replaceAll("\x1b[0m","");
      data = data.replaceAll("\x1b[30;1m","");
      this.handleOutput(data);
    });

    this.gdb4hpcPty.onExit((e) => {
      console.log(e); 
      this.output_panel.dispose();
      this.mi_log.dispose();
    });

    this.gdb4hpcReady = true;
  }

  protected handleOutput(data: any): void {
    let record: Record | null;
    this.buffer+=data;
    
    if (!this.buffer){
      return;
    }

    const nPos = this.buffer.lastIndexOf('\n');
    if (nPos !== -1) {
      const lines = this.buffer.substring(0, nPos).split('\n') as string[];
      this.buffer = this.buffer.substring(nPos + 1);

      lines.forEach(line => {
        line = line.trim();
        record = this.parser.parseRecord(line)
        record?this.handleParsedRecord(record):null;
      });
    }
  }

  private handleParsedRecord(record: Record) {
    switch (record.getType()) {
      case '*':
        this.handleAsyncRecord(record);
        break;

      case '~':
        this.emit('output', record.printRecord(), 'console');
        break;

      case 'mi':
          this.mi_log.appendLine(record.printRecord());
          break;
      
      case '@':
        this.output_panel.appendLine(record.printRecord());
        this.output_panel.show(); //always display output panel if output was added
        break;
      
      case '^':
        if (!isNaN(record.getToken())) {
          const handler = this.handlers[record.getToken()];
    
          if (handler) {
            handler(record);
            delete this.handlers[record.getToken()];
          } 
        }else{
          if(record.getReason()=="error"){
            vscode.window.showErrorMessage(record.getInfo('msg'))
            this.error_log.appendLine(record.getInfo('msg'));
          }
        }
        break;
    }
  }

  private handleAsyncRecord(record: Record) {
    switch (record.getReason()) {
      case 'stopped':
        {
          this.inferiorRunning = false;
          let stoppedReason = record.getInfo('reason');

          switch (stoppedReason) {
            case 'breakpoint-hit':
            case 'end-stepping-range':
              
              const fileName=record.getInfo('frame')["fullname"];
              const line = parseInt(record.getInfo('frame')["line"]);
              
              if (stoppedReason === 'breakpoint-hit') {
                const bkptnum = parseInt(record.getInfo('bkptno'));
                const breakpointIDs: number[] = [];
                breakpointIDs.push(bkptnum);

                if (bkptnum===0){
                  this.breakpoints.set(fileName, breakpointIDs);
                  stoppedReason='entry'
                }              
              }
              var openPath = vscode.Uri.file(fileName);
              
              vscode.workspace.openTextDocument(openPath).then(doc => 
              {
                  vscode.window.showTextDocument(doc).then(editor => 
                  {
                    let range = editor.document.lineAt(line-1).range;
                    editor.selection =  new vscode.Selection(range.start, range.end);
                    editor.revealRange(range);
                  });
              });
              
              this.gdb4hpcReady=true;
              //FIX: hard coded 1 thread- will add task to fix
              this.emit(stoppedReason, 1);
              break;

            case 'exited-normally':
              this.sendCommand('-quit');
              this.emit('exited-normally');
              break;

            default:
              console.error('Unknown stop reason');
          }
        }
        break;

      case 'running':
        this.inferiorRunning = true;
        break;
    }
  }

  public clearBreakpoints(fileName: string): Promise<boolean> {
    return new Promise(resolve => {
      const breakpoints = this.breakpoints.get(fileName);
      if (breakpoints) {
        breakpoints.forEach((breakpoint: number) => {
          this.sendCommand(`-break-delete ${breakpoint}`);
        });
        this.breakpoints.delete(fileName);
      }
      resolve(true); 
    });
  }

  public getThreads(): Promise<Thread[]> {
    return new Promise(resolve => {
      this.sendCommand('-thread-info').then((record: Record) => {
        const threadsResult: Thread[] = [];
        if (record.getInfo('msgs')){
          record.getInfo('msgs').forEach((message:any)=>{
            message['threads'].forEach((thread: any) => {
              threadsResult.push(new Thread(parseInt(thread.id), thread.name));
            });
          })  
        }else if (record.getInfo('threads')) {
          record.getInfo('threads').forEach((thread: any) => {
            threadsResult.push(new Thread(parseInt(thread.id), thread.name));
          });
        }
        resolve(threadsResult);
      });
    });
  }


  public continue(): Promise<any> {
      return this.sendCommand('-exec-continue');
  }

  public getVariable(name: string): DebuggerVariable | undefined {
    return [...this.variables.values()].find(variable => {
      if(variable.name == name){
        return variable;
      }else{
        return undefined
      }
    });
  }

  public getDebuggerVariable(name: string): DebuggerVariable | undefined {
    return [...this.variables.values()].find(variable => {
      if(variable.debuggerName == name){
        return variable;
      }else{
        return undefined
      }
    });
  }

  //mi variable sends value in format of $a{i}: val\n$b{j}: val\n...etc. 
  //parse out the procset, group, and value for each procset in the value
  public parseVariableRecord(str: string): any[]{
    if (!str) return [{}];
    let split_variables = /\\n(?=\w+\{?[\S]*\}?\:)/;
    let variables_str = str.split(split_variables);
    let result: any[] = [];
    variables_str.forEach(variable=>{
      let regex = /(\w+)\{([\S]*)\}\:([\s\S]*)/;
      let match: any[]|null;
      if ((match = regex.exec(variable))) {
        result.push({'procset':match[1],'group':match[2], 'value':match[3]})
      }
    })
    return(result)
  }

  //send var-create command to gdb4hpc and retreive output
  public createVariable(name: string): Promise<Map<string,DebuggerVariable>> {
    return new Promise(resolve => {
      this.sendCommand(`-var-create - * "${name}"`).then(
        gdbVariable => {
          const childCount = parseInt(gdbVariable.getInfo('numchild')) || parseInt(gdbVariable.getInfo('dynamic')) 
                              || parseInt(gdbVariable.getInfo('has_more')) || 0;
          const newReferenceID = this.variables.size + 1;
          let variables_match = this.parseVariableRecord(gdbVariable.getInfo('value'));
          let vals:any[]=[];

          //create an array of values from mi message
          variables_match.forEach(variable=>{
            vals.push({'procset':variable['procset'],'group':variable['group'],'value':variable['value']})
          });

          //create a new variable with all of the variations of values in one variable
          const newVariable: DebuggerVariable = {
            name: name,
            debuggerName: gdbVariable.getInfo('name'),
            numberOfChildren: childCount,
            referenceID: childCount ? newReferenceID : 0, 
            type: gdbVariable.getInfo('type'),
            values: vals
          };

          this.variables.set(name, newVariable);
          resolve(this.variables);
        }
      );
    });
  }
  
  public updateVariables(): Promise<Map<string, DebuggerVariable>> {
    return new Promise(resolve => {
      this.sendCommand(`-var-update --all-values *`).then((record:Record)=> {
        record.getInfo('changelist').forEach(variableRecord => {

          //get saved variable and update it
          let variable = this.getDebuggerVariable(variableRecord.name);
          if (!variable) return resolve(this.variables)
          let variables_match = this.parseVariableRecord(variableRecord.value);
          variables_match.forEach(variable_match =>{
            //find saved variable corresponding to variable being updated from mi message
            let found = variable!.values.find(obj => {
              return obj.procset === variable_match['procset'] && obj.group === variable_match['group']
            });
            //if variable has a value stored for matching procset and group, update the value
            //otherwise add it
            if (found){
              found.value = variable_match['value']
            }else{
              variable!.values.push({'procset':variable_match['procset'],'group':variable_match['group'],'value':variable_match['value']})
            }
          })
          
          //filter out so any old variable values that were not updated are removed.
          variable!.values = variable!.values.filter((el) => {
            return variables_match.some((f) => {
              return f.procset === el.procset && f.group === el.group;
            });
          });
        });
        resolve(this.variables);
      });
    });
  }

  public getVariables(): Promise<Map<string, DebuggerVariable>> {
    return new Promise(resolve => {
      this.sendCommand(`-stack-list-variables`).then((record: Record) => {
        const pending: Promise<Map<string,DebuggerVariable>>[] = [];

        record.getInfo('variables').forEach(variable => {
          if (!this.getVariable(variable.name)){
            pending.push(this.createVariable(variable.name));
          }
        });

        Promise.all(pending).then(() => {
          this.updateVariables().then(()=>{
            resolve(this.variables);
          });
        });
      });
    });
  }

  public stack(startFrame: number, endFrame: number): Promise<DebugProtocol.StackFrame[]> {
    return new Promise(resolve => {
      this.sendCommand(`-stack-list-frames`).then(
        (record: Record) => {
          const stackFinal: DebugProtocol.StackFrame[] = [];
          let stack = record.getInfo('stack');
          for (let i = startFrame; i < Math.min(endFrame, stack.length); i++) {
            let frame = stack[i];
            frame = frame[1];
            const sf: DebugProtocol.StackFrame = new StackFrame(i,frame.func,new Source(frame.file? frame.file.split('\\').pop().split('/').pop() : '??',frame.fullname),parseInt(frame.line));
            sf.instructionPointerReference = frame.addr;
            stackFinal.push(sf);
          }
          resolve(stackFinal);
        })
      });
	}

  public next(): Promise<Record> {
    return this.sendCommand(`-exec-next`);
  }

  public pause(): Promise<boolean> {
    return new Promise(resolve => {
      if (!this.inferiorRunning) {
        resolve(true);
      } else {
        this.sendCommand(`-exec-interrupt`).then(() => {
          resolve(false);
        });
      }
    });
  }

  public sendCommand(command: string): Promise<Record> {
    return new Promise(resolve => {
      if (!command.includes("-")) command = `${command}\n`;
      else command = `${++this.token +command}\n`;
      this.gdb4hpcPty.write(command);
      this.handlers[this.token] = (record: Record) => {
        resolve(record);
      };
    });
  }


  public setBreakpoints(fileName: string, breakpoints: DebugProtocol.SourceBreakpoint[]): Promise<Breakpoint[]> {
    return new Promise(resolve => {
      if (this.gdb4hpcReady) {
        this.clearBreakpoints(fileName).then(() => {
          const breakpointsPending: Promise<void>[] = [];
          const breakpointsConfirmed: Breakpoint[] = [];
          const breakpointIDs: number[] = [];

          breakpoints.forEach(srcBreakpoint => {
            breakpointsPending.push(
              this.sendCommand(`-break-insert ${fileName}:${srcBreakpoint.line}`).then((breakpoint: Record) => {
                const bkpt = breakpoint.getInfo('bkpt');
                if (!bkpt){
                  return;
                }
                breakpointsConfirmed.push(new Breakpoint(!bkpt.pending, bkpt.line));
                breakpointIDs.push(parseInt(bkpt.number));
              })
            );
          });

          Promise.all(breakpointsPending).then(() => {
            this.breakpoints.set(fileName, breakpointIDs);
            resolve(breakpointsConfirmed);
          });
        });
      } else {
        const intv = setInterval(() => {
          if (!this.inferiorRunning) {
            clearInterval(intv);
            this.setBreakpoints(fileName, breakpoints).then(bps =>resolve(bps));
          }
        }, 500);
      }
    });
  }

  public stepIn(): Promise<Record> {
    return this.sendCommand(`-exec-step`);
  }

  public terminate(): Promise<any> {
    return this.sendCommand('-gdb-exit');
  }

  public launchApps():  Promise<any> {
    return new Promise(resolve => {
      this.apps.forEach(app=>{
        this.sendCommand(`launch $`+ app.procset + ` ` + app.program + ` ` + app.args);
      });
      resolve(true); 
    });
  }

  public addProcset(name: string, procset: string): Promise<boolean> {
    return new Promise(resolve => {
      this.sendCommand(`-procset-define $${name} ${procset}`).then(() => {
        resolve(true);
      });
    })
	}

  public getProcsetList(): Promise<Procset[]> {
    return new Promise(resolve => {
      this.sendCommand(`-procset-list`).then((record: Record) => {
        record.getInfo('pe_sets').forEach(set =>{
          if (!pe_list.some(pe => pe.name === set.name)){
            pe_list.push(new Procset(set['name'], set['proc_set']))
          }
        })
        resolve(pe_list);
      })  
    });
	}

  public changeFocus(pe_name: string): Promise<boolean> {
    return new Promise(resolve => {
      this.sendCommand(`-procset-focus $${pe_name}`).then((record: Record) => {
        let name = record.getInfo('focus')['name'];
        let found = false;
        pe_list.forEach((pe)=>{
          if (pe.name == name){
            pe.isSelected = true;
            found = true;
          }else{
            pe.isSelected = false;
          }
          pe.updateLabel();
        })
        //trigger window refresh
        this.emit('end-stepping-range', 1);
        resolve(found);
      })
    });
	}

  public buildDecomposition(new_decomp: any): Promise<any> {
    return new Promise(resolve => {
      if (new_decomp.length == 0){
        resolve(false);
      }
      
      let cmds = new_decomp.join("\n");
      this.sendCommand(cmds);
      this.sendCommand(`-decomposition-list`).then((record: Record)=>{
        let decomps = record.getInfo("decompositions");
        resolve(decomps);
      });      
    });
  }

  private runComparison(comparison: any): Promise<boolean> {
    return new Promise(resolve => {
      this.sendCommand(`-compare ${comparison.text}`).then((record: Record) => {
        if(record.getReason()=="error"){
          vscode.window.showErrorMessage(record.getInfo('msg'));
        }else{
          comparison.result = record.getInfo("compare")["result"];
          comparison.result=comparison.result.replace(/\\n/g, "\n");
        }
      })
      resolve(true)
    });
  }

  public runComparisons(): Promise<any> {
    return new Promise(resolve => {
      let pending: Promise<boolean>[] = [];
      compare_list.forEach( (comparison)=>{
        if (comparison.checked){
          pending.push(this.runComparison(comparison))
        }else{
          comparison.result ="";
        }
      })
      Promise.all(pending).then(() => {
        resolve(compare_list);
      });
    });
	}

  public buildAssertionScript(new_script: any): Promise<boolean> {
    return new Promise(resolve => {
      let cmds:string[] =[];
      cmds.push(`build $${new_script.name}`);
      cmds.push(`set stop ${new_script.stopOnError}`);
      //verbose breakpoints will cause gdb4hpc to emit breakpoint events as they happen, updating display
      cmds.push(`set verbose-breakpoints on`);

      new_script.asserts.forEach(assert =>{
        cmds.push(`assert ${assert.str}`);
      })
      cmds.push(`end`);
      this.sendCommand(cmds.join("\n")).then((record: Record) => {
        resolve(true);
      });
      
    });
  }

  public runAssertionScript(assertion: any): Promise<boolean> {
    return new Promise(resolve => {
      this.sendCommand(`start $${assertion.name}`).then(() => {
        resolve(true);
      })
    });
  }

  public getAssertionResults(script: any): Promise<boolean> {
    return new Promise(resolve => {
      this.sendCommand(`-script-list $${script.name} results`).then((record: Record) => {
        let assert_results = record.getInfo("script_result")["assertions"];
        assert_results.forEach((assert, i)=>{
          let updated = script.asserts[i]
          updated["pass"] = assert.pass;
          updated["warn"] = assert.warn;
          updated["fail"] = assert.fail;
        })
        resolve(true);
      })
    });
  }
}
