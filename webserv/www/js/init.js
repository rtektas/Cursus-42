function initAll(){
    dosChart = makeChart('dos-chart', '#f85149', 60);
    netChart = makeChart('net-chart', '#58a6ff', 30);
    spChart  = makeChart('sp-chart',  '#f85149', 40);
    buildInj();
    buildNetPresets();
    initMath();
    renderLB();
}

/* Auto-login au chargement de la page */
(function(){
    var s = loadSession(); if(!s) return;
    CURRENT_USER = s.user;
    document.getElementById('login-ov').classList.add('hide');
    document.getElementById('app').classList.add('on');
    document.getElementById('uname').textContent = '👤 '+s.user;
    initAll(); restoreState(); startWatchdog();
}());
