(function(){
    var c=document.getElementById('embers');
    var cols=['#ff6a00','#ff8c00','#ffd700','#ff4500','#ffaa00'];
    var i=-1;
    while(++i<36){
        var el=document.createElement('div');el.className='ember';
        var sz=2+Math.random()*3,col=cols[i%5];
        el.style.cssText='left:'+(Math.random()*100)+'%;animation-delay:'+(Math.random()*8)+'s;animation-duration:'+(4+Math.random()*5)+'s;width:'+sz+'px;height:'+sz+'px;background:'+col+';box-shadow:0 0 '+(sz*2)+'px '+col+';--d:'+((Math.random()-.5)*110)+'px;';
        c.appendChild(el);
    }
}());
