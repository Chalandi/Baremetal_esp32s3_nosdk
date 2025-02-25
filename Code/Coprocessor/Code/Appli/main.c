
#include<stdint.h>

volatile uint32_t dummy;

void main(void)
{
  dummy = 0xa5a5a5a5;
  for(;;);
}