#include "Task_Ui.h"
#include "Task_Judge.h"

static uint8_t send_seq = 0;

union 
{
	UiFrame_t Frame;
	uint8_t UiFrameBuffer[128];
}UiFrame;

void Task_Ui(void *parameters)
{
    InitPeripheral_UI();
    TickType_t xLastWakeUpTime;
    xLastWakeUpTime = xTaskGetTickCount();
    while (1)
    {
			FrameUpdata();
			vTaskDelayUntil(&xLastWakeUpTime, 500);
    }
}

/**
 * @brief  判断当前哪个graphic为被使用
 * @note   
 * @retval 当前第一个空的graphic
 */
uint8_t check_empty_graphic(void)
{
    for(int i = 0; i < GRAPHIC_NUM; i++)
    {
        if(graphic_data[i].operate_tpye == OPERATE_NULL)
        {
            return i;
        }
    }
    return GRAPHIC_NUM;
}
/**
 * @brief  判断是否为红方队伍
 * @note   
 * @retval 0 未收到 1 红 2 蓝
 */
uint8_t WhichTeam(void)
{
    if(ext_game_robot_state.robot_id == 0)
    {
        return 0;
    }
    else if(ext_game_robot_state.robot_id < 10)
    {
        return 1;
    }
    else if(ext_game_robot_state.robot_id > 100)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  数据帧内容更新
 * @note   需要画什么，怎么画在这里面更新
 * @retval 0 不成功，此时不发送 1：更新成功
 */
uint8_t FrameUpdata(void)
{
    if(header_data.sender_ID == 0 || header_data.receiver_ID == 0)
    {
        if(InitPeripheral_UI() == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief 初始化UI
 * @note
 * @retval
 */
uint8_t InitPeripheral_UI(void)
{
    //这个封装我没试过，可能有问题，开学了试试
    /*
    UiFrame.Frame.frame_header.Frame.SOF = 0XA5;
    UiFrame.Frame.frame_header.Frame.seq = 0;
    UiFrame.Frame.frame_header.Frame.data_length.Frame = 0;
	  */
    /*
		|SOF1|data_length2|seq1|CRC81|
		|frame_header-----5----------|cmd_id 2|data n|
		|UiFrame_t(UiFrameBuffer[128])-----------|
	  */
    UiFrame.UiFrameBuffer[0] = 0XA5;//SOF
    UiFrame.UiFrameBuffer[5] = 0X01;//cmd_id
    UiFrame.UiFrameBuffer[6] = 0X03;

    header_data.sender_ID = ext_game_robot_state.robot_id;

    switch (WhichTeam())
    {
    case 0:
        /* code */
        return 0;

    case 1: // robotID 1~7  clientID 0x0101~0x0107
        header_data.receiver_ID = (ext_game_robot_state.robot_id | 0x0100);
        break;
    case 2: // robotID 101~107  clientID 0x0165~0x016A
        header_data.receiver_ID = ext_game_robot_state.robot_id + 256;
        break;
    default:
        return 0;
    }
    return 1;
}

/**
 * @brief  画直线
 * @note   调用该函数画直线(我现在没检查坐标，因为我不知道多少是越界，用的时候自己注意一下)
 * @param  start_x: 直线起始x坐标
 * @param  start_y: 直线起始y坐标
 * @param  end_x: 直线结束x坐标
 * @param  end_y: 直线结束y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t draw_line(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if(index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].operate_tpye = OPERATE_ADD;
    graphic_data[index].graphic_tpye = GRAPHIC_LINE;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }
		return 1;
}

/**
 * @brief  画矩形
 * @note   调用该函数画矩形(没检查坐标)
 * @param  start_x: 起始x坐标
 * @param  start_y: 起始y坐标
 * @param  end_x: 结束x坐标
 * @param  end_y: 结束y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t draw_rect(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if(index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].operate_tpye = OPERATE_ADD;
    graphic_data[index].graphic_tpye = GRAPHIC_RECT;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }   
		return 1;
}
/**
 * @brief  画圆
 * @note   调用该函数画圆(没检查坐标)
 * @param  start_x: 圆心x坐标
 * @param  start_y: 圆心y坐标
 * @param  radius: 半径
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t draw_circle(uint32_t start_x,uint32_t start_y,uint32_t radius,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if(index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].operate_tpye = OPERATE_ADD;
    graphic_data[index].graphic_tpye = GRAPHIC_CIRCLE;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].radius = radius;

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }   
		return 1;
}

/**
 * @brief  画椭圆
 * @note   调用该函数画椭圆(没检查坐标)
 * @param  start_x: 圆心x坐标
 * @param  start_y: 圆心y坐标
 * @param  end_x: x半轴长
 * @param  end_y: y半轴长
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t draw_ellipse(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if(index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].operate_tpye = OPERATE_ADD;
    graphic_data[index].graphic_tpye = GRAPHIC_ELLIPSE;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }   
		return 1;
}

/**
 * @brief  画弧
 * @note   调用该函数画弧(没检查坐标)
 * @param  start_x: 圆心x坐标
 * @param  start_y: 圆心y坐标
 * @param  end_x: x半轴长
 * @param  end_y: y半轴长
 * @param  start_angle: 起始角度
 * @param  end_angle: 停止角度
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t draw_arc(uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint32_t start_angle,uint32_t end_angle,uint8_t name[],uint32_t width,uint8_t layer,uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if(index >= GRAPHIC_NUM)
    {
        return 0;
    }

    graphic_data[index].graphic_name[0] = name[0];
    graphic_data[index].graphic_name[1] = name[1];
    graphic_data[index].graphic_name[2] = name[2];

    graphic_data[index].width = width;
    graphic_data[index].operate_tpye = OPERATE_ADD;
    graphic_data[index].graphic_tpye = GRAPHIC_ELLIPSE;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;
    graphic_data[index].start_angle = start_angle;
    graphic_data[index].end_angle = end_angle;

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }   
		return 1;
}

/**
 * @brief  画字符
 * @note   调用该函数画字符(没检查坐标)
 * @param  start_x: 左顶点x坐标
 * @param  start_y: 左顶点y坐标
 * @param  radius: 字体大小
 * @param  text_lenght: 字符长度
 * @param  text[]: 字符
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零)
 * @param  width: 线宽
 * @param  layer: 图层
 * @param  color: 颜色
 * @retval 1:配置正确  0:所有graphic已经堆满  2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t draw_text(uint32_t start_x,uint32_t start_y,uint32_t radius,uint32_t text_lenght,uint8_t text[],uint8_t name[],uint32_t width,uint8_t layer,uint8_t color)
{
    uint8_t index;
    index = check_empty_graphic();

    if(index >= GRAPHIC_NUM)
    {
        return 0;
    }

    custom_character.graphic_name[0] = name[0];
    custom_character.graphic_name[1] = name[1];
    custom_character.graphic_name[2] = name[2];

    custom_character.width = width;
    custom_character.operate_tpye = OPERATE_ADD;
    custom_character.graphic_tpye = GRAPHIC_CIRCLE;
    
    custom_character.start_x = start_x;
    custom_character.start_y = start_y;
    custom_character.radius = radius;
    custom_character.text_lenght = text_lenght;
		int i=0;
		while(i<30)
		{
			custom_character.text_data[i]= text[i];
			i++;
		}

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        graphic_data[index].layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        graphic_data[index].color = color;
    }   
		return 1;
}

/**
 * @brief  
 * @note   
 * @retval 
 */
uint8_t send_graphic(void)
{
    uint8_t graphic_num;
    graphic_num = check_empty_graphic();
    uint8_t data_size;
    if(graphic_num == 0)
    {
        //全是空的不要发
        return 0;
    }
    else if(graphic_num == 1)
    {
        header_data.data_cmd_id = 0X0101;
        UiFrame.UiFrameBuffer[1] = 21;//data_length
    }
    else if(graphic_num == 2)
    {
        header_data.data_cmd_id = 0X0102;
        UiFrame.UiFrameBuffer[1] = 36;
    }
    else if(graphic_num > 2 && graphic_num <= 5)
    {
        header_data.data_cmd_id = 0X0103;
        UiFrame.UiFrameBuffer[1] = 81;
    }
    else if(graphic_num > 5 && graphic_num <= 7)
    {
        header_data.data_cmd_id = 0X0104;
        UiFrame.UiFrameBuffer[1] = 111;
    }
    else
    {
        header_data.data_cmd_id = 0X0104;
        UiFrame.UiFrameBuffer[1] = 111;
    }
    send_seq++;
    UiFrame.UiFrameBuffer[3] = send_seq;
    data_size = 15 * graphic_num;

    memcpy(UiFrame.UiFrameBuffer+JUDGE_DATA_OFFSET, &header_data, sizeof(ext_student_interactive_header_data_t));
    memcpy(UiFrame.UiFrameBuffer+JUDGE_DATA_OFFSET + sizeof(ext_student_interactive_header_data_t), graphic_data, data_size);
    Append_CRC_Check_Sum(UiFrame.UiFrameBuffer, UiFrame.UiFrameBuffer[1]);
    HAL_UART_Transmit_DMA(&huart3,UiFrame.UiFrameBuffer,(UiFrame.UiFrameBuffer[1]+9)); //头帧加校验共9字节
    //相当于清空数据
    for(int i = 0; i < graphic_num; i++)
    {
			graphic_data[i].operate_tpye = OPERATE_NULL;
    }
		return 1;
}

