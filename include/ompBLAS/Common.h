#ifndef COMMON_H
#define COMMON_H

enum Transpose : int {
  NoTrans = 111,
  Trans = 112,
  ConjTrans = 113,
};

enum Order : int {
  RowMajor = 101,
  ColMajor = 102,
};

#endif
