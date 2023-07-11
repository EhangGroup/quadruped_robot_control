
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "pid.h"
#include "bsp_can.h"
#include "mytype.h"
#include "tim.h"

#include "stdio.h"
#include "DataScope_DP.h"
#include "moto_ctrl.h"

unsigned char DataScope_OutPut_Buffer[42] = {0};	   //���ڷ��ͻ�����

unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
extern uint16_t temp_angle;

//����˵�����������ȸ�������ת��4�ֽ����ݲ�����ָ����ַ
//����˵�����û�����ֱ�Ӳ����˺���
//target:Ŀ�굥��������
//buf:��д������
//beg:ָ��������ڼ���Ԫ�ؿ�ʼд��
//�����޷���
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  //�õ�float�ĵ�ַ
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}


//����˵������������ͨ���ĵ����ȸ�������д�뷢�ͻ�����
//Data��ͨ������
//Channel��ѡ��ͨ����1-10��
//�����޷���
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
    if ( (Channel > 10) || (Channel == 0) ) return;  //ͨ����������10�����0��ֱ����������ִ�к���
    else
    {
        switch (Channel)
        {
        case 1:
            Float2Byte(&Data,DataScope_OutPut_Buffer,1);
            break;
        case 2:
            Float2Byte(&Data,DataScope_OutPut_Buffer,5);
            break;
        case 3:
            Float2Byte(&Data,DataScope_OutPut_Buffer,9);
            break;
        case 4:
            Float2Byte(&Data,DataScope_OutPut_Buffer,13);
            break;
        case 5:
            Float2Byte(&Data,DataScope_OutPut_Buffer,17);
            break;
        case 6:
            Float2Byte(&Data,DataScope_OutPut_Buffer,21);
            break;
        case 7:
            Float2Byte(&Data,DataScope_OutPut_Buffer,25);
            break;
        case 8:
            Float2Byte(&Data,DataScope_OutPut_Buffer,29);
            break;
        case 9:
            Float2Byte(&Data,DataScope_OutPut_Buffer,33);
            break;
        case 10:
            Float2Byte(&Data,DataScope_OutPut_Buffer,37);
            break;
        }
    }
}


//����˵�������� DataScopeV1.0 ����ȷʶ���֡��ʽ
//Channel_Number����Ҫ���͵�ͨ������
//���ط��ͻ��������ݸ���
//����0��ʾ֡��ʽ����ʧ��
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
    if ( (Channel_Number > 10) || (Channel_Number == 0) ) {
        return 0;    //ͨ����������10�����0��ֱ����������ִ�к���
    }
    else
    {
        DataScope_OutPut_Buffer[0] = '$';  //֡ͷ

        switch(Channel_Number)
        {
        case 1:
            DataScope_OutPut_Buffer[5]  =  5;
            return  6;
            break;
        case 2:
            DataScope_OutPut_Buffer[9]  =  9;
            return 10;
            break;
        case 3:
            DataScope_OutPut_Buffer[13] = 13;
            return 14;
            break;
        case 4:
            DataScope_OutPut_Buffer[17] = 17;
            return 18;
            break;
        case 5:
            DataScope_OutPut_Buffer[21] = 21;
            return 22;
            break;
        case 6:
            DataScope_OutPut_Buffer[25] = 25;
            return 26;
            break;
        case 7:
            DataScope_OutPut_Buffer[29] = 29;
            return 30;
            break;
        case 8:
            DataScope_OutPut_Buffer[33] = 33;
            return 34;
            break;
        case 9:
            DataScope_OutPut_Buffer[37] = 37;
            return 38;
            break;
        case 10:
            DataScope_OutPut_Buffer[41] = 41;
            return 42;
            break;
        }
    }
    return 0;
}


void DataScope_Send_Msg()
{

    DataScope_Get_Channel_Data( moto_chassis[0].total_angle, 1 ); //������ 1.0  д��ͨ�� 1//
    DataScope_Get_Channel_Data( moto_chassis[0].angle, 2 );  //������ 2.0  д��ͨ�� 2
    DataScope_Get_Channel_Data( temp_angle, 3 );  //������ 3.0  д��ͨ�� 3
    DataScope_Get_Channel_Data( 2.5, 4 );  //������ 2.5  д��ͨ�� 4
    /*
    	DataScope_Get_Channel_Data( 5.0 , 5 ); //������ 5.0  д��ͨ�� 5
    	DataScope_Get_Channel_Data( 6.0 , 6 ); //������ 6.0  д��ͨ�� 6
    	DataScope_Get_Channel_Data( 7.0 , 7 ); //������ 7.0  д��ͨ�� 7
    	DataScope_Get_Channel_Data( 8.0 , 8 ); //������ 8.0  д��ͨ�� 8
    	DataScope_Get_Channel_Data( 9.0 , 9 ); //������ 9.0  д��ͨ�� 9
    	DataScope_Get_Channel_Data( 10.0 , 10); //������ 10.0 д��ͨ�� 10
    */
    Send_Count = DataScope_Data_Generate(4); //����10��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���

//		for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������
//		{
//		while((USART2->SR&0X40)==0);
//			USART2->DR = DataScope_OutPut_Buffer[i]; //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ
//		}

    vTaskDelay(100);
}








