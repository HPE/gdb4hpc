// Copyright 2024 Hewlett Packard Enterprise Development LP.

import * as vscode from 'vscode';

export var script_list: any[] =[];

export class AssertionProvider implements vscode.WebviewViewProvider {
  
  public static readonly viewType = 'assertView';

	public _view: vscode.WebviewView;

	constructor(
		private readonly _extensionUri: vscode.Uri,
		private readonly session: any
	) {	}

	public resolveWebviewView(webviewView: vscode.WebviewView,context: vscode.WebviewViewResolveContext,_token: vscode.CancellationToken) {
		this._view = webviewView || null;

		this._view.webview.options = {
			// Allow scripts in the webview
			enableScripts: true,

			localResourceRoots: [
				this._extensionUri
			]
		};

		//reference to display of panel
		this._view.webview.html = this._getHtmlForWebview(this._view.webview);
	
		//received message from script
		this._view.webview.onDidReceiveMessage( message => {
      switch (message.command) {
        case 'toggledAsserts':
					script_list.forEach((script,i) =>{
						i == message.id? script.checked = message.checked : script.checked = false;
					})
					break;
      }
    })		
	}

	addAssertionScript(): void {

    let input_script_name_box: vscode.InputBoxOptions = {
      prompt: "New Assertion Script Name",
      placeHolder: "script"+script_list.length,
			value: "script"+script_list.length
    }

		let input_script_stop_box: vscode.InputBoxOptions = {
      prompt: "Set Stop on Error? (y/n)",
      placeHolder: "n",
			value: "n"
    }

		let input_script_assert_box: vscode.InputBoxOptions = {
      prompt: "Add Assertion Script or press Escape",
      placeHolder: "$App0{1}::a@file:32 == $App0{2}::a@file:32",
    }

		let asserts: any[]= [];
		let name = "";
		let stopOnError = false;

		//show input box for an assertion until input box is empty
	 	var add_new_assert= () => new Promise(function(resolve) {
			vscode.window.showInputBox(input_script_assert_box).then(value =>{
				if (value==undefined || value ==""){
					resolve(true);
				}else{
					asserts.push({str:value, pass:"0", warn:"0", fail:"0"})
					resolve(add_new_assert());
				}
			})
		})
		
		
		//show input box for name
		vscode.window.showInputBox(input_script_name_box).then(value => {
			if (!value) return;
			name = value;
			//show input box for stopOnError
			vscode.window.showInputBox(input_script_stop_box).then(value => {
				stopOnError = value=="y"?true:false;
				//add asserts
				add_new_assert().then(()=>{
					let new_script={name:name, stopOnError: stopOnError, asserts:asserts, checked: false}
					
					//build assert script
					this.session._debugger.buildAssertionScript(new_script).then(()=>{
						script_list.push(new_script)
						this._view?.webview.postMessage({type:'scriptsUpdated', value: script_list});
					},null);
				})
			});
		}); 
  }

	runAssertionScript(){
		let choice = script_list.filter((el) => el.checked );
		if (!choice[0].name){
			return;
		}
		this.session._debugger.runAssertionScript(choice[0]);
	}

	getAssertionResults(){
		let choice = script_list.filter((el) => el.checked );
		if (!choice){
			return;
		}
		this.session._debugger.getAssertionResults(choice[0]).then(()=>{
			this._view?.webview.postMessage({type:'scriptsUpdated', value: script_list});
		});
	}

	private _getHtmlForWebview(webview: vscode.Webview) {
		//references to style sheets for vscode styling
		const styleResetUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'media', 'reset.css'));
		const styleVSCodeUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'media', 'vscode.css'));

		//reference to script used by assertion display
		const scriptUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'src', 'assertionScript.js'));

		//security policy reference
		function getNonce() {
			let text = '';
			const possible = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
			for (let i = 0; i < 32; i++) {
				text += possible.charAt(Math.floor(Math.random() * possible.length));
			}
			return text;
		}

		const nonce = getNonce();

		return `<!DOCTYPE html>
    <html lang="en">
    <head>
				<meta charset="UTF-8">

				<!--
					Use a content security policy to only allow loading styles from our extension directory,
					and only allow scripts that have a specific nonce.
					(See the 'webview-sample' extension sample for img-src content security policy examples)
				-->
				<meta http-equiv="Content-Security-Policy" content="default-src 'none'; style-src ${webview.cspSource}; script-src 'nonce-${nonce}';">

				<meta name="viewport" content="width=device-width, initial-scale=1.0">
				<link href="${styleResetUri}" rel="stylesheet">
				<link href="${styleVSCodeUri}" rel="stylesheet">
    </head>
    <body>
				<dl id='script-list'>
				</dl>
				<script nonce="${nonce}" src="${scriptUri}"></script>
		</body>
    </html>`;
	}
}
