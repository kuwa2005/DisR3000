#include <windows.h>
#include <stdio.h>

typedef LONG (WINAPI *PFN_DisR3000_Init)(void);
typedef LONG (WINAPI *PFN_DisR3000_Open)(void);
typedef LONG (WINAPI *PFN_DisR3000_Sample)(PBYTE, PBYTE, LONG, LONG);
typedef LONG (WINAPI *PFN_DisR3000_Close)(void);
typedef LONG (WINAPI *PFN_DisR3000_End)(void);
typedef LONG (WINAPI *PFN_DisR3000_GetError)(PBYTE);

int main(void) {
    HMODULE h = LoadLibraryA("DisR3000.dll");
    BYTE err[1024] = {0};

    if (!h) {
        printf("LoadLibrary failed: %lu\n", GetLastError());
        return 1;
    }

    PFN_DisR3000_Init DisR3000_Init = (PFN_DisR3000_Init)GetProcAddress(h, "DisR3000_Init");
    PFN_DisR3000_Open DisR3000_Open = (PFN_DisR3000_Open)GetProcAddress(h, "DisR3000_Open");
    PFN_DisR3000_Sample DisR3000_Sample = (PFN_DisR3000_Sample)GetProcAddress(h, "DisR3000_Sample");
    PFN_DisR3000_Close DisR3000_Close = (PFN_DisR3000_Close)GetProcAddress(h, "DisR3000_Close");
    PFN_DisR3000_End DisR3000_End = (PFN_DisR3000_End)GetProcAddress(h, "DisR3000_End");
    PFN_DisR3000_GetError DisR3000_GetError = (PFN_DisR3000_GetError)GetProcAddress(h, "DisR3000_GetError");

    if (!DisR3000_Init || !DisR3000_Open || !DisR3000_Sample || !DisR3000_Close || !DisR3000_End || !DisR3000_GetError) {
        printf("GetProcAddress failed\n");
        FreeLibrary(h);
        return 1;
    }

    if (DisR3000_Init() != 0) printf("Init failed\n");
    if (DisR3000_Open() != 0) printf("Open failed\n");

    if (DisR3000_Sample((PBYTE)"input.bin", (PBYTE)"output.txt", 0, 0) != 0) {
        LONG code = DisR3000_GetError(err);
        printf("Sample failed. code=%ld msg=%s\n", code, err);
    } else {
        printf("Disassembly complete: output.txt\n");
    }

    DisR3000_Close();
    DisR3000_End();
    FreeLibrary(h);
    return 0;
}
