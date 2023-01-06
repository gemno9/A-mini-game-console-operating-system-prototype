#include"clear_screen.h"
#include"game.h"
#include"keyword.h"
#include"print.h"
#include"UEFI_boot_param_info.h"
#include "random.h"
#include "8259A.h"

unsigned int battle_result;

struct ability
{	char career;
	unsigned int atk;
	unsigned int hp; 
	unsigned int def; 
};

struct  ability player;

 struct ability warrior = {
	.career='w',        //career
	.atk=2,       //atk
	.hp=5,         //hp
	.def=1         //def
};

 struct ability magician = {
	.career='m',       //career
	.atk=3,          //atk
	.hp=3,          //hp
	.def=0           //def
};

 struct ability monster_warrior_level_1={
	 .career='w',
	 .atk=2,
	 .hp=4,
	 .def=1
 };

 struct ability monster_magician_level_1 = {
	.career = 'm',
	.atk = 3,
	.hp = 2,
	.def = 0
 };

 struct ability monster_normal_level_1 = {
   .career = 'n',
   .atk = 2,
   .hp = 3,
   .def = 1
 };


 struct ability Boss_warrior = {
	.career = 'w',
	.atk = 10,
	.hp = 12,
	.def = 7
 };

 struct ability Boss_magician = {
   .career = 'm',
   .atk = 18,
   .hp = 13,
   .def = 0
 };

 unsigned int battle(struct ability player, struct ability monster);
 struct ability right_w(struct ability monster);
 struct ability right_m(struct ability monster);
 struct ability right_n(struct ability monster);
 struct ability normnal_monster(struct ability monster);
 struct ability player_n(struct ability player);
 struct ability normnal_w(struct ability normalmonster);
 struct ability normnal_m(struct ability normalmonster);
 struct ability player_w(struct ability player);
 struct ability player_m(struct ability player);


void kill_the_monster()
{
	//init player
	player.career = 'n';
	player.atk = 0;
	player.hp = 0;
	player.def = 0;

	//init warrior monster
	monster_warrior_level_1.career = 'w';
	monster_warrior_level_1.atk = 2;
	monster_warrior_level_1.hp = 4;
	monster_warrior_level_1.def = 1;

	//init magician monster
	monster_magician_level_1.career = 'm';
	monster_magician_level_1.atk = 3;
	monster_magician_level_1.hp = 2;
	monster_magician_level_1.def = 1;

	//init normal monster
	monster_normal_level_1.career = 'n';
	monster_normal_level_1.atk = 2;
	monster_normal_level_1.hp = 3;
	monster_normal_level_1.def = 1;

	clear_screen();
	print("hello ,welcome to kill the monster \n pusn 'b' to exit game \n now,choose your career\n 1.warrior ATK:2  HP:5  DEF:1 \n 2.magician ATK:3  HP:3  DEF:0 \n ");
	
	int round = 1;

choose_career:	
		game_select_start = 1;
		if (select_on != 0)
		{
			switch (getkey())
			{
			case 0x02: {player = warrior; break; };
			case 0x03: {player = magician; break; };
			case 0x30: {goto game_over; };
			default: {goto choose_career; };
			}
		}
		else
			goto choose_career;

	print("your career:%c \n ATK %d \n HP %d \n DEF %d \n ", player.career, player.atk, player.hp, player.def);
enemy_inf:
	print("round:%d \n", round);
	print("choose enemy:\n ");
	print("1.warrior:        ATK:%d	HP:%d	DEF:%d \n ", monster_warrior_level_1.atk, monster_warrior_level_1.hp, monster_warrior_level_1.def);
	print("2.magician:       ATK:%d	HP:%d	DEF:%d \n ", monster_magician_level_1.atk, monster_magician_level_1.hp, monster_magician_level_1.def);
	print("3.normal monster: ATK:%d	HP:%d	DEF:%d \n ", monster_normal_level_1.atk, monster_normal_level_1.hp, monster_normal_level_1.def);

choose_1_enemy:
	battle_result = 0;
	game_select_start = 1;
	if (select_on != 0)
	{
		switch (getkey())
		{
		case 0x02:            //player choose warrior enemy
		{ battle_result = battle(player, monster_warrior_level_1);         //battel result
		if (player.career == 'w')
		{
			player.hp = player.hp + 1;
			player.def = player.def + 1;
			monster_warrior_level_1 = right_w(monster_warrior_level_1);
			monster_magician_level_1 = normnal_m(monster_magician_level_1);
			monster_normal_level_1 = normnal_monster(monster_normal_level_1);
		}
		else
			monster_warrior_level_1 = normnal_w(monster_warrior_level_1);
		player = player_w(player);         	
			break; 
		};
		case 0x03: 
		{ battle_result = battle(player, monster_magician_level_1);
		if (player.career == 'm')
		{
			player.atk = player.atk + 1;
			player.hp = player.hp + 1;
			monster_magician_level_1 = right_m(monster_magician_level_1);
			monster_warrior_level_1 = normnal_w(monster_magician_level_1);
			monster_normal_level_1 = normnal_monster(monster_normal_level_1);
		}
		else
			monster_magician_level_1 = normnal_m(monster_magician_level_1);
		player = player_m(player);		
		break; 
		};
		case 0x04: 
		{ 
			battle_result = battle(player, monster_normal_level_1); 
			player = player_n(player);
			monster_normal_level_1=normnal_monster(monster_normal_level_1);
			break; 
		};
		case 0x30: {goto game_over; };
		default: {goto choose_1_enemy; };
		}
	}
	else
		goto choose_1_enemy;

	if (battle_result == 0)
	{
		print("you loss!");
		goto game_over;
	}
	else
	{
		clear_screen();
		print("you win! \n");
	}

	print("your ability now:\n career: %c \n ATK:%d \n HP: %d \n DEF:%d \n ", player.career, player.atk, player.hp, player.def);


	if ( round < 10)
	{
		round++;
		goto enemy_inf;
	}
	else
	{
		print("Boss Battle! \n");
		print("1.warrior:        ATK:%d	HP:%d	DEF:%d \n ", Boss_warrior.atk, Boss_warrior.hp, Boss_warrior.def);
		print("2.magician:       ATK:%d	HP:%d	DEF:%d \n ", Boss_magician.atk, Boss_magician.hp, Boss_magician.def);

Boss_battle:
		battle_result = 0;
		game_select_start = 1;

		switch (getkey())
		{
		case 0x02: {battle_result = battle(player, Boss_warrior); break; };
		case 0x03: {battle_result = battle(player, Boss_magician); break; };
		default:
			goto Boss_battle;
		}

	}


	if (battle_result == 0)
	{
		print("you loss!");
		goto game_over;
	}
	else
		print("congratulations! You beat the boss!! \n");

game_over:
	print("game is over,now,you will go back to the menu\n");
}

