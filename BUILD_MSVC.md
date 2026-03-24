# MSVC Build Guide

This document explains the minimum steps to build `DisR3000` with Microsoft Visual C++.

## Prerequisites

- Visual Studio Community 2026 (or compatible)
- C++ workload: Desktop development with C++

## x64 Build

```powershell
cd C:\00_Project\DisR3000
mkdir dist\x64 -Force
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && cl /nologo /O2 /W4 /I.\src /c src\DisR3000.c src\DisR3000Main.c /Fo.\dist\x64\ && link /DLL /OUT:dist\x64\DisR3000.dll /IMPLIB:dist\x64\DisR3000.lib dist\x64\DisR3000.obj dist\x64\DisR3000Main.obj kernel32.lib'
```

## x86 Build

```powershell
cd C:\00_Project\DisR3000
mkdir dist\x86 -Force
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat" && cl /nologo /O2 /W4 /I.\src /c src\DisR3000.c src\DisR3000Main.c /Fo.\dist\x86\ && link /DLL /OUT:dist\x86\DisR3000.dll /IMPLIB:dist\x86\DisR3000.lib dist\x86\DisR3000.obj dist\x86\DisR3000Main.obj kernel32.lib'
```

## Build Artifacts

- `dist\x64\DisR3000.dll`, `dist\x64\DisR3000.lib`, `dist\x64\DisR3000.exp`, `dist\x64\*.obj`
- `dist\x86\DisR3000.dll`, `dist\x86\DisR3000.lib`, `dist\x86\DisR3000.exp`, `dist\x86\*.obj`

## Notes

- Current sources can emit warnings, but DLL generation succeeds.
- To reduce warnings, fix pointer type mismatches and replace unsafe CRT calls.