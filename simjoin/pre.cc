#include "PartJoin.h"

extern int MinDictLen;
extern int MaxDictLen;
extern vector<string> dict;
extern int N;
extern char D;
extern char PN;

vector<int> idSeqMap;
char **matrix;
char **_matrix;
bool *quickRef;
int **partLen;
int **partPos;
int *dist;
int *comLen;
uint64_t *power;
InvLists **invLists;
vector<PIndex> **partIndex;

void init() {
  quickRef = new bool[N + 1];

  partPos = new int *[PN + 1];
  partLen = new int *[PN];
  invLists = new InvLists *[PN];
  partIndex = new vector<PIndex> *[PN];
  power = new uint64_t[MaxDictLen + 1];
  comLen = new int[N + 1];

  power[0] = 1;
  for (int lp = 1; lp <= MaxDictLen / PN + 2; lp++)
    power[lp] = 131 * power[lp - 1];

  for (int lp = 0; lp < PN; lp++) {
    partPos[lp] = new int[MaxDictLen + 1];
    partLen[lp] = new int[MaxDictLen + 1];
    invLists[lp] = new InvLists[MaxDictLen + 1];
    partIndex[lp] = new vector<PIndex>[MaxDictLen + 1];
  }
  partPos[PN] = new int[MaxDictLen + 1];

  dist = new int[MaxDictLen + 2];
  dist[MaxDictLen + 1] = N;
  matrix = new char *[MaxDictLen + 1];
  _matrix = new char *[MaxDictLen + 1];
  for (int lp = 0; lp <= MaxDictLen; lp++) {
    dist[lp] = -1;
    matrix[lp] = new char[2 * D + 1];
    _matrix[lp] = new char[2 * D + 1];
  }

  // init
  for (int lp = 0; lp <= D; lp++) matrix[0][D + lp] = _matrix[0][D + lp] = lp;

  for (int lp = 0; lp < N; lp++) idSeqMap.push_back(lp);

  for (int len = MinDictLen; len <= MaxDictLen; len++) {
    partPos[0][len] = 0;
    partLen[0][len] = len / PN;
    partPos[PN][len] = len;
  }
  for (int pid = 1; pid < PN; pid++) {
    for (int len = MinDictLen; len <= MaxDictLen; len++) {
      partPos[pid][len] = partPos[pid - 1][len] + partLen[pid - 1][len];
      if (pid == (PN - len % PN)) partLen[pid][len] = partLen[pid - 1][len] + 1;
      else partLen[pid][len] = partLen[pid - 1][len];
    }
  }
}

inline bool strLessT(const string &s1, const string &s2) {
  if (s1.length() < s2.length()) return true;
  else if (s1.length() > s2.length()) return false;
  return s1 < s2;
}

inline bool idSeq(const int id1, const int id2) {
  return dict[id1] < dict[id2];
}

void sortData() {
  std::sort(dict.begin(), dict.end(), strLessT);
  //std::sort(idSeqMap.begin(), idSeqMap.end(), idSeq);
}

inline bool PIndexLess(const PIndex &p1, const PIndex &p2) {
  if (p1.stPos + p1.partLen < p2.stPos + p2.partLen) return true;
  else if (p1.stPos + p1.partLen > p2.stPos + p2.partLen) return false;

  if (p1.stPos < p2.stPos) return true;
  else if (p1.stPos > p2.stPos) return false;
/*
  if (p1.Lo < p2.Lo) return true;
  else if (p1.Lo > p2.Lo) return false;

  if (p1.partLen < p2.partLen) return true;
  else if (p1.partLen > p2.partLen) return false;
*/
  return false;
}

inline int min(int a, int b, int c) {
  return (a <= b && a <= c) ? a : (b <= c ? b : c);
}

inline int max(int a, int b, int c) {
  return (a >= b && a >= c) ? a : (b >= c ? b : c);
}

void prepare() {
  int clen = 0;
  for (int id = 1; id < N; id++) {
    int lp = 0;
    for (; lp < dict[id - 1].length(); lp++)
      if (dict[id - 1][lp] != dict[id][lp]) break;
    comLen[id] = lp;
  }
  for (int id = 0; id < N; id++) {
    if (clen == dict[id].length()) continue;
    for (int lp = clen + 1; lp <= dict[id].length(); lp++) dist[lp] = id;
    clen = dict[id].length();
    comLen[id] = 0;
    // The first part
    for (int len = std::max(clen - D, MinDictLen); len <= clen; len++) {
      if (dist[len] == dist[len + 1]) continue;
      partIndex[0][clen].push_back(PIndex(0, 0, partLen[0][len], len));
    }

    // The middle parts
    for (int pid = 1; pid < PN - 1; pid++) {
      for (int len = std::max(clen - D, MinDictLen); len <= clen; len++) {
        if (dist[len] == dist[len + 1]) continue;
        for (int stPos = max(partPos[pid][len] - D + PN - pid - 1,
		 partPos[pid][len] - (D - (clen - len)) / 2,
		 clen - (len -partPos[pid][len]) - (PN - pid - 1));
            	stPos <= min(partPos[pid][len] + D - PN + pid + 1,
			 partPos[pid][len] + (D + (clen - len)) / 2,
			 clen - (len - partPos[pid][len]) + (PN - pid - 1)); stPos++)
          partIndex[pid][clen].push_back(PIndex(stPos, partPos[pid][len], partLen[pid][len], len));
      }
      sort(partIndex[pid][clen].begin(), partIndex[pid][clen].end(), PIndexLess);
    }

    // The last part
    for (int len = std::max(clen - D, MinDictLen); len <= clen; len++) {
      if (dist[len] == dist[len + 1]) continue;
      partIndex[PN - 1][clen].push_back(PIndex(clen - partLen[PN - 1][len], partPos[PN - 1][len], partLen[PN - 1][len], len));
    }
    sort(partIndex[PN - 1][clen].begin(), partIndex[PN - 1][clen].end(), PIndexLess);

#ifndef NDEBUG
    fprintf(stderr, "~~~~~~~~~~~ Current Length : %d ~~~~~~~~~~~\n", clen);
    for (int pid = 0; pid < PN; pid++) {
      fprintf(stderr, "    #### Part Id : %d ####\n", pid);
      for (int lp = 0; lp < partIndex[pid][clen].size(); lp++)
        fprintf(stderr, "stPos: %d\tendPos: %d\tpartLen: %d\tLo: %d\tlen: %d\n", partIndex[pid][clen][lp].stPos,
           partIndex[pid][clen][lp].stPos + partIndex[pid][clen][lp].partLen, partIndex[pid][clen][lp].partLen,
           partIndex[pid][clen][lp].Lo, partIndex[pid][clen][lp].len);
    }
#endif
  }
}
