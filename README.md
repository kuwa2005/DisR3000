# DisR3000

DisR3000 is a Windows DLL tool for disassembling R3000 machine code.

## Project Structure

- `src/`: all C sources and headers (and `Makefile`)
- `sample/`: usage sample programs
- `tests/`: local validation tools
- `API_SPEC.md`: API specification
- `BUILD_MSVC.md`: MSVC build guide
- `LICENSE`: MIT license text

## Build (MSVC x64)

```powershell
cd C:\00_Project\DisR3000
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" && cl /nologo /LD /O2 /W4 /I.\src src\DisR3000.c src\DisR3000Main.c /link /OUT:DisR3000.dll kernel32.lib'
```

## Build (MSVC x86)

```powershell
cd C:\00_Project\DisR3000
cmd /c '"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars32.bat" && cl /nologo /LD /O2 /W4 /I.\src src\DisR3000.c src\DisR3000Main.c /link /OUT:DisR3000.dll kernel32.lib'
```

## Build (GCC / Makefile in src)

```powershell
cd C:\00_Project\DisR3000\src
make
```

## Sample

- `sample/use_disr3000.c`

## License

This project is licensed under the MIT License.
See `LICENSE` for details.