unsigned int battle(struct ability player,struct ability monster)
{
	signed int real_damage_player = player.atk - monster.def;
	signed int real_damage_monster = monster.atk - player.def;
	signed int i = monster.hp;
	signed int j = player.hp;
	signed int result;
	if (real_damage_player <= 0) 
	{
		result = 0;
		goto end_battle;
	}
	if (real_damage_monster <= 0) 
	{
		result = 1;
		goto end_battle;
	}
	while (1)
	{
		i = i - real_damage_player;
		if (i <= 0) 
		{
			result = 1;
			break;
		}
		else
			j = j - real_damage_monster;
		if (j <= 0) 
		{
			result = 0;
			break;
		}
	}
end_battle:
	return result;
}

struct ability right_w(struct ability rightmonster)
{
	unsigned long int ability_plus[3];
	ability_plus[0] = randrange(1, 100);
	ability_plus[1] = randrange(1, 100);
	ability_plus[2] = randrange(1, 100);

	unsigned long int ex_plus = randrange(1, 100);

	int i;

	for (i = 0; i < 3; i++)
	{
		if (ability_plus[i] <= 10)
			rightmonster.atk = rightmonster.atk + 1;
		else if (ability_plus[i] <= 60)
		{
			rightmonster.def = rightmonster.def + 1;
		}
		else
			rightmonster.hp = rightmonster.hp + 1;
	}

	if (ex_plus > 50)
	{
		if (ex_plus <= 55)
			rightmonster.atk = rightmonster.atk + 1;
		else if (ex_plus <= 95)
			rightmonster.hp = rightmonster.hp + 1;
		else
			rightmonster.def = rightmonster.def + 1;
	}

	return rightmonster;

}

struct ability right_m(struct ability rightmonster)
{
	unsigned long int ability_plus[3];
	ability_plus[0] = randrange(1, 100);
	ability_plus[1] = randrange(1, 100);
	ability_plus[2] = randrange(1, 100);

	unsigned long int ex_plus = randrange(1, 100);

	int i;

