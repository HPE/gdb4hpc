// Copyright 2024 Hewlett Packard Enterprise Development LP.

//script for assertion webview panel
(function () {
  const tsvscode = acquireVsCodeApi();

  window.addEventListener('message', event => {
    const message = event.data;
    switch (message.type) {
      //when script_list is updated, update the display
      case 'scriptsUpdated':
        assertions = message.value;
        const list = document.getElementById('script-list');
        list.innerHTML = "";
        assertions.forEach((item,i) => {
          //create checkbox
          var box = document.createElement('INPUT');
          box.setAttribute('type', 'radio');
          box.setAttribute('name', "scripts");
          box.setAttribute('id',   i);
          box.setAttribute('value', item.name);
          box.checked = item.checked;
          box.addEventListener('change', ()=> {
            tsvscode.postMessage({
              command: 'toggledAsserts',
              id: box.id,
              checked: box.checked
            });
          })
          
          //create label corresponding to radio button
          var scriptLabel = document.createElement('LABEL');
          scriptLabel.setAttribute('for',item.name);
          scriptLabel.appendChild(box);
          scriptLabel.appendChild(document.createTextNode(item.name));
          
          //create term and description tags
          var dt = document.createElement("dt");
          //assertion string as dt
          dt.appendChild(scriptLabel);
          list?.append(dt);

          item.asserts.forEach(assert=>{
            var dd = document.createElement("dd");
            var assert_dd = document.createElement("dd");
            assert_dd.innerHTML=assert.str;
            //assertion result as dd

            let result = "";
            if(parseInt(assert.pass)!=0 || parseInt(assert.warn!=0) || parseInt(assert.fail)!=0){
              result="Passed: "+assert.pass+", Passed with warning: " + assert.warn + ", Failed: "+ assert.fail;
            }
            dd.innerHTML = result;

            //append both to list
            list?.append(assert_dd);
            list?.append(dd);

          })
        });
        break;
    }
  });
}());
