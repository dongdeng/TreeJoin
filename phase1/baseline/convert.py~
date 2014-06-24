import xml.etree.cElementTree as ET

def convert(root):
	print root.tag
	if len(root) == 0:
		print 1
		print root.text
		print 0
	else :
		print len(root)
	for child_of_root in root:
		convert(child_of_root)

tree = ET.ElementTree(file='dblp.xml')
root = tree.getroot()
convert(root)
#print len(root)