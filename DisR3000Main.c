//R3000 簡易逆アセンブラ By 1997-2009 PCM  http://lil.la/
#include <windows.h>
#include <stdio.h>

#include "DisR3000Common.h"
#include "DisR3000Error.h"
#include "DisR3000Main.h"

/* Proto Types */
LONG R06_05(LONG,LONG,PBYTE);	//	______12345_____________________	// Register
LONG R11_05(LONG,LONG,PBYTE);	//	___________12345________________	// Register
LONG R16_05(LONG,LONG,PBYTE);	//	________________12345___________	// Register
LONG R21_05(LONG,LONG,PBYTE);	//	________________12345___________	// Register
LONG I06_20(LONG,LONG,PBYTE);	//	________________12345___________	// Register
LONG I16_16(LONG,LONG,PBYTE);	//	________________12345___________	// Register
LONG A06_26(LONG,LONG,PBYTE);	//	________________12345___________	// Register
LONG P07_15(LONG,LONG,PBYTE);
LONG O16_16(LONG,LONG,PBYTE);
LONG OFS_B_(LONG,LONG,PBYTE);
LONG S21_05(LONG,LONG,PBYTE);	//	______12345_____1234567890123456	// offset(base)
								//	______bbbbb_____oooooooooooooooo 

extern	LONG	glLastError;
extern	BYTE	gbLastErrorMessage[1024];


