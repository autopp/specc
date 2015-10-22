#ifndef SPECC_MACRO_UTIL_H_
#define SPECC_MACRO_UTIL_H_

#define specc_concat_token_sub(x, y) x ## y

/**
 * INTERNAL: Concat token x and y
 * @param  x
 * @param  y
 */
#define specc_concat_token(x, y) specc_concat_token_sub(x, y)

#define specc_sizeof_va_args_sub(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x18, x19, x20, len, ...) len

/**
 * INTERNAL: Caluculate size of __VA_ARGS__ (up to 20)
 */
#define specc_sizeof_va_args(...)\
  specc_sizeof_va_args_sub(__VA_ARGS__ ##,\
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11,\
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define specc_map_macro_sub1(f, x1) f(x1)
#define specc_map_macro_sub2(f, x1, x2)\
  f(x1) specc_map_macro_sub1(x2)
#define specc_map_macro_sub3(f, x1, x2, x3)\
  f(x1) specc_map_macro_sub2(x2, x3)
#define specc_map_macro_sub4(f, x1, x2, x3, x4)\
  f(x1) specc_map_macro_sub3(x2, x3, x4)
#define specc_map_macro_sub5(f, x1, x2, x3, x4, x5)\
  f(x1) specc_map_macro_sub4(x2, x3, x4, x5)
#define specc_map_macro_sub6(f, x1, x2, x3, x4, x5, x6)\
  f(x1) specc_map_macro_sub5(x2, x3, x4, x5, x6)
#define specc_map_macro_sub7(f, x1, x2, x3, x4, x5, x6, x7)\
  f(x1) specc_map_macro_sub6(x2, x3, x4, x5, x6, x7)
#define specc_map_macro_sub8(f, x1, x2, x3, x4, x5, x6, x7, x8)\
  f(x1) specc_map_macro_sub7(x2, x3, x4, x5, x6, x7, x8)
#define specc_map_macro_sub9(f, x1, x2, x3, x4, x5, x6, x7, x8, x9)\
  f(x1) specc_map_macro_sub8(x2, x3, x4, x5, x6, x7, x8, x9)
#define specc_map_macro_sub10(f, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)\
  f(x1) specc_map_macro_sub9(x2, x3, x4, x5, x6, x7, x8, x9, x10)
#define specc_map_macro_sub11(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11)\
  f(x1) specc_map_macro10(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11\
)
#define specc_map_macro_sub12(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12)\
  f(x1) specc_map_macro11(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12\
)
#define specc_map_macro_sub13(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13)\
  f(x1) specc_map_macro12(x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13\
)
#define specc_map_macro_sub14(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14)\
  f(x1) specc_map_macro13(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14\
)
#define specc_map_macro_sub15(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15)\
  f(x1) specc_map_macro14(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15\
)
#define specc_map_macro_sub16(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16)\
  f(x1) specc_map_macro15(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16\
)
#define specc_map_macro_sub17(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17)\
  f(x1) specc_map_macro16(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17\
)
#define specc_map_macro_sub18(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17, x18)\
  f(x1) specc_map_macro17(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17, x18\
)
#define specc_map_macro_sub19(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17, x18, x19)\
  f(x1) specc_map_macro18(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17, x18, x19\
)
#define specc_map_macro_sub20(\
  x1, x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17, x18, x19, x20)\
  f(x1) specc_map_macro19(x2, x3, x4, x5, x6, x7, x8, x9, x10,\
  x11, x12, x13, x14, x15, x16, x17, x18, x19, x20\
)

/**
 * INTERNAL: Apply each argument to function macro f
 * @param  f Function macro
 */
#define specc_map_macro(f, ...)\
  specc_concat_token(specc_map_macro_sub, specc_sizeof_va_args)(f, __VA_ARGS__)

#endif
