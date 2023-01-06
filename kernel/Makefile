all: system_tmp kallsyms.o
	ld -b elf64-x86-64 -z muldefs -o system head.o entry.o main.o print.o trap.o memory.o interrupt.o task.o sys.o syscalls.o kallsyms.o clear_screen.o game.o kill_the_monster.o 8259A.o time.o random.o -T Kernel.lds 
	objcopy -I elf64-x86-64 -S -R ".eh_frame" -R ".comment" -O binary system kernel.bin

system_tmp: head.o entry.o main.o print.o trap.o memory.o interrupt.o task.o sys.o syscalls.o clear_screen.o game.o kill_the_monster.o 8259A.o time.o random.o
	ld -b elf64-x86-64 -z muldefs -o system_tmp head.o entry.o main.o print.o trap.o memory.o interrupt.o task.o sys.o syscalls.o clear_screen.o game.o kill_the_monster.o 8259A.o time.o random.o -T Kernel.lds 

head.o:	head.S
	gcc -E  head.S > head.s
	as --64 -o head.o head.s

entry.o: entry.S
	gcc -E  entry.S > entry.s
	as --64 -o entry.o entry.s
	
main.o:	main.c
	gcc  -mcmodel=large -fno-builtin -m64 -c main.c
	
print.o: print.c
	gcc  -mcmodel=large -fno-builtin -m64 -c print.c

trap.o: trap.c
	gcc  -mcmodel=large -fno-builtin -m64 -c trap.c

memory.o: memory.c
	gcc  -mcmodel=large -fno-builtin -m64 -c memory.c

interrupt.o: interrupt.c
	gcc  -mcmodel=large -fno-builtin -m64 -c interrupt.c
	
task.o: task.c
	gcc  -mcmodel=large -fno-builtin -m64 -c task.c	

sys.o: sys.c
	gcc  -mcmodel=large -fno-builtin -m64 -c sys.c	

syscalls.o: syscalls.c
	gcc  -mcmodel=large -fno-builtin -m64 -c syscalls.c	

kallsyms.o: kallsyms.c system_tmp
	gcc -o kallsyms kallsyms.c
	nm -n system_tmp | ./kallsyms > kallsyms.S
	gcc -c kallsyms.S
clear_screen.o:	clear_screen.c
	gcc  -mcmodel=large -fno-builtin -m64 -c clear_screen.c
	
game.o: game.c
	gcc  -mcmodel=large -fno-builtin -m64 -c game.c
kill_the_monster.o: kill_the_monster.c
	gcc  -mcmodel=large -fno-builtin -m64 -c kill_the_monster.c
8259A.o:
	gcc  -mcmodel=large -fno-builtin -m64 -c 8259A.c
time.o:
	gcc  -mcmodel=large -fno-builtin -m64 -c time.c
random.o:
	gcc  -mcmodel=large -fno-builtin -m64 -c random.c
clean:
	rm -rf *.o *.s~ *.s *.S~ *.c~ *.h~ system system_tmp Makefile~ Kernel.lds~ kernel.bin kallsyms kallsyms.o kallsyms.S

