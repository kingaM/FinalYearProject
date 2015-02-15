from string import Template
import subprocess
import os
from multiprocessing import Process
import json
import fnmatch

home = "/home/kinga/Documents/GitHub/"


def runOmnetpp(gpCode):
    os.chdir(home + "FinalYearProject/GP")
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
    os.chdir(home + "FinalYearProject/DirDeff")
    subprocess.check_output("make")
    processes = []
    for i in xrange(start, end):
        p = Process(target=runOmnetExeScaleFree, args=(i, ))
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
    for j in xrange(0, 3):
        subprocess.check_output(("./out/gcc-debug/DirDeff -u Cmdenv" +
                                 " -f ../RandomNetworks/scalefree.ini" +
                                 " -c RandomNetwork" + str(i) + "_" + str(j) +
                                 " -n ../RandomNetworks/").split())


def getResults(gpResult):
    runOmnetpp(gpResult['code'])
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
    os.chdir(home + "FinalYearProject/GP")
    results = open('resultsSummary.txt')
    data = json.load(results)['results']
    for result in data:
        if networks is None or result['name'] in networks:
            getResults(result)
    results.close()

if __name__ == "__main__":
    networks = ["results_orig", "results_data_14", "results_data_15",
                "results_16", "results_17", "results_data_18"]
    network = "ScaleFree"
    if network == "Random":
        networkDir = "RandomNetworks"
        start = 2
        end = 7
        run = runOmnetExeRandom
    elif network == "ScaleFree":
        networkDir = "ScaleFreeNetworks"
        start = 0
        end = 10
        run = runOmnetExeScaleFree
    else:
        Exception("Undefined network type")
    getAllResults(networks)
