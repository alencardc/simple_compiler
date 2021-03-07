# How to use:
# Run make
# Run ./etapa3 < {file_path} > tests/ast/out.txt | python3 tests/ast/visualizer.py tests/ast/out.txt
# Where {file_path} is the test file path
# Image output at tests/ast/tree.png

import sys
from graphviz import Source

out_f = open(sys.argv[1], "r", encoding="utf8")

graph_description = "digraph G {\n"
dict_nodes = {}
node_count = 0

for line in out_f:
  if "[" not in line:
    nodes = line.replace("\n", "").split(", ")
    for node in nodes:
      if node not in dict_nodes.keys():
        dict_nodes[str(node)] = "n" + str(node_count)
        node_count += 1
        
    new_line = line.replace(", ", " -> ")
    for node in nodes:
      new_line = new_line.replace(node, dict_nodes[str(node)])

    graph_description = graph_description + new_line.replace("\n", "") + ";\n"
  else:
    node = line.split(" ")[0]
    new_line = line.replace(node, dict_nodes[str(node)])
    graph_description = graph_description + new_line

graph_description = graph_description + "\n}"
source = Source(graph_description)
source.render("tests/ast/tree", format="png", cleanup=True)