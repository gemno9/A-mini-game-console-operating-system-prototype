#include "trap.h"
#include "gate.h"
#include "ptrace.h"
#include "task.h"

int lookup_kallsyms(unsigned long address,int level)
{
	int index = 0;
	int level_index = 0;
	char * string =(char *) &kallsyms_names;
	for(index = 0;index < kallsyms_syms_num;index++)
		if(address >= kallsyms_addresses[index] && address < kallsyms_addresses[index+1])
			break;
	if(index < kallsyms_syms_num)
	{
		for(level_index = 0;level_index < level;level_index++)
			print("  ");
		print("+---> ");

		print("address:%#018lx \t(+) %04d function:%s\n",address,address - kallsyms_addresses[index],&string[kallsyms_index[index]]);
		return 0;
	}
	else
		return 1;
}

void backtrace(struct pt_regs * regs)
{
	unsigned long *rbp = (unsigned long *)regs->rbp;
	unsigned long ret_address = regs->rip;
	int i = 0;

	print("====================== Task Struct Information =====================\n");
	print("regs->rsp:%#018lx,current->thread->rsp0:%#018lx,current:%#018lx\n",regs->rsp,current->thread->rsp0,current);
	print("====================== Kernel Stack Backtrace ======================\n");

	for(i = 0;i<10;i++)
	{
		if(lookup_kallsyms(ret_address,i))
			break;
		if((unsigned long)rbp < (unsigned long)regs->rsp || (unsigned long)rbp > current->thread->rsp0)
			break;

		ret_address = *(rbp+1);
		rbp = (unsigned long *)*rbp;
	}
}

/*
	0
*/

void do_divide_error(struct pt_regs * regs,unsigned long error_code)
{
	print("do_divide_error(0),ERROR_CODE:%#018lx\n",error_code);
	backtrace(regs);
	while(1)
		;
}

/*
	1
*/

