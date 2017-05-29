import re
import os
import sys

# types for which no getters/ setter should be generated
SKIPTYPES = ("World",
             "Route")

class ClassType:
    def __init__(self, name):
        self.name = name
        self.parent = None
        self.pure_virtual = False
        self.setters = None
        self.getters = None
    
    def __repr__(self):
        return "{}->{}".format(self.name, self.parent)

class MemberFun:
    def __init__(self, name,  type):
        self.name = name
        self.type = type
    
    def __repr__(self):
        return "{}({})".format(self.name, self.type)
    
def parse_hdr(hdr):
    # class and inherited name pattern. Uses first parent in case of multiple inheritance
    cls = re.compile(".*class ([^:^\s]+)\s*(?::\s*public\s+(\S+)\s*)?.*{.*")
    
    # inherited class pattern
    # prt = re.compile("\s*public\s+(\S+)\s*{.*")
    
    # member setter method
    setr = re.compile(".*void\s+(\S+)\(const\s*(\S+)&\s*[^,]+\s*\)")
    
    # deprected setter pattern that also matches raw pointers
    # setr = re.compile("\s*void\s+(\S+)\((?:const)?\s*(\S+)[\*&]\s*[^,]+\s*\)")
    
    # member getter method
    getr = re.compile("\s*const\s+(\S+)&\s*(\S+)\(\s*\).*const.*")
    
    # pure virtual pattern
    pvirt = re.compile(".*virtual.*=\s*0\s*;.*")
    
    f = open(hdr)
    
    n = None
    
    public_section = False
    
    setters = []
    getters = []
    
    for l in f:
        if "public:" in l:
            public_section = True
        
        if "private:" in l or "protected:" in l:
            public_section = False
        
        if pvirt.match(l):
            n.pure_virtual = True
        
        if public_section:
            m = setr.match(l)
            
            if m:
                setters.append(MemberFun(m.group(1), m.group(2)))
                continue
            
            m = getr.match(l)
            
            if m:
                getters.append(MemberFun(m.group(2), m.group(1)))
                continue
        
        # get class name and parent
        m = cls.match(l)
        
        if not m:
            continue
                
        type = m.group(1)
        
        n = ClassType(type)
        
        if m.lastindex > 1:
            n.parent = m.group(2)
        
    n.setters = setters
    n.getters = getters
    n.include = hdr
    
    return n

def reflect_typeinfo(cls_type):
    meminfo = []
    
    for setter in cls_type.setters:            
        if setter.type in SKIPTYPES:
            print("skipping", setter)
            continue
        
        meminfo.append('        ti.addSetter<{0}, &{2}::{1}>("{1}");'.format(setter.type, setter.name, cls_type.name))
    
    for getter in cls_type.getters:            
        if getter.type in SKIPTYPES:
            print("skipping", getter)
            continue
        
        meminfo.append('        ti.addGetter<{0}, &{2}::{1}>("{1}");'.format(getter.type, getter.name, cls_type.name))
    
    setter = """    {{static TypeInfo<{0}> ti("{0}", "{1}");\n{2}}}""".format(cls_type.name, cls_type.parent, "\n".join(meminfo))
    return setter

def generate_reflect_db(parsed):
    includes = []
    constructors = []
    
    nnodes = 0
    
    # node in vector of nodes
    for n in parsed:
        if n.pure_virtual:
            #print "skipping class '{}' because it is pure virtual".format(n.name)
            #continue
            pass
        
        path, header = os.path.split(n.include)
        path, module = os.path.split(path)
        
        includes.append("#include <{}/{}>".format(module, header))
        constructors.append(reflect_typeinfo(n))
        nnodes += 1
    
    #constructors.insert(0, "    _typeDB.reserve({0})\n".format(nnodes))
    #constructors.insert(0, "    _rttiDB.reserve({0})\n".format(nnodes))
    
    db_in = open("db.cpp.in").read()
    db_in = db_in.replace("<<includes>>", "\n".join(includes))
    db_in = db_in.replace("<<constructors>>", "\n".join(constructors))
    
    print("DB with {} Nodes created".format(nnodes))
    
    open("db.cpp", "w").write(db_in)
    
if __name__ == "__main__": 
    parsed = [parse_hdr(h) for h in sys.argv[1:]]
    generate_reflect_db(parsed)
        