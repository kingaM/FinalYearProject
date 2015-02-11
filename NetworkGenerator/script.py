import subprocess
import os

# To compile Java code: javac -cp .:freemarker.jar Topogen.java

iniTemplate = """
[Config {0}]
network = {0}
"""
path = "../RandomNetworks/"
try:
    os.remove(path + 'random.ini')
except OSError:
    pass
ini = open(path + 'random.ini', 'a')
ini.write('[General]\nsim-time-limit = 1000s\nrecord-eventlog = false\n')
for i in range(2, 9):
    for j in range(0, 3):
        network = "RandomNetwork" + str(i) + "_" + str(j)
        nodes = 2**i
        edges = int(nodes * 2)
        seed = 100 * (j + 1)
        p = subprocess.check_output(("java -cp .:freemarker.jar Topogen " +
                                     str(nodes) + " " + str(edges) +
                                     " " + str(seed) + " " + network).split())
        try:
            os.remove(path + network + '.ned')
        except OSError:
            pass
        file = open(path + network + '.ned', 'a')
        file.write(p)
        file.close()
        ini.write(iniTemplate.format(network))
ini.close()
