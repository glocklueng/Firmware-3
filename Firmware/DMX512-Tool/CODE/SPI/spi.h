#ifndef _SPI_H
#define _SPI_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   _SPI1   1 //SI4463
#define   _SPI2   1 //LCD

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
#if _SPI1 > 0
extern void SPI1_init(void);
//extern void SPI1_WriteByte(u8 data);
//extern u8 SPI1_ReadByte(void);
extern u8 SPI1_ExchangeByte(u8 data);
#endif

#if _SPI2 > 0
extern void SPI2_init(void);
extern void SPI2_WriteByte(u8 data);
//extern u8 SPI2_ReadByte(void);
#endif

#endif

