#include <windows.h>
#include <stdio.h>

#define NORMAL_END 0
#define FILE_OPEN_ERROR 10000

typedef LONG (WINAPI *PFN_DisR3000_Init)(void);
typedef LONG (WINAPI *PFN_DisR3000_Open)(void);
typedef LONG (WINAPI *PFN_DisR3000_Sample)(PBYTE, PBYTE, LONG, LONG);
typedef LONG (WINAPI *PFN_DisR3000_Exec)(void);
typedef LONG (WINAPI *PFN_DisR3000_Close)(void);
typedef LONG (WINAPI *PFN_DisR3000_End)(void);
typedef LONG (WINAPI *PFN_DisR3000_GetError)(PBYTE);

static int assert_eq_long(const char* name, LONG actual, LONG expected) {
    if (actual != expected) {
        printf("[FAIL] %s: expected %ld, got %ld\n", name, expected, actual);
        return 1;
    }
    printf("[PASS] %s: %ld\n", name, actual);
    return 0;
}

int main(void) {
    int failed = 0;
    HMODULE dll = LoadLibraryA("DisR3000.dll");
    if (!dll) {
        printf("[FAIL] LoadLibraryA(DisR3000.dll) failed: %lu\n", GetLastError());
        return 1;
    }

    PFN_DisR3000_Init DisR3000_Init = (PFN_DisR3000_Init)GetProcAddress(dll, "DisR3000_Init");
    PFN_DisR3000_Open DisR3000_Open = (PFN_DisR3000_Open)GetProcAddress(dll, "DisR3000_Open");
    PFN_DisR3000_Sample DisR3000_Sample = (PFN_DisR3000_Sample)GetProcAddress(dll, "DisR3000_Sample");
    PFN_DisR3000_Exec DisR3000_Exec = (PFN_DisR3000_Exec)GetProcAddress(dll, "DisR3000_Exec");
    PFN_DisR3000_Close DisR3000_Close = (PFN_DisR3000_Close)GetProcAddress(dll, "DisR3000_Close");
    PFN_DisR3000_End DisR3000_End = (PFN_DisR3000_End)GetProcAddress(dll, "DisR3000_End");
    PFN_DisR3000_GetError DisR3000_GetError = (PFN_DisR3000_GetError)GetProcAddress(dll, "DisR3000_GetError");

    if (!DisR3000_Init || !DisR3000_Open || !DisR3000_Sample || !DisR3000_Exec || !DisR3000_Close || !DisR3000_End || !DisR3000_GetError) {
        printf("[FAIL] GetProcAddress failed for one or more symbols\n");
        FreeLibrary(dll);
        return 1;
    }

    failed |= assert_eq_long("DisR3000_Init", DisR3000_Init(), NORMAL_END);
    failed |= assert_eq_long("DisR3000_Open", DisR3000_Open(), NORMAL_END);
    failed |= assert_eq_long("DisR3000_Exec", DisR3000_Exec(), NORMAL_END);
    failed |= assert_eq_long("DisR3000_Close", DisR3000_Close(), NORMAL_END);
    failed |= assert_eq_long("DisR3000_End", DisR3000_End(), NORMAL_END);

    {
        LONG rc = DisR3000_Sample((PBYTE)"tests\\no_such_input.bin", (PBYTE)"tests\\out_invalid.txt", 0, 0);
        failed |= assert_eq_long("DisR3000_Sample(invalid path)", rc, FILE_OPEN_ERROR);
    }

    {
        BYTE testData[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
        DWORD written = 0;
        HANDLE f = CreateFileA("tests\\sample_input.bin", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (f == INVALID_HANDLE_VALUE) {
            printf("[FAIL] create sample_input.bin failed: %lu\n", GetLastError());
            failed = 1;
        } else {
            if (!WriteFile(f, testData, (DWORD)sizeof(testData), &written, NULL) || written != sizeof(testData)) {
                printf("[FAIL] write sample_input.bin failed\n");
                failed = 1;
            }
            CloseHandle(f);
        }

        if (!failed) {
            LONG rc = DisR3000_Sample((PBYTE)"tests\\sample_input.bin", (PBYTE)"tests\\sample_output.txt", 0, 0);
            failed |= assert_eq_long("DisR3000_Sample(valid path)", rc, NORMAL_END);

            HANDLE out = CreateFileA("tests\\sample_output.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (out == INVALID_HANDLE_VALUE) {
                printf("[FAIL] sample output file not created\n");
                failed = 1;
            } else {
                DWORD size = GetFileSize(out, NULL);
                if (size == 0 || size == INVALID_FILE_SIZE) {
                    printf("[FAIL] sample output file is empty or invalid\n");
                    failed = 1;
                } else {
                    printf("[PASS] sample output file created, size=%lu bytes\n", size);
                }
                CloseHandle(out);
            }
        }
    }

    {
        BYTE errbuf[1024] = {0};
        LONG err = DisR3000_GetError(errbuf);
        printf("[INFO] DisR3000_GetError returned=%ld, message='%s'\n", err, errbuf);
    }

    FreeLibrary(dll);
    if (failed) {
        printf("RESULT: FAILED\n");
        return 1;
    }
    printf("RESULT: PASSED\n");
    return 0;
}
