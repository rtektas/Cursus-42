/* ── Constantes ─────────────────────────────────────────── */
var PHI     = (1 + Math.sqrt(5)) / 2;  // 1.6180339887...
var PHI_INV = PHI - 1;                 // 1/φ = 0.6180339887...  (aussi φ - 1)
var PHI_SQ  = PHI * PHI;              // φ² = φ + 1 = 2.6180339887...

/* ── Mise à jour du calculateur section dorée ───────────── */
function updateGolden(){
    var raw  = document.getElementById('m-input').value.replace(',','.');
    var val  = parseFloat(raw);
    var mode = document.getElementById('m-mode').value;
    if(isNaN(val) || val <= 0){
        document.getElementById('m-result-area').style.display = 'none';
        return;
    }
    document.getElementById('m-result-area').style.display = 'block';

    var total, major, minor;
    if(mode === 'total'){
        total = val;
        major = val / PHI;        // val × (1/φ) ≈ val × 0.618
        minor = val / PHI_SQ;     // val × (1/φ²) ≈ val × 0.382
    } else if(mode === 'major'){
        major = val;
        total = val * PHI;        // major × φ
        minor = val * PHI_INV;    // major / φ  = major × (φ-1)
    } else {                       // minor
        minor = val;
        major = val * PHI;        // minor × φ
        total = val * PHI_SQ;     // minor × φ²
    }

    document.getElementById('m-total').textContent = fmt(total);
    document.getElementById('m-major').textContent = fmt(major);
    document.getElementById('m-minor').textContent = fmt(minor);
    document.getElementById('m-r1').textContent    = fmt(total / major) + ' ≈ φ';
    document.getElementById('m-r2').textContent    = fmt(major / minor) + ' ≈ φ';

    var pctMajor = (major / total * 100);
    var pctMinor = 100 - pctMajor;
    document.getElementById('m-bar-major').style.width = pctMajor.toFixed(4) + '%';
    document.getElementById('m-bar-minor').style.width = pctMinor.toFixed(4) + '%';
    document.getElementById('m-pct').textContent =
        pctMajor.toFixed(3) + '% — ' + pctMinor.toFixed(3) + '%';

    buildMultiTable(val);
}

/* ── Table des rapports avec toutes les constantes ─────── */
function buildMultiTable(v){
    var rows = [
        { op:'× φ',     desc:'Extension dorée',      r: v * PHI        },
        { op:'÷ φ  (× 1/φ)', desc:'Réduction dorée', r: v * PHI_INV    },
        { op:'× φ²',    desc:'Double extension',      r: v * PHI_SQ     },
        { op:'÷ φ²',    desc:'Double réduction',      r: v / PHI_SQ     },
        { op:'× φ³',    desc:'Triple extension',      r: v * PHI*PHI_SQ },
        { op:'× π',     desc:'Périmètre / cercle',    r: v * Math.PI    },
        { op:'÷ π',     desc:'Rayon / périmètre',     r: v / Math.PI    },
        { op:'× 2π',    desc:'Circonférence (r=v)',   r: v * 2*Math.PI  },
        { op:'× e',     desc:'Croissance exponentielle', r: v * Math.E  },
        { op:'÷ e',     desc:'Décroissance naturelle',   r: v / Math.E  },
        { op:'× √2',    desc:'Diagonale carré (côté=v)', r: v * Math.SQRT2 },
        { op:'÷ √2',    desc:'Côté carré (diag=v)',      r: v / Math.SQRT2 },
        { op:'× √3',    desc:'Diagonale hexa / alt tri', r: v * Math.sqrt(3) },
        { op:'× √5',    desc:'Base φ (= 2φ - 1)',     r: v * Math.sqrt(5) },
        { op:'× (φ-1)', desc:'Complément (1 - 1/φ)',  r: v * (PHI - 1)  },
    ];
    var html = '';
    rows.forEach(function(row){
        html +=
            '<div class="m-row">'+
            '<span class="m-op">'  + row.op   + '</span>'+
            '<span class="m-desc">'+ row.desc  + '</span>'+
            '<span class="m-res">' + fmt(row.r) + '</span>'+
            '</div>';
    });
    document.getElementById('m-table').innerHTML = html;
}

/* ── Fibonacci / convergence vers φ ────────────────────── */
function buildFibTable(){
    var html = '';
    var a = 1, b = 1;
    for(var i = 0; i < 20; i++){
        var ratio = i > 0 ? fmt(b / a) : '—';
        var diff  = i > 0 ? fmtSci(Math.abs(b / a - PHI)) : '—';
        html +=
            '<div class="m-row">'+
            '<span class="m-op" style="width:40px;text-align:right;color:var(--muted)">F'+(i+2)+'</span>'+
            '<span class="m-desc" style="font-family:\'Courier New\',monospace;color:var(--f2)">'+b+'</span>'+
            '<span class="m-res">'+ratio+'</span>'+
            '<span style="font-size:.7rem;color:var(--muted);width:110px;text-align:right">Δφ = '+diff+'</span>'+
            '</div>';
        var c = a + b; a = b; b = c;
    }
    document.getElementById('m-fib-table').innerHTML = html;
}

function fmt(n){
    if(Math.abs(n) >= 1e9 || (Math.abs(n) < 1e-4 && n !== 0)) return n.toExponential(6);
    return n.toFixed(8).replace(/\.?0+$/, '') || '0';
}
function fmtSci(n){ return n < 1e-15 ? '0' : n.toExponential(3); }

function initMath(){
    buildFibTable();
    document.getElementById('m-input').addEventListener('input', updateGolden);
    document.getElementById('m-mode').addEventListener('change', updateGolden);
}
