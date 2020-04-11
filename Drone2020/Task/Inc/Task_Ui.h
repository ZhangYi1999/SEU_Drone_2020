#ifndef _TASK_UI_H_
#define _TASK_UI_H_

#include "System.h"

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

uint8_t FrameUpdata(void);

uint8_t InitPeripheral_UI(void);

void Task_Ui(void *parameters);

void Task_UITransimit(void *parameters);

uint8_t draw_line(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_rect(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_circle(uint32_t start_x,uint32_t start_y,uint32_t radius,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_ellipse(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_arc(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint32_t start_angle,uint32_t end_angle,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_text(uint32_t start_x,uint32_t start_y,uint32_t radius,uint32_t text_lenght,uint8_t text[],uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t send_graphic(void);

uint8_t check_empty_graphic(void);
#endif
