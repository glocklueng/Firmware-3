#ifndef _TM1829_H
#define _TM1829_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   TM1829_DATA   PCout(1)

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void TM1829_WriteCurrent(u8 Ir, u8 Ig, u8 Ib);
extern void SendTM1829(u8 *buff, u16 len);

#endif

