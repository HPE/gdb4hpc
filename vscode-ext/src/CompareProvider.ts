// Copyright 2024 Hewlett Packard Enterprise Development LP.

import * as vscode from 'vscode';

export var compare_list: any[] =[];

export class CompareProvider implements vscode.WebviewViewProvider {
  
  public static readonly viewType = 'compareView';

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
        case 'toggledCompare':
					compare_list[message.id].checked = message.checked;
      }
    })		
	}

	addComparison(): void {
    let input_comp_box: vscode.InputBoxOptions = {
      prompt: "New Comparison ",
      placeHolder: "$App0{0}::u=$App0{0}::u"
    }

    //show input boxes for name and procset and then send defset message
    vscode.window.showInputBox(input_comp_box).then(value => {
      if (!value) return;
			compare_list.push({text:value,result:"",checked: false})
			//post message to script
			this._view?.webview.postMessage({type:'comparesUpdated', value: compare_list});
    });    
  }

	runComparisons(){
		this.session._debugger.runComparisons().then(()=>{
			this._view?.webview.postMessage({type:'comparesUpdated', value: compare_list});
		});
	}

	private _getHtmlForWebview(webview: vscode.Webview) {
		//references to style sheets for vscode styling
		const styleResetUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'media', 'reset.css'));
		const styleVSCodeUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'media', 'vscode.css'));

		//reference to script used by compare display
		const scriptUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'src', 'compareScript.js'));

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
				<dl id='compare-list'>
				</dl>
				<script nonce="${nonce}" src="${scriptUri}"></script>
		</body>
    </html>`;
	}
}
