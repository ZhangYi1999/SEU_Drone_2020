#include "Task_UI.h"
#include "Task_Judge.h"

#define REFEREE_HUART huart3
#define TASK_UI_INTERVAL (500)
#define bullet_remaining_bar_length   500 / bullet_max * (uint32_t)ext_bullet_remaining.bullet_remaining_num

//需要一个数组记录当前有多少图形要画！！！！！ 3.15晚上

void Task_Ui(void *parameters)
{
    InitPeripheral_UI();
    TickType_t xLastWakeUpTime;
    xLastWakeUpTime = xTaskGetTickCount();
    while (1)
    {
			FrameUpdata();
			vTaskDelayUntil(&xLastWakeUpTime, TASK_UI_INTERVAL);
    }
}

/* --------------------------   变量定义    ----------------------------------*/
/*
一共有帧头 5+cmdid 2+数据 121 一共长128

|SOF|data_length|seq|CRC8|
|frame_header------------|cmd_id|data|
|UiFrame_t(UiFrameBuffer[128])-------|
这是裁判系统协议联合体，可能因结构体字节对齐影响出现问题！！需后期测试
*/
union 
{
	UiFrame_t Frame;
	uint8_t UiFrameBuffer[128];
}UiFrame;

uint8_t* fric_chars;//UI发送摩擦轮状态用
uint8_t* auto_chars;//UI发送自瞄是否开启用
uint8_t* attacking_chars;//UI发送正在攻击目标用

static uint8_t send_seq; //包序号

/* --------------------------   函数定义    ----------------------------------*/
/**
 * @brief  数据帧内容更新
 * @note   需要画什么，怎么画在这里面更新
 * @retval 0 不成功，此时不发送 1：更新成功
 */
