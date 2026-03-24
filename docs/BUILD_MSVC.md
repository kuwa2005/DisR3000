# MSVC Build Guide

This document explains the minimum steps to build `DisR3000` with Microsoft Visual C++.

## Prerequisites

- Visual Studio Community 2026 (or compatible)
- C++ workload: Desktop development with C++

## x64 Build

```powershell
cd C:\00_Project\DisR3000
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && cl /nologo /LD /O2 /W4 /I. DisR3000.c DisR3000Main.c /link /OUT:DisR3000.dll kernel32.lib'
```

## x86 Build

```powershell
cd C:\00_Project\DisR3000
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat" && cl /nologo /LD /O2 /W4 /I. DisR3000.c DisR3000Main.c /link /OUT:DisR3000.dll kernel32.lib'
```

## Build Artifacts

- `DisR3000.dll`
- `DisR3000.lib`
- `DisR3000.exp`
- `DisR3000.obj`
- `DisR3000Main.obj`

## Notes

- Current sources can emit warnings, but DLL generation succeeds.
- To reduce warnings, fix pointer type mismatches and replace unsafe CRT calls.