var G = {
    feed: [],
    netCounts: {c2:0, c4:0, c5:0},
    _raf: null,
    _splitT: 0,

    emit: function(type, data){
        this.feed.unshift({type:type, data:data, ts:Date.now()});
        if(this.feed.length > 200) this.feed.pop();
        if(!this._raf){
            var self = this;
            this._raf = requestAnimationFrame(function(){
                self._raf = null;
                self.renderFeeds();
            });
        }
    },

    renderFeeds: function(){
        var html = this._buildHtml(30);
        var ids = ['feed0','feed1','feed2','feed4','sp-feed','sp-feed-net'];
        var i = -1;
        while(++i < ids.length){
            var el = document.getElementById(ids[i]);
            if(el) el.innerHTML = html;
        }
        this.syncSplit();
    },

    _buildHtml: function(limit){
        var icons = {dos:'⚡', net:'📡', inj:'💉', ssh:'🔑'};
        var html = ''; var i = -1;
        while(++i < Math.min(this.feed.length, limit)){
            var e = this.feed[i], d = e.data;
            var code = d.code || 0;
            var cls = code>=500?'c5':code>=400?'c4':code>=300?'c3':code>0?'c2':'';
            var t = new Date(e.ts);
            var ts = ('0'+t.getHours()).slice(-2)+':'+('0'+t.getMinutes()).slice(-2)+':'+('0'+t.getSeconds()).slice(-2);
            html += '<div class="fevent">'+
                    '<span class="ftime">'+ts+'</span>'+
                    '<span>'+(icons[e.type]||'•')+'</span>'+
                    '<span class="'+cls+'" style="width:38px;flex-shrink:0">'+(code||'—')+'</span>'+
                    '<span style="color:var(--muted);width:48px;flex-shrink:0">'+(d.method||'GET')+'</span>'+
                    '<span class="fpath">'+(d.path||d.url||'')+'</span>'+
                    '<span style="color:var(--muted);flex-shrink:0;width:50px;text-align:right">'+(d.lat||0)+'ms</span>'+
                    '</div>';
        }
        return html;
    },

    syncSplit: function(){
        if(!splitOn) return;
        var now = Date.now();
        if(now - this._splitT < 200) return;
        this._splitT = now;

        var map = {rps:'d-rps', tot:'d-tot', err:'d-err', lat:'d-lat'};
        var k;
        for(k in map){
            var src = document.getElementById(map[k]);
            var dst = document.getElementById('sp-'+k);
            if(src && dst) dst.textContent = src.textContent;
        }

        var nc = this.netCounts;
        ['2xx','4xx','5xx'].forEach(function(k){
            var el = document.getElementById('sp-'+k);
            if(el) el.textContent = nc['c'+k[0]];
        });

        var nr = document.getElementById('n-resp');
        var sr = document.getElementById('sp-resp');
        if(nr && sr) sr.innerHTML = nr.innerHTML;

        var iv = document.getElementById('inj-verd');
        var sv = document.getElementById('sp-verd');
        if(iv && sv){ sv.textContent = iv.textContent; sv.style.color = iv.style.color || ''; }

        var ir = document.getElementById('inj-resp');
        var si = document.getElementById('sp-inj');
        if(ir && si) si.innerHTML = ir.innerHTML;

        var ih = document.getElementById('inj-hist');
        var sh = document.getElementById('sp-inj-hist');
        if(ih && sh) sh.innerHTML = ih.innerHTML;

        var ps = document.getElementById('p-score');
        var ss = document.getElementById('sp-score');
        if(ps && ss) ss.textContent = ps.textContent;
    }
};