uint8_t FrameUpdata(void)
{
	static uint8_t FirstDraw = 1;//判断是初始化UI界面还是更新UI界面
	uint8_t* uname;//uint8_t数组，必须三字节
	
	if(FirstDraw == 1)
	{
		uname = (unsigned char*)"apv";//准心竖直部分aim_point_vertical
		draw_line(1, 960, 540, 960, 405, uname, 12, 6, COLOR_GREEN);//竖直长度100，宽度5，图层6，绿色，有待调整
		uname = (unsigned char*)"aph";//准心水平部分aim_point_horizontal
		draw_line(1, 855, 405, 1065, 405, uname, 12, 6, COLOR_GREEN);//水平长度60，宽度5，图层6，绿色，有待调整
		
		uname = (unsigned char*)"epb";//能量点背景energy_point_background
		draw_rect(1, 1410, 820, 1550, 890, uname, 12, 5, COLOR_BLACK);//图层5，黑色，矩形是实心的吗？有待调整。
		uname = (unsigned char*)"epv";//能量点数值energy_point_value
		write_int(1, 1410, 820, uname, 1, 10, 7, COLOR_WHITE, (uint32_t)aerial_robot_energy.energy_point);//线宽1，字体10，图层7， 白色，有待调整
		
		uname = (unsigned char*)"atb";//攻击时间背景attack_time_background
		draw_rect(1, 1280, 375, 1420, 445, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"atv";//攻击时间数值attack_time_value
		write_int(1, 1280, 375, uname, 1, 10, 7, COLOR_WHITE, (uint32_t)aerial_robot_energy.attack_time);//有待调整
		
		send_graphic();//以上六个图形，调用一次send_graphic()
		
		uname = (unsigned char*)"brb";//剩余子弹数目背景bullet_remaining_num_background
		draw_rect(1, 1280, 305, 1420, 375, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"brv";//剩余子弹数目数值bullet_remaining_num_value
		write_int(1, 1280, 30, uname, 1, 10, 7, COLOR_WHITE, (uint32_t)ext_bullet_remaining.bullet_remaining_num);
		
		uname = (unsigned char*)"brb";//剩余子弹数目进度条bullet_remaining_num_bar，最大x长度610-1110
		draw_line(1, 610, 335, 610, 345, uname, 10, 7, COLOR_CYAN);//有待调整
        uname = (unsigned char*)"bsb";//发射子弹数目进度条bullet_shot_num_bar
		draw_line(1, 1110, 335, 1110, 345, uname, 10, 7, COLOR_YELLOW);
		
		
		/*uname = (unsigned char*)"lsg";//外观灯状态背景led_status_background
		draw_rect(582, 297, 722, 227, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"lsv";//外观灯状态数值led_status_value
		write_chars(1, 582, 297, 722, 227, uname, 1, 10, 7, COLOR_WHITE, ??? , 2);*/
		
		send_graphic();//以上五个图形，调用一次send_graphic()
		
		Get_Shoot_Status();
		
		uname = (unsigned char*)"asb";//自瞄状态背景auto_status_background
		draw_rect(1, 1660, 565, 1800, 635, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"asv";//自瞄状态数值auto_status_value
		write_chars(1, 1660, 565, uname, 1, 10, 7, COLOR_WHITE, auto_chars, 8);
		
		uname = (unsigned char*)"fsb";//摩擦轮状态背景fric_status_background
		draw_rect(1, 1660, 495, 1800, 565, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"fsv";//摩擦轮状态数值fric_status_value
		write_chars(1, 1660, 495, uname, 1, 10, 7, COLOR_WHITE, fric_chars, 8);
		
		uname = (unsigned char*)"fsb";//攻击状态背景attacking_status_background
		draw_rect(1, 1520, 425, 1800, 495, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"fsv";//攻击状态数值attacking_status_value
		write_chars(1, 1520, 425, uname, 1, 10, 7, COLOR_WHITE, attacking_chars, 14);
		
		uname = (unsigned char*)"xcb";//x轴坐标背景x_coordinate_background
		draw_rect(1, 255, 825, 395, 895, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"xcv";//x轴坐标数值x_coordinate_value
		write_float(1, 255, 825, uname, 1, 10, 2, 7, COLOR_WHITE, ext_game_robot_pos.x);
		
		uname = (unsigned char*)"xcb";//y轴坐标背景y_coordinate_background
		draw_rect(1, 395, 825, 535, 895, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"xcv";//y轴坐标数值y_coordinate_value
		write_float(1, 395, 825, uname, 1, 10, 2, 7, COLOR_WHITE, ext_game_robot_pos.y);
		
		send_graphic();//以上七个图形，调用一次send_graphic()
		
		/*uname = (unsigned char*)"xcb";//z轴坐标背景z_coordinate_background
		draw_rect(1, 535, 825, 675, 895, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"xcv";//z轴坐标数值z_coordinate_value
		write_float(1, 535, 825, uname, 1, 10, 2, 7, COLOR_WHITE, ???);*/
		
		uname = (unsigned char*)"pab";//Pitch角度背景pitch_angle_background
		draw_rect(1, 255, 755, 395, 825, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"pav";//Pitch角度数值pitch_angle_value
		write_float(1, 255, 755, uname, 1, 10, 2, 7, COLOR_WHITE, PitchMotor.RealAngle);
		
		uname = (unsigned char*)"yab";//Yaw角度背景yaw_angle_background
		draw_rect(1, 395, 755, 535, 825, uname, 12, 5, COLOR_BLACK);//有待调整。
		uname = (unsigned char*)"yav";//Yaw角度数值yaw_angle_value
		write_float(1, 395, 755, uname, 1, 10, 2, 7, COLOR_WHITE, YawMotor.RealAngle);

		send_graphic();//以上六个图形，调用一次send_graphic()
		
		FirstDraw = 0;
	}
	
	else
	{
		uname = (unsigned char*)"epv";//更新能量点数值
		write_int(2, 1410, 820, uname, 1, 10, 7, COLOR_WHITE, (uint32_t)aerial_robot_energy.energy_point);//线宽1，字体10，图层7， 白色，有待调整
		
		uname = (unsigned char*)"atv";//更新攻击时间数值
		write_int(2, 1280, 375, uname, 1, 10, 7, COLOR_WHITE, (uint32_t)aerial_robot_energy.attack_time);//有待调整
		
		uname = (unsigned char*)"brv";//更新剩余子弹数目数值
		write_int(2, 1280, 30, uname, 1, 10, 7, COLOR_WHITE, (uint32_t)ext_bullet_remaining.bullet_remaining_num);//有待调整
		
		uname = (unsigned char*)"brb";//更新剩余子弹数目进度条bullet_remaining_num_bar
		draw_line(1, 610, 335, 610 + bullet_remaining_bar_length, 345, uname, 10, 7, COLOR_CYAN);//有待调整
		uname = (unsigned char*)"bsb";//更新发射子弹数目进度条bullet_shot_num_bar
		draw_line(1, 1110 - bullet_remaining_bar_length, 335, 1110, 345, uname, 10, 7, COLOR_YELLOW);
		
		send_graphic();//以上五个图形，调用一次send_graphic()
		
		/*uname = (unsigned char*)"lsv";//更新外观灯状态数值
		write_chars(2, 582, 297, 722, 227, uname, 1, 10, 7, COLOR_WHITE, ??? , 2);*/
			
		Get_Shoot_Status();//更新射击状态数值
		
		uname = (unsigned char*)"asv";//更新自瞄状态数值
		write_chars(2, 1660, 565, uname, 1, 10, 7, COLOR_WHITE, auto_chars, 8);
		
		uname = (unsigned char*)"fsv";//更新摩擦轮状态数值
		write_chars(2, 1660, 495, uname, 1, 10, 7, COLOR_WHITE, fric_chars, 8);
		
		uname = (unsigned char*)"fsv";//更新攻击状态数值
		write_chars(2, 1520, 425, uname, 1, 10, 7, COLOR_WHITE, attacking_chars, 14);
		
		uname = (unsigned char*)"xcv";//更新x轴坐标数值
		write_float(2, 255, 825, uname, 1, 10, 2, 7, COLOR_WHITE, ext_game_robot_pos.x);
		
		uname = (unsigned char*)"xcv";//更新y轴坐标数值
		write_float(2, 395, 825, uname, 1, 10, 2, 7, COLOR_WHITE, ext_game_robot_pos.y);
		
		/*uname = (unsigned char*)"xcv";//更新z轴坐标数值
		write_float(2, 535, 825, uname, 1, 10, 2, 7, COLOR_WHITE, ???);*/
		
		uname = (unsigned char*)"pav";//更新Pitch角度数值
		write_float(2, 255, 755, uname, 1, 10, 2, 7, COLOR_WHITE, PitchMotor.RealAngle);
		
		uname = (unsigned char*)"yav";//更新Yaw角度数值
		write_float(2, 395, 755, uname, 1, 10, 2, 7, COLOR_WHITE, YawMotor.RealAngle);
		
		send_graphic();//以上五个图形，调用一次send_graphic()
	}
	
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
    UiFrame.UiFrameBuffer[0] = 0XA5;
	UiFrame.Frame.frame_header.Frame.seq = 0;//包序号初始化
    UiFrame.Frame.frame_header.Frame.data_length.Frame = 0;//数据长度初始化
	UiFrame.Frame.frame_header.Frame.CRC8 = 0;//CRC校验初始化
    UiFrame.UiFrameBuffer[5] = 0X01;
    UiFrame.UiFrameBuffer[6] = 0X03;//cmd_id设置为0x0301机器人间交互
	memset(UiFrame.Frame.data, 0, 121);//数据初始化

    header_data.sender_ID = ext_game_robot_state.robot_id;//发送方id

    switch (WhichTeam())//接收方id
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
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
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
uint8_t draw_line(uint32_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],
                  uint32_t width,uint8_t layer,uint8_t color)
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

    switch (operate)
    {
        case 1:
        {
            graphic_data[index].operate_tpye = OPERATE_ADD;
            break;
        }
        case 2:
        {
            graphic_data[index].operate_tpye = OPERATE_CHANGE;
            break;
        }
    }

    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_LINE;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;
		
    //下面三个用不到的变量为空
    graphic_data[index].start_angle = 0;
    graphic_data[index].end_angle = 0;
    graphic_data[index].radius = 0;

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
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
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
uint8_t draw_rect(uint32_t operate, uint32_t start_x,uint32_t start_y,uint32_t end_x,uint32_t end_y,uint8_t name[],
                  uint32_t width,uint8_t layer,uint8_t color)
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

    switch (operate)
    {
        case 1:
        {
            graphic_data[index].operate_tpye = OPERATE_ADD;
            break;
        }
        case 2:
        {
            graphic_data[index].operate_tpye = OPERATE_CHANGE;
            break;
        }
    }
		
    graphic_data[index].width = width;
    graphic_data[index].graphic_tpye = GRAPHIC_RECT;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    graphic_data[index].end_x = end_x;
    graphic_data[index].end_y = end_y;
		
    //下面三个用不到的变量为空
    graphic_data[index].start_angle = 0;
    graphic_data[index].end_angle = 0;
    graphic_data[index].radius = 0;

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
 * @brief  写整型数
 * @note   调用该函数写整型数(没检查坐标)
 * @param  start_x: 字符起始x坐标
 * @param  start_y: 字符起始y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  size: 字体大小
 * @param  layer: 图层
 * @param  color: 颜色
 * @param  contents: 内容
 * @retval 0:所有graphic已经堆满 1:正常发送 2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t write_int(uint32_t operate,uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width,uint32_t size,uint8_t layer,
                  uint8_t color,uint32_t contents)
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
	
    switch (operate)
        {
            case 1:
            {
                graphic_data[index].operate_tpye = OPERATE_ADD;
                break;
            }
            case 2:
            {
                graphic_data[index].operate_tpye = OPERATE_CHANGE;
                break;
            }
        }

    graphic_data[index].graphic_tpye = GRAPHIC_INT;
    graphic_data[index].width = width;
    graphic_data[index].start_angle = size;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    
    graphic_data[index].radius = (contents>>22);
    graphic_data[index].end_x = (contents>>11);
    graphic_data[index].end_y = contents;
        
    //用不到的变量为空
    graphic_data[index].end_angle = 0;

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
 * @brief  写浮点数
 * @note   调用该函数写浮点数(没检查坐标)
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 字符起始x坐标
 * @param  start_y: 字符起始y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  size: 字体大小
 * @param  decimal: 小数有效位数
 * @param  layer: 图层
 * @param  color: 颜色
 * @param  contents: 内容
 * @retval 0:所有graphic已经堆满 1:正常发送 2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t write_float(uint32_t operate,uint32_t start_x,uint32_t start_y,uint8_t name[],uint32_t width,uint32_t size,uint32_t decimal,
                    uint8_t layer,uint8_t color,uint32_t contents)
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

    switch (operate)
    {
        case 1:
        {
            graphic_data[index].operate_tpye = OPERATE_ADD;
            break;
        }
        case 2:
        {
            graphic_data[index].operate_tpye = OPERATE_CHANGE;
            break;
        }
    }
			
    graphic_data[index].graphic_tpye = GRAPHIC_FLOAT;
    graphic_data[index].width = width;
    graphic_data[index].start_angle = size;
    graphic_data[index].end_angle = decimal;
    
    graphic_data[index].start_x = start_x;
    graphic_data[index].start_y = start_y;
    
    graphic_data[index].radius = (contents>>22);
    graphic_data[index].end_x = (contents>>11);
    graphic_data[index].end_y = contents;

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
 * @brief  写字符串
 * @note   调用该函数写字(没检查坐标)
           个人感觉字符串发送帧的内容空间配置和其他图形的发送不同，
           无法涵盖在send_graphic()函数里发送，所以里面单独进行发送，不需再调用send_graphic() by陈宇涵
 * @param  operate: 需要进行的操作类型，1-增加，2-修改
 * @param  start_x: 字符起始x坐标
 * @param  start_y: 字符起始y坐标
 * @param  name:  图形名字(必须3个uint8_t，不够记得补零，实在懒得写判断了....)
 * @param  width: 线宽
 * @param  size:  字体大小
 * @param  layer: 图层
 * @param  color: 颜色
 * @param  contents: 内容
 * @param   length: 长度
 * @retval 1:正常发送 2:坐标错误 3:图层错误 4:颜色错误 
 */
uint8_t write_chars(uint32_t operate, uint32_t start_x, uint32_t start_y, uint8_t name[], uint32_t width, uint32_t size, uint8_t layer,uint8_t color, uint8_t contents[], uint32_t length)
{
    custom_character.graphic_data_struct.graphic_name[0] = name[0];
    custom_character.graphic_data_struct.graphic_name[1] = name[1];
    custom_character.graphic_data_struct.graphic_name[2] = name[2];
		
    switch (operate)
    {
        case 1:
        {
            custom_character.graphic_data_struct.operate_tpye = OPERATE_ADD;
            break;
        }
        case 2:
        {
            custom_character.graphic_data_struct.operate_tpye = OPERATE_CHANGE;
            break;
        }
    }

    custom_character.graphic_data_struct.graphic_tpye = GRAPHIC_CHAR;
    custom_character.graphic_data_struct.width = width;
    custom_character.graphic_data_struct.start_angle = size;
    custom_character.graphic_data_struct.end_angle = length;
    
    custom_character.graphic_data_struct.start_x = start_x;
    custom_character.graphic_data_struct.start_y = start_y;
    
    //下面三个用不到的变量为空
    custom_character.graphic_data_struct.radius = 0;
    custom_character.graphic_data_struct.end_x = 0;
    custom_character.graphic_data_struct.end_y = 0;

    if(layer > LAYER_NUM)
    {
        return 3;
    }
    else
    {
        custom_character.graphic_data_struct.layer = layer;
    }
        
    if(color >= COLOR_NUM)
    {
        return 4;
    }
    else
    {
        custom_character.graphic_data_struct.color = color;
    }

    header_data.data_cmd_id = 0x0110;
    
    load_chars(contents, length);
    UiFrame.UiFrameBuffer[1] = 36;

    send_seq++;
    UiFrame.UiFrameBuffer[3] = send_seq;

    //进行数据发送，所以外部不需要调用send_graphic()
    memcpy(UiFrame.UiFrameBuffer+JUDGE_DATA_OFFSET, &header_data, sizeof(ext_student_interactive_header_data_t));
    memcpy(UiFrame.UiFrameBuffer+JUDGE_DATA_OFFSET + sizeof(ext_student_interactive_header_data_t), &custom_character, 45);
    Append_CRC_Check_Sum(UiFrame.UiFrameBuffer, UiFrame.UiFrameBuffer[1]);
    HAL_UART_Transmit_DMA(&REFEREE_HUART,UiFrame.UiFrameBuffer,(UiFrame.UiFrameBuffer[1]+9)); //头帧加校验共9字节
		
    return 1;
}

/**
 * @brief 发送图形
 * @note   
* @retval 1:发送成功,0:没有发送
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
        header_data.data_cmd_id = 0X0101;//发送一个图形的内容id
        UiFrame.UiFrameBuffer[1] = 21;//发送一个图形的长度
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
    HAL_UART_Transmit_DMA(&REFEREE_HUART,UiFrame.UiFrameBuffer,(UiFrame.UiFrameBuffer[1]+9)); //头帧加校验共9字节
    //相当于清空数据
    for(int i = 0; i < graphic_num; i++)
    {
        graphic_data[i].operate_tpye = OPERATE_NULL;
    }
		return 1;
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
 * @brief  获得当前射击状态（摩擦轮开启与否，自瞄开启与否，当前正在攻击的目标是啥）
 * @note   不论状态，attacking_chars均赋值14字节的字符串，故字符串内不够14字节就补空格;
                     auto_chars均赋值8字节的字符串，故字符串内不够8字节就补空格;
                     fric_chars均赋值8字节的字符串，故字符串内不够8字节就补空格;
 * @retval 
 */

uint8_t Get_Shoot_Status(void)
{
	switch (Gimbal.aim_mode)//当前自瞄开没开，当前无人机正在攻击什么目标
	{
		case AimMode_Auto:
		{
			attacking_chars = (unsigned char*)"attack: Free  ";//正在自由攻击
			auto_chars = (unsigned char*)"auto: ON";
			break;
		}
	  case AimMode_Outpost:
		{
			attacking_chars = (unsigned char*)"attack:Outpost";//正在攻击前哨站
			auto_chars = (unsigned char*)"auto:OFF";
			break;
		}
		case AimMode_Base:
		{
			attacking_chars = (unsigned char*)"attack: Base  ";//正在攻击基地
			auto_chars = (unsigned char*)"auto:OFF";
			break;
		}
		case AimMode_Manual:
		{
			attacking_chars = (unsigned char*)"attack: Free  ";//正在自由攻击
			auto_chars = (unsigned char*)"auto:OFF";
			break;
		}
		default:
		{
			attacking_chars = (unsigned char*)"attack: Null  ";//不在攻击
			auto_chars = (unsigned char*)"auto:OFF";
			break;
		}
	}
		
	switch (Gimbal.fric_status)//当前摩擦轮开没开
	{
		case FricStatus_Working:
		{
			fric_chars = (unsigned char*)"Fric: ON";
			break;
		}
		case FricStatus_Stop:
		{
			fric_chars = (unsigned char*)"Fric:OFF";
			break;
		}
		default:
			break;
	}

	return 1;
}

/**
 * @brief  向custom_character中装入字符串，需要给定字符串长度（不需要计算'\0'）
 * @note   
 * @retval 正常录入 1
 */
uint8_t load_chars(uint8_t chars_to_send[], uint8_t length)
{
	uint8_t count = 0;

	for(count = 0; count < length; count++)
	{
		custom_character.data[count] = chars_to_send[count];
	}
//	for(; count < 30; count++)//不够30字节的补0
//	{
//		custom_character.data[count] = 0;
//	}
		
	return 1;
}

/*------------下面是暂时用不上的函数，目前先保留宁俊以的写法未作改动---------------*/
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

/*---------------load_chars()另一个版本的函数，感觉用不到，但因为尚未上机测试所以先留着好了by陈宇涵------------*/
/**
 * @brief  向custom_character中装入字符串（字符串长于30则只录30个字符，不包括传入字符串的'\0'）
 * @note   
 * @retval 正常录入 1
 */
/*uint8_t load_chars(uint8_t chars_to_send[])
{
	uint8_t count = 0;
	uint8_t length = 0;
	for(count = 0; count < 30; count++)
	{
		if(chars_to_send[count] == '\0')
			break;
		custom_character.data[count] = chars_to_send[count];
	}
	for(; count < 30; count++)//不够30字节的补0
	{
		custom_character.data[count] = 0;
	}
		
	return 1;
}*/

