#include <stdbool.h>
#include <stdint.h>

#include "utils.h"


void ftoa(float f,char *buf)
{
  int pos=0,ix,dp,num;
  if (f<0)
  {
    buf[pos++]='-';
    f = -f;
  }
  dp=0;
  while (f>=10.0)
  {
    f=f/10.0;
    dp++;
  }
  for (ix=1;ix<8;ix++)
  {
    num = (int)f;
    f=f-num;
    if (num>9)
      buf[pos++]='#';
    else
      buf[pos++]='0'+num;
    if (dp==0) buf[pos++]='.';
    f=f*10.0;
    dp--;
  }
  buf[pos] = '\0';
}
