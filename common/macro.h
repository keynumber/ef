/*
 * Author: number
 * Date  : Nov 1, 2015
 */

#ifndef __COMMON_MACRO_H_H___
#define __COMMON_MACRO_H_H___

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#endif /* __COMMON_MACRO_H__ */
