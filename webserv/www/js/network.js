var netChart = null, netHist = [];

/* ── Requêtes pré-faites ────────────────────────────────── */
var NET_PRESETS = [
    /* ── GET de base ── */
    { label:'GET /',            method:'GET',    path:'/',               headers:'',                                          body:'' },
    { label:'GET /index.html',  method:'GET',    path:'/index.html',     headers:'',                                          body:'' },
    { label:'GET avec Accept',  method:'GET',    path:'/',               headers:'Accept: text/html\nAccept-Language: fr-FR', body:'' },
    { label:'GET no-cache',     method:'GET',    path:'/',               headers:'Cache-Control: no-cache\nPragma: no-cache', body:'' },

    /* ── HEAD ── */
    { label:'HEAD /',           method:'HEAD',   path:'/',               headers:'',                                          body:'' },

    /* ── POST ── */
    { label:'POST form-data',   method:'POST',   path:'/',               headers:'Content-Type: application/x-www-form-urlencoded', body:'username=admin&password=test&action=login' },
    { label:'POST JSON',        method:'POST',   path:'/api',            headers:'Content-Type: application/json\nAccept: application/json', body:'{"username":"admin","password":"test"}' },
    { label:'POST texte brut',  method:'POST',   path:'/upload',         headers:'Content-Type: text/plain',                  body:'Hello webserv from PhenixGinx Lab' },
    { label:'POST gros body',   method:'POST',   path:'/upload',         headers:'Content-Type: application/octet-stream',    body: new Array(2049).join('X') },

    /* ── PUT / DELETE / PATCH ── */
    { label:'PUT ressource',    method:'PUT',    path:'/resource/1',     headers:'Content-Type: application/json',            body:'{"name":"updated","value":42}' },
    { label:'DELETE ressource', method:'DELETE', path:'/resource/1',     headers:'',                                          body:'' },
    { label:'PATCH ressource',  method:'PATCH',  path:'/resource/1',     headers:'Content-Type: application/json',            body:'{"value":99}' },

    /* ── OPTIONS ── */
    { label:'OPTIONS (CORS)',   method:'OPTIONS',path:'/',               headers:'Origin: http://localhost\nAccess-Control-Request-Method: POST', body:'' },

    /* ── Headers spéciaux ── */
    { label:'Host custom',      method:'GET',    path:'/',               headers:'Host: example.com',                         body:'' },
    { label:'User-Agent curl',  method:'GET',    path:'/',               headers:'User-Agent: curl/7.88.0',                   body:'' },
    { label:'Connexion keep-alive', method:'GET',path:'/',               headers:'Connection: keep-alive\nKeep-Alive: timeout=5, max=100', body:'' },
    { label:'Accept tout',      method:'GET',    path:'/',               headers:'Accept: */*\nAccept-Encoding: gzip, deflate', body:'' },
    { label:'Auth Basic',       method:'GET',    path:'/admin',          headers:'Authorization: Basic YWRtaW46cGFzc3dvcmQ=', body:'' },
    { label:'Range (partial)',  method:'GET',    path:'/file',           headers:'Range: bytes=0-1023',                       body:'' },
    { label:'If-Modified-Since',method:'GET',    path:'/',               headers:'If-Modified-Since: Mon, 01 Jan 2024 00:00:00 GMT', body:'' },
];

