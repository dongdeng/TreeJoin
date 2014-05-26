#include <iostream>
#include "tree.h"
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

int getMin(int a, int b) {
	if (a == -1)
		return b;
	if (b == -1)
		return a;
	return a > b ? b : a;
}

int costFunc(const string &a, const string &b) {
	if (a == b)
		return 0;
	return 1;	
}

string getPostorderedString(TreeNode *root) {
	string ret = "";
	for (auto & i : root->child)
		ret.append(getPostorderedString(i) + "$");
	ret.append(root->label);
	return ret;
}

int dfs(TreeNode *f1, TreeNode *f2, int **ans, int sum1, int sum2);

int treeED(TreeNode *f1, TreeNode *f2) {
	int n = f1->getSum(), m = f2->getSum();
	int **ans = new int*[n];
	for (int i = 0; i < n; ++i) {
		ans[i] = new int[m];
		memset(ans[i], -1, sizeof(int) * m);
	}
	dfs(f1, f2, ans, 0, 0);
	int ret = ans[0][0];
	delete ans;
	return ret;
}

int dfs(TreeNode *f1, TreeNode *f2, int **ans, int sum1, int sum2) {
	if (f1 == NULL && f2 == NULL)
		return 0;
	if (ans[sum1][sum2] != -1)
		return ans[sum1][sum2];
	int ret = -1;
	//cout << "0" << endl;
	if (f1->getSize() > 0) {
		int temp = f1->getSize();
		TreeNode *v = f1->deleteRightmostChild();
		string s = "";
		ret = getMin(ret, dfs(f1, f2, ans, sum1 + 1, sum2) + costFunc(v->label, s));
		while (f1->getSize() > temp - 1)
			f1->deleteRightmostTree();
		f1->insertChild(v);
	}
	//cout << "1" << endl;
	if (f2->getSize() > 0) {
		int temp = f2->getSize();
		TreeNode *w = f2->deleteRightmostChild();
		string s = "";
		ret = getMin(ret, dfs(f1, f2, ans, sum1, sum2 + 1) + costFunc(s, w->label);
		while (f2->getSize() > temp - 1)
			f2->deleteRightmostTree();
		f2->insertChild(w);
	}
	//cout << "2" << endl;
	if (f1->getSize() > 0 && f2->getSize() > 0) {
		TreeNode *v = f1->deleteRightmostTree();
		TreeNode *w = f2->deleteRightmostTree();
		ret = getMin(ret, treeED(v, w) + dfs(f1, f2, ans, sum1 + v->getSum(), sum2 + w->getSum()) + costFunc(v->label, w->label);
		f1->insertChild(v);
		f2->insertChild(w);
	}
	if (ret == -1)
		ret = 0;
	ans[sum1][sum2] = ret;
	return ret;
}

int generatePostorderedString(TreeNode *root, char *filename) {
	cout << "start generating postordered string" << endl;
	ofstream fout(filename);
	int count = 0;
	for (auto & i : root->child) {
		string temp = getPostorderedString(i);
		if (temp != "") {
			fout << temp << endl;
			++count;
		}
	}
	fout.close();
	cout << "geneating finished" << endl;
	return count;
}

vector<pair<string, int> > M;

void addToMap(TreeNode *root) {
	M[root->eulerString] += 1;
	for (auto & i : root->child)
		addToMap(i);
}

void addToList(vector<pair<TreeNode*, int> > &list, TreeNode *root) {
	list.push_back(make_pair(root, M[root->eulerString]));
	for (auto & i : root->child)
		addToList(list, i);
}

bool PairCompare(const pair<TreeNode*, int> &a, const pair<TreeNode*, int> &b) {
	return a.second == b.second ? (a.first)->eulerString.length() > (b.first)->eulerString.length() : a.second > b.second;
}

void TreeJoin(vector<TreeNode*> &f, int threshold, vector<pair<int, int> > &result) {
	result.clear();

	//get the map
	int n = f.size();
	for (int i = 0; i < n; ++i) {
		addToMap(f[i]);
	}

	unordered_map<string, vector<int> > L;
	for (int i = 0; i < n; ++i) {
		//get the list
		vector<pair<TreeNode*, int> > list;
		addToList(list, f[i]);
		sort(list.begin(), list.end(), PairCompare);

		//get the prefix
		int m = list.size(), num = 1;
		vector<int> flag(m, 1);
		for (k = 1; k < m; ++k) {
			for (int j = 0; j < k; ++j)
				if (list[j].first == (list[k].first)->father) {
					if (flag[j] == 1) {
						flag[j] = 0;
					}
					else {
						++num;
					}
					break;
				}
			if (num == threshold + 1)
				break;
		}

		//get the candidates
		vector<int> candidates;
		for (int j = 0; j <= k; ++j)
			if (flag[j] == 1) {
				if (L.find((list[j].first)->eulerString) != L.end()) {
					for (auto & l : L[(list[j].first)->eulerString]) {
						//some pruning techniques
					}
				}
			}

		//verification
		for (auto & j : candidates)
			if (treeED(f[i], f[j]) <= threshold) {
				result.push_back(make_pair(i, j));
			}

		//indexing all the prefix
		for (auto & j : list) {
			if (L.find((j.first)->eulerString) == L.end()) {
				vector<int> temp;
				L[(j.first)->eulerString] = temp;
			}
			L[(j.first)->eulerString].push_back(i);
		}
	}
}

int main(int argc, char **argv) {
	if (argc < 3) {
		cout << "Usage : " << endl;
		cout << "main filename1 filename2" << endl;
		return 0;
	}
	cout << "reading..." << endl;
	TreeNode *f1 = new TreeNode();
	f1->readFile(argv[1]);
	TreeNode *f2 = new TreeNode();
	f2->readFile(argv[2]);
	cout << "reading finished" << endl;

	int n = generatePostorderedString(f1, "strings.txt");
	cout << "totalNum = " << n << endl;

	vector<TreeNode*> f;
	for (int i = 0; i < n; ++i) {
		f.push_back(f1->child[i]);
		f1->child[i]->father = f;
	}
	f->calc();

	vector<pair<int, int> > result;
	for (int i = 1; i <= 20; ++i) {
		cout << "the threshold = " << i << endl;
		clock_t begin = clock();
		TreeJoin(f, i, result);
		clock_t end = clock();
		cout << "the number of the final pairs = " << result.size() << endl;
		cout << "the time of TreeJoin = " << (end - begin) / CLOCKS_PER_SEC << endl;
		cout << "-----------------------------------------------------" << endl;
	}
}
