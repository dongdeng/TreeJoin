#include "tree.h"
#include <fstream>
#include <vector>

using namespace std;

TreeNode::TreeNode() {
}

TreeNode::TreeNode(string label) {
	m_label = label;
}

TreeNode::~TreeNode() {
}

TreeNode *TreeNode::read(ifstream &fin) {
	string tag;
	getline(fin, tag);
	TreeNode *ret = new TreeNode(tag);
	int n;
	fin >> n;
	if (n > 5000)
		n = 0;
	getline(fin, tag);
	for (int i = 0; i < n; ++i)
		ret->insertChild(read(fin));
	return ret;
}

void TreeNode::readFile(char *filename) {
	ifstream fin(filename);
	string tag;
	getline(fin, tag);
	setLabel(tag);
	int n;
	fin >> n;
	getline(fin, tag);
	for (int i = 0; i < n; ++i)
		insertChild(read(fin));
}

void TreeNode::setLabel(string label) {
	m_label = label;
}

string TreeNode::getLabel() {
	return m_label;
}

bool TreeNode::insertChild(TreeNode *child) {
	m_child.push_back(child);
	return true;
}

TreeNode *TreeNode::deleteRightmostTree() {
	if (m_child.empty())
		return NULL;
	TreeNode *ret = m_child.back();
	m_child.pop_back();
	return ret;
}

TreeNode *TreeNode::deleteRightmostChild() {
	TreeNode *rightmostChild = deleteRightmostTree();
	if (rightmostChild == NULL)
		return NULL;
	vector<TreeNode*> &childList = rightmostChild->getChild();
	for (auto & i : childList)
		m_child.push_back(i);
	return rightmostChild;
}

int TreeNode::getSize() {
	return m_child.size();
}

int TreeNode::getSum() {
	int ret = 0;
	for (auto & i : m_child)
		ret += i->getSum();
	++ret;
	return ret;
}

vector<TreeNode*> &TreeNode::getChild() {
	return m_child;
}