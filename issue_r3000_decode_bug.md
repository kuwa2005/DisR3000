## Summary
R3000 disassembly has multiple decoding/output defects that produce incorrect mnemonics and branch targets.

## Confirmed defects
- `O16_16` does not sign-extend 16-bit branch offsets, so negative branches decode to invalid far addresses.
- `SLL/SRA/SRL` use register field extraction for the 3rd operand instead of `shamt` immediate.
- funct `0x12` mnemonic typo: `MHLO` should be `MFLO`.
- opcode `0x08` mnemonic typo: `ADD` should be `ADDI`.

## Repro examples
- `BEQ $zero,$zero,-1` is rendered with a far target instead of current-address branch.
- `SLL $a0,$zero,1` is rendered as `SLL $a0,$zero,$at`.
- `opcode 0x08` renders as `ADD` immediate form.

## Expected
Disassembly output should follow R3000/MIPS encoding semantics and canonical mnemonics.
