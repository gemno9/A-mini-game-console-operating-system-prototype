
#ifndef __random_H__
#define __random_H__

static unsigned long int seed = 1;    //seed

void srand_my();                       //init seed
unsigned long int rand_my();           // generate a random number
unsigned long int randrange(int x, int y);  //Generate a random number in a range

#endif