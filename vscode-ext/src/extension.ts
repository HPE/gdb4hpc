// Copyright 2024 Hewlett Packard Enterprise Development LP.

'use strict';

import * as vscode from 'vscode';
import {WorkspaceFolder,DebugConfiguration,CancellationToken,ProviderResult} from 'vscode';
import {DebugSession} from './DebugSession';
import { Procset, FocusProvider} from './FocusProvider';
import { CompareProvider } from './CompareProvider';
import { AssertionProvider } from './AssertionProvider';
import { DecompositionProvider } from './DecompostionProvider';


class GDB4HPCConfigurationProvider implements vscode.DebugConfigurationProvider {

  public apps: any;

  /**
   * Massage a debug configuration just before a debug session is being launched,
   * e.g. add all missing attributes to the debug configuration.
   */
  resolveDebugConfiguration(folder: WorkspaceFolder | undefined, config: DebugConfiguration, token?: CancellationToken): ProviderResult<DebugConfiguration> {
    // if launch.json is missing or empty
    if (!config.type && !config.request && !config.name) {
      const editor = vscode.window.activeTextEditor;
      
      if (editor && (editor.document.languageId === 'cpp' || editor.document.languageId === 'c')) {
        config.type = 'gdb4hpc';
        config.name = 'Launch GDB4HPC';
        config.request = 'launch';
        config.stopOnEntry = true;
      }
    }

    if (!config.apps){
			return vscode.window.showInformationMessage("Cannot find a program to debug").then(_ => {
				return undefined;	// abort launch
			});
    }else{
      this.apps = config.apps;
    }
    return config;
  }
}

export function activate(context: vscode.ExtensionContext) {
  const provider = new GDB4HPCConfigurationProvider();
  context.subscriptions.push(vscode.debug.registerDebugConfigurationProvider('gdb4hpc', provider));

  let factory = new InlineDebugAdapterFactory();
	context.subscriptions.push(vscode.debug.registerDebugAdapterDescriptorFactory('gdb4hpc', factory));
  context.subscriptions.push(vscode.commands.registerCommand('getContext', () => context));

  let session = factory.getSession();
  
  //Add Focus Panel to sidebar
  let focusProvider = new FocusProvider();
  vscode.window.registerTreeDataProvider('focus', focusProvider);
  vscode.commands.registerCommand('focus.refreshEntry', () => {focusProvider.refresh(session)});
  vscode.commands.registerCommand('focus.addEntry', () => {focusProvider.addPe(session)});
  vscode.commands.registerCommand('focus.selectEntry', (entry: Procset) => {focusProvider.changeFocus(session,entry.name)});

  //Add decomposition panel to sidebar
  let decompositionProvider = new DecompositionProvider(context.extensionUri, session);
  vscode.window.registerWebviewViewProvider("decompView", decompositionProvider);
  vscode.commands.registerCommand('decompView.addEntry', () => {decompositionProvider.addDecomposition()});

  //Add comparison panel to sidebar
  let compareProvider = new CompareProvider(context.extensionUri, session);
  vscode.window.registerWebviewViewProvider("compareView", compareProvider);
  vscode.commands.registerCommand('compareView.runCompares', () => {compareProvider.runComparisons()});
  vscode.commands.registerCommand('compareView.addEntry', () => {compareProvider.addComparison();});

  //Add assertion panel to sidebar
  let assertionProvider = new AssertionProvider(context.extensionUri, session);
  vscode.window.registerWebviewViewProvider("assertView", assertionProvider);
  vscode.commands.registerCommand('assertView.runScripts', () => {assertionProvider.runAssertionScript()});
  vscode.commands.registerCommand('assertView.getInfo', () => {assertionProvider.getAssertionResults()});
  vscode.commands.registerCommand('assertView.addEntry', () => {assertionProvider.addAssertionScript()});
  
  //get events from providers
  context.subscriptions.push(vscode.debug.onDidReceiveDebugSessionCustomEvent((e) => {
    if (e.event == "refreshPeEvent"){
      focusProvider.refresh(session);
    }else if(e.event == "changeFocus"){
      focusProvider.changeFocus(session,"all");
    }
  }));    
  
  /* 
  //Extra debug data to development console
  vscode.debug.registerDebugAdapterTrackerFactory('*', {
    createDebugAdapterTracker(session: vscode.DebugSession) {
      return {
        onWillReceiveMessage: m => console.log(`> ${JSON.stringify(m, undefined, 2)}`),
        onDidSendMessage: m => console.log(`< ${JSON.stringify(m, undefined, 2)}`)
      };
    }
  });
  */
}

export function deactivate() {
	// nothing to do
}

class InlineDebugAdapterFactory implements vscode.DebugAdapterDescriptorFactory {
  private session: any = new DebugSession();
	createDebugAdapterDescriptor(_session: vscode.DebugSession): ProviderResult<vscode.DebugAdapterDescriptor> {
		return new vscode.DebugAdapterInlineImplementation(this.session);
	}

  getSession(){
    return this.session;
  }

}

