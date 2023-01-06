#include "random.h"
#include "time.h"

void srand_my()
{
	seed = (unsigned int)get_second_now();
}

unsigned long int rand_my()
{
	seed = seed * 1103515245 + 12345;
	return (unsigned int)(seed / 65536) % 32768;
}



unsigned long int randrange(int x, int y)       //rand range [x,y]
{
	return rand_my() % (y - x + 1) + x;
}