#ifndef __inc_disr3000thread_h__
#define __inc_disr3000thread_h__

#include <windows.h>
#include "DisR3000Common.h"

#define DIS_R3000_THREAD_MAX	8

Struct(DIS_R3000_THREAD_LIST) {
	HANDLE	hDisR3000ThreadHandle;	// R30000 逆アセンブルメインスレッドハンドル
	HANDLE	hWndHandle;				// 呼び出し元ウインドウハンドル
	HANDLE	hWriteHandle;			// 書き込み先ハンドル
	HANDLE	hReadHandle;			// 読み込み元ファイルハンドル
	LONG	lBaseAdr;				// ベースアドレス
	LONG	lAsmSize;				// アセンブル終了アドレス
	LONG	lNowPC;					// 現在アセンブルしている位置
};

#endif
