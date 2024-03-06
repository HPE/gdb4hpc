// Copyright 2024 Hewlett Packard Enterprise Development LP.

//script for Comparison webview panel
(function () {
  const tsvscode = acquireVsCodeApi();

  window.addEventListener('message', event => {
    const message = event.data;
    switch (message.type) {
      //when compare_list is updated, update the display
      case 'comparesUpdated':
        compares = message.value;
        const list = document.getElementById('compare-list');
        list.innerHTML = "";
        compares.forEach((item,i) => {
          //create checkbox
          var box = document.createElement('INPUT');
          box.setAttribute('type', 'checkbox');
          box.setAttribute('id',   i);
          box.setAttribute('value', item.text);
          box.checked = item.checked;
          box.addEventListener('change', ()=> {
            tsvscode.postMessage({
              command: 'toggledCompare',
              id: box.id,
              checked: box.checked
            });
          })
          
          //create label corresponding to checkbox
          var compLabel = document.createElement('LABEL');
          compLabel.setAttribute('for',item.text);
          compLabel.appendChild(box);
          compLabel.appendChild(document.createTextNode(item.text));
          
          //create term and description tags
          var dt = document.createElement("dt");
          var dd = document.createElement("dd");
          
          //compare string as dt
          dt.appendChild(compLabel);
          //compare result as dd
          dd.innerHTML = item.result;

          //append both to list
          list?.append(dt);
          list?.append(dd);
      
        });
        break;
    }
  });
}());