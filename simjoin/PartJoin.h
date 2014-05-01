#define __STDC_FORMAT_MACROS
#include <stdint.h>
#include <string>
#include <vector>
#include <tr1/unordered_map>
#include <fstream>
#include <algorithm>
#define STORE_HASH

using std::vector;
using std::string;
using std::pair;

#if defined(STORE_HASH)
typedef std::tr1::unordered_map<uint64_t, vector<int> > InvLists;
#else
typedef std::tr1::unordered_map<string, vector<int> > InvLists;
#endif

struct PIndex {
  int stPos;
  int Lo;
  int partLen;
  int len;
  PIndex (int _s, int _o, int _p, int _l)
    : stPos(_s), Lo(_o), partLen(_p), len(_l) { }
};


/* io.h */
void readDistance(char *);
void readData(char *);

/* pre.cc */
void init();
void sortData();
void prepare();

/* join.cc */
void join();
char RTL(int xid, int yid, int xlen, int ylen, int Tau);
char LTR(int xid, int yid, int xlen, int ylen, int M, int Tau);
char CLTR(int xid, int yid, int xlen, int ylen, int xpos, int ypos, int M, int Tau);

/* free.cc */
void freeAll();
