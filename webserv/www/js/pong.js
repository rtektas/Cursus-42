function predict(bx, by, vx, vy, spd, ty){
    if(vy >= 0) return bx; var s = 0;
    while(by > ty && s < 800){
        bx += vx*spd; by += vy*spd;
        if(bx < .02){ vx = Math.abs(vx); bx = .02; }
        if(bx > .98){ vx = -Math.abs(vx); bx = .98; }
        s++;
    }
    return bx;
}

function initPong(){
    if(pongStarted) return; pongStarted = true;
    var cv = document.getElementById('pong-canvas'); if(!cv){ pongStarted=false; return; }
    var ctx = cv.getContext('2d'), W = cv.width, H = cv.height;
    pongMini = document.getElementById('pong-mini');

    function py(y){ return H*.07 + y*H*.86; }
    function px(x, y){ return W/2 + (x-.5)*W*(.42+y*.58); }
    function pw(w, y){ return w*W*(.42+y*.58); }

    var ball   = {x:.5, y:.5, vx:.008, vy:.007};
    var player = {x:.5, w:.18};
    var ai     = {x:.5, w:.15};
    var score  = {p:0, ai:0}, running = false, spd = 1, parts = [];
    var aiTick = 0;

    /* Écouter document — utiliser pong-mini comme référence en split view */
    document.addEventListener('mousemove', function(e){
        var ref = (splitOn && pongMini) ? pongMini : cv;
        var r = ref.getBoundingClientRect();
        player.x = Math.max(.05, Math.min(.95, (e.clientX - r.left) / r.width));
    });
    cv.addEventListener('touchmove', function(e){
        e.preventDefault();
        var r = cv.getBoundingClientRect();
        var t = e.touches[0];
        player.x = Math.max(.05, Math.min(.95, (t.clientX-r.left)/r.width));
    }, {passive:false});
    window.addEventListener('keydown', function(e){
        if(e.code === 'Space'){
            e.preventDefault(); running = !running;
            var inf = document.getElementById('p-info');
            if(inf) inf.textContent = running ? 'En cours…' : 'Pause';
            var pb = document.getElementById('pong-pause-btn');
            if(pb)  pb.textContent  = running ? '⏸ Pause'   : '▶ Reprendre';
            cv.style.cursor = running ? 'none' : 'default';
        }
    });

    function spark(x, y, n){
        var cols = ['#ff6a00','#ffd700','#ff4500','#fff']; var i=-1;
        while(++i < (n||10)) parts.push({x:x,y:y,vx:(Math.random()-.5)*7,vy:(Math.random()-.5)*7,life:1,col:cols[i%4]});
    }
    function resetBall(){
        ball.x=.5; ball.y=.5;
        ball.vx = (Math.random()>.5?1:-1)*(.007+Math.random()*.004);
        ball.vy = .006+Math.random()*.004; spd=1;
    }
    function updateAI(){
        var diff = parseInt(document.getElementById('p-diff').value), tx;
        if(diff<=3){
            tx = ball.x+(Math.random()-.5)*(.5-diff*.08); ai.x+=(tx-ai.x)*(.008+diff*.003);
        } else if(diff<=6){
            var pr = ball.vy<0 ? predict(ball.x,ball.y,ball.vx,ball.vy,spd,.06) : ball.x;
            tx = pr+(Math.random()-.5)*(.25-diff*.025); ai.x+=(tx-ai.x)*(.015+diff*.005);
        } else {
            var pr = ball.vy<0 ? predict(ball.x,ball.y,ball.vx,ball.vy,spd,.06) : ball.x;
            tx = pr+(Math.random()-.5)*Math.max(0,.12-(diff-6)*.025);
            ai.x += (tx-ai.x)*Math.min(.08+diff*.02,.35);
        }
        ai.x = Math.max(.07, Math.min(.93, ai.x));

        /* Log décision toutes les 45 frames (~0.75s) */
        if(running && ++aiTick % 45 === 0){
            var log = document.getElementById('sp-ai-log'); if(!log) return;
            var delta = tx - ai.x;
            var dir   = delta >  0.01 ? '→ droite' : delta < -0.01 ? '← gauche' : '⏸ stable';
            var col   = delta >  0.01 ? 'var(--f1)' : delta < -0.01 ? 'var(--blu)' : 'var(--muted)';
            var d = document.createElement('div');
            d.style.color = col;
            d.textContent = dir + '  cible:' + tx.toFixed(3)
                          + '  balle:(' + ball.x.toFixed(2) + ',' + ball.y.toFixed(2) + ')'
                          + '  spd:×' + spd.toFixed(1);
            log.insertBefore(d, log.firstChild);
            if(log.children.length > 30) log.removeChild(log.lastChild);
        }
    }
    function update(){
        if(!running) return; updateAI();
        ball.x += ball.vx*spd; ball.y += ball.vy*spd;
        if(ball.x<.02){ ball.vx=Math.abs(ball.vx); ball.x=.02; }
        if(ball.x>.98){ ball.vx=-Math.abs(ball.vx); ball.x=.98; }
        if(ball.y>.88&&ball.y<.96){
            var p2=player.w/2;
            if(ball.x>player.x-p2&&ball.x<player.x+p2){
                ball.vy=-Math.abs(ball.vy); ball.vx+=(ball.x-player.x)*.025;
                spd=Math.min(spd*1.04,3); spark(px(ball.x,.91),py(.91),10);
            }
        }
        if(ball.y<.12&&ball.y>.04){
            var a2=ai.w/2;
            if(ball.x>ai.x-a2&&ball.x<ai.x+a2){ ball.vy=Math.abs(ball.vy); spark(px(ball.x,.07),py(.07),6); }
        }
        if(ball.y>1.05){
            score.ai++; document.getElementById('p-score').textContent=score.p+' — '+score.ai;
            spark(px(.5,1),py(1),25); resetBall();
        }
        if(ball.y<-.05){
            score.p++; document.getElementById('p-score').textContent=score.p+' — '+score.ai;
            spark(px(.5,0),py(0),25); saveScore(score.p); resetBall();
        }
    }
    function drawParticles(){
        var alive=[]; var i=-1;
        while(++i<parts.length){
            var p=parts[i]; p.x+=p.vx; p.y+=p.vy; p.vy+=.1; p.life-=.04;
            if(p.life<=0) continue;
            ctx.globalAlpha=p.life; ctx.fillStyle=p.col;
            ctx.beginPath(); ctx.arc(p.x,p.y,3*p.life,0,Math.PI*2); ctx.fill(); alive.push(p);
        }
        ctx.globalAlpha=1; parts=alive;
    }
    function draw(){
        ctx.clearRect(0,0,W,H);
        var bg=ctx.createLinearGradient(0,0,0,H);bg.addColorStop(0,'#02020a');bg.addColorStop(1,'#0d1117');
        ctx.fillStyle=bg; ctx.fillRect(0,0,W,H);
        ctx.strokeStyle='rgba(255,106,0,.08)'; ctx.lineWidth=1;
        var i=-1;
        while(++i<=10){ ctx.beginPath(); ctx.moveTo(W/2,py(0)); ctx.lineTo((i/10)*W,H); ctx.stroke(); }
        i=-1;
        while(++i<=7){ var yy=py(i/7); ctx.beginPath(); ctx.moveTo(px(0,i/7),yy); ctx.lineTo(px(1,i/7),yy); ctx.stroke(); }
        ctx.setLineDash([4,10]); ctx.strokeStyle='rgba(255,215,0,.15)';
        ctx.beginPath(); ctx.moveTo(px(.5,0),py(0)); ctx.lineTo(px(.5,1),py(1)); ctx.stroke(); ctx.setLineDash([]);
        ctx.fillStyle='#58a6ff'; ctx.shadowColor='#58a6ff'; ctx.shadowBlur=10;
        ctx.fillRect(px(ai.x,.05)-pw(ai.w,.05)/2, py(.05)-3, pw(ai.w,.05), 6); ctx.shadowBlur=0;
        var g=ctx.createLinearGradient(0,0,0,10); g.addColorStop(0,'#ffd700'); g.addColorStop(1,'#ff4500');
        ctx.fillStyle=g; ctx.shadowColor='#ff6a00'; ctx.shadowBlur=14;
        ctx.fillRect(px(player.x,.93)-pw(player.w,.93)/2, py(.93)-5, pw(player.w,.93), 10); ctx.shadowBlur=0;
        var bx2=px(ball.x,ball.y),by2=py(ball.y),br=4+ball.y*5;
        var gr=ctx.createRadialGradient(bx2,by2,0,bx2,by2,br);
        gr.addColorStop(0,'#fff'); gr.addColorStop(.4,'#ffd700'); gr.addColorStop(1,'#ff4500');
        ctx.fillStyle=gr; ctx.shadowColor='#ffd700'; ctx.shadowBlur=18;
        ctx.beginPath(); ctx.arc(bx2,by2,br,0,Math.PI*2); ctx.fill(); ctx.shadowBlur=0;
        drawParticles();
        ctx.textAlign='center'; ctx.font='bold 13px system-ui';
        ctx.fillStyle='rgba(88,166,255,.5)'; ctx.fillText('IA',W/2,py(.02));
        ctx.fillStyle='rgba(255,215,0,.5)';  ctx.fillText('VOUS',W/2,py(.98)+16); ctx.textAlign='left';
        if(!running){
            ctx.fillStyle='rgba(13,17,23,.5)'; ctx.fillRect(0,0,W,H);
            ctx.fillStyle='#ffd700'; ctx.font='bold 18px system-ui'; ctx.textAlign='center';
            ctx.fillText('[ ESPACE ]',W/2,H/2); ctx.textAlign='left';
        }
        if(pongMini){ var mc=pongMini.getContext('2d'); mc.drawImage(cv,0,0,pongMini.width,pongMini.height); }
        var ss=document.getElementById('sp-score'); if(ss) ss.textContent=score.p+' — '+score.ai;
    }
    function loop(){ update(); draw(); requestAnimationFrame(loop); }
    loop();
}

