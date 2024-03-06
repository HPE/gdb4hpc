// Copyright 2024 Hewlett Packard Enterprise Development LP.

//script for Decomposition webview panel
(function () {
  window.addEventListener('message', event => {
    const message = event.data;
    switch (message.type) {
      //when compare_list is updated, update the display
      case 'decompsUpdated':
        decomps = message.value;
        const list = document.getElementById('decomp-list');
        list.innerHTML = "";
        decomps.forEach((element) => {
          var dt = document.createElement('dt');
          dt.innerHTML=dt.innerHTML + element.name;
          list?.append(dt);

          var dim_dd = document.createElement('dd');
          dim_dd.innerHTML=dim_dd.innerHTML + "dimension: " + element.dimension;
          list?.append(dim_dd);

          var dis_dd = document.createElement('dd');
          dis_dd.innerHTML=dis_dd.innerHTML + "distribute: " + element.distribute;
          list?.append(dis_dd);

          var pg_dd = document.createElement('dd');
          pg_dd.innerHTML=pg_dd.innerHTML + "proc_grid: " + element.proc_grid;
          list?.append(pg_dd);

          var dim_o_dd = document.createElement('dd');
          dim_o_dd.innerHTML=dim_o_dd.innerHTML + "dim_order: " + element.dim_order;
          list?.append(dim_o_dd);

        });
        break;
    }
  });
}());