/*
MASK     CODE     

0xFFFFFFFF,0x0000000C,"SYSCALL          ________________________________ 
0xFFFFFFFF,0x00000000,"NOP              ________________________________ 
0xFC00003F,0x0000000D,"BREAK	c       ______cccccccccccccccccccc______ 
0xFFE0003F,0x00000000,"SLL	d,t,s       ___________tttttdddddsssss______ 
0xFFE0003F,0x00000003,"SRA	d,t,s       ___________tttttdddddsssss______ 
0xFFE0003F,0x00000002,"SRL	d,t,s       ___________tttttdddddsssss______ 
0xFFE007FF,0x00000021,"MOVE	t,d         ___________tttttddddd___________ 
0xFC1F07FF,0x00000009,"JALR	d,s         ______sssss_____ddddd___________ 
0xFC1FFFFF,0x00000008,"JR	s           ______sssss_____________________ 
0xFFFF07FF,0x00000010,"MFHI	d           ________________ddddd___________ 
0xFFFF07FF,0x00000011,"MTHI	d           ________________ddddd___________ 
0xFFFF07FF,0x00000012,"MHLO	d           ________________ddddd___________ 
0xFFFF07FF,0x00000013,"MTLO	d           ________________ddddd___________ 
0xFC00FFFF,0x00000018,"MULT	s,t         ______sssssttttt________________ 
0xFC00FFFF,0x00000019,"MULTU	s,t     ______sssssttttt________________ 
0xFC00FFFF,0x0000001A,"DIV	s,t         ______sssssttttt________________ 
0xFC00FFFF,0x0000001B,"DIVU	s,t         ______sssssttttt________________ 
0xFC0007FF,0x00000020,"ADD 	d,s,t       ______sssssrrrrrddddd___________ 
0xFC0007FF,0x00000021,"ADDU	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000024,"AND	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000026,"XOR	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000006,"SRLV	d,t,s       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000022,"SUB	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000023,"SUBU	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000027,"NOR	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000025,"OR	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000004,"SLLV	d,t,s       ______ssssstttttddddd___________ 
0xFC0007FF,0x0000002A,"SLT	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x0000002B,"SLTU	d,s,t       ______ssssstttttddddd___________ 
0xFC0007FF,0x00000007,"SRAV	d,t,s       ______ssssstttttddddd___________ 
0xFFE007FF,0x00000021,"MOVE	t,d         ___________tttttddddd___________ 
0xFC000000,0x08000000,"J	a           ______aaaaaaaaaaaaaaaaaaaaaaaaaa 
0xFC000000,0x0C000000,"JAL	a           ______aaaaaaaaaaaaaaaaaaaaaaaaaa 
0xFC000000,0x10000000,"BEQ	t,s,o       ______ssssstttttoooooooooooooooo 
0xFC000000,0x14000000,"BNE	t,s,o       ______ssssstttttoooooooooooooooo 
0xFC000000,0x20000000,"ADD 	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x24000000,"ADDIU	t,s,i   ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x28000000,"SLTI	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x2C000000,"SLTIU	t,s,i   ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x30000000,"ANDI	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x34000000,"ORI	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x38000000,"XORI	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
0xFC000000,0x80000000,"LB	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0x84000000,"LH	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0x88000000,"LWL	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0x8C000000,"LW	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0x90000000,"LBU	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0x94000000,"LHU	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0x98000000,"LWR	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xA0000000,"SB	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xA4000000,"SH	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xA8000000,"SWL	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xAC000000,"SW	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xB8000000,"SWR	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xE0000000,"SWC0	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xE4000000,"SWC1	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xE8000000,"SWC2	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xEC000000,"SWC3	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xC0000000,"LWC0	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xC4000000,"LWC1	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xC8000000,"LWC2	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFC000000,0xCC000000,"LWC3	t,o(b)      ______bbbbbtttttoooooooooooooooo 
0xFFE00000,0x24000000,"LI	t,i         ___________tttttiiiiiiiiiiiiiiii 
0xFFE00000,0x3C000000,"LUI	t,i         ___________tttttiiiiiiiiiiiiiiii 
0xFFFF0000,0x10000000,"B	offset      ________________oooooooooooooooo 
0xFFFF0000,0x41000000,"BC0F	offset      ________________oooooooooooooooo 
0xFFFF0000,0x45000000,"BC1F	offset      ________________oooooooooooooooo 
0xFFFF0000,0x49000000,"BC2F	offset      ________________oooooooooooooooo 
0xFFFF0000,0x4D000000,"BC3F	offset      ________________oooooooooooooooo 
0xFFFF0000,0x41010000,"BC0T	offset      ________________oooooooooooooooo 
0xFFFF0000,0x45010000,"BC1T	offset      ________________oooooooooooooooo 
0xFFFF0000,0x49010000,"BC2T	offset      ________________oooooooooooooooo 
0xFFFF0000,0x4D010000,"BC3T	offset      ________________oooooooooooooooo 
0xFFE007FF,0x40000000,"MFC0	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x40400000,"CFC0	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x40800000,"MTC0	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x40C00000,"CTC0	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x44000000,"MFC1	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x44400000,"CFC1	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x44800000,"MTC1	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x44C00000,"CTC1	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x48000000,"MFC2	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x48400000,"CFC2	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x48800000,"MTC2	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x48C00000,"CTC2	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x4C000000,"MFC3	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x4C400000,"CFC3	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x4C800000,"MTC3	t,d         ___________tttttddddd___________ 
0xFFE007FF,0x4CC00000,"CTC3	t,d         ___________tttttddddd___________ 
0xFC1F0000,0x04010000,"BGEZ	s,o         ______sssss_____oooooooooooooooo 
0xFC1F0000,0x04000000,"BLTZ	s,o         ______sssss_____oooooooooooooooo 
0xFC1F0000,0x04100000,"BLTZAL	s,o     ______sssss_____oooooooooooooooo 
0xFC1F0000,0x04110000,"BGEZAL	s,o     ______sssss_____oooooooooooooooo 
0xFC1F0000,0x18000000,"BLEZ	s,o         ______sssss_____oooooooooooooooo 
0xFC1F0000,0x1C000000,"BGTZ	s,o         ______sssss_____oooooooooooooooo 
0xFE000000,0x42000000,"COP0	p           _______ppppppppppppppppppppppppp 
0xFE000000,0x46000000,"COP1	p           _______ppppppppppppppppppppppppp 
0xFE000000,0x4A000000,"COP2	p           _______ppppppppppppppppppppppppp 
0xFE000000,0x4E000000,"COP3	p           _______ppppppppppppppppppppppppp 

				01234567890123456789012345678901
1:R06_05		______12345_____________________ 
2:R11_05		___________12345________________ 
3:R16_05		________________12345___________ 
4:R21_05		_____________________12345______ 
5:I06_20		______12345678901234567890______	// Immediate (Code)
6:I16_16		________________1234567890123456	// Immediate
7:A06_26		______12345678901234567890123456	// Address(Offset)
8:P07_15        _______1234567890123456789012345	// Immedate (Code)
9:O16_16		________________1234567890123456	// OffSet
A:OFS_B_		______12345_____1234567890123456	// offset(base)
				______bbbbb_____oooooooooooooooo 
*/

