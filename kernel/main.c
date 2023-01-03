#include "UEFI_boot_param_info.h"
#include "lib.h"
#include "print.h"
#include "gate.h"
#include "trap.h"
#include "memory.h"
#include "task.h"
#include "interrupt.h"
#include"clear_screen.h"
#include "keyword.h"

struct KERNEL_BOOT_PARAMETER_INFORMATION *boot_para_info = (struct KERNEL_BOOT_PARAMETER_INFORMATION *)0xffff800000060000;

void Start_Kernel(void)
{
	int *addr = (int *)0xffff800003000000;
	int i;
	memset((void *)&_bss, 0, (unsigned long)&_ebss - (unsigned long)&_bss);

	Pos.XResolution = boot_para_info->Graphics_Info.HorizontalResolution;
	Pos.YResolution = boot_para_info->Graphics_Info.VerticalResolution;

	Pos.FB_addr = (int *)0xffff800003000000;
	Pos.FB_length = (Pos.XResolution * Pos.YResolution * 4 + PAGE_4K_SIZE - 1) & PAGE_4K_MASK;

	load_TR(10);

	set_tss64(_stack_start, _stack_start, _stack_start, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00, 0xffff800000007c00);

	sys_vector_init();

	memory_management_struct.start_code = (unsigned long)& _text;
	memory_management_struct.end_code   = (unsigned long)& _etext;
	memory_management_struct.end_data   = (unsigned long)& _edata;
	memory_management_struct.end_brk    = (unsigned long)& _end;

	clear_screen();


	print("memory init \n");
	init_memory();

	print("interrupt init \n");
	init_interrupt();

	print("task_init \n");
	task_init();

	while(1);
}
