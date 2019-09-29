/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_TYPE_H_
#define INCLUDE_TYPE_H_

#ifndef NULL
#define NULL 	(void*)0
#endif

#ifndef TRUE
	#define TRUE 	1
	#define FALSE 	0
#endif

typedef unsigned __attribute__((__mode__(QI))) int8_t;
typedef unsigned __attribute__((__mode__(QI))) uint8_t;
typedef int      __attribute__((__mode__(HI))) int16_t;
typedef unsigned __attribute__((__mode__(HI))) uint16_t;
typedef int      __attribute__((__mode__(SI))) int32_t;
typedef unsigned __attribute__((__mode__(SI))) uint32_t;
typedef int      __attribute__((__mode__(DI))) int64_t;
typedef unsigned __attribute__((__mode__(DI))) uint64_t;

typedef int32_t pid_t;
typedef uint64_t reg_t;
typedef uint64_t ptr_t;
typedef uint64_t uintptr_t;
typedef int64_t intptr_t;
typedef uint64_t size_t;

#endif
