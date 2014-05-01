// TODO: remove index
#include "PartJoin.h"

#define UINIT -1
extern char **matrix;
extern char **_matrix;
extern int MaxDictLen;
extern int MinDictLen;
extern vector<string> dict;
extern vector<PIndex> **partIndex;
extern InvLists **invLists;
extern bool *quickRef;
extern int **partPos;
extern int **partLen;
extern uint64_t *power;
extern int *dist;
extern int *comLen;
extern int N;
extern char PN;
extern char D;

uint64_t candNum = 0;
uint64_t realNum = 0;
vector<int> cands;

inline uint64_t strHash(const string &str, int stPos, int len) {
  uint64_t __h = 0; int i = 0;
  while (i < len) __h = __h * 131 + str[stPos + i++];
  return __h;
}

inline char min(char a, char b, char c) {
  return (a <= b && a <= c) ? a : (b <= c ? b : c);
}

bool valid;
int lt, rt;
int left, right;
int _left, _right;
int prevNum, prevId;
int _prevNum, _prevId;

struct hashValue {
  int id;
  int stPos;
  int enPos;
  uint64_t value;
  hashValue(int in_id, int in_stPos, int in_enPos, uint64_t in_value)
    : id(in_id), stPos(in_stPos), enPos(in_enPos), value(in_value) { }
  hashValue(int in_id, int in_stPos, int in_enPos)
    : id(in_id), stPos(in_stPos), enPos(in_enPos) 
  {
    value = strHash(dict[id], stPos, enPos - stPos + 1);
  }
  hashValue() { }
  void update(int newid, int newStPos, int newEnPos) {
    if (newStPos > stPos + 1 || newEnPos > enPos + 1 || newid != id) {
      enPos = newEnPos;
      stPos = newStPos;
      id = newid;
      value = strHash(dict[id], stPos, enPos - stPos + 1);
    } else {
      if (newStPos == stPos + 1) {
        value = value - dict[id][stPos] * power[enPos - stPos];
        stPos = newStPos;
      }
      if (newEnPos == enPos + 1) {
        value = value * 131 + dict[id][newEnPos];
        enPos = newEnPos;
      }
    }
  }
};

