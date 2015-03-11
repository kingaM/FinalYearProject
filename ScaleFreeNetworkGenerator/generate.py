import networkx as nx
from networkx.readwrite import json_graph
import random
from jinja2 import Template
import matplotlib.pyplot as plt
import pickle
import os
import csv


class NetworkGenerator:

    ##
    # @param nodes The number of nodes
    # @param edges The number of edges to attach from a new node to existing
    #   nodes
    # @param seeds A list of seeds for network generation
    # @param major Boolean indicating whether the attacker should be one of the
    #   major nodes or minor nodes
    def __init__(self, nodes, edges, seeds, major=True):
        self.nodes = nodes
        self.edges = edges
        self.seeds = seeds
        self.major = major
        self.attackers = []
        self.sinks = []
        self.sources = []
        self.graph = None
        self.percentage = 0.08
        self.generateNetwork()
        self.generateSinks(1)
        self.generateSources(1)
        self.generateAttackers(1)

    ##
    # Generates a Barabasi-Albert graph
    # @return Networkx graph object
    def generateNetwork(self):
        g = nx.barabasi_albert_graph(self.nodes, self.edges, self.seeds[0])
        self.graph = g
        return g

    ##
    # @return JSON of the graph. In form:
    #   {'directed': False, 'graph': [('name', 'barabasi_albert_graph(<nodes>,
    #        <edges>, <seed>)')],
    #   'nodes': [{'id': <id:int>}],
    #   'edges': [{'source': <node:int>, 'target': <node:int>}]
    def formatGraph(self):
        if self.graph is None:
            raise Exception("The graph has not been generated")
        return json_graph.node_link_data(self.graph)

    ##
    # Plots the graph
    def plotGraph(self, path):
        if self.graph is None:
            raise Exception("The graph has not been generated")
        values = [self.colorScheme(n) for n in self.graph.nodes()]
        nx.draw_spring(self.graph, cmap='jet', node_color=values,
                       node_size=100)
        plt.savefig(path)
        plt.close()

    def colorScheme(self, x):
        if x in self.attackers:
            return 1
        elif x in self.sources:
            return 0.25
        elif x in self.sinks:
            return 0
        else:
            return 0.5

    ##
    # Saves graph to a file. Overwrites previous version of the file, if
    # such exists
    # @param path The path to the file
    def saveObject(self, path):
        try:
            os.remove(path)
        except OSError:
            pass
        f = open(path, 'a')
        pickle.dump(self, f)
        f.close()

    ##
    # Saves the degree of the nodes to the specified file in CSV format
    # @param path The path to the file
    def writeDegreesToCsv(self, path):
        f = open(path, 'a')
        w = csv.writer(f)
        w.writerow(nx.degree(self.graph).values())

    ##
    # Retrieves an object from a file
    # @param path The path to the file
    @staticmethod
    def loadObject(path):
        f = open(path, 'r')
        obj = pickle.load(f)
        f.close()
        return obj

    ##
    # Calculates the the betweeness centrality metric between nodes and
    # returns a percentage of the nodes with the highest metric.
    # @return List of nodes
    def getMajorNodes(self):
        data = nx.betweenness_centrality(self.graph)
        num = int(round(self.nodes * self.percentage))
        return sorted(data, key=data.get, reverse=True)[:num]

    ##
    # Calculates the the betweeness centrality metric between nodes and
    # returns a percentage of the nodes with the lowest metric.
    # @return List of nodes
    def getMinorNodes(self):
        data = nx.betweenness_centrality(self.graph)
        num = int(round(self.nodes * self.percentage))
        return sorted(data, key=data.get, reverse=False)[:num]

    ##
    # Returns a list of nodes that should act as attackers
    # @param number The number of attackers to set
    # @return A list of nodes that are set to attackers
    def generateAttackers(self, number):
        self.attackers = []
        random.seed(self.seeds[1])
        i = 0
        if self.major:
            nodes = self.getMajorNodes()
        else:
            nodes = self.getMinorNodes()
        if len(nodes) < number:
            raise Exception(
                "The number of nodes requested is greater than the number of" +
                " major/minor nodes in the graph")
        while i < number:
            a = nodes[random.randint(0, len(nodes) - 1)]
            g = self.graph.copy()
            g.remove_node(a)
            # We have to make sure that the graph is still connected when the
            # "attacker" node is not present, as the attacker does not forward
            # packets like the other nodes
            if (a not in self.sinks and a not in self.sources and
                    nx.is_connected(g)):
                self.attackers.append(a)
                i += 1
        return self.attackers

    ##
    # Returns a list of nodes that should act as sources
    # @param number The number of sources to set
    # @return A list of nodes that are set to sources
    def generateSources(self, number):
        self.sources = []
        random.seed(self.seeds[2])
        i = 0
        minorNodes = self.getMinorNodes()
        if len(minorNodes) < number:
            raise Exception(
                "The number of nodes requested is greater than the number of" +
                " minor nodes in the graph")
        while i < number:
            a = minorNodes[random.randint(0, len(minorNodes) - 1)]
            if a not in self.sinks and a not in self.attackers:
                self.sources.append(a)
                i += 1

        return self.sources

    ##
    # Returns a list of nodes that should act as sinks
    # @param number The number of sinks to set
    # @return A list of nodes that are set to sinks
    def generateSinks(self, number):
        self.sinks = []
        random.seed(self.seeds[3])
        i = 0
        minorNodes = self.getMinorNodes()
        if len(minorNodes) < number:
            raise Exception(
                "The number of nodes requested is greater than the number of" +
                " minor nodes in the graph")
        while i < number:
            a = minorNodes[random.randint(0, len(minorNodes) - 1)]
            if a not in self.sources and a not in self.attackers:
                self.sinks.append(a)
                i += 1

        return self.sinks

    ##
    # @return a dictionary of general statistics of the graph
    def getNodeStats(self):
        stats = {'attackers': self.attackers,
                 'sources': self.sources,
                 'sinks': self.sinks,
                 'routes': []}
        g = self.graph.copy()
        # Remove attackers from the graph to calculate metrics between the
        # sources and sinks, since the attacker will never be on the path in
        # the algorithm (due to its unusual behaviour)
        for n in self.attackers:
            g.remove_node(n)
        for sink in self.sinks:
            for source in self.sources:
                path = nx.shortest_path(g, source=sink, target=source)
                distance_from_attacker = [
                    {'attacker': a,
                     'distance':
                     self.getShortestDistance(a, path)}
                    for a in self.attackers]
                stats['routes'].append({'sink': sink, 'source': source,
                                        'distance': len(path),
                                        'distance_from_attacker':
                                            distance_from_attacker
                                        })
        return stats

    ##
    # @return The shortest distance to any of the nodes on the path from
    #   the specified node
    def getShortestDistance(self, node, path):
        dist = []
        for n in path:
            dist.append(nx.shortest_path_length(self.graph,
                                                source=n, target=node))
        return min(dist)

    ##
    # Generates a string from the template
    # @param targetName The desired name of the network
    def populateTemplate(self, targetName):
        f = open('network.tmpl')
        t = Template(f.read())
        context = {
            'graph': self.formatGraph(),
            'numNodes': self.nodes,
            'numLinks': self.edges,
            'seeds': self.seeds,
            'targetName': targetName,
            'attackers': self.attackers,
            'sources': self.sources,
            'sinks': self.sinks,
            'stats': self.getNodeStats(),
            'major': self.major
        }
        return t.render(context)


