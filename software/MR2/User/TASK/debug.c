
#include "debug.h"

uint8_t len;
uint16_t time=0;
uint16_t Number_of_digits;		//算出的每个电机 数据中 数据域的长度
int16_t Result;
int16_t	Real_Number[20];		//用ascii码算出相应位中的真实数值 0-9对应48-57

int8_t Abs_Pos;		//数据的绝对位置
uint8_t act_flag=0 ;


/*******************************************************************************************
	*@ 函数名称：void task2_task(void *pvParameters)
	*@ 功能： 接收上位机指令 进入调试模式 监听uart2数据 更改电机速度 位置 寄存器
	*@ 备注：使用Uart2
	*@ 使用说明：最多可以同时控制八个电机 电机号1-8对应字母a-h
	*@ 命令说明：如pa10b-10c50d100 位置模式 1号电机正转10度 2号电机反转10度 3号电机正转50度 4号电机正转100度
 *******************************************************************************************/
void Debug_task(void *pvParameters)
{

    for(;;)
        InterpretCommand();
}

/**
* NAME: void InterpretCommand(void)
* FUNCTION : 命令解析
*/
float temp_angle_;
void InterpretCommand(void)
{
    switch(usart2_buf[0])
    {
    /*************************************************步态参数选择区*************************************************/
    case 0x01:
        state = WALK;
        printf("\r\n**********state = WALK**********\r\n");
        break;
    case 0x02:
        state = WALK_BACK;
        printf("\r\n**********state = WALK_BACK**********\r\n");
        break;
    case 0x03:
        state = BOUND;
        printf("\r\n**********state = BOUND**********\r\n");
        break;
    case 0x04:
        state = GALLOP;
        printf("\r\n**********state = GALLOP**********\r\n");
        break;
    case 0x05:
        state = ROTAT_LEFT;
        printf("\r\n**********state = ROTAT_LEFT**********\r\n");
        break;
    case 0x06:
        state = ROTAT_RIGHT;
        printf("\r\n**********state = ROTAT_RIGHT**********\r\n");
        break;
    case 0x07:
        break;
    case 0x08:
        state = TROT;
        printf("\r\n**********state = TROT**********\r\n");
        break;
    case 0x09:
        break;

    case 0x0b:
        //  state = TEST;
        printf("\r\n**********state = TEST**********\r\n");
        break;
    case 0x0c:
        state = STOP;
        printf("\r\n**********state = STOP**********\r\n");
        break;
    case 0x0d:
        StartJump(HAL_GetTick());
        printf("JUMP");
        break;
    case 0x0e:
        state = START;
        break;
    case 0x0f:
        state = END;
        break;

    /*************************************************步态参数调试区*************************************************/
    case 0x10:
        state_detached_params[state].detached_params_0.stance_height+=1;
        state_detached_params[state].detached_params_2.stance_height+=1;
        state_detached_params[state].detached_params_1.stance_height+=1;
        state_detached_params[state].detached_params_3.stance_height+=1;
        break;
    case 0x11:
        state_detached_params[state].detached_params_0.stance_height-=1;
        state_detached_params[state].detached_params_2.stance_height-=1;
        state_detached_params[state].detached_params_1.stance_height-=1;
        state_detached_params[state].detached_params_3.stance_height-=1;
        break;
    case 0x12:
        state_detached_params[state].detached_params_0.step_length+=1;
        state_detached_params[state].detached_params_2.step_length+=1;
        state_detached_params[state].detached_params_1.step_length+=1;
        state_detached_params[state].detached_params_3.step_length+=1;
        break;
    case 0x13:
        state_detached_params[state].detached_params_0.step_length-=1;
        state_detached_params[state].detached_params_2.step_length-=1;
        state_detached_params[state].detached_params_1.step_length-=1;
        state_detached_params[state].detached_params_3.step_length-=1;
        break;
    case 0x14:
        state_detached_params[state].detached_params_0.up_amp+=1;
        state_detached_params[state].detached_params_2.up_amp+=1;
        state_detached_params[state].detached_params_1.up_amp+=1;
        state_detached_params[state].detached_params_3.up_amp+=1;
        break;
    case 0x15:
        state_detached_params[state].detached_params_0.up_amp-=1;
        state_detached_params[state].detached_params_2.up_amp-=1;
        state_detached_params[state].detached_params_1.up_amp-=1;
        state_detached_params[state].detached_params_3.up_amp-=1;
        break;
    case 0x16:
        state_detached_params[state].detached_params_0.flight_percent+=0.01;
        state_detached_params[state].detached_params_2.flight_percent+=0.01;
        state_detached_params[state].detached_params_1.flight_percent+=0.01;
        state_detached_params[state].detached_params_3.flight_percent+=0.01;
        break;
    case 0x17:
        state_detached_params[state].detached_params_0.flight_percent-=0.01;
        state_detached_params[state].detached_params_2.flight_percent-=0.01;
        state_detached_params[state].detached_params_1.flight_percent-=0.01;
        state_detached_params[state].detached_params_3.flight_percent-=0.01;
        break;
    case 0x18:
        state_detached_params[state].detached_params_0.freq+=0.1;
        state_detached_params[state].detached_params_2.freq+=0.1;
        state_detached_params[state].detached_params_1.freq+=0.1;
        state_detached_params[state].detached_params_3.freq+=0.1;
        break;
    case 0x19:
        state_detached_params[state].detached_params_0.freq-=0.1;
        state_detached_params[state].detached_params_2.freq-=0.1;
        state_detached_params[state].detached_params_1.freq-=0.1;
        state_detached_params[state].detached_params_3.freq-=0.1;
        break;
    case 0x26:
        state_detached_params[state].detached_params_0.down_amp+=1;
        state_detached_params[state].detached_params_2.down_amp+=1;
        state_detached_params[state].detached_params_1.down_amp+=1;
        state_detached_params[state].detached_params_3.down_amp+=1;
        break;
    case 0x27:
        state_detached_params[state].detached_params_0.down_amp-=1;
        state_detached_params[state].detached_params_2.down_amp-=1;
        state_detached_params[state].detached_params_1.down_amp-=1;
        state_detached_params[state].detached_params_3.down_amp-=1;
        break;

    /******************************************************END******************************************************/

    /*************************************************舵机调试区*************************************************/

    case 0x20:
        TIM4->CCR1+=20;
        break;
    case 0x21:
        TIM4->CCR1-=20;
        break;
    case 0x22:
        TIM4->CCR2+=20;
        break;
    case 0x23:
        TIM4->CCR2-=20;
        break;
    case 0x24:
        TIM4->CCR3+=20;
        break;
    case 0x25:
        TIM4->CCR3-=20;
        break;

    case 0x30:
        test_speed+=1000;
        break;
    case 0x31:
        test_speed-=1000;
        break;



    case 0x32:
        temp_angle_+=100;
        CAN_RoboModule_DRV_Position_Mode(0,1,1000,temp_angle_*4*15.15);		//杆子升起来  2100
        printf("temp_angle_ %f\r\n",temp_angle_);
        break;

    case 0x33:
        temp_angle_-=100;
        CAN_RoboModule_DRV_Position_Mode(0,1,1000,temp_angle_*4*15.15);		//杆子升起来  2100
        printf("temp_angle_ %f\r\n",temp_angle_);
        break;




//        case 'E':
//            state = STOP;
//            vTaskDelay(100);
//            vTaskSuspend(MotorControlTask_Handler);
//            break;
//        case 'R':
//            state = STOP;
//            vTaskDelay(100);
//            vTaskResume(MotorControlTask_Handler);
//            break;
//
//        case 'm':
//            Servo1_PEAK;
////            TIM4->CCR1-=20;
////				printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;
//        case 'n':
//            Servo1_DOWN;
////            TIM4->CCR1+=20;
////				printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;
//        case 'k':
//            Servo2_PEAK_POS;
////            TIM4->CCR2-=20;
////				printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;
//        case 'l':
//            Servo2_DOWN_POS;
////            TIM4->CCR2+=20;
////				printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;

//        case 'z':

//            TIM4->CCR1+=20;
//            printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;
//        case 'x':

//            TIM4->CCR1-=20;
//            printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;
//        case 'v':

//            TIM4->CCR2+=20;
//            printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);
//            break;
//        case 'y':

//            TIM4->CCR2-=20;
//            printf("\r\n CCR1 %d  CCR2 %d \r\n",(int)TIM4->CCR1,(int)TIM4->CCR2);

//            break;

//        default :
//            //printf("\r\n%d	%d	%d	%d     %d \r\n",USART_RX_BUF[0],USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3],len);
//           // printf("\r\n  Unknown command   \r\n");
//            break;

        //vTaskDelay(20);
    }
    //usart2_buf[0]=0;
    //while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC)!=SET);		//等待发送结束
    // printf("\r\n/*************收到命令****************/\r\n");//插入换行

//        for(int i=0 ; i<len; i++)
//            USART_RX_BUF[i]=0;

//        USART_RX_STA=0;
    memset(&usart2_buf,0,sizeof(usart2_buf));//清除usart2buff里面的数据
    vTaskDelay(200);
//    } else
//    {
//        time++;
//        if(time%4000==0)
//        {
//            printf("\r\n/*************HBUT ROBOCON 2019 调试模式****************/ \r\n");
//            //  printf("命令格式  s速度   p位置  注意无空格 \r\n\r\n\r\n");
//        }
//        if(time%100==0)
//        {
//            // printf(" 步高 %2.1f  步长% 2.1f  抬腿高 %2.1f  压腿高 %2.1f  飞行占比 %2.2f  频率% 2.1f\r\n",test_gait_params.stance_height,test_gait_params.step_length,test_gait_params.up_amp,test_gait_params.down_amp,test_gait_params.flight_percent,test_gait_params.freq);

//        }
//        vTaskDelay(10);
//    }

}