void join() {
  InvLists::iterator inv_it;
  vector<int>::iterator vit;
  hashValue hv[PN];
  int len, stPos, pLen, Lo;
  int id = 0;
  int partId;

  for (int clen = MinDictLen; clen <= MaxDictLen; clen++) {
    for (; id < dist[clen + 1]; id++) { // join
      // for the first partition, there is no need to shift
      partId = 0;
      for (int lp = 0; lp < partIndex[partId][clen].size(); lp++) {
        len = partIndex[partId][clen][lp].len;
        Lo = partIndex[partId][clen][lp].Lo;
        pLen = partIndex[partId][clen][lp].partLen;
        stPos = partIndex[partId][clen][lp].stPos;

        hv[partId].update(id, stPos, stPos + pLen - 1);
        inv_it = invLists[partId][len].find(hv[partId].value);

        if (inv_it != invLists[partId][len].end()) {
          vit = inv_it->second.begin();
          _prevNum = -1;
          _prevId = -1;
          _left = (D + (len - clen)) / 2;
          _right = (D - (len - clen)) / 2;
          while (vit != inv_it->second.end()) {
            if (CLTR(*vit, id, len - pLen, clen - pLen, pLen, pLen, 1, D) <= D) {
              realNum++;
              quickRef[*vit] = 1;
              cands.push_back(*vit);
            }
            ++vit;
          }
        }
      }

      // for the middle parts, do shift stPos
      for (partId = 1; partId < PN - 1; partId++) {
        int lpn = PN - partId - 1;
        for (int lp = 0; lp < partIndex[partId][clen].size(); lp++) {
          len = partIndex[partId][clen][lp].len;
          Lo = partIndex[partId][clen][lp].Lo;
          pLen = partIndex[partId][clen][lp].partLen;
          stPos = partIndex[partId][clen][lp].stPos;

          hv[partId].update(id, stPos, stPos + pLen - 1);
          inv_it = invLists[partId][len].find(hv[partId].value);

          if (inv_it != invLists[partId][len].end()) {
            vit = inv_it->second.begin();
            int M = D - abs(len - Lo - clen + stPos);
            if (M > D - lpn) M = D - lpn;
            prevNum = -1;
            prevId = -1;
            int pastId = -2;
            int red = -1;
            left = (M + (Lo - stPos)) / 2;
            right = (M - (Lo - stPos)) / 2;
            while (vit != inv_it->second.end()) {
              // if (comLen[*vit] < Lo + pLen) {
              if (pastId + 1 != *vit || comLen[*vit] < Lo + pLen) {
                if (quickRef[*vit]) {
                  red = -1;
                  pastId = *vit;
                  ++vit;
                  continue;
                }
                red = RTL(*vit, id, Lo, stPos, M);
                if (red <= M) {
                  _left = (D - red + (len - Lo - clen + stPos)) /2 ;
                  _right = (D - red - (len - Lo - clen + stPos)) / 2;
                  _prevNum = _prevId = -1;
                }
              }
              if (quickRef[*vit]) {
                pastId = *vit;
                ++vit;
                continue;
              }
              if (red == -1) {
                red = RTL(*vit, id, Lo, stPos, M);
                if (red <= M) {
                  _prevNum = _prevId = -1;
                  _left = (D - red + (len - Lo - clen + stPos)) /2 ;
                  _right = (D - red - (len - Lo - clen + stPos)) / 2;
                }
              }
              if (red <= M) {
                if (CLTR(*vit, id, len - Lo - pLen, clen - stPos - pLen, Lo + pLen, stPos + pLen,
                      D - red - lpn + 1, D - red) <= D - red) {
                  quickRef[*vit] = 1;
                  cands.push_back(*vit);
                  realNum++;
                }
              }
              pastId = *vit;
              ++vit;
            }
          }
        }
      }

      // for the last partition, no need to shift
      partId = PN - 1;
      for (int lp = 0; lp < partIndex[partId][clen].size(); lp++) {
        len = partIndex[partId][clen][lp].len;
        Lo = partIndex[partId][clen][lp].Lo;
        pLen = partIndex[partId][clen][lp].partLen;
        stPos = partIndex[partId][clen][lp].stPos;

        hv[partId].update(id, stPos, stPos + pLen - 1);
        inv_it = invLists[partId][len].find(hv[partId].value);

        if (inv_it != invLists[partId][len].end()) {
          vit = inv_it->second.begin();
          prevNum = -1;
          prevId = -1;
          left = (D + (len - clen)) / 2;
          right = (D - (len - clen)) / 2;
          while (vit != inv_it->second.end()) {
            if (!quickRef[*vit]) {
              if (RTL(*vit, id, len - pLen, clen - pLen, D) <= D){
                realNum++;
              }
            }
            ++vit;
          }
        }
      }

      // clean
      vit = cands.begin();
      while (vit != cands.end()) {
        quickRef[*vit] = 0;
        ++vit;
      }
      cands.clear();

      // add current entry to index
      partId = 0;
      while (partId < PN) {
        pLen = partLen[partId][clen];
        stPos = partPos[partId][clen];
        invLists[partId][clen][strHash(dict[id], stPos, pLen)].push_back(id);
        ++partId;
      }
    }
  }
}

char CLTR(int xid, int yid, int xlen, int ylen, int xpos, int ypos, int M, int Tau) {
  int i, j;
  for (i = 1; i <= _prevNum; i++)
    if (dict[xid][xpos + i - 1] != dict[_prevId][xpos + i - 1]) break;

  if (i <= _prevNum || _prevNum == UINIT) {
    int pid = M + 1;
    while (xpos + i - 1 >= partPos[pid][dict[xid].length()]) {
      ++M; ++pid;
    }
    lt = M > _left ? _left : M;
    rt = M > _right ? _right : M;
    for (; i <= xlen; i++) {
      if (xpos + i - 1 >= partPos[pid][dict[xid].length()]) {
        ++M; ++pid;
        if (M <= _left) { _matrix[i - 1][D - M] = D + 1; ++lt; }
        if (M <= _right) { _matrix[i - 1][D + M] = D + 1; ++rt; }
      }
      valid = 0;
      if (i <= lt) {
        _matrix[i][D - i] = i;
        if (i <= M && i + abs(xlen - ylen - i) <= Tau) valid = 1;
      }
      for (j = (i - lt >= 1 ? i - lt : 1); j <= (i + rt <= ylen ? i + rt : ylen); j++) {
        if (dict[xid][xpos + i - 1] == dict[yid][ypos + j - 1])
          _matrix[i][j - i + D] = _matrix[i - 1][j - i + D];
        else
          _matrix[i][j - i + D] = min(_matrix[i - 1][j - i + D],
              j - 1 >= i - lt ? _matrix[i][j - i + D - 1] : D + 1,
              j + 1 <= i + rt ? _matrix[i - 1][j - i + D + 1] : D + 1) + 1;
        if (_matrix[i][j - i + D] <= M && _matrix[i][j - i + D] <= (Tau - abs(xlen - ylen - i + j))) valid = 1;
      }
      if (!valid) { _prevNum = i; _prevId = xid; return D + 1; }
    }
    _prevId = xid;
    _prevNum = xlen;
    return _matrix[xlen][ylen + D - xlen];
  }
  return D + 1;
}

