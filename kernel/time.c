
#include "time.h"
#include "lib.h"

#define CMOS_READ(addr) ({ \
io_out8(0x70,0x80 | addr); \
io_in8(0x71); \
})

int get_time(struct time *time)
{
	cli();

	do
	{	time->year =   CMOS_READ(0x09) + CMOS_READ(0x32) * 0x100;
		time->month =  CMOS_READ(0x08);
		time->day =    CMOS_READ(0x07);	
		time->hour =   CMOS_READ(0x04);	
		time->minute = CMOS_READ(0x02);
		time->second = CMOS_READ(0x00);
	}while(time->second != CMOS_READ(0x00));
	
	io_out8(0x70,0x00); 

	sti();
}

int get_second_now()
{


	cli();

	int second_cmos = CMOS_READ(0x00);
	io_out8(0x70, 0x00);

	sti();

	return second_cmos;

}