/**
* NAME: void coor_sort(void)
* FUNCTION : 命令解析  调试中 暂时不可用
*/
//void coor_sort(void)
//{
//    double x=0,y=0;
//    Abs_Pos=0;		//清0初始化
//    for(int i=0 ; i<4; i++)
//        coor_calc.R[i]=0;

//    for(int i =0; i<len; i++)
//    {
//        switch(USART_RX_BUF[i])
//        {
//        case	'x'	:					//x轴
//            Abs_Pos=i;
//            x=decimal_converter();
//            printf("\r\nx%d ",(int)decimal_converter());
//            break;
//        case	'y'	:					//y轴
//            Abs_Pos=i;
//            y=decimal_converter();		// *3591/187*8191/360
//            printf("y%d ",(int)decimal_converter());
//            break;
//        }
//    }
////    coordinate_trans(x,y);
//    //vTaskDelay(5);

////    for(int i=0; i<4; i++)
////        temp_pid.ref_agle[i]=coor_calc.R[i]*3591/187*8191/360;


//    //printf("b%d ",(int)temp_pid.ref_agle[0]);
//    Ready_Flag=1;		//数据填充完毕标志位
//}

///**
//* NAME: void debug_sort(void)
//* FUNCTION : 命令解析
//*/
//void debug_sort(void)
//{
//    Abs_Pos=0;		//清0初始化
//    for(int i =0; i<len; i++)
//    {
//        switch(USART_RX_BUF[i])
//        {
//        case	'a'	:					//1号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[0]=decimal_converter()*3591/187*8191/360;
//            printf("\r\na%d ",(int)decimal_converter());
//            break;
//        case	'b'	:					//2号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[1]=decimal_converter()*3591/187*8191/360;		// *3591/187*8191/360
//            printf("b%d ",(int)decimal_converter());
//            break;
//        case	'c'	:					//3号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[2]=decimal_converter()*3591/187*8191/360;		// *3591/187*8191/360
//            printf("c%d ",(int)decimal_converter());
//            break;
//        case	'd'	:					//4号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[3]=decimal_converter()*3591/187*8191/360;		// *3591/187*8191/360
//            printf("d%d ",(int)decimal_converter());
//            break;
//        case	'e'	:					//5号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[4]=decimal_converter()*3591/187*8191/360;
//            printf("e%d ",(int)decimal_converter());
//            break;
//        case	'f'	:					//6号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[5]=decimal_converter()*3591/187*8191/360;		// *3591/187*8191/360
//            printf("f%d ",(int)decimal_converter());
//            break;
//        case	'g'	:					//7号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[6]=decimal_converter()*3591/187*8191/360;		// *3591/187*8191/360
//            printf("g%d ",(int)decimal_converter());
//            break;
//        case	'h'	:					//8号电机
//            Abs_Pos=i;
//            temp_pid.ref_agle[7]=decimal_converter()*3591/187*8191/360;		// *3591/187*8191/360
//            printf("h%d ",(int)decimal_converter());
//            break;

