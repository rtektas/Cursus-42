/* ── Upload ─────────────────────────────────────────────── */
function getFilesHost(){
    var sel = document.getElementById('f-host-preset');
    return sel.value === 'custom' ? document.getElementById('f-host-custom').value : sel.value;
}
function filesPresetChange(){
    var sel = document.getElementById('f-host-preset');
    var f   = document.getElementById('f-host-custom-field');
    f.style.display = sel.value === 'custom' ? 'block' : 'none';
}

function uploadFile(){
    var input = document.getElementById('f-upload-input');
    var path  = document.getElementById('f-upload-path').value || '/upload';
    var host  = getFilesHost();
    if(!input.files.length){
        setUploadResp('⚠ Aucun fichier sélectionné — utilise le bouton "Choisir un fichier"', 'var(--f2)');
        return;
    }
    var file = input.files[0];
    var url  = host + path;
    setUploadResp('⏳ Envoi de ' + file.name + ' vers ' + url + '…', 'var(--muted)');

    var formData = new FormData();
    formData.append('file', file, file.name);

    var xhr = new XMLHttpRequest();
    xhr.open('POST', url);
    /* Ne pas fixer Content-Type : le navigateur ajoute
       "multipart/form-data; boundary=..." automatiquement,
       requis par le parsing serveur (build_upload.cpp). */

    xhr.upload.onprogress = function(e){
        if(e.lengthComputable){
            var pct = Math.round(e.loaded / e.total * 100);
            document.getElementById('f-upload-bar').style.width = pct + '%';
            setUploadResp('⏳ ' + pct + '% — ' + fmtSize(e.loaded) + ' / ' + fmtSize(e.total), 'var(--muted)');
        }
    };
    xhr.onload = function(){
        document.getElementById('f-upload-bar').style.width = xhr.status >= 200 && xhr.status < 300 ? '100%' : '0%';
        var ok  = xhr.status >= 200 && xhr.status < 300;
        var hdr = '';
        /* Afficher les headers de réponse */
        try { hdr = '\n─── Headers ───\n' + xhr.getAllResponseHeaders(); } catch(e){}
        var body = xhr.responseText ? '\n─── Body ───\n' + xhr.responseText.slice(0, 500) : '';
        setUploadResp(
            (ok ? '✅' : '🔴') + ' HTTP ' + xhr.status + ' ' + xhr.statusText +
            '\nURL  : POST ' + url +
            '\nFichier : ' + file.name + '  (' + fmtSize(file.size) + ')' +
            hdr + body,
            ok ? 'var(--grn)' : 'var(--red)'
        );
        addFileHist('UP', file.name, xhr.status, path);
    };
    xhr.onerror = function(){
        setUploadResp('🔴 Erreur réseau ou CORS bloqué\nVérifie que le serveur tourne sur ' + host, 'var(--red)');
    };
    xhr.send(formData);
}

function setUploadResp(msg, color){
    var el = document.getElementById('f-upload-resp'); if(!el) return;
    el.textContent = msg; el.style.color = color;
}

/* ── Download ────────────────────────────────────────────── */
function downloadFile(){
    var path     = document.getElementById('f-dl-path').value;
    var filename = document.getElementById('f-dl-name').value || path.split('/').pop() || 'fichier';
    var host     = getFilesHost();
    if(!path){ setFileMsg('dl', '⚠ Chemin vide', 'var(--f2)'); return; }
    setFileMsg('dl', '⏳ Téléchargement de ' + path + '…', 'var(--muted)');
    var t0 = Date.now();
    fetch(host + path, {method:'GET', cache:'no-store'}).then(function(r){
        var lat = Date.now()-t0;
        if(!r.ok){ setFileMsg('dl', '🔴 HTTP ' + r.status + ' (' + lat + 'ms)', 'var(--red)'); return; }
        return r.blob().then(function(blob){
            var url = URL.createObjectURL(blob);
            var a   = document.createElement('a');
            a.href = url; a.download = filename; a.click();
            URL.revokeObjectURL(url);
            setFileMsg('dl', '✅ HTTP ' + r.status + ' — ' + fmtSize(blob.size) + ' (' + lat + 'ms)', 'var(--grn)');
            addFileHist('DL', filename, r.status, path);
        });
    }).catch(function(e){
        setFileMsg('dl', '🔴 Erreur: ' + e.message, 'var(--red)');
    });
}

/* ── Helpers ─────────────────────────────────────────────── */
var fileHist = [];
function setFileMsg(type, msg, color){
    var el = document.getElementById('f-'+type+'-msg'); if(!el) return;
    el.textContent = msg; el.style.color = color;
}
function fmtSize(n){
    if(n < 1024) return n + ' o';
    if(n < 1048576) return (n/1024).toFixed(1) + ' Ko';
    return (n/1048576).toFixed(2) + ' Mo';
}
function addFileHist(type, name, code, path){
    fileHist.unshift({type:type, name:name, code:code, path:path});
    if(fileHist.length > 30) fileHist.pop();
    var cls = {2:'c2',3:'c3',4:'c4',5:'c5'};
    var html = ''; var i = -1;
    while(++i < fileHist.length){
        var h = fileHist[i]; var c = cls[Math.floor(h.code/100)]||'c4';
        html += '<div class="hitem">'+
                '<span style="color:var(--muted);width:28px;flex-shrink:0">'+h.type+'</span>'+
                '<span class="'+c+'" style="width:36px;flex-shrink:0">'+h.code+'</span>'+
                '<span>'+h.name+'</span>'+
                '<span style="margin-left:auto;color:var(--muted);font-size:.7rem">'+h.path+'</span>'+
                '</div>';
    }
    document.getElementById('f-hist').innerHTML = html;
}
function fileDragOver(e){ e.preventDefault(); document.getElementById('f-dropzone').classList.add('dz-over'); }
function fileDragLeave(){ document.getElementById('f-dropzone').classList.remove('dz-over'); }
function fileDrop(e){
    e.preventDefault(); fileDragLeave();
    var files = e.dataTransfer.files;
    if(files.length){ document.getElementById('f-upload-input').files = files; fileInputChange(); }
}
function fileInputChange(){
    var input    = document.getElementById('f-upload-input');
    var selected = document.getElementById('f-selected');
    var noFile   = document.getElementById('f-no-file');
    if(input.files.length){
        var f = input.files[0];
        document.getElementById('f-file-name').textContent = f.name;
        document.getElementById('f-file-size').textContent = fmtSize(f.size) + '  —  ' + (f.type || 'type inconnu');
        selected.style.display = 'flex';
        noFile.style.display   = 'none';
    } else {
        selected.style.display = 'none';
        noFile.style.display   = 'block';
    }
    document.getElementById('f-upload-bar').style.width = '0%';
    var path  = (document.getElementById('f-upload-path')||{}).value || '/upload';
    var urlEl = document.getElementById('f-upload-url');
    if(urlEl) urlEl.textContent = '→ POST ' + getFilesHost() + path;
}
