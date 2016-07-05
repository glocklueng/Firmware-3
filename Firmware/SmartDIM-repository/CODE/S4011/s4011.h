#ifndef _S4011_H
#define _S4011_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"
#include "i2c.h"

/*------------------------------*
*  macros for const             *
*-------------------------------*/
#define   S4011_WRITE   0xA2
#define   S4011_READ    0xA3

/*------------------------------*
* macros for s4011 base address *
*-------------------------------*/
#define   S4011_BASE_ADDR_USER   0x6000
#define   S4011_BASE_ADDR_EPC    0x2004
#define   S4011_BASE_ADDR_TID    0x4000

/*------------------------------*
* macros for s4011 address area *
*-------------------------------*/
#define   S4011_ADDR_TYPE      S4011_BASE_ADDR_EPC
#define   S4011_ADDR_VERSION   S4011_BASE_ADDR_EPC + 2
#define   S4011_ADDR_POWER     S4011_BASE_ADDR_EPC + 4
#define   S4011_ADDR_PERCENT   S4011_BASE_ADDR_EPC + 6

/*------------------------------*
*  public functions             *
*-------------------------------*/
extern u8 S4011_ReadByte(u16 addr);
extern void S4011_WriteByte(u16 addr, u8 byte);

#endif