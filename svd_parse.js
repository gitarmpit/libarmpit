'use strict';

var fs = require('fs');
var util = require('util');

var parser = require('xml2json');

var fname = process.argv[2];
var data = fs.readFileSync(fname);
var json = parser.toJson(data);
var obj = JSON.parse(json);


function PerfEntry(perf, group, newGroup) {
    this.perf = perf;
    this.group = group;
    this.newGroup = newGroup;
}

var perMap = new Map();

function Group(name, desc) {
    this.name = name;
    this.desc = desc;
    this.members = [];
    this.lines = [];
}
var groups = [];

var pers = obj.device.peripherals.peripheral;
var title = obj.device.description + " " + obj.device.version;


for (let i in pers) {
    procPer(pers[i])
}

for (const v of perMap.values()) {
    if (v.newGroup) {
        var regs = v.perf.registers.register;
        if (regs.length !== undefined) {
            for (let i in regs) {
                var r = regs[i];
                procReg(r, v.group);
            }
        }
        else {
            procReg(v.perf.registers.register, v.group);
        }
    }
}

for (const g of groups) {

    var fd = fs.openSync("out/" + g.name + ".h", 'w');

    fs.writeSync(fd, util.format("#ifndef _%s_H\n", g.name));
    fs.writeSync(fd, util.format("#define _%s_H\n\n", g.name));
    fs.writeSync(fd, "#include <common.h>\n\n");
    fs.writeSync(fd, "// " + title + "\n\n");

    var members = "//";
    for (const m of g.members) {
        members += " " + m;
    }

    fs.writeSync(fd, members + "\n\n");
    fs.writeSync(fd, "// " + g.desc + "\n\n");

    for (const l of g.lines) {
        fs.writeSync(fd, l);
    }

    fs.writeSync(fd, "\n#endif\n");
    fs.closeSync(fd);
}

//////////////////////////////////////////////////////////////////////////////
function procPer(p) {
    var derivedFrom = p.derivedFrom;
    var baseAddress;
    var group;
    var name;

    if (derivedFrom != null) {
        baseAddress = p.baseAddress;
        name = p.name;
        var obj = perMap.get(derivedFrom);
        p = obj.perf;
        group = obj.group;
        group.members.push(name);
    }
    else {
        var newGroup = true;
        for (const v of perMap.values()) {
            if (v.group.name == p.groupName) {
                group = v.group;
                newGroup = false;
                break;
            }
        }
        if (newGroup) {
            var desc = p.description.replace(/\n/g, '');
            desc = desc.replace(/\s+/g, ' ');
            var groupName = p.groupName;
            if (groupName == undefined) {
                groupName = p.name;
            }
            group = new Group(groupName, desc);
            groups.push(group);
        }
        group.members.push(p.name);
        var pe = new PerfEntry(p, group, newGroup);
        perMap.set(p.name, pe);
        baseAddress = p.baseAddress;
        name = p.name;
    }

    group.lines.push(util.format("#define %s_BASE %s \n\n", name, baseAddress));
    var regs = p.registers.register
    if (regs.length !== undefined) {
        for (let i in regs) {
            var r = regs[i];
            var regname = (name + "_" + r.name).padEnd(20);
            var desc = r.description.replace(/\n/g, '');
            desc = desc.replace(/\s+/g, ' ');
            var addressOffset = r.addressOffset;
            if (addressOffset.length == 2) {
                addressOffset = addressOffset[1];
            }
            group.lines.push(util.format("#define %s (*(volatile uint32_t*)(%s_BASE + %s)) // %s\n",
                regname, name, addressOffset, desc));
        }
    }
    else {
        var regname = (name + "_" + regs.name).padEnd(20);
        var desc = regs.description.replace(/\n/g, '');
        var addressOffset = regs.addressOffset;
        if (addressOffset.length == 2) {
            addressOffset = addressOffset[1];
        }
        desc = desc.replace(/\s+/g, ' ');
        group.lines.push(util.format("#define %s (*(volatile uint32_t*)(%s_BASE + %s)) // %s\n",
            regname, name, addressOffset, desc));

    }

    group.lines.push("\n");
}

function procReg(r, group) {
    var desc = r.description.replace(/\n/g, '');
    desc = desc.replace(/\s+/g, ' ');
    var comment = "// " + r.name + ": " + desc + ". Size: " + r.size;
    group.lines.push("\n" + comment + "\n");

    var len;
    if (r.fields !== undefined) {
        var fields = r.fields.field;
        len = fields.length;
    }

    if (len !== undefined) {
        var pad = 40;
        for (let i in fields) {
            var f = fields[i];
            var regname = group.name + "_" + r.name + "_" + f.name;
            desc = f.description.replace(/\n/g, '');
            desc = desc.replace(/\s+/g, ' ');
            if (f.bitWidth == 1) {
                group.lines.push("#define " + regname.padEnd(pad) + util.format(" (1<<%s)", f.bitOffset));
                group.lines.push(" // " + desc + "\n");
            }
            else {

                var bitWidth = f.bitWidth;
                var bitOffset = f.bitOffset;
                if (bitWidth == undefined) {
                   var match = f.bitRange.match(/\d+/g);
                    if (match.length != 2) {
                        throw new Error("error parsing bit range");
                    }
                    bitWidth = match[0] - match[1] + 1;
                    bitOffset = match[1];
                }

                var len = Math.pow(2, bitWidth) - 1;
                if (len <= 4) {
                    for (let j = 0; j < len; ++j) {
                        var fullregname = regname + "_" + j;
                        group.lines.push("#define " + fullregname.padEnd(pad) + util.format(" (%d<<%s)", j, bitOffset));
                        group.lines.push(" // " + desc + "\n");
                        if (f.enumeratedValues != undefined) {
                            len = f.enumeratedValues.length;
                        }
                    }
                }
                else {
                    group.lines.push("#define " + (regname + "_OFFSET").padEnd(pad) + util.format(" %d", bitOffset));
                    group.lines.push(" // " + desc + "\n");
                }
                group.lines.push("#define " + (regname + "_BITWIDTH").padEnd(pad) + util.format(" %d\n", bitWidth));
                group.lines.push("#define " + (regname + "_CLEARMASK").padEnd(pad) + util.format(" (0x%s<<%s)\n", len.toString(16), bitOffset));
            }
            if (f.enumeratedValues != undefined) {
                if (f.enumeratedValues.enumeratedValue != undefined) {
                    len = f.enumeratedValues.enumeratedValue.length;
                    for (let i = 0; i < len; ++i) {
                        group.lines.push(" ".repeat(10 + pad) + " // " + i + " : " + f.enumeratedValues.enumeratedValue[i].name + "\n");
                    }
                }
                else {
                    console.log("undef");
                }
            }

        }
    }
}