//        }
//    }
//    Ready_Flag=1;		//数据填充完毕标志位
//}

///**
//* NAME: void decimal_converter(void)
//* FUNCTION : 十进制转换
//*/
//float decimal_converter(void)
//{

//    Result=0;
//    Number_of_digits=0;

//    Number_of_digits=get_result_digitals();
//    //printf("\r\n Number_of_digits=    %d  \r\n",Number_of_digits);

//    if(USART_RX_BUF[Abs_Pos+1]==45)		//数据区域第一位是减号	负数
//    {
//        Number_of_digits-=1;
//        for(int i =0; i<Number_of_digits; i++)//控制循环次数  将消息里面的数字取出来存到Real_Number 里面
//        {
//            Real_Number[i]=-(USART_RX_BUF[Abs_Pos+2+i]-48);		//+2跳过负号

//            //printf("\r\n Real_Number    %d  \r\n",Real_Number[i]);  调试用
//        }

//        for(int j=0; j<Number_of_digits; j++)		//循环从高位依次赋值给低位 将数字相乘起来输出
//        {
//            Result+=Real_Number[j]*pow(10.0,(Number_of_digits-j-1));
//            //printf("\r\n .....    %d ......%f \r\n",Real_Number[j], Real_Number[j]*pow(10,Number_of_digits-j-1) );
//        }