void do_debug(struct pt_regs * regs,unsigned long error_code)
{
	print("do_debug(1),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	2
*/

void do_nmi(struct pt_regs * regs,unsigned long error_code)
{
	print("do_nmi(2),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	3
*/

void do_int3(struct pt_regs * regs,unsigned long error_code)
{
	print("do_int3(3),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	4
*/

void do_overflow(struct pt_regs * regs,unsigned long error_code)
{
	print("do_overflow(4),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	5
*/

void do_bounds(struct pt_regs * regs,unsigned long error_code)
{
	print("do_bounds(5),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	6
*/

void do_undefined_opcode(struct pt_regs * regs,unsigned long error_code)
{
	print("do_undefined_opcode(6),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	7
*/

void do_dev_not_available(struct pt_regs * regs,unsigned long error_code)
{
	print("do_dev_not_available(7),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	8
*/

void do_double_fault(struct pt_regs * regs,unsigned long error_code)
{
	print("do_double_fault(8),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	9
*/

void do_coprocessor_segment_overrun(struct pt_regs * regs,unsigned long error_code)
{
	print("do_coprocessor_segment_overrun(9),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	10
*/

void do_invalid_TSS(struct pt_regs * regs,unsigned long error_code)
{
	print("do_invalid_TSS(10),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);

	if(error_code & 0x01)
		print("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		print("Refers to a gate descriptor in the IDT;\n");
	else
		print("Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			print("Refers to a segment or gate descriptor in the LDT;\n");
		else
			print("Refers to a descriptor in the current GDT;\n");

	print("Segment Selector Index:%#010x\n",error_code & 0xfff8);

	while(1);
}

/*
	11
*/

void do_segment_not_present(struct pt_regs * regs,unsigned long error_code)
{
	print("do_segment_not_present(11),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);

	if(error_code & 0x01)
		print("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		print("Refers to a gate descriptor in the IDT;\n");
	else
		print("Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			print("Refers to a segment or gate descriptor in the LDT;\n");
		else
			print("Refers to a descriptor in the current GDT;\n");

	print("Segment Selector Index:%#010x\n",error_code & 0xfff8);

	while(1);
}

/*
	12
*/

void do_stack_segment_fault(struct pt_regs * regs,unsigned long error_code)
{
	print("do_stack_segment_fault(12),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);

	if(error_code & 0x01)
		print("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		print("Refers to a gate descriptor in the IDT;\n");
	else
		print("Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			print("Refers to a segment or gate descriptor in the LDT;\n");
		else
			print("Refers to a descriptor in the current GDT;\n");

	print("Segment Selector Index:%#010x\n",error_code & 0xfff8);

	while(1);
}

/*
	13
*/

void do_general_protection(struct pt_regs * regs,unsigned long error_code)
{
	print("do_general_protection(13),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);

	if(error_code & 0x01)
		print("The exception occurred during delivery of an event external to the program,such as an interrupt or an earlier exception.\n");

	if(error_code & 0x02)
		print("Refers to a gate descriptor in the IDT;\n");
	else
		print("Refers to a descriptor in the GDT or the current LDT;\n");

	if((error_code & 0x02) == 0)
		if(error_code & 0x04)
			print("Refers to a segment or gate descriptor in the LDT;\n");
		else
			print("Refers to a descriptor in the current GDT;\n");

	print("Segment Selector Index:%#010x\n",error_code & 0xfff8);

	while(1);
}

/*
	14
*/

void do_page_fault(struct pt_regs * regs,unsigned long error_code)
{
	unsigned long cr2 = 0;

	__asm__	__volatile__("movq	%%cr2,	%0":"=r"(cr2)::"memory");

	print("do_page_fault(14),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);

	if(!(error_code & 0x01))
		print("Page Not-Present,\t");

	if(error_code & 0x02)
		print("Write Cause Fault,\t");
	else
		print("Read Cause Fault,\t");

	if(error_code & 0x04)
		print("Fault in user(3)\t");
	else
		print("Fault in supervisor(0,1,2)\t");

	if(error_code & 0x08)
		print(",Reserved Bit Cause Fault\t");

	if(error_code & 0x10)
		print(",Instruction fetch Cause Fault");

	print("\n");

	print("CR2:%#018lx\n",cr2);

	while(1);
}

/*
	16
*/

void do_x87_FPU_error(struct pt_regs * regs,unsigned long error_code)
{
	print("do_x87_FPU_error(16),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	17
*/

void do_alignment_check(struct pt_regs * regs,unsigned long error_code)
{
	print("do_alignment_check(17),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	18
*/

void do_machine_check(struct pt_regs * regs,unsigned long error_code)
{
	print("do_machine_check(18),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	19
*/

void do_SIMD_exception(struct pt_regs * regs,unsigned long error_code)
{
	print("do_SIMD_exception(19),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*
	20
*/

void do_virtualization_exception(struct pt_regs * regs,unsigned long error_code)
{
	print("do_virtualization_exception(20),ERROR_CODE:%#018lx,RSP:%#018lx,RIP:%#018lx\n",error_code , regs->rsp , regs->rip);
	while(1);
}

/*

*/

void sys_vector_init()
{
	set_trap_gate(0,0,divide_error);
	set_trap_gate(1,0,debug);
	set_intr_gate(2,0,nmi);
	set_system_gate(3,0,int3);
	set_system_gate(4,0,overflow);
	set_system_gate(5,0,bounds);
	set_trap_gate(6,0,undefined_opcode);
	set_trap_gate(7,0,dev_not_available);
	set_trap_gate(8,0,double_fault);
	set_trap_gate(9,0,coprocessor_segment_overrun);
	set_trap_gate(10,0,invalid_TSS);
	set_trap_gate(11,0,segment_not_present);
	set_trap_gate(12,0,stack_segment_fault);
	set_trap_gate(13,0,general_protection);
	set_trap_gate(14,0,page_fault);
	//15 Intel reserved. Do not use.
	set_trap_gate(16,0,x87_FPU_error);
	set_trap_gate(17,0,alignment_check);
	set_trap_gate(18,0,machine_check);
	set_trap_gate(19,0,SIMD_exception);
	set_trap_gate(20,0,virtualization_exception);

	//set_system_gate(SYSTEM_CALL_VECTOR,0,system_call);

}