Struct(STR_R3000ASM) {
	LONG	lMask;			// マスク
	LONG	lCode;			// 実行コード
	PBYTE	bAsm;			// ニーモニック
	LONG	(*pFunc[4])();	// 取得レジスタ
};

static STR_R3000ASM R3000Asm[] = {
	{ 0xFFFFFFFF,0x0000000C,"SYSCALL",{                     NULL}},//                ________________________________ 
	{ 0xFFFFFFFF,0x00000000,"NOP"	 ,{                     NULL}},//                ________________________________ 
	{ 0xFC00003F,0x0000000D,"BREAK"	 ,{I06_20,				NULL}},//	c           ______cccccccccccccccccccc______ 
	{ 0xFFE0003F,0x00000000,"SLL"	 ,{R16_05,R11_05,S21_05,NULL}},//	d,t,s       ___________tttttdddddsssss______ 
	{ 0xFFE0003F,0x00000003,"SRA"	 ,{R16_05,R11_05,S21_05,NULL}},//	d,t,s       ___________tttttdddddsssss______ 
	{ 0xFFE0003F,0x00000002,"SRL"	 ,{R16_05,R11_05,S21_05,NULL}},//	d,t,s       ___________tttttdddddsssss______ 
	{ 0xFFE007FF,0x00000021,"MOVE"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFC1F07FF,0x00000009,"JALR"	 ,{R16_05,R06_05,       NULL}},//	d,s         ______sssss_____ddddd___________ 
	{ 0xFC1FFFFF,0x00000008,"JR"	 ,{R06_05,              NULL}},//	s           ______sssss_____________________ 
	{ 0xFFFF07FF,0x00000010,"MFHI"	 ,{R16_05,              NULL}},//	d           ________________ddddd___________ 
	{ 0xFFFF07FF,0x00000011,"MTHI"	 ,{R16_05,              NULL}},//	d           ________________ddddd___________ 
	{ 0xFFFF07FF,0x00000012,"MFLO"	 ,{R16_05,              NULL}},//	d           ________________ddddd___________ 
	{ 0xFFFF07FF,0x00000013,"MTLO"	 ,{R16_05,              NULL}},//	d           ________________ddddd___________ 
	{ 0xFC00FFFF,0x00000018,"MULT"	 ,{R06_05,R11_05,       NULL}},//	s,t         ______sssssttttt________________ 
	{ 0xFC00FFFF,0x00000019,"MULTU"	 ,{R06_05,R11_05,       NULL}},//	s,t         ______sssssttttt________________ 
	{ 0xFC00FFFF,0x0000001A,"DIV"	 ,{R06_05,R11_05,       NULL}},//	s,t         ______sssssttttt________________ 
	{ 0xFC00FFFF,0x0000001B,"DIVU"	 ,{R06_05,R11_05,       NULL}},//	s,t         ______sssssttttt________________ 
	{ 0xFC0007FF,0x00000020,"ADD"	 ,{R21_05,R06_05,R11_05,NULL}},// 	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000021,"ADDU"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000024,"AND"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000026,"XOR"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000006,"SRLV"	 ,{R16_05,R11_05,R21_05,NULL}},//	d,t,s       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000022,"SUB"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000023,"SUBU"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000027,"NOR"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000025,"OR"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000004,"SLLV"	 ,{R16_05,R11_05,R21_05,NULL}},//	d,t,s       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x0000002A,"SLT"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x0000002B,"SLTU"	 ,{R21_05,R06_05,R11_05,NULL}},//	d,s,t       ______ssssstttttddddd___________ 
	{ 0xFC0007FF,0x00000007,"SRAV"	 ,{R16_05,R11_05,R21_05,NULL}},//	d,t,s       ______ssssstttttddddd___________ 
	{ 0xFFE007FF,0x00000021,"MOVE"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFC000000,0x08000000,"J"		 ,{A06_26,				NULL}},//	a           ______aaaaaaaaaaaaaaaaaaaaaaaaaa 
	{ 0xFC000000,0x0C000000,"JAL"	 ,{A06_26,			    NULL}},//	a           ______aaaaaaaaaaaaaaaaaaaaaaaaaa 
	{ 0xFC000000,0x10000000,"BEQ"	 ,{R11_05,R06_05,O16_16,NULL}},//	t,s,o       ______ssssstttttoooooooooooooooo 
	{ 0xFC000000,0x14000000,"BNE"	 ,{R11_05,R06_05,O16_16,NULL}},//	t,s,o       ______ssssstttttoooooooooooooooo 
	{ 0xFC000000,0x20000000,"ADDI"	 ,{R11_05,R06_05,I16_16,NULL}},// 	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x24000000,"ADDIU"	 ,{R11_05,R06_05,I16_16,NULL}},//	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x28000000,"SLTI"	 ,{R11_05,R06_05,I16_16,NULL}},//	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x2C000000,"SLTIU"	 ,{R11_05,R06_05,I16_16,NULL}},//	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x30000000,"ANDI"	 ,{R11_05,R06_05,I16_16,NULL}},//	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x34000000,"ORI"	 ,{R11_05,R06_05,I16_16,NULL}},//	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x38000000,"XORI"	 ,{R11_05,R06_05,I16_16,NULL}},//	t,s,i       ______ssssstttttiiiiiiiiiiiiiiii 
	{ 0xFC000000,0x80000000,"LB"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0x84000000,"LH"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0x88000000,"LWL"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0x8C000000,"LW"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0x90000000,"LBU"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0x94000000,"LHU"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0x98000000,"LWR"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xA0000000,"SB"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xA4000000,"SH"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xA8000000,"SWL"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xAC000000,"SW"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xB8000000,"SWR"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xE0000000,"SWC0"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xE4000000,"SWC1"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xE8000000,"SWC2"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xEC000000,"SWC3"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xC0000000,"LWC0"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xC4000000,"LWC1"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xC8000000,"LWC2"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFC000000,0xCC000000,"LWC3"	 ,{R11_05,OFS_B_,       NULL}},//	t,o(b)      ______bbbbbtttttoooooooooooooooo 
	{ 0xFFE00000,0x24000000,"LI"	 ,{R11_05,I16_16,       NULL}},//	t,i         ___________tttttiiiiiiiiiiiiiiii 
	{ 0xFFE00000,0x3C000000,"LUI"	 ,{R11_05,I16_16,       NULL}},//	t,i         ___________tttttiiiiiiiiiiiiiiii 
	{ 0xFFFF0000,0x10000000,"B"		 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x41000000,"BC0F"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x45000000,"BC1F"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x49000000,"BC2F"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x4D000000,"BC3F"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x41010000,"BC0T"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x45010000,"BC1T"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x49010000,"BC2T"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFFF0000,0x4D010000,"BC3T"	 ,{O16_16,              NULL}},//	offset      ________________oooooooooooooooo 
	{ 0xFFE007FF,0x40000000,"MFC0"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x40400000,"CFC0"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x40800000,"MTC0"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x40C00000,"CTC0"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x44000000,"MFC1"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x44400000,"CFC1"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x44800000,"MTC1"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x44C00000,"CTC1"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x48000000,"MFC2"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x48400000,"CFC2"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x48800000,"MTC2"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x48C00000,"CTC2"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x4C000000,"MFC3"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x4C400000,"CFC3"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x4C800000,"MTC3"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFFE007FF,0x4CC00000,"CTC3"	 ,{R11_05,R16_05,       NULL}},//	t,d         ___________tttttddddd___________ 
	{ 0xFC1F0000,0x04010000,"BGEZ"	 ,{R06_05,O16_16,       NULL}},//	s,o         ______sssss_____oooooooooooooooo 
	{ 0xFC1F0000,0x04000000,"BLTZ"	 ,{R06_05,O16_16,       NULL}},//	s,o         ______sssss_____oooooooooooooooo 
	{ 0xFC1F0000,0x04100000,"BLTZAL" ,{R06_05,O16_16,       NULL}},//	s,o         ______sssss_____oooooooooooooooo 
	{ 0xFC1F0000,0x04110000,"BGEZAL" ,{R06_05,O16_16,       NULL}},//	s,o         ______sssss_____oooooooooooooooo 
	{ 0xFC1F0000,0x18000000,"BLEZ"	 ,{R06_05,O16_16,       NULL}},//	s,o         ______sssss_____oooooooooooooooo 
	{ 0xFC1F0000,0x1C000000,"BGTZ"	 ,{R06_05,O16_16,       NULL}},//	s,o         ______sssss_____oooooooooooooooo 
	{ 0xFE000000,0x42000000,"COP0"	 ,{P07_15,              NULL}},//	p           _______ppppppppppppppppppppppppp 
	{ 0xFE000000,0x46000000,"COP1"	 ,{P07_15,              NULL}},//	p           _______ppppppppppppppppppppppppp 
	{ 0xFE000000,0x4A000000,"COP2"	 ,{P07_15,              NULL}},//	p           _______ppppppppppppppppppppppppp 
	{ 0xFE000000,0x4E000000,"COP3"	 ,{P07_15,              NULL}},//	p           _______ppppppppppppppppppppppppp 
};

