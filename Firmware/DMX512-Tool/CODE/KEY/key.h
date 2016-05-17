#ifndef _KEY_H
#define _KEY_H	 

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   KEEP_EN     1

#define   KEY_TRUE    0
#define   KEY_FALSE   1

#define   KEY_CNT     5

#if KEEP_EN > 0
#define   KEY_KEEP_CNT         KEY_CNT+155
#define   KEY_KEEP_INIT        KEY_CNT+150
#endif

#define   KEY_UP       PDin(2)
#define   KEY_DOWN     PAin(3)
#define   KEY_LEFT     PBin(6)
#define   KEY_RIGHT    PBin(5)
#define   KEY_OK       PBin(4)
#define   KEY_RETURN   PBin(7)
#define   KEY_POWER    PBin(8)

#define   KEY_POWER_KEEP_CNT   250
#define   KEY_POWER_TRUE    1
#define   KEY_POWER_FALSE   0

/*------------------------------*
*  type definitions             *
*-------------------------------*/
typedef struct
{
	u8 value;
	u16 cnt;
	u8 lock;
	void (*handler)(void);
} KeyDef;

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void KEY_init(void);
extern void KeyPowerCheck(void);
extern void KeyScan(void);
extern void KeyService(void);

/*------------------------------*
*  others for task *
*-------------------------------*/
extern OS_STK KEY_TASK_STK[KEY_STK_SIZE];
extern void key_task(void *pdata);

#endif
