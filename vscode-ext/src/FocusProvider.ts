// Copyright 2024 Hewlett Packard Enterprise Development LP.

import * as vscode from 'vscode';
import { pe_list } from './GDB4HPC';

export class FocusProvider implements vscode.TreeDataProvider<Procset> {
  
  constructor() {
  }

  private _onDidChangeTreeData: vscode.EventEmitter<Procset | undefined | null | void> = new vscode.EventEmitter<Procset | undefined | null | void>();
  readonly onDidChangeTreeData: vscode.Event<Procset | undefined | null | void> = this._onDidChangeTreeData.event;

  refresh(session: any): void {
    session._debugger.getProcsetList();
    this._onDidChangeTreeData.fire();
  }

  changeFocus(session: any, pe_name:string): void {    
    session._debugger.changeFocus(pe_name);
    this._onDidChangeTreeData.fire();
  }

  addPe(session: any): void {
    let input_name_box: vscode.InputBoxOptions = {
      prompt: "Name for new PE ",
      placeHolder: "abc"
    }

    let input_procset_box: vscode.InputBoxOptions = {
      prompt: "Name for new PE ",
      placeHolder: "$App0{2},$App1{0..3}"
    }

    let name = "";
    let procset = "";
  
    //show input boxes for name and procset and then send defset message
    vscode.window.showInputBox(input_name_box).then(value => {
      if (!value) return;
      name = value;
      vscode.window.showInputBox(input_procset_box).then(value => {
        if (!value) return;
        procset = value;
        session._debugger.addProcset(name, procset).then(()=> this.refresh(session))
      });
    });    
  }

  getTreeItem(element: Procset): vscode.TreeItem {
    return element;
  }

  getChildren(element?: Procset): Thenable<Procset[]> {
    if (!pe_list) {
      vscode.window.showInformationMessage('No pe list available');
      return Promise.resolve([]);
    }

    return Promise.resolve(pe_list);
  }

}

export class Procset extends vscode.TreeItem {
  isSelected: boolean = false;
  constructor(
    public readonly name: string,
    public readonly procset: string
  ) {
    super(name);
    this.tooltip = `${this.name}`
    this.isSelected = false;
    this.label = `  ${this.name}`
    this.description = `${this.procset}`
  }

  iconPath = {
    light: `${this.label}`,
    dark: `${this.label}`
  };

  updateLabel(){
    if(this.isSelected){
      this.label = `* ${this.name}`;
    }
    else{
      this.label = `  ${this.name}`;
    }
  }
}
