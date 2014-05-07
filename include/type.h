/*****************************************************************************
 *   type.h:  Type definition Header file for NXP LPC230x Family 
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TYPE_H__
#define __TYPE_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   BOOL;

typedef char               S8;
typedef unsigned char      U8;
typedef short              S16;
typedef unsigned short     U16;
typedef int                S32;
typedef unsigned int       U32;
typedef long long          S64;
typedef unsigned long long U64;
typedef unsigned char      BIT;
typedef unsigned int       BOOL;


typedef unsigned char     INT8U;
typedef unsigned int      INT32U;


//config.h before
#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define U_ON  1
#define U_OFF  0

#define U_YES  1
#define U_NO  0

#ifndef ARRAYSIZE
#define ARRAYSIZE(A) sizeof(A)/sizeof(A[0])
#endif

/******************************************************************************
//
******************************************************************************/


/******************************************************************************
//
******************************************************************************/
typedef  unsigned  char  tBoolean;
typedef  unsigned char   uchar;                   /* 无符号8位整型变量                        */
typedef  unsigned char   uint8;                   /* 无符号8位整型变量                        */
typedef  signed   char   int8;                    /* 有符号8位整型变量                        */
typedef  unsigned short  uint16;                  /* 无符号16位整型变量                       */
typedef  signed   short  int16;                   /* 有符号16位整型变量                       */
typedef  unsigned int    uint32;                  /* 无符号32位整型变量                       */
typedef  signed   int    int32;                   /* 有符号32位整型变量                       */
typedef  float           fp32;                    /* 单精度浮点数（32位长度）                 */
typedef  double          fp64;                    /* 双精度浮点数（64位长度）                 */
typedef  unsigned long   ulong;




#endif  /* __TYPE_H__ */