def readCsv():
    f = open('scaleFreeNetworksSeeds.csv', 'rb')
    seeds = csv.reader(f)
    result = []
    for r in seeds:
        result.append(map(int, r))
    f.close()
    return result


def generateTemplates():
    iniTemplate = """
[Config {0}]
network = {0}
**.numOfNodes = {1}
**.run = {2}
    """
    path = "../ScaleFreeNetworks/"
    try:
        os.remove(path + 'scalefree.ini')
        os.remove("degrees.csv")
    except OSError:
        pass
    ini = open(path + 'scalefree.ini', 'a')
    ini.write('[General]\nsim-time-limit = 1000s\nrecord-eventlog = false\n')
    seeds = readCsv()
    for i in range(0, 50):
        network = "ScaleFreeNetwork" + str(i)
        nodes = 2**7
        net = NetworkGenerator(nodes, 2, seeds[i], i % 2)
        p = net.populateTemplate(network)
        try:
            os.remove(path + network + '.ned')
            pass
        except OSError:
            pass
        file = open(path + network + '.ned', 'a')
        net.plotGraph(network + ".png")
        net.writeDegreesToCsv("degrees.csv")
        file.write(p)
        file.close()
        ini.write(iniTemplate.format(network,
                                     nodes + nx.number_of_edges(net.graph),
                                     i))
    ini.close()

if __name__ == "__main__":
    generateTemplates()
