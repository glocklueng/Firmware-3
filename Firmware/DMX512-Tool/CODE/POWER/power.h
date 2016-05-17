#ifndef _POWER_H
#define _POWER_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   CHG_STAT   PAin(2)
#define   POWER_EN   PBout(9)

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void POWER_init(void);

#endif

