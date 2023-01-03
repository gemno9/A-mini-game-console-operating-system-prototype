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
#include "game.h"

void game()
{
game_menu:
	print("now,you can play game in this system,this system now supoort only one game kill the monster \n 1.kill the monster. \n");
	while (1)
	{
		game_select_start = 1;
		if (select_on != 0)
			choosing_game(getkey());
		if (menu == 1)
		{
			menu = 0;
			goto game_menu;
		}
	}

}

void choosing_game(unsigned int x)
{
	switch (x)
	{
	case 0x02: { kill_the_monster(); menu = 1; break; };
	default:
	{
		;                     //nothing to do
	}
	};

}

unsigned int choosing_option(unsigned int x)
{
	currentword = 0;
	switch (x)
	{
	case 0x02: {return 1; };
	case 0x03: {return 2; };
	case 0x04: {return 3; };
	case 0x05: {return 4; };
	case 0x06: {return 5; };
	case 0x07: {return 6; };
	case 0x08: {return 7; };
	case 0x09: {return 8; };
	case 0x0a: {return 9; };
	case 0x30: {return 10; };           //push 'b' to exit game
	default:
	{
		return 0;
	};
	};
}