//    }
//    else {					//正数 不进行负号处理

//        for(int i =0; i<Number_of_digits; i++)//控制循环次数  将消息里面的数字取出来存到Real_Number 里面
//        {
//            Real_Number[i]=USART_RX_BUF[Abs_Pos+1+i]-48;
//            //printf("\r\n Real_Number    %d  \r\n",Real_Number[i]);
//        }

//        for(int j=0; j<Number_of_digits; j++)		//循环从高位依次赋值给低位 将数字相乘起来输出
//        {
//            Result+=Real_Number[j]*pow(10.0,(Number_of_digits-j-1));
//            //printf("\r\n .....    %d ......%f \r\n",Real_Number[j], Real_Number[j]*pow(10,Number_of_digits-j-1) );
//        }
//        //printf("\r\n Result    %d  \r\n",Result);
//    }
//    return Result ;
//    //printf("\r\n Result    %d  \r\n",Result);
//}

///**
//* NAME: uint16_t get_result_digitals(void)
//* FUNCTION : 得到单个电机命令的具体长度
//*/
//uint16_t get_result_digitals(void)
//{
//    int n=0,temp=0,flag=1;
//    temp=Abs_Pos;
//    while(flag) {
//        //vTaskDelay(5);
//        temp++;
//        if(USART_RX_BUF[temp]<=57&&USART_RX_BUF[temp]>=45)		//此位上是数字和负号
//            ;
//        else	{	//此位上是字母
//            n=temp-Abs_Pos-1;
//            flag=0;
//        }
//    }
//    return n;
//}

