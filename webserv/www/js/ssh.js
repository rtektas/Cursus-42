var sshPairs = [], sshIdx = 0, sshFound = [];

function toggleSsh(){ sshRunning ? stopSsh() : startSsh(); }

function startSsh(){
    var users  = document.getElementById('ssh-users').value.trim().split('\n').filter(Boolean);
    var passes = document.getElementById('ssh-passes').value.trim().split('\n').filter(Boolean);
    sshPairs = [];
    users.forEach(function(u){ passes.forEach(function(p){ sshPairs.push({u:u.trim(), p:p.trim()}); }); });
    if(!sshPairs.length) return;
    sshIdx = 0; sshFound = []; sshStats = {att:0, hit:0, fail:0}; sshRunning = true;
    var btn = document.getElementById('ssh-btn'); btn.textContent='■ Stopper'; btn.className='btn btn-stop';
    var st  = document.getElementById('ssh-st');  st.textContent='▶ En cours'; st.className='status-badge status-running';
    document.getElementById('ssh-log').innerHTML = '';
    document.getElementById('ssh-found').textContent = 'Aucune pour l\'instant';
    sshStep();
}
function sshStep(){
    if(!sshRunning || sshIdx >= sshPairs.length){ stopSsh(); return; }
    var pair  = sshPairs[sshIdx++];
    var host  = document.getElementById('ssh-host').value.trim();
    var port  = document.getElementById('ssh-port').value.trim();
    var delay = parseInt(document.getElementById('ssh-delay').value);
    document.getElementById('ssh-current').textContent = host+':'+port+' → '+pair.u+' / '+pair.p;
    sshStats.att++;
    document.getElementById('ssh-att').textContent = sshStats.att;
    var t0 = Date.now();
    fetch('http://'+host+':'+port+'/', {mode:'no-cors', cache:'no-store'})
    .then(function(){
        var lat = Date.now()-t0; sshStats.hit++;
        document.getElementById('ssh-hit').textContent = sshStats.hit;
        sshFound.push(pair.u+':'+pair.p);
        document.getElementById('ssh-found').textContent = sshFound.join('  |  ');
        sshLog('[RÉPONSE] '+pair.u+':'+pair.p+' ('+lat+'ms)', 'var(--grn)');
        G.emit('ssh', {method:'SSH', path:host+':'+port, code:200, lat:lat});
    }).catch(function(e){
        var lat = Date.now()-t0; sshStats.fail++;
        document.getElementById('ssh-fail').textContent = sshStats.fail;
        var timeout = e.name === 'TimeoutError' || lat > 2900;
        sshLog((timeout?'[TIMEOUT] ':'[REFUSÉ]  ')+pair.u+':'+pair.p+' ('+lat+'ms)', 'var(--muted)');
        G.emit('ssh', {method:'SSH', path:host+':'+port, code:timeout?408:403, lat:lat});
    }).finally(function(){ if(sshRunning) sshTimer = setTimeout(sshStep, delay); });
}
function stopSsh(){
    sshRunning = false; clearTimeout(sshTimer);
    var btn = document.getElementById('ssh-btn'); btn.textContent='▶ Lancer'; btn.className='btn btn-teal';
    var st  = document.getElementById('ssh-st');
    st.textContent = sshStats.att > 0 ? 'Terminé — '+sshStats.att+' tentatives' : 'Inactif';
    st.className = 'status-badge '+(sshStats.att > 0 ? 'status-ok' : 'status-idle');
}
function sshLog(msg, color){
    var log = document.getElementById('ssh-log'); if(!log) return;
    var t = new Date();
    var ts = ('0'+t.getHours()).slice(-2)+':'+('0'+t.getMinutes()).slice(-2)+':'+('0'+t.getSeconds()).slice(-2);
    var d = document.createElement('div'); d.style.color = color||'var(--muted)';
    d.textContent = '['+ts+'] '+msg; log.insertBefore(d, log.firstChild);
    if(log.children.length > 100) log.removeChild(log.lastChild);
}