	for (i = 0; i < 3; i++)
	{
		if (ability_plus[i] <= 60)
			rightmonster.atk = rightmonster.atk + 1;
		else if (ability_plus[i] <= 70)
		{
			rightmonster.def = rightmonster.def + 1;
		}
		else
			rightmonster.hp = rightmonster.hp + 1;
	}

	if (ex_plus > 50)
	{
		if (ex_plus <= 55)
			rightmonster.atk = rightmonster.atk + 1;
		else if (ex_plus <= 95)
			rightmonster.hp = rightmonster.hp + 1;
		else
			rightmonster.def = rightmonster.def + 1;
	}

	return rightmonster;

}

struct ability normnal_monster(struct ability normalmonster)
{
	unsigned long int ability_plus[2];
	ability_plus[0] = randrange(1, 100);
	ability_plus[1] = randrange(1, 100);

	int i;

	for (i = 0; i < 2; i++)
	{
		if (ability_plus[i] <= 50)
			normalmonster.atk = normalmonster.atk + 1;
		else if (ability_plus[i] > 50 && ability_plus[i] <= 75)
		{
			normalmonster.def = normalmonster.def + 1;
		}
		else
			normalmonster.hp = normalmonster.hp + 1;
	}

	return normalmonster;

}

struct ability normnal_w(struct ability normalmonster)
{
	unsigned long int ability_plus[2];
	ability_plus[0] = randrange(1, 100);
	ability_plus[1] = randrange(1, 100);

	int i;

	for (i = 0; i < 2; i++)
	{
		if (ability_plus[i] <= 5)
			normalmonster.atk = normalmonster.atk + 1;
		else if (ability_plus[i] <= 85)
		{
			normalmonster.def = normalmonster.def + 1;
		}
		else
			normalmonster.hp = normalmonster.hp + 1;
	}

	return normalmonster;
}

struct ability normnal_m(struct ability normalmonster)
{
	unsigned long int ability_plus[2];
	ability_plus[0] = randrange(1, 100);
	ability_plus[1] = randrange(1, 100);

	int i;

	for (i = 0; i < 2; i++)
	{
		if (ability_plus[i] <= 80)
			normalmonster.atk = normalmonster.atk + 1;
		else if (ability_plus[i] <= 85)
		{
			normalmonster.def = normalmonster.def + 1;
		}
		else
			normalmonster.hp = normalmonster.hp + 1;
	}

	return normalmonster;
}


struct ability player_w(struct ability playerex)
{

	unsigned long int ability_plus = randrange(1, 100);

	unsigned long int ex_plus = randrange(1, 100);

	if (ability_plus <= 50)
		playerex.atk = playerex.atk + 1;
	else if (ability_plus <= 75)
	{
		playerex.def = playerex.def + 1;
	}
	else
		playerex.hp = playerex.hp + 1;

	if (ex_plus > 50)
	{
		if (ex_plus <= 67)
			playerex.atk = playerex.atk + 1;
		else if (ex_plus <= 84)
			playerex.hp = playerex.hp + 1;
		else
			playerex.def = playerex.def + 1;
	}

	return playerex;

}

struct ability player_m(struct ability playerex)
{

	unsigned long int ability_plus = randrange(1, 100);

	unsigned long int ex_plus = randrange(1, 100);

	if (ability_plus <= 70)
		playerex.atk = playerex.atk + 1;
	else if (ability_plus <= 80)
	{
		playerex.def = playerex.def + 1;
	}
	else
		playerex.hp = playerex.hp + 1;

	if (ex_plus > 50)
	{
		if (ex_plus <= 67)
			playerex.atk = playerex.atk + 1;
		else if (ex_plus <= 84)
			playerex.hp = playerex.hp + 1;
		else
			playerex.def = playerex.def + 1;
	}

	return playerex;

}

struct ability player_n(struct ability playerex)
{

	unsigned long int ability_plus = randrange(1, 100);

	unsigned long int ex_plus = randrange(1, 100);

	if (ability_plus <= 35)
		playerex.atk = playerex.atk + 1;
	else if (ability_plus <= 70)
	{
		playerex.def = playerex.def + 1;
	}
	else
		playerex.hp = playerex.hp + 1;

	if (ex_plus > 50)
	{
		if (ex_plus <= 67)
			playerex.atk = playerex.atk + 1;
		else if (ex_plus <= 84)
			playerex.hp = playerex.hp + 1;
		else
			playerex.def = playerex.def + 1;
	}

	return playerex;

}