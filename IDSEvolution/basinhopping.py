from scipy.optimize import basinhopping
from pyevolve import Util
import os
import subprocess
from multiprocessing import Process
import fnmatch


rmse_accum = Util.ErrorAccumulator()

home = "/home/kinga/Documents/GitHub/"
dirName = "RandomNetwork"
iniName = "random.ini"


def getResults():
    os.chdir(home +
             "FinalYearProject/" + dirName + "s/results")
    files = getAllScaFiles()
    results = []
    for file in files:
        results.append(getValuesFromSca(file, ""))
        file.close()
    print results
    return results


def getAllScaFiles():
    files = []
    for file in os.listdir("."):
        if fnmatch.fnmatch(file, "*.sca"):
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


def runOmnetpp(x):
    os.chdir(home + "FinalYearProject/IDSEvolution")
    tmpl = open('dc.tmpl')
    # d = dict(enumerate(x))
    path = "../DirDeff/AIS/"
    try:
        os.remove(path + 'DendricCell.cc')
    except OSError:
        pass
    evil = open(path + 'DendricCell.cc', 'a')
    src = tmpl.read()
    t = src.format(*x)
    # print t
    evil.write(t)
    tmpl.close()
    evil.close()
    os.chdir(home + "FinalYearProject/DirDeff")
    subprocess.check_output("make")
    processes = []
    for i in xrange(2, 4):
        p = Process(target=runOmnetExe, args=(i, ))
        processes.append(p)
        p.start()
    for p in processes:
        p.join()


def runOmnetExe(i):
    subprocess.check_output(("./out/gcc-debug/DirDeff" +
                             " -u Cmdenv -f ../" + dirName + "s/" + iniName +
                             " -c " + dirName + str(i) + "_0 -n ../" + dirName +
                             "s/").split())


def eval_func(x):
    global rmse_accum
    rmse_accum.reset()
    print x
    runOmnetpp(x)
    results = getResults()
    for i in xrange(0, 2):
        evaluated = (2 * results[i]['rcvd'] + results[i]['p'] +
                     results[i]['r']) / 4
        target = 1
        rmse_accum += (target, evaluated)
    print rmse_accum.getRMSE()
    return rmse_accum.getRMSE()


# basinhopping(func, x0, niter=100, T=1.0, stepsize=0.5, minimizer_kwargs=None, take_step=None, 
    # accept_test=None, callback=None, interval=50, disp=False, niter_success=None)

print basinhopping(eval_func, [0, 0, 0, 2, 2, 2, 6, 4, 4, -1, -1, -1], disp=True, niter=200,
    minimizer_kwargs={"method": "L-BFGS-B", "bounds": [(-100, 100)] * 12})
# runOmnetpp()
