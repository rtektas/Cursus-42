var SESSION_TIMEOUT = 10*60*1000, sessInterval = null;

function getUsers(){
    var raw = localStorage.getItem('px_users');
    if(!raw) return {admin:{pass:hash('admin'), score:0}};
    return JSON.parse(raw);
}
function saveUsers(u){ localStorage.setItem('px_users', JSON.stringify(u)); }
function hash(s){ var h=5381,i=-1; while(++i<s.length) h=((h<<5)+h)^s.charCodeAt(i); return(h>>>0).toString(16); }

function createSession(u){
    var tok = (Math.random().toString(36).slice(2)) + Date.now().toString(36);
    localStorage.setItem('px_session', JSON.stringify({token:tok, user:u, lastActivity:Date.now()}));
    document.cookie = 'px_tok='+tok+'; path=/; SameSite=Strict; max-age=36000';
}
function loadSession(){
    var raw = localStorage.getItem('px_session'); if(!raw) return null;
    var s; try{ s=JSON.parse(raw); }catch(e){ return null; }
    var cm = document.cookie.match(/px_tok=([^;]+)/);
    if(!cm || cm[1] !== s.token){ clearSess(); return null; }
    if(Date.now() - s.lastActivity > SESSION_TIMEOUT){ saveState(); clearSess(); return null; }
    return s;
}
function clearSess(){ localStorage.removeItem('px_session'); document.cookie='px_tok=; path=/; max-age=0'; }
function touchSess(){
    var raw = localStorage.getItem('px_session'); if(!raw) return;
    var s; try{ s=JSON.parse(raw); }catch(e){ return; }
    s.lastActivity = Date.now(); localStorage.setItem('px_session', JSON.stringify(s));
}
['click','keydown','mousemove','input'].forEach(function(ev){
    document.addEventListener(ev, touchSess, {passive:true});
});

function startWatchdog(){
    clearInterval(sessInterval);
    sessInterval = setInterval(function(){
        var raw = localStorage.getItem('px_session'); if(!raw){ autoLogout(); return; }
        var s; try{ s=JSON.parse(raw); }catch(e){ autoLogout(); return; }
        var rem = SESSION_TIMEOUT - (Date.now() - s.lastActivity);
        if(rem <= 0){ autoLogout(); return; }
        var m = Math.floor(rem/60000), sc = Math.floor((rem%60000)/1000);
        var el = document.getElementById('sess-timer');
        el.textContent = '⏱ '+m+':'+(sc<10?'0':'')+sc;
        el.className = 'sess-badge'+(rem<120000?' warn':'');
    }, 1000);
}
function autoLogout(){
    clearInterval(sessInterval); saveState(); clearSess(); stopDos(); stopSsh();
    CURRENT_USER = null; pongStarted = false;
    document.getElementById('app').classList.remove('on');
    document.getElementById('reconnect-notice').style.display = 'block';
    document.getElementById('login-ov').classList.remove('hide');
    setErr('⏰ Session expirée — reconnectez-vous pour reprendre');
}
function saveState(){
    var s = {
        tab: currentTab,
        dosPath: (document.getElementById('dos-path')||{}).value||'/',
        tc:      (document.getElementById('tc')||{}).value||'12',
        nPath:   (document.getElementById('n-path')||{}).value||'/'
    };
    localStorage.setItem('px_state', JSON.stringify(s));
}
function restoreState(){
    var raw = localStorage.getItem('px_state'); if(!raw) return;
    var s; try{ s=JSON.parse(raw); }catch(e){ return; }
    if(s.tab !== undefined && s.tab !== currentTab) switchTab(s.tab);
    if(s.dosPath) document.getElementById('dos-path').value = s.dosPath;
    if(s.tc){ document.getElementById('tc').value=s.tc; document.getElementById('tc-lbl').textContent=s.tc; }
    if(s.nPath) document.getElementById('n-path').value = s.nPath;
    localStorage.removeItem('px_state');
}

function doLogin(){
    var u = document.getElementById('lu').value.trim();
    var p = document.getElementById('lp').value;
    if(!u || !p){ setErr('Champs requis'); return; }
    var users = getUsers();
    if(!users[u]){ setErr('Utilisateur inconnu'); return; }
    if(hash(p) !== users[u].pass){ setErr('Mot de passe incorrect'); return; }
    CURRENT_USER = u; createSession(u);
    document.getElementById('login-ov').classList.add('hide');
    document.getElementById('reconnect-notice').style.display = 'none';
    document.getElementById('app').classList.add('on');
    document.getElementById('uname').textContent = '👤 '+u;
    document.getElementById('lerr').textContent = '';
    document.getElementById('lu').value = ''; document.getElementById('lp').value = '';
    initAll(); restoreState(); startWatchdog();
}
function doRegister(){
    var u = document.getElementById('lu').value.trim();
    var p = document.getElementById('lp').value;
    var p2 = document.getElementById('lp2').value;
    if(!u || !p){ setErr('Champs requis'); return; }
    if(u.length < 2){ setErr('Nom trop court (≥ 2 car.)'); return; }
    if(p !== p2){ setErr('Mots de passe différents'); return; }
    var users = getUsers();
    if(users[u]){ setErr('Utilisateur déjà existant'); return; }
    users[u] = {pass:hash(p), score:0}; saveUsers(users);
    setErr('✅ Compte créé — connectez-vous', true);
    document.getElementById('reg-sec').style.display = 'none';
}
function setErr(msg, ok){
    var el = document.getElementById('lerr');
    el.style.color = ok ? 'var(--grn)' : 'var(--red)';
    el.textContent = msg;
}
function toggleReg(){
    var r = document.getElementById('reg-sec');
    r.style.display = r.style.display === 'none' ? 'block' : 'none';
}
function logout(){
    saveState(); clearSess(); clearInterval(sessInterval); stopDos(); stopSsh();
    CURRENT_USER = null; pongStarted = false;
    document.getElementById('app').classList.remove('on');
    document.getElementById('login-ov').classList.remove('hide');
    document.getElementById('reconnect-notice').style.display = 'none';
}
