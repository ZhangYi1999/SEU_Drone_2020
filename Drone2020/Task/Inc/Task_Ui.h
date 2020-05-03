#ifndef _TASK_UI_H_
#define _TASK_UI_H_

#include "System.h"

#define UI_PRIORITY   2
#define UI_STACK_SIZE 256

#define GRAPHIC_NUM (7)
#define LAYER_NUM   (9)

/* ---------------------------- Global functions ---------------------------- */
uint8_t InitPeripheral_UI(void);

uint8_t FrameUpdata(void);

uint8_t draw_line(uint32_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_rect(uint32_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t write_int(uint32_t operate,uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width,uint32_t size, uint8_t layer,uint8_t color,uint32_t contents);

uint8_t write_float(uint32_t operate,uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width,uint32_t size,uint32_t decimal,uint8_t layer,uint8_t color,uint32_t contents);

uint8_t write_chars(uint32_t operate, uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width, uint32_t size, uint8_t layer,uint8_t color, uint8_t contents[], uint32_t length);


uint8_t draw_circle(uint32_t start_x,uint32_t start_y,uint32_t radius,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_ellipse(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);

uint8_t draw_arc(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint32_t start_angle,uint32_t end_angle,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color);


uint8_t send_graphic(void);

uint8_t check_empty_graphic(void);

uint8_t load_chars(uint8_t chars_to_send[], uint8_t length);

uint8_t Get_Shoot_Status(void);

#endif
