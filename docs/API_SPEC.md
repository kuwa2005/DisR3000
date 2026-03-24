# DisR3000 API Specification

Header: `src/DISR3000.H`
DLL: `DisR3000.dll`
Calling convention: `WINAPI`
Return type: `LONG` (0 means success in current implementation)

## Exported Functions

### `LONG DisR3000_Init(void)`
Initialize internal state.

### `LONG DisR3000_Open(void)`
Open resources (currently a no-op).

### `LONG DisR3000_Sample(PBYTE pInputFileName, PBYTE pOutputFileName, LONG lStartAdress, LONG lAsmSize)`
Disassemble input binary file and write text output.

- `pInputFileName`: input binary path (ANSI string)
- `pOutputFileName`: output text path (ANSI string)
- `lStartAdress`: currently not used
- `lAsmSize`: currently not used

Return codes:
- `0` (`NORMAL_END`): success
- `10000` (`FILE_OPEN_ERROR`): input open failed
- `10001` (`FILE_READ_ERROR`): input read failed
- `10002` (`READ_MEM_ALLOC_ERROR`): memory allocation failed

### `LONG DisR3000_Exec(void)`
Execute processing step (currently a no-op).

### `LONG DisR3000_Close(void)`
Close resources (currently a no-op).

### `LONG DisR3000_End(void)`
Finalize internal state (currently a no-op).

### `LONG DisR3000_GetError(PBYTE pRetMsg)`
Get last error code and message, then clear stored error state.

- `pRetMsg`: output buffer for message

## Typical Call Sequence

1. `DisR3000_Init`
2. `DisR3000_Open`
3. `DisR3000_Sample`
4. `DisR3000_Close`
5. `DisR3000_End`
6. `DisR3000_GetError` (optional)