PBYTE pRegisters[]={
	"$zero",		// "r00",
	"$at",			// "r01",
	"$v0",			// "r02",
	"$v1",			// "r03",
	"$a0",			// "r04",
	"$a1",			// "r05",
	"$a2",			// "r06",
	"$a3",			// "r07",
	"$t0",			// "r08",
	"$t1",			// "r09",
	"$t2",			// "r10",
	"$t3",			// "r11",
	"$t4",			// "r12",
	"$t5",			// "r13",
	"$t6",			// "r14",
	"$t7",			// "r15",
	"$s0",			// "r16",
	"$s1",			// "r17",
	"$s2",			// "r18",
	"$s3",			// "r19",
	"$s4",			// "r20",
	"$s5",			// "r21",
	"$s6",			// "r22",
	"$s7",			// "r23",
	"$t8",			// "r24",
	"$t9",			// "r25",
	"$k0",			// "r26",
	"$k1",			// "r27",
	"$gp",			// "r28",
	"$sp",			// "r29",
	"$s8",			// "r30",
	"$ra",			// "r31"
};

///////////////////////////////////////////
//	______12345_____________________	// Register
///////////////////////////////////////////
LONG
R06_05(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//            123456789012345678901 
	// 00000011111000000000000000000000 = 0x3E00000
	lDat = lDat & 0x3E00000;
	lDat = lDat >> 21;

	sprintf(pAsm,"%s%s",pAsm,pRegisters[lDat]);

	return	lRtn;
}

