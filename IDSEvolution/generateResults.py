# Python script to generate omnet++ results from IDS evolution results

from string import Template
import subprocess
import os
from multiprocessing import Process
import json
import fnmatch

home = "/home/kinga/Documents/GitHub/"


def populateAttacker(network):
    os.chdir(home + "FinalYearProject/GP")
    results = open('resultsSummary.txt')
    data = json.load(results)['results']
    for result in data:
        if result['name'] == network:
            gpCode = result["code"]
    results.close()
    tmpl = open('evilNode.tmpl')
    d = {'gp': gpCode}
    path = "../DirDeff/"
    try:
        os.remove(path + 'EvilNode.cc')
    except OSError:
        pass
    evil = open(path + 'EvilNode.cc', 'a')
    src = Template(tmpl.read())
    evil.write(src.substitute(d))
    tmpl.close()
    evil.close()


def runOmnetpp(x):
    os.chdir(home + "FinalYearProject/IDSEvolution")
    tmpl = open('dc.tmpl')
    path = "../DirDeff/AIS/"
    try:
        os.remove(path + 'DendricCell.cc')
    except OSError:
        pass
    evil = open(path + 'DendricCell.cc', 'a')
    src = tmpl.read()
    t = src.format(*x)
    evil.write(t)
    tmpl.close()
    evil.close()
    populateAttacker("results_16")
    os.chdir(home + "FinalYearProject/DirDeff")
    print subprocess.check_output("make")
    processes = []
    for i in xrange(start, end):
        p = Process(target=run, args=(i, ))
        processes.append(p)
        p.start()
    for p in processes:
        p.join()


def runOmnetExeRandom(i):
    for j in xrange(0, 3):
        subprocess.check_output(("./out/gcc-debug/DirDeff" +
                                 " -u Cmdenv -f ../RandomNetworks/random.ini" +
                                 " -c RandomNetwork" + str(i) + "_" + str(j) +
                                 " -n ../RandomNetworks/").split())


def runOmnetExeScaleFree(i):
    subprocess.check_output(("./out/gcc-debug/DirDeff -u Cmdenv" +
                             " -f ../ScaleFreeNetworks/scalefree.ini" +
                             " -c ScaleFreeNetwork" + str(i) +
                             " -n ../ScaleFreeNetworks/").split())


def getResults(gpResult):
    runOmnetpp(gpResult['weights'])
    os.chdir(home + "FinalYearProject/" + networkDir)
    try:
        subprocess.check_output(("rm -rf " + gpResult['name']).split())
    except Exception, e:
        print e
    subprocess.check_output(("cp -r results/ " + gpResult['name']).split())
    updateIndex(gpResult['name'])


def updateIndex(dir):
    for file in os.listdir(dir + "/"):
        if fnmatch.fnmatch(file, "*.vec"):
            subprocess.check_output(
                ("scavetool index " + dir + "/" + file).split())


def getAllResults(networks=None):
    os.chdir(home + "FinalYearProject/IDSEvolution")
    results = open('resultsSummary.json')
    data = json.load(results)['results']
    for result in data:
        if networks is None or result['name'] in networks:
            getResults(result)
    results.close()

if __name__ == "__main__":
    network = "ScaleFree"
    if network == "Random":
        networkDir = "RandomNetworks"
        start = 2
        end = 8
        run = runOmnetExeRandom
    elif network == "ScaleFree":
        networkDir = "ScaleFreeNetworks"
        start = 0
        end = 50
        run = runOmnetExeScaleFree
    else:
        Exception("Undefined network type")
    getAllResults()
