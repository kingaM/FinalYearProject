import subprocess
import os

# To compile Java code: javac -cp .:freemarker.jar Topogen.java

iniTemplate = """
[Config {0}]
network = {0}
"""
try:
    os.remove('random.ini')
except OSError:
    pass
ini = open('random.ini', 'a')
ini.write('[General]\nsim-time-limit = 1000s\n')
for i in range(2, 10):
    network = "RandomNetwork" + str(i)
    nodes = 2**i
    edges = int(nodes * 2)
    p = subprocess.check_output(("java -cp .:freemarker.jar Topogen " +
                                 str(nodes) + " " + str(edges) + 
                                 " 101 " + network).split())
    try:
        os.remove(network + '.ned')
    except OSError:
        pass
    file = open(network + '.ned', 'a')
    file.write(p)
    file.close()
    ini.write(iniTemplate.format(network))
ini.close()
