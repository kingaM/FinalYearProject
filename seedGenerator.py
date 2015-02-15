import random
import csv


def generateSeeds(rows, columns, step):
    maxint = 2**32
    random.seed(0)
    seeds = []
    allseeds = set()
    r = 0
    for i in xrange(0, rows):
        print i
        tmp = []
        for j in xrange(0, columns):
            for k in xrange(0, step):
                r = random.randint(0, maxint)
            while r in allseeds:
                r = random.randint(0, maxint)
            allseeds.add(r)
            tmp.append(r)
        seeds.append(tmp)
    return seeds


def saveToCsv(list, path):
    f = open(path, 'wb')
    writer = csv.writer(f)
    writer.writerows(list)
    f.close()


def readFromCsv(path):
    f = open('test', 'rb')
    spamreader = csv.reader(f)
    for row in spamreader:
        print map(int, row)
    f.close()


saveToCsv(generateSeeds(30, 4, 1000), "scaleFreeNetworksSeeds.csv")
saveToCsv(generateSeeds(30, 1, 1000), "randomNetworksSeeds.csv")
saveToCsv(generateSeeds(200, 1000, 20000), "simulationSeeds.csv")
