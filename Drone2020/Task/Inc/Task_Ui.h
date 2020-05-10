#ifndef _TASK_UI_H__
#define _TASK_UI_H__

#include "Sys_Def.h"

#define UI_PRIORITY   2
#define UI_STACK_SIZE 256

#define GRAPHIC_NUM (7)//UI画图图画数
#define LAYER_NUM   (9)//UI画图图层数

/*-------------------------------- UI枚举定义开始-------------------------------- */
typedef enum
{
  OPERATE_NULL = 0,
  OPERATE_ADD,
  OPERATE_CHANGE,
  OPERATE_DELETE,
}OPERATE_TYPE;

//图形类型，英文能看懂
typedef enum
{
  GRAPHIC_LINE = 0,
  GRAPHIC_RECT,
  GRAPHIC_CIRCLE,
  GRAPHIC_ELLIPSE,
  GRAPHIC_ARC,//圆弧，我翻译搜的，你懂就行
  GRAPHIC_FLOAT,
  GRAPHIC_INT,
  GRAPHIC_CHAR,
}GRAPHIC_TYPE;

typedef enum
{
  COLOR_TEAM = 0,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_ORANGE,
  COLOR_AMARANTH, //紫红色
  COLOR_PINK,
  COLOR_CYAN,
  COLOR_BLACK,
  COLOR_WHITE,
  COLOR_NUM,
}COLOR_TYPE;
/*-------------------------------- UI枚举变量定义结束-------------------------------- */
/*-------------------------------- UI结构体定义开始---------------------------------- */
typedef __packed struct 
{
    uint16_t data_cmd_id;
    uint16_t sender_ID;
    uint16_t receiver_ID;
}ext_student_interactive_header_data_t; //交互数据帧头

typedef __packed struct
{ 
    uint8_t* data;
}robot_interactive_data_t;//交互数据

typedef __packed struct 
{
    uint8_t graphic_name[3];
    uint32_t operate_tpye:3;
    uint32_t graphic_tpye:3;
    uint32_t layer:4;
    uint32_t color:4;
    uint32_t start_angle:9;
    uint32_t end_angle:9;
    uint32_t width:10;
    uint32_t start_x:11;
    uint32_t start_y:11;
    uint32_t radius:10;
    uint32_t end_x:11;
    uint32_t end_y:11;
}graphic_data_struct_t;

typedef __packed struct
{ 
	  graphic_data_struct_t graphic_data_struct;
    uint8_t data[30]; 
} ext_client_custom_character_t;

typedef __packed struct 
{   
  graphic_data_struct_t  grapic_data_struct; 
} ext_client_custom_graphic_single_t;

typedef __packed struct 
{ 
  graphic_data_struct_t  grapic_data_struct[2]; 
} ext_client_custom_graphic_double_t; 

typedef __packed struct 
{ 
  graphic_data_struct_t  grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

typedef __packed struct 
{ 
  graphic_data_struct_t  grapic_data_struct[7]; 
} ext_client_custom_graphic_seven_t;

/*
这里是要发送数据的帧头				|帧格式
															|frame_header (5-byte) 
															|cmd_id (2-byte) 
															|data (n-byte) 
															|frame_tail (2-byte，CRC16，整包校验)

															|Frame header 格式:
															|SOF    data_length seq     CRC8 
															|1-byte 2-byte      1-byte  1-byte 
*/
typedef __packed struct 
{
	uint8_t SOF;
	__packed union 
	{
		uint16_t Frame;
		uint8_t FrameBuffer[2];
	}data_length;
	uint8_t seq;
	uint8_t CRC8;
}frame_header_t;

/*
这里是把帧头,cmd_id与数据合起来
数据长度为121
但是还没有帧尾
*/
typedef __packed struct
{
	__packed union
	{
		frame_header_t Frame;
		uint8_t FrameBuffer[5];
	}frame_header;

	__packed union
	{
		uint16_t Frame;
		uint8_t FrameBuffer[2];
	}cmd_id;

	uint8_t data[121];
}UiFrame_t;
/*-------------------------------- UI结构体定义结束---------------------------------- */

extern ext_student_interactive_header_data_t header_data;
extern robot_interactive_data_t robot_interactive_data;
extern ext_client_custom_character_t custom_character;
extern graphic_data_struct_t graphic_data[GRAPHIC_NUM];

uint8_t InitPeripheral_UI(void);

uint8_t FrameUpdata(void);

uint8_t draw_line(uint8_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_rect(uint8_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t write_int(uint8_t operate,uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width,uint32_t size, uint8_t layer,uint8_t color,uint32_t contents);

uint8_t write_float(uint8_t operate,uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width,uint32_t size,uint32_t decimal,uint8_t layer,uint8_t color,uint32_t contents);

uint8_t write_chars(uint8_t operate, uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width, uint32_t size, uint8_t layer,uint8_t color, uint8_t contents[], uint32_t length);


uint8_t draw_circle(uint8_t operate, uint32_t start_x,uint32_t start_y,uint32_t radius,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_ellipse(uint8_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_arc(uint8_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint32_t start_angle,uint32_t end_angle,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);


uint8_t send_graphic(void);

uint8_t check_empty_graphic(void);

uint8_t load_chars(uint8_t chars_to_send[], uint8_t length);

uint8_t Get_Shoot_Status(void);

#endif