// M = Tau - K
char LTR(int xid, int yid, int xlen, int ylen, int M, int Tau) {
  int i, j;
  _left = (Tau + (xlen - ylen)) / 2;
  _right = (Tau - (xlen - ylen)) / 2;
  lt = M > _left ? _left : M;
  rt = M > _right ? _right : M;
  int pid = PN - 1;
  for (i = 1; i <= xlen; i++) {
    if (dict[xid].length() - i < partPos[pid][dict[xid].length()]) {
      ++M; --pid;
      if (M <= _left) { _matrix[i - 1][D - M] = D + 1; ++lt; }
      if (M <= _right) { _matrix[i - 1][D + M] = D + 1; ++rt; }
    }
    valid = 0;
    if (i <= lt) {
      _matrix[i][D - i] = i;
      if (i <= M && i + abs(xlen - ylen - i) <= Tau) valid = 1;
    }
    for (j = (i - lt >= 1 ? i - lt : 1); j <= (i + rt <= ylen ? i + rt : ylen); j++) {
      if (dict[xid][dict[xid].length() - i] == dict[yid][dict[yid].length() - j])
        _matrix[i][j - i + D] = _matrix[i - 1][j - i + D];
      else
        _matrix[i][j - i + D] = min(_matrix[i - 1][j - i + D],
            j - 1 >= i - lt ? _matrix[i][j - i + D - 1] : D + 1,
            j + 1 <= i + rt ? _matrix[i - 1][j - i + D + 1] : D + 1) + 1;
      if (_matrix[i][j - i + D] <= M && _matrix[i][j - i + D] <= (Tau - abs(xlen - ylen - i + j))) valid = 1;
    }
    if (!valid) return D + 1;
  }
  return _matrix[xlen][ylen + D - xlen];
}

char RTL(int xid, int yid, int xlen, int ylen, int Tau) {
  int i, j;
  for (i = 1; i <= prevNum; i++)
    if (dict[xid][i - 1] != dict[prevId][i - 1]) break;

  if (i <= prevNum || prevNum == UINIT) {
    for (; i <= xlen; i++) {
      valid = 0;
      if (i <= left) { matrix[i][D - i] = i; if (i + abs(xlen - ylen - i) <= Tau) valid = 1; }
      for (j = (i - left >= 1 ? i - left : 1); j <= (i + right <= ylen ? i + right : ylen); j++) {
        if (dict[xid][i - 1] == dict[yid][j - 1])
          matrix[i][j - i + D] = matrix[i - 1][j - i + D];
        else
          matrix[i][j - i + D] = min(matrix[i - 1][j - i + D],
              j - 1 >= i - left ? matrix[i][j - i + D - 1] : D,
              j + 1 <= i + right ? matrix[i - 1][j - i + D+ 1] : D) + 1;
        if (abs(xlen - ylen - i + j) + matrix[i][j - i + D] <= Tau) valid = 1;
      }
      if (!valid) { prevNum = i; prevId = xid; return D + 1; }
    }
    prevId = xid;
    prevNum = xlen;
    return matrix[xlen][ylen - xlen + D];
  } else if (prevNum == xlen) {
    return matrix[xlen][ylen - xlen + D];
  }
  return D + 1;
}
