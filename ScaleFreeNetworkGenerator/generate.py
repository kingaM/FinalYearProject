import networkx as nx
from networkx.readwrite import json_graph
import random
from jinja2 import Template
import matplotlib.pyplot as plt
import pickle
import os


class NetworkGenerator:

    ##
    # @param nodes The number of nodes
    # @param edges The number of edges to attach from a new node to existing
    #   nodes
    # @param seeds A list of seeds for network generation
    def __init__(self, nodes, edges, seeds):
        self.nodes = nodes
        self.edges = edges
        self.seeds = seeds
        self.attackers = []
        self.sinks = []
        self.sources = []
        self.graph = None
        self.percentage = 0.2
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
    def plotGraph(self):
        if self.graph is None:
            raise Exception("The graph has not been generated")
        nx.draw(self.graph)
        plt.show()

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
        majorNodes = self.getMajorNodes()
        if len(majorNodes) < number:
            raise Exception(
                "The number of nodes requested is greater than the number of" +
                " major nodes in the graph")
        while i < number:
            a = majorNodes[random.randint(0, len(majorNodes) - 1)]
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
        print stats
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
        }
        return t.render(context)

if __name__ == "__main__":
    net = NetworkGenerator(10, 5, [1, 2, 3, 4])
    print net.populateTemplate('xyz')
