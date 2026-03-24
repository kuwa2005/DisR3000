#include <windows.h>
#include <stdio.h>
typedef LONG (WINAPI *PFN)(PBYTE,PBYTE,LONG,LONG);
int main(){HMODULE h=LoadLibraryA("DisR3000.dll"); if(!h){return 1;} PFN f=(PFN)GetProcAddress(h,"DisR3000_Sample"); if(!f){return 2;} LONG r=f((PBYTE)"tests\\review_input.bin",(PBYTE)"tests\\review_output.txt",0,0); printf("%ld\n",r); return 0;}