///////////////////////////////////////////
//	___________12345________________	// Register
///////////////////////////////////////////
LONG
R11_05(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                  1234567890123456
	//	00000000000111110000000000000000	// Register
	lDat = lDat & 0x1F0000;
	lDat = lDat >> 16;

	sprintf(pAsm,"%s%s",pAsm,pRegisters[lDat]);

	return	lRtn;
}

///////////////////////////////////////////
//	________________12345___________	// Register
///////////////////////////////////////////
LONG
R16_05(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                       12345678901
	//	00000000000000001111100000000000	// Register
	lDat = lDat & 0xF800;
	lDat = lDat >> 11;

	sprintf(pAsm,"%s%s",pAsm,pRegisters[lDat]);

	return	lRtn;
}


///////////////////////////////////////////
//	_____________________12345______	// Register
///////////////////////////////////////////
LONG
R21_05(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                            123456
	//	00000000000000000000011111000000	// Register
	lDat = lDat & 0x7C0;
	lDat = lDat >> 6;

	sprintf(pAsm,"%s%s",pAsm,pRegisters[lDat]);

	return	lRtn;
}

///////////////////////////////////////////
//	_____________________12345______	// Shift 量
///////////////////////////////////////////
LONG
S21_05(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                            123456
	//	00000000000000000000011111000000	// Shift 量
	lDat = lDat & 0x7C0;
	lDat = lDat >> 6;

	sprintf(pAsm,"%s%x",pAsm,lDat);

	return	lRtn;
}

