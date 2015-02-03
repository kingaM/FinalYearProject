from pyevolve import Util
from pyevolve import GTree
from pyevolve import GSimpleGA
from pyevolve import Consts
import math
from string import Template
import subprocess
import os

rmse_accum = Util.ErrorAccumulator()

def getResults():
   os.chdir("/home/kinga/Documents/GitHub/FinalYearProject/GP")
   x = subprocess.check_output("Rscript ../resultAnalysis.R".split())
   p = x.split("\n")[0].split(" ")[1:]
   r = x.split("\n")[1].split(" ")[1:]
   return (p, r)

def runOmnetpp(gpCode):
   os.chdir("/home/kinga/Documents/GitHub/FinalYearProject/GP")
   tmpl = open('evilNode.tmpl')
   d = {'gp' : gpCode}
   path = "../DirDeff/"
   try:
       os.remove(path + 'evilNode.cc')
   except OSError:
       pass
   evil = open(path + 'evilNode.cc', 'a')
   src = Template(tmpl.read())
   evil.write(src.substitute(d))
   tmpl.close()
   evil.close()
   os.chdir("/home/kinga/Documents/GitHub/FinalYearProject/DirDeff")
   # subprocess.check_output("make clean".split())
   # subprocess.check_output(("make -f makemakefiles").split())
   subprocess.check_output("make")
   for i in xrange(2, 4):
      subprocess.check_output(("./out/gcc-debug/DirDeff" + 
         " -u Cmdenv -f ../RandomNetworks/random.ini " + 
         "-c RandomNetwork" + str(i) + " -n ../RandomNetworks/").split())
      return 0
   
def eval_func(chromosome):
   global rmse_accum
   rmse_accum.reset()
   print chromosome.getPreOrderExpression()
   code_comp = chromosome.getCompiledCode()
   result = runOmnetpp(chromosome.getPreOrderExpression())
   rmse_accum += (0, 1)
   results = getResults()
   for a in xrange(0, 2):
         evaluated     = (float(results[0][a]) + float(results[1][a])) / 2
         target        = 0
         rmse_accum   += (target, evaluated)

   return rmse_accum.getRMSE()

def main_run():
   genome = GTree.GTreeGP()
   genome.setParams(max_depth=15, method="ramped")
   genome.evaluator += eval_func

   ga = GSimpleGA.GSimpleGA(genome)
   ga.setParams(gp_terminals = ['0', '4'],
      gp_function_set = {"wait" : 1, "broadcastInterest" : 1})

   ga.setMinimax(Consts.minimaxType["minimize"])
   ga.setGenerations(50)
   ga.setCrossoverRate(1.0)
   ga.setMutationRate(0.25)
   ga.setPopulationSize(80)
   
   ga(freq_stats=10)
   best = ga.bestIndividual()
   print best

if __name__ == "__main__":
   main_run()
