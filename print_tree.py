import graphviz

dot = graphviz.Digraph()

nodes = set()

with open("e3.csv", 'r') as csv:
	lines = csv.readlines()
	for l in lines:
		tokens = l.split()
		pai = tokens[0][:-1]
		filho = tokens[1]

		if not pai in nodes:
			nodes.add(pai)
			dot.node(pai)
		if not filho in nodes:
			nodes.add(filho)
			dot.node(filho)