///////////////////////////////////////////
//	______12345678901234567890______	// Immediate (Code)
///////////////////////////////////////////
LONG
I06_20(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                            123456
	//	00000011111111111111111111000000	// Immediate (Code)
	lDat = lDat & 0x3FFFFC0;
	lDat = lDat >> 6;

	sprintf(pAsm,"%s%x",pAsm,lDat);

	return	lRtn;
}


///////////////////////////////////////////
//	________________1234567890123456	// Immediate
///////////////////////////////////////////
LONG
I16_16(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                  
	//	00000000000000001111111111111111	// Immediate
	lDat = lDat & 0xFFFF;

	sprintf(pAsm,"%s%x",pAsm,lDat);

	return	lRtn;
}


///////////////////////////////////////////
//	______12345678901234567890123456	// Address(Offset)
///////////////////////////////////////////
LONG
A06_26(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//	00000011111111111111111111111111	// Address(Offset)
	lDat = lDat & 0x3FFFFFF;

	lDat = (lAdr & 0xf0000000) | ( lDat << 2 );	// Offset値算出

	sprintf(pAsm,"%s%x",pAsm,lDat);

	return	lRtn;
}


///////////////////////////////////////////
//	_______1234567890123456789012345	// Immedate (Code)
///////////////////////////////////////////
LONG
P07_15(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//	00000001111111111111111111111111	// Immedate (Code)
	lDat = lDat & 0x1FFFFFF;

	sprintf(pAsm,"%s%x",pAsm,lDat);

	return	lRtn;
}


///////////////////////////////////////////
//	________________1234567890123456	// OffSet
///////////////////////////////////////////
LONG
O16_16(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lRtn = NORMAL_END;

	//                  
	//	00000000000000001111111111111111	// OffSet
	lDat = (SHORT)(lDat & 0xFFFF);
	lDat = lAdr + 4 + ( ( (INT) lDat ) * 4);	// OffSet 値算出

	sprintf(pAsm,"%s%x",pAsm,lDat);

	return	lRtn;
}


///////////////////////////////////////////
//	______12345_____1234567890123456	// offset(base)
//	______bbbbb_____oooooooooooooooo 
///////////////////////////////////////////
LONG
OFS_B_(
	LONG	lAdr,
	LONG	lDat,
	PBYTE	pAsm)
{
	LONG	lDat1;
	LONG	lDat2;
	LONG	lRtn = NORMAL_END;

	//	00000000000000001111111111111111	// OffSet
	lDat1 = lDat & 0xFFFF;
	if (lDat1 > 0x7fff) {
		lDat1 += 0xffff0000;	// 符号拡張（これでええんかいな？）
	}

	//	lDat1 = lAdr + 4 + ( ( (INT) lDat1 ) * 4);	// OffSet 値算出

	//             123456789012345678901	
	//	00000011111000000000000000000000	// Base 
	lDat2 = lDat & 0x3E00000;
	lDat2 = lDat2 >> 21;

	sprintf(pAsm,"%s%x(%s)",pAsm,lDat1,pRegisters[lDat2]);

	return	lRtn;
}

