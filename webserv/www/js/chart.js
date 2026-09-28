function makeChart(id, color, maxPts){
    var cv = document.getElementById(id); if(!cv) return null;
    var ctx = cv.getContext('2d'); var pts = [];

    function push(v){
        pts.push(v);
        if(pts.length > (maxPts||60)) pts.shift();
        draw();
    }

    function draw(){
        var w = cv.offsetWidth||400, h = cv.offsetHeight||140;
        cv.width = w; cv.height = h;
        ctx.clearRect(0,0,w,h);

        ctx.strokeStyle = 'rgba(255,255,255,.04)'; ctx.lineWidth = 1;
        var i = -1;
        while(++i < 5){ ctx.beginPath(); ctx.moveTo(0,h*i/4); ctx.lineTo(w,h*i/4); ctx.stroke(); }
        if(pts.length < 2) return;

        var mx = Math.max.apply(null, pts) || 1;

        ctx.beginPath(); ctx.moveTo(0, h);
        i = -1;
        while(++i < pts.length){ ctx.lineTo(i/(pts.length-1)*w, h-(pts[i]/mx)*h*.88); }
        ctx.lineTo(w, h); ctx.closePath();
        var g = ctx.createLinearGradient(0,0,0,h);
        g.addColorStop(0, color+'55'); g.addColorStop(1, color+'05');
        ctx.fillStyle = g; ctx.fill();

        ctx.beginPath(); i = -1;
        while(++i < pts.length){
            var x = i/(pts.length-1)*w, y = h-(pts[i]/mx)*h*.88;
            i===0 ? ctx.moveTo(x,y) : ctx.lineTo(x,y);
        }
        ctx.strokeStyle = color; ctx.lineWidth = 2;
        ctx.shadowColor = color; ctx.shadowBlur = 8;
        ctx.stroke(); ctx.shadowBlur = 0;

        ctx.fillStyle = color; ctx.font = 'bold 11px Courier New';
        ctx.textAlign = 'right';
        ctx.fillText(pts[pts.length-1].toFixed(0), w-6, 15);
        ctx.textAlign = 'left';
    }

    return { push: push };
}