function netPresetChange(){
    var sel = document.getElementById('n-host-preset');
    var f   = document.getElementById('net-custom-field');
    f.style.display = sel.value === 'custom' ? 'block' : 'none';
    if(sel.value !== 'custom') document.getElementById('n-host').value = sel.value;
}
function getNetHost(){
    var sel = document.getElementById('n-host-preset');
    return sel.value === 'custom' ? document.getElementById('n-host').value : sel.value;
}
function esc(s){ return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;'); }

function buildNetPresets(){
    var sel = document.getElementById('n-req-preset');
    if(!sel) return;
    sel.innerHTML = '<option value="">— Choisir une requête type —</option>';
    NET_PRESETS.forEach(function(p, i){
        var o = document.createElement('option');
        o.value = i; o.textContent = p.label; sel.appendChild(o);
    });
}
function applyNetPreset(){
    var sel = document.getElementById('n-req-preset');
    if(!sel || sel.value === '') return;
    var p = NET_PRESETS[parseInt(sel.value)];
    if(!p) return;
    document.getElementById('n-meth').value  = p.method;
    document.getElementById('n-path').value  = p.path;
    document.getElementById('n-hdrs').value  = p.headers;
    document.getElementById('n-body').value  = p.body;
}

function sendRequest(){
    var method = document.getElementById('n-meth').value;
    var host   = getNetHost();
    var path   = document.getElementById('n-path').value;
    var hdrs   = document.getElementById('n-hdrs').value;
    var body   = document.getElementById('n-body').value;
    var opts   = {method:method, headers:{}, mode:'cors', cache:'no-store'};
    if(body && /^(POST|PUT|PATCH)$/.test(method)){
        opts.body = body;
        opts.headers['Content-Type'] = 'text/plain';
    }
    hdrs.split('\n').forEach(function(l){
        var i = l.indexOf(':'); if(i > 0) opts.headers[l.slice(0,i).trim()] = l.slice(i+1).trim();
    });
    var t0 = Date.now();
    document.getElementById('n-resp').textContent = 'Envoi en cours…';
    fetch(host+path, opts).then(function(r){
        var lat = Date.now()-t0, code = r.status;
        return r.text().then(function(rb){
            var cls = code<300?'c2':code<400?'c3':code<500?'c4':'c5';
            if(code>=200&&code<300) G.netCounts.c2++;
            else if(code>=400&&code<500) G.netCounts.c4++;
            else if(code>=500) G.netCounts.c5++;
            updNetCounts();
            var h=''; r.headers.forEach(function(v,k){ h+=k+': '+v+'\n'; });
            document.getElementById('n-resp').innerHTML =
                '<span class="'+cls+'">HTTP '+code+' · '+lat+'ms</span>\n'+
                '<span style="color:var(--muted)">'+esc(h)+'</span>\n'+
                esc(rb.slice(0,1500));
            addNetHist(method, path, code, lat);
            if(netChart) netChart.push(lat);
            G.emit('net', {method:method, path:path, code:code, lat:lat});
        });
    }).catch(function(e){
        var lat = Date.now()-t0;
        document.getElementById('n-resp').innerHTML =
            '<span class="c5">ERREUR ('+lat+'ms): '+esc(e.message)+'</span>';
        addNetHist(method, path, 0, lat);
        if(netChart) netChart.push(lat);
        G.emit('net', {method:method, path:path, code:0, lat:lat});
    });
}
function updNetCounts(){
    var nc = G.netCounts;
    ['2xx','4xx','5xx'].forEach(function(k){
        document.getElementById('n-'+k).textContent = nc['c'+k[0]];
    });
}
function addNetHist(method, path, code, lat){
    netHist.unshift({method:method, path:path, code:code, lat:lat});
    if(netHist.length > 60) netHist.pop();
    var cls = {2:'c2',3:'c3',4:'c4',5:'c5'}; var html=''; var i=-1;
    while(++i < netHist.length){
        var h=netHist[i]; var c=cls[Math.floor(h.code/100)]||'c4';
        html += '<div class="hitem" onclick="loadHist('+i+')">'+
                '<span class="'+c+'" style="width:36px;flex-shrink:0">'+h.code+'</span>'+
                '<span style="color:var(--blu);width:52px;flex-shrink:0">'+h.method+'</span>'+
                '<span>'+h.path+'</span>'+
                '<span style="margin-left:auto;color:var(--muted)">'+h.lat+'ms</span></div>';
    }
    document.getElementById('n-hist').innerHTML = html;
}
function loadHist(i){
    document.getElementById('n-path').value = netHist[i].path;
    document.getElementById('n-meth').value = netHist[i].method;
}
