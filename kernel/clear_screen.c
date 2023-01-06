#include "UEFI_boot_param_info.h"
#include "lib.h"
#include "print.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"
#include "task.h"
#include "interrupt.h"


void clear_screen()
{
	int* addr = (int*)0xffff800003000000;
	int i;

	Pos.XResolution = boot_para_info->Graphics_Info.HorizontalResolution;
	Pos.YResolution = boot_para_info->Graphics_Info.VerticalResolution;

	Pos.FB_addr = (int*)0xffff800003000000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4 + PAGE_4K_SIZE - 1) & PAGE_4K_MASK;

	for (i = 0; i < Pos.XResolution * Pos.YResolution; i++)
	{
		*((char*)addr + 0) = (char)0x00;
		*((char*)addr + 1) = (char)0x00;
		*((char*)addr + 2) = (char)0x00;
		*((char*)addr + 3) = (char)0x00;
		addr += 1;
	}

	Pos.YPosition = 0;
	Pos.XPosition = 0;
}