///////////////////////////////////////////
// ファイルをメモリへ読み込む
///////////////////////////////////////////
LONG
GetFile(
	PBYTE	pbFileName ,		// 読み込むファイル名
	HGLOBAL	*pReadMem  ,		// 読み込み先メモリアドレス
	PLONG	plFileSize )		// 読み込んだサイズ
{
	HANDLE	hFp=NULL;			// 読み込み元ファイルハンドル
	LONG	lFileSize = 0;		// ファイルサイズ

	LONG	lRtn = NORMAL_END;
	HGLOBAL	pDat = NULL;
//	LONG	lDummy;

	// ファイルオープン
	hFp = CreateFile(pbFileName,
					 GENERIC_READ	,
					 FILE_SHARE_READ,
					 NULL,
					 OPEN_EXISTING,
					 FILE_ATTRIBUTE_NORMAL,
					 NULL);

	if (hFp <= 0x7fffffff ) {
		// ファイルサイズを求める
		lFileSize = GetFileSize( hFp , NULL) - 1;
		// 読み込み先メモリの確保
		pDat = GlobalAlloc(GMEM_FIXED,lFileSize);
		if (pDat != NULL) {
			// ファイルをメモリに読み込む
			if (ReadFile( hFp , pDat , lFileSize , plFileSize  , NULL) == TRUE) {
				*pReadMem = pDat;			// ファイルの読み込みに成功
			} else {	
				lRtn = FILE_READ_ERROR;		// ファイルの読み込みに失敗
				pDat = GlobalFree(pDat);
			}
		} else {
			lRtn = READ_MEM_ALLOC_ERROR;	// メモリの確保に失敗
		}
		CloseHandle(hFp);
	} else {
		lRtn = FILE_OPEN_ERROR;				// ファイルのオープンに失敗
	}

	hFp = NULL;

	return	lRtn;
}

void R3000Main(PBYTE pReadData , LONG lReadSize,PBYTE	pOutputFileName)
{
	LONG	lPC = 0;
	LONG	lDat;
	INT		i,j;
	BYTE	bOutBuf[256];

	LONG	lRtn = NORMAL_END;

	FILE *fw;
	fw = fopen( pOutputFileName , "w" );

	for(lPC=0;lPC<lReadSize;lPC+=4) {
		
		lDat = pReadData[lPC+3] * 0x1000000 + pReadData[lPC+2] * 0x10000 + 
			   pReadData[lPC+1] * 0x0000100 + pReadData[lPC+0];

		i=0;
		
		bOutBuf[0]=0;
		sprintf(bOutBuf,
			"%08X %02X %02X %02X %02X",
			0x80000000+lPC,
			pReadData[lPC+3],pReadData[lPC+2],
			pReadData[lPC+1],pReadData[lPC  ] );

		while ( R3000Asm[i].lMask != 0) {
			if ( (lDat & R3000Asm[i].lMask) == R3000Asm[i].lCode ) {
				j = 0;
				sprintf(bOutBuf,"%s\t%-8s ",bOutBuf,R3000Asm[i].bAsm );
				while (*R3000Asm[i].pFunc[j] != NULL ) {
					lRtn = (*R3000Asm[i].pFunc[j])(0x80000000+lPC,lDat,bOutBuf);
					j++;
					if (*R3000Asm[i].pFunc[j] != NULL ) {
						sprintf(bOutBuf,"%s,",bOutBuf);
					}
				}
				break;
			}
			i++;
		}
		fprintf(fw,"%s\n",bOutBuf);
	}
	fclose(fw);
}
