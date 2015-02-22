import subprocess
import os
import csv

# To compile Java code: javac -cp .:freemarker.jar Topogen.java


def readCsv():
    f = open('randomNetworksSeeds.csv', 'rb')
    seeds = csv.reader(f)
    result = []
    for r in seeds:
        result.append(map(int, r))
    f.close()
    return result

iniTemplate = """
[Config {0}]
network = {0}
**.numOfNodes = {1}
**.run = {2}
"""
path = "../RandomNetworks/"
try:
    os.remove(path + 'random.ini')
except OSError:
    pass
ini = open(path + 'random.ini', 'a')
ini.write('[General]\nsim-time-limit = 1000s\nrecord-eventlog = false\n')
seeds = readCsv()
for i in range(2, 8):
    for j in range(0, 5):
        network = "RandomNetwork" + str(i) + "_" + str(j)
        nodes = 2**i
        seed = seeds[(8 * j + i - 2)][0]
        print seed
        p = subprocess.check_output(("java -cp .:freemarker.jar Topogen " +
                                     str(nodes) +
                                     " " + str(seed) + " " + network).split())
        p = p.rsplit("\n", 2)
        try:
            os.remove(path + network + '.ned')
            pass
        except OSError:
            pass
        file = open(path + network + '.ned', 'a')
        file.write(p[0])
        file.close()
        ini.write(iniTemplate.format(network, nodes + int(p[1]),
                  (8 * j + i - 2)))
ini.close()
