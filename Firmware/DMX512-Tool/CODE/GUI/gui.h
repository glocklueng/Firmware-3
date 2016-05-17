#ifndef _GUI_H
#define _GUI_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#if MENU_EXTRA_EN > 0
#define   COLOUR_MENU_SIZE   3
#else
#define   COLOUR_MENU_SIZE   2
#endif

#if MENU_EXTRA_EN > 0
#define   MENU_EXTRA_HDR1   0x7C
#define   MENU_EXTRA_HDR2   0xB6
#define   MENU_EXTRA_HDR3   0xF5
#endif

enum {
	LANGUAGE_CHINESE = 0,
	LANGUAGE_ENGLISH
};

enum {
	JOB_MODE_SELF = 0,
	JOB_MODE_HOST,
	JOB_MODE_SLAVE
};

enum {
	MODBUS_DMX = 0,
	MODBUS_9221,
	MODBUS_1829,
	MODBUS_1804
};

enum {
	PWM_OUTPUT = 0,
	TM18XX_400K,
	MY9221,
	MBI6020,
	MBI6021,
	MBI6023,
	MBI6030,
	WS2803,
	LPD8806,
	TLS3006_8,
	TLS3001,
	TM18XX_800K,
	TM1829,
	MY9231,
	TLC5971,
	MBI6033,
};

enum {
	PWM_60HZ = 1,
	PWM_120HZ,
	PWM_240HZ,
	PWM_480HZ,
	PWM_960HZ,
	PWM_1920HZ,
	PWM_3840HZ,
};

enum {
	BAUD_SELF = 0,
	BAUD_250K = 2,
};

enum {
	VALID_HIGH = 0,
	VALID_LOW  = 1,
};

enum {
	SELF_NO = 0,
	SELF_R_G_B,
	SELF_A,
	SELF_B,
	SELF_C,
	SELF_D,
	SELF_ALL,
	SELF_CHANGE,
	SELF_FAST_SPEED,
	SELF_SPEED,
	SELF_SLOW_SPEED,
};

enum {
	GAMMA_1_0 = 0,
	GAMMA_2_2,
	GAMMA_3_0,
};

/*------------------------------*
*  type definitions             *
*-------------------------------*/
typedef struct Menu {
	u8 index;
	u16 ChildrenMenuTotal;
	struct Menu *ParentMenu;
	struct Menu *ChildrenMenu;
	void (*display)(void);
} MenuDef;

typedef enum {
	MENU_INDEX_MAIN = 0,
	
	MENU_INDEX_ADDRESS,
	MENU_INDEX_ADDRESS_LINE,
	MENU_INDEX_ADDRESS_RELAY,
	MENU_INDEX_ADDRESS_LINE_FINISH,
	MENU_INDEX_ADDRESS_RELAY_FINISH,
	
	MENU_INDEX_PARAMETERS,
	MENU_INDEX_PARAMETERS_FINISH,
	
	MENU_INDEX_COLOUR,
	MENU_INDEX_COLOUR_ALL,
	MENU_INDEX_COLOUR_CH,	
	MENU_INDEX_COLOUR_CHANGE_3,
	MENU_INDEX_COLOUR_CHANGE_4,
	MENU_INDEX_COLOUR_ROTATION_3,
	MENU_INDEX_COLOUR_ROTATION_4,
	MENU_INDEX_COLOUR_ROTATION_CH,
	MENU_INDEX_COLOUR_CHANGE_1,
	
	MENU_INDEX_PATTERN,
	
	MENU_INDEX_SETTING,
	
	MENU_INDEX_SYSTEM,	
	
	MENU_INDEX_EXTRA = 0xBB,
	MENU_INDEX_EXTRA_FINISH,
} MenuIndexDef;

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern u8 language;
extern u8 job_mode;
extern u8 modbus;

extern u16 light_start_num;
extern u16 light_total;
extern u16 light_start_address;

extern u8 chip_type;
extern u8 gray_level;
extern u8 pwm_frequency;
extern u8 dmx_channel_total;
extern u8 output_type;
extern u8 serial_baud;
extern u8 output_polarity;
extern u8 chip_self_check;
extern u8 half_power_output_temperature;
extern u8 close_light_temperature;
extern u8 colour_mode;
extern u8 gamma;

#define   CH_SIZE   513 //因DMX512第一个起始码在数组内因此需512+1个
extern u8 channel[CH_SIZE];

extern u8 rotation_ch_status;
extern u16 rotation_ch_cnt;

extern u16 display_row;
extern u8 pointer_row;
extern u8 pointer_col;

extern u16 last_display_row;
extern u8 last_pointer_row;

extern u8 colour_total;

extern u8 pattern_step;
extern u8 pattern_r, pattern_g, pattern_b, pattern_w;

extern u8 packet_num;

extern u8 dmxbuf[64];

/*------------------------------*
*  macros for menu              *
*-------------------------------*/
extern MenuDef *menu;
/*------------------一级菜单分割线------------------*/
extern MenuDef StartMenuVar;
/*------------------二级菜单分割线------------------*/
extern MenuDef MainMenu[6];
/*------------------三级菜单分割线------------------*/
extern MenuDef AddressMenu[2];
extern MenuDef AddressLineFinishMenuVar;
extern MenuDef AddressRelayFinishMenuVar;
extern MenuDef ParametersFinishMenuVar;
extern MenuDef ColourMenu[COLOUR_MENU_SIZE];
extern MenuDef PatternMenu[6];
#if MENU_EXTRA_EN > 0
extern MenuDef ExtraFinishMenuVar;
#endif

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void GUI_init(void);
extern void crc16_1021(u8 *message, u8 len);
extern void YT_send_flicker(void);
extern void YT_send_address_line(u16 num, u16 addr_interval, u16 addr);
extern void YT_send_address_start(void);
extern void YT_send_data(u16 num, u16 addr_interval, u16 addr);
extern void YT_send_parameters_start(void);
extern void YT_send_stop(void);
extern void YT_send_gamma_1_0(void);
extern void YT_send_gamma_2_2(void);
extern void YT_send_gamma_3_0(void);
extern void GUI_display_number(u8 row, u8 col, u16 num);
extern void GUI_clear_number(u8 row, u8 col);

/*------------------------------*
*  others for task *
*-------------------------------*/
extern OS_STK GUI_TASK_STK[GUI_STK_SIZE];
extern void gui_task(void *pdata);

#endif

