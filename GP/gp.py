from pyevolve import Util
from pyevolve import GTree
from pyevolve import GSimpleGA
from pyevolve import Consts
from string import Template
import subprocess
import os
from multiprocessing import Process
import fnmatch

rmse_accum = Util.ErrorAccumulator()

home = "/home/azureuser/"
dirName = "RandomNetworks"
networkName = "RandomNetwork"


def getResults():
    os.chdir(home +
             "FinalYearProject/" + dirName + "/results")
    files = getAllScaFiles()
    results = []
    for file in files:
        results.append(getValuesFromSca(file, "Filter"))
        file.close()
    print results
    return results


def getAllScaFiles():
    files = []
    for file in os.listdir("."):
        if fnmatch.fnmatch(file, networkName + "*.sca"):
            files.append(open(file))
    return files


"""
@param type - "" for DC and "Filter" for packet filter
"""


def getValuesFromSca(f, type):
    tpf = 0
    tnf = 0
    fpf = 0
    fnf = 0
    rec = 0
    gen = 0
    for line in f:
        if "truePositive" + type + ":count" in line:
            tpf += getNumFromLineInSca(line)
        elif "trueNegative" + type + ":count" in line:
            tnf += getNumFromLineInSca(line)
        elif "falePositive" + type + ":count" in line:
            fpf += getNumFromLineInSca(line)
        elif "falseNegative" + type + ":count" in line:
            fnf += getNumFromLineInSca(line)
        elif "recievedPackets:count" in line:
            rec += getNumFromLineInSca(line)
        elif "dataGenerated:count" in line:
            gen += getNumFromLineInSca(line)
    if tpf == 0 and fpf == 0:
        p = 1
    else:
        p = float(tpf) / (tpf + fpf)
    if tpf == 0 and fnf == 0:
        r = 1
    else:
        r = float(tpf) / (tpf + fnf)
    rcvd = float(rec) / gen
    return {'p': p, 'r': r, 'rcvd': rcvd}


def getNumFromLineInSca(line):
    return int(line.split("\t")[2].split("\n")[0])


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
    for i in xrange(2, 7):
        p = Process(target=runOmnetExe, args=(i, ))
        processes.append(p)
        p.start()
    for p in processes:
        p.join()


def runOmnetExe(i):
    subprocess.check_output(("./out/gcc-debug/DirDeff" +
                             " -u Cmdenv -f ../" + dirName + "/random.ini " +
                             "-c " + networkName + str(i) + "_0 -n ../" +
                             dirName + "/").split())


def eval_func(chromosome):
    global rmse_accum
    rmse_accum.reset()
    print chromosome.getPreOrderExpression()
    runOmnetpp(chromosome.getPreOrderExpression())
    results = getResults()
    for i in xrange(0, 5):
        evaluated = (results[i]['r'] + results[i]['r'])/2
        target = 0
        rmse_accum += (target, evaluated)
    print rmse_accum.getRMSE()
    return rmse_accum.getRMSE()


def main_run():
    genome = GTree.GTreeGP()
    genome.setParams(max_depth=30, method="ramped")
    genome.evaluator += eval_func

    ga = GSimpleGA.GSimpleGA(genome)
    ga.setParams(gp_terminals=['0'],
                 gp_function_set={"wait": 1,
                                  "broadcastInterest": 1,
                                  "sendDataPacket": 1
                                  })

    ga.setMinimax(Consts.minimaxType["minimize"])
    ga.setGenerations(50)
    ga.setCrossoverRate(0.7)
    ga.setMutationRate(0.25)
    ga.setPopulationSize(100)

    ga(freq_stats=5)
    best = ga.bestIndividual()
    print best

if __name__ == "__main__":
    print "Generation: 50, Crossover Rate: 0.7, Mutation Rate: 0.25, ",
    print "Population Size: 100, Depth: 30, Sample: Random 2-6"
    print "Fitness Function: rcvd, With data"
    main_run()
