function switchTab(n){
    /* Si le split est actif, le fermer d'abord puis afficher l'onglet demandé */
    if(splitOn){
        splitOn = false;
        document.getElementById('split').classList.remove('on');
        document.getElementById('tbs').classList.remove('act');
        /* Cacher toutes les sections proprement */
        SEC_IDS.forEach(function(id){
            document.getElementById(id).className = 'section s-hide';
        });
        currentTab = -1; /* forcer le switch même si n === ancien currentTab */
    }
    if(n === currentTab) return;
    var cur = document.getElementById('sec'+currentTab);
    if(cur){
        cur.className = 'section s-left';
        setTimeout(function(){ cur.className = 'section s-hide'; }, 450);
    }
    var nxt = document.getElementById('sec'+n);
    if(!nxt) return;
    nxt.className = 'section s-right';
    setTimeout(function(){ nxt.className = 'section s-active'; }, 50);
    currentTab = n;
    ['tb0','tb1','tb2','tb3','tb4','tb5','tb6'].forEach(function(id){
        var b = document.getElementById(id);
        if(b) b.classList.toggle('act', parseInt(id.replace('tb','')) === n);
    });
    if(n === 3) initPong();
    sshRunning = false;
    clearTimeout(sshTimer);
    try{ saveState(); }catch(e){}
}

function currentTabIs(){ return currentTab; }

function toggleSplit(){
    splitOn = !splitOn;
    var sp = document.getElementById('split');
    var tb = document.getElementById('tbs');
    if(splitOn){
        sp.classList.add('on');
        tb.classList.add('act');
        /* FIX: SEC_IDS était undefined — les sections ne se cachaient jamais */
        SEC_IDS.forEach(function(id){
            document.getElementById(id).className = 'section s-hide';
        });
        initPong();
        G.renderFeeds();
    } else {
        sp.classList.remove('on');
        tb.classList.remove('act');
        document.getElementById('sec'+currentTab).className = 'section s-active';
    }
}
