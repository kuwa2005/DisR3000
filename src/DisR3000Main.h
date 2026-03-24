#ifndef __inc_dis_r3000_main_h__
#define __inc_dis_r3000_main_h__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void R3000Main(PBYTE pReadData, LONG lReadSize, PBYTE pOutputFileName);
LONG GetFile(PBYTE pbFileName, HGLOBAL *pReadMem, PLONG plFileSize);

#ifdef __cplusplus
}
#endif

#endif