function saveScore(s){
    if(!CURRENT_USER) return;
    var users = getUsers(); if(!users[CURRENT_USER]) return;
    if(s > (users[CURRENT_USER].score||0)){ users[CURRENT_USER].score=s; saveUsers(users); renderLB(); }
}
function renderLB(){
    var users = getUsers();
    var entries = Object.keys(users).map(function(u){ return {name:u, score:users[u].score||0}; });
    entries.sort(function(a,b){ return b.score-a.score; });
    var m=['🥇','🥈','🥉']; var html=''; var i=-1;
    while(++i < Math.min(entries.length,8)){
        html += '<div class="lb-item"><span>'+(m[i]||'')+' '+entries[i].name+'</span><span style="color:var(--f2);font-weight:600">'+entries[i].score+'</span></div>';
    }
    var el = document.getElementById('p-lb'); if(el) el.innerHTML=html;
}
function pausePong(){
    if(!pongStarted) return;
    window.dispatchEvent(new KeyboardEvent('keydown', {code:'Space', bubbles:true}));
}
function resetPong(){
    pongStarted=false; pongMini=null;
    var sc=document.getElementById('p-score'); if(sc) sc.textContent='0 — 0';
    var inf=document.getElementById('p-info'); if(inf) inf.textContent='Tape / Espace pour démarrer';
    var pb=document.getElementById('pong-pause-btn'); if(pb) pb.textContent='▶ Démarrer';
    initPong();
}
