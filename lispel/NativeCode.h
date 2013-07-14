#ifndef NATIVE_CODE_H_lispel_
#define NATIVE_CODE_H_lispel_ 1

typedef LispelCell *(*LispelNativeFunction)( LispelContext *ctx_, LispelCell *args_, unsigned int argc_);

#endif /*NATIVE_CODE_H_lispel_*/
