#ifndef __inc_disr3000_h__
#define __inc_disr3000_h__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(DISR3000_API)
#  if defined(_WIN32)
#    if defined(DIS_R3000_DLL_BUILD)
#      define DISR3000_API __declspec(dllexport)
#    else
#      define DISR3000_API __declspec(dllimport)
#    endif
#  else
#    define DISR3000_API
#  endif
#endif

DISR3000_API LONG WINAPI DisR3000_Init(void);
DISR3000_API LONG WINAPI DisR3000_Open(void);
DISR3000_API LONG WINAPI DisR3000_Sample(PBYTE pInputFileName,
                                         PBYTE pOutputFileName,
                                         LONG lStartAdress,
                                         LONG lAsmSize);
DISR3000_API LONG WINAPI DisR3000_Exec(void);
DISR3000_API LONG WINAPI DisR3000_Close(void);
DISR3000_API LONG WINAPI DisR3000_End(void);
DISR3000_API LONG WINAPI DisR3000_GetError(PBYTE pRetMsg);

#ifdef __cplusplus
}
#endif

#endif /* __inc_disr3000_h__ */
