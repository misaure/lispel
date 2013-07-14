#ifndef CELL_H_lispel_
#define CELL_H_lispel_

#include "lispel/ByteCode.h"
#include "lispel/NativeCode.h"

typedef unsigned char LispelTypeTag;

typedef struct {
  LispelTypeTag tag;
  union {
    int vInteger;
    struct {
      unsigned int length;
      char *data;
    } vString;
    struct {
      unsigned int length;
      struct _LispelCell *data;
    } vVector;
    struct {
      struct _LispelCell *bindings;
      LispelCode *byteCode;
    } vClosure;
    struct {
      struct _LispelCell *bindings;
      LispelNativeFunction cFunc;
    } vNativeFunction;
  } value;
} LispelCell;

typedef struct {
  char *const name;
  void (*finalize)( LispelCell *);
  void (*duplicate)( LispelCell *);
} LispelTypeDescriptor;

#endif /*CELL_H_lispel_*/
