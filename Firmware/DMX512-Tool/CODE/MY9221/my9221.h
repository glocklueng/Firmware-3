#ifndef _MY9221_H
#define _MY9221_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for others            *
*-------------------------------*/
#define   MY9221_CMD   0x0000

#define   MY9221_DATA   PCout(1)
#define   MY9221_CLK    PCout(0)

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void MY9221_init(void);
extern void SendMY9221(u8 *buff, u16 len);

#endif

