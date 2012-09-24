import sys

class HuffmanTreeNodeBase :
    def get_length(self) :
	""" returns the number of nodes and leaves under the current node """
	# return 1 as we have the current node
	return 1

class HuffmanTreeLeaf(HuffmanTreeNodeBase) :
    def __init__(self, data) :
	self.data = data

class HuffmanTreeNode(HuffmanTreeNodeBase) :
    def __init__(self) :
	self.left = None
	self.right = None

    def get_left(self) : return self.left
    def get_right(self) : return self.right
    def set_left(self, left) : self.left = left
    def set_right(self, right) : self.right = right

    def get_length(self) :
	return 1 + self.left.get_length() + self.right.get_length()

def load_huffman_table(tpath, count1) :
    f = open(tpath, "r")
    lines = f.readlines()
    f.close()

    data = []

    for l in lines :
	l = l.strip()

	# skip comments)
	if l[0] == '#' or len(l) == 0 : continue

	# make sure only one space is used to separate columns
	while l.find("  ") != -1 :
	    l = l.replace("  ", " ")

	cols = l.split(" ")
	data += [{"data" : cols[:-1], "hcode" : cols[-1]}]

    return data

def build_huffman_tree(data, root) :
    for d in data :
	node = root

	# the current bitstream
	bs = d["hcode"]

	# go through the bitstream associated with the current data
	for i in range(len(bs)) :
	    c = bs[i]
	    last = i == len(bs) - 1

	    if c == "0" :
		cnode = node.get_left()

		if last :
		    if cnode is not None : raise "shit happens"
		    node.set_left(HuffmanTreeLeaf(d["data"]))
		elif cnode :
		    node = cnode
		else :
		    node.set_left(HuffmanTreeNode())
		    node = node.get_left()
	    elif c == "1" :
		cnode = node.get_right()
		if last :
		    if cnode is not None : raise "shit happens"
		    node.set_right(HuffmanTreeLeaf(d["data"]))
		elif cnode :
		    node = cnode
		else :
		    node.set_right(HuffmanTreeNode())
		    node = node.get_right()

def generate_huffman_tree_array(node) :
    if isinstance(node, HuffmanTreeLeaf) :
	# for leaves the 'x' and 'y' member stores the real X and Y values
	return [{"type" : "D", "data" : node.data}]

    # for nodes the 'x' stores the relative distance of the left subtree in the array while 'y' stores the same value
    # for the right subtree
    result = [{"type" : "X", "left" : 1, "right" : 1 + node.get_left().get_length()}]

    result += generate_huffman_tree_array(node.get_left())
    result += generate_huffman_tree_array(node.get_right())

    return result

def print_tree(level, node) :
    indent = ""

    if level > 0 :
	indent += "|  " * (level - 1)
        indent += "+->"
	
    if isinstance(node, HuffmanTreeNode) :
	print "%sX" % indent 
	print_tree(level + 1, node.get_left())
	print_tree(level + 1, node.get_right())
    elif isinstance(node, HuffmanTreeLeaf) :
	print "%s%s" % (indent, node.data)
    else :
	print "%sNone" % indent

def test_huffman_tree_array(bta, bs) :
    pos = 0
    node = bta[pos]

    for b in bs :
	if b == "0" :
	    pos += node["x"]
	elif b == "1" :
	    pos += node["y"]

	node = bta[pos]

    return node

def format_c_array(ht_array, count1) :
    print "[%d][3] =" % len(ht_array)
    print "{"
    sys.stdout.write("   ")
    for i in range(len(ht_array)) :
	ht_item = ht_array[i]
	last = i == len(ht_array) - 1
	if i > 0 and i % 10 == 0 :
	    sys.stdout.write("\n   ")

	ending = "" if last else ","
	if ht_item["type"] == "X" :
	    sys.stdout.write(" {0, %s}%s" % (", ".join([str(ht_item["left"]), str(ht_item["right"])] + ["0" for i in range(2 if count1 else 0)]), ending))
	else :
	    sys.stdout.write(" {1, %s}%s" % (", ".join(ht_item["data"]), ending))
    print
    print "}"

def usage() :
    print "%s type input_table" % sys.argv[0]
    print
    print "Types: bigvalue count1"
    sys.exit(0)

# check command line parameters
if len(sys.argv) != 3 :
    usage()

# load the huffman table definition from the input file
huff_table = load_huffman_table(sys.argv[2], sys.argv[1] == "count1")
#print huff_table

# build huffman tree from the loaded data
ht_root = HuffmanTreeNode()
build_huffman_tree(huff_table, ht_root)

#print_tree(0, ht_root)

# generate an array that stores the generated huffman tree and can be used for decoding easily
h_array = generate_huffman_tree_array(ht_root)
#print h_array

# format the array that it can be inserted directly into C/C++ source
format_c_array(h_array, sys.argv[1] == "count1")
