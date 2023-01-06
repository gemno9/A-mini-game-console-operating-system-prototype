
//This program obtains the real world time from the "RTC" and is used for the implementation of the "Random Program".

//The time data of "RTC" is saved in CMOS register.We access the "CMOS" registers with the help of I/O ports "0x70" and "0x71".

#ifndef __TIME_H__

#define __TIME_H__

struct time
{
	//  Function	index value      Value range
	int second;       //00              0-59  
	int minute;	      //02              0-59
	int hour;	      //04              12 hours 1-12 , 24 hours :0-23
	int day;	      //07              1-7
	int month;	      //08              1-31
	int year;	      //09+32           0-99
};

struct time time;

#define	BCD2BIN(value)	(((value) & 0xf) + ((value) >> 4 ) * 10)

int get_time(struct time *time);

int get_second_now();

#endif
