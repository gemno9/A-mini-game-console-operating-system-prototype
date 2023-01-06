#include "errno.h"
#include "print.h"


unsigned long no_system_call(void)
{
	print("no_system_call is calling\n");
	return -ENOSYS;
}


unsigned long sys_putstring(char *string)
{
	print(string);
	return 0;
}





