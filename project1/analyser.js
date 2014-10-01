#!/usr/bin/nodejs
/*jslint node: true*/

var x = parseInt(process.argv[2], 10) || 1,
    y = parseInt(process.argv[3], 10) || 4;

process.stdin.setEncoding('utf8');
var data = "";
process.stdin.on('readable', function () {
    "use strict";
    var chunk = process.stdin.read();
    if (chunk !== null) {
        data += chunk;
    }
});

function avg(stat) {
    "use strict";
    var sum = stat.reduce(function (acc, i) {
        return acc + i;
    }, 0);
    return sum / stat.length;
}

function print(other) {
    "use strict";
    return function (stat, i) {
        if (stat) {
            console.log((i + "\t" + stat + "\t" + other[i]).replace(/\./g, ","));
        }
    };
}

process.stdin.on('end', function () {
    "use strict";
    
    var stat = [],
        s;
    data.split("\n").forEach(function (line) {
        var args = line.split("\t"),
            b = parseInt(args[x], 10),
            ms = parseInt(args[y], 10),
            i,
            struct;
        if (args.length < Math.max(x, y)) {
            return;
        }
        i = args[0];
        stat[i] = stat[i] || [];
        struct = stat[i];
        struct[b] = struct[b] || [];
        struct[b].push(ms);
    });
    
    for (s in stat) {
        if (stat.hasOwnProperty(s)) {
            console.error(stat[s]);
            stat[s].map(avg).forEach(print);
        }
    }
});