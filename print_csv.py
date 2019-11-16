import graphviz
import sys

dot = graphviz.Digraph()

nodes = set()

with open("e3.csv", 'r') as csv:
	with open("csv_map.txt", 'r') as csv_map:
		maplines = csv_map.readlines()
		mydict = dict()
		for l in maplines:
			tokens = l.split()
			mydict[tokens[0]] = tokens[1]

		lines = csv.readlines()
		for l in lines:
			tokens = l.split()
			pai = tokens[0][:-1]
			filho = tokens[1]

			if not pai in nodes:
				nodes.add(pai)
				dot.node(pai, mydict[pai])
				#dot.node(pai)
			if not filho in nodes:
				nodes.add(filho)
				#dot.node(filho)
				dot.node(filho, mydict[filho])

			dot.edge(pai, filho)


dot.render(cleanup=True, format="png", view=True)


