#include <iostream>
#include "tree.h"
#include <vector>

using namespace std;

int getMin(int a, int b) {
	if (a == -1)
		return b;
	if (b == -1)
		return a;
	return a > b ? b : a;
}

int costFunc(const string &a, const string &b) {
	int len_a = a.length(), len_b = b.length();
	vector<int> d0(len_b + 1, 0), d1(len_b + 1, 0);
	for(int i = 0; i <= len_a; ++ i) {
        for(int j = 0; j <= len_b; ++ j) {
            if(i == 0)
                d1[j] = j;
            else if(j == 0)
                d1[j] = i;
            else {
				if(a[i - 1] == b[j - 1])
                    d1[j] = d0[j - 1];
                else
                    d1[j] = d0[j - 1] + 1;
                d1[j] = min(d1[j], 1 + min(d1[j - 1], d0[j]));
            }
        }
        swap(d0, d1);
    }
    return d0[len_b];
}

int treeED(TreeNode *f1, TreeNode *f2) {
	if (f1 == NULL && f2 == NULL)
		return 0;
	int ret = -1;
	//cout << "0" << endl;
	if (f1->getSize() > 0) {
		int temp = f1->getSize();
		TreeNode *v = f1->deleteRightmostChild();
		string s = "";
		ret = getMin(ret, treeED(f1, f2) + costFunc(v->getLabel(), s));
		while (f1->getSize() > temp - 1)
			f1->deleteRightmostTree();
		f1->insertChild(v);
	}
	//cout << "1" << endl;
	if (f2->getSize() > 0) {
		int temp = f2->getSize();
		TreeNode *w = f2->deleteRightmostChild();
		string s = "";
		ret = getMin(ret, treeED(f1, f2) + costFunc(s, w->getLabel()));
		while (f2->getSize() > temp - 1)
			f2->deleteRightmostTree();
		f2->insertChild(w);
	}
	//cout << "2" << endl;
	if (f1->getSize() > 0 && f2->getSize() > 0) {
		TreeNode *v = f1->deleteRightmostTree();
		TreeNode *w = f2->deleteRightmostTree();
		ret = getMin(ret, treeED(v, w) + treeED(f1, f2) + costFunc(v->getLabel(), w->getLabel()));
		f1->insertChild(v);
		f2->insertChild(w);
	}
	if (ret == -1)
		ret = 0;
	return ret;
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
	cout << treeED(f1, f2) << endl;
}