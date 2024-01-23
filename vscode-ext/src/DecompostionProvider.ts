import * as vscode from 'vscode';

//export var decomp_list: any[] =[];

export class DecompositionProvider implements vscode.WebviewViewProvider {
  
  public static readonly viewType = 'decompView';

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
	}

	addDecomposition(): void {
		let input_decomp_cmd_box: vscode.InputBoxOptions = {
			prompt: "New Decompostion Command or Name",
			placeHolder: "$a" +" or $a 10/2",
			value: ""
		}

		let input_sub_cmd_box: vscode.InputBoxOptions = {
			prompt: "Add sub command or press Escape",
			placeHolder: "dimension, distribute, proc_grid, or dim_order commands (i.e. dimension 2,2)",
			value: ""
		}

		let decomp_cmds: string[] = [];
		//show input box for adding sub command until input box is empty
	 	var add_sub_command = () => new Promise(function(resolve) {
			vscode.window.showInputBox(input_sub_cmd_box).then(value =>{
				if (value==undefined || value ==""){
					resolve(true);
				}else{
					decomp_cmds.push(value);
					resolve(add_sub_command());
				}
			})
		})

		//if only name was provided previously, all 4 sub commands are required.
		var add_missing_content = () => new Promise(function(resolve) {
			if(decomp_cmds.length==1){
				resolve(false);
			}
			let dim_len = decomp_cmds[1].split(" ")[1].split(",").length;
			let placeholder = "*"+",*".repeat(dim_len-1);

			if(!decomp_cmds.find(a =>a.includes("distribute"))) decomp_cmds.push("distribute "+placeholder);
			if(!decomp_cmds.find(a =>a.includes("dimension"))) decomp_cmds.push("dimension "+placeholder);
			if(!decomp_cmds.find(a =>a.includes("proc_grid"))) decomp_cmds.push("proc_grid "+placeholder);
			if(!decomp_cmds.find(a =>a.includes("dim_order"))) decomp_cmds.push("dim_order "+placeholder);
			decomp_cmds.push("end");

			resolve(true)
		})
		
		vscode.window.showInputBox(input_decomp_cmd_box).then(value => {
			if (!value) return;
			console.log("value: "+ value)
			decomp_cmds.push('decomposition '+value);
			
			if (value.split(" ").length>1){
				this.session._debugger.buildDecomposition(decomp_cmds).then((decomps)=>{
					this._view?.webview.postMessage({type:'decompsUpdated', value: decomps});
				})
			}else{
				//add subcommands
				add_sub_command().then(()=>{
					add_missing_content().then((resolved)=>{
						if (resolved){
							this.session._debugger.buildDecomposition(decomp_cmds).then((decomps)=>{
								this._view?.webview.postMessage({type:'decompsUpdated', value: decomps});
							})
						}
					})
				})
			}
		}); 
  }

	private _getHtmlForWebview(webview: vscode.Webview) {
		//references to style sheets for vscode styling
		const styleResetUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'media', 'reset.css'));
		const styleVSCodeUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'media', 'vscode.css'));

		//reference to script used by assertion display
		const scriptUri = webview.asWebviewUri(vscode.Uri.joinPath(this._extensionUri, 'src', 'decompositionScript.js'));

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
				<ul id='decomp-list'>
				</ul>
				<script nonce="${nonce}" src="${scriptUri}"></script>
		</body>
    </html>`;
	}
}
