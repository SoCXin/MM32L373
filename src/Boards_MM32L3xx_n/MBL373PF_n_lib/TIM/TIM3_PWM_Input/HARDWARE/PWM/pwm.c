/**
******************************************************************************
* @file    pwm.c
* @author  AE Team
* @version V1.3.9
* @date    28/08/2019
* @brief   This file provides all the pwm firmware functions.
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/

/* Includes ------------------------------------------------------------------*/
#include "pwm.h"
#include "uart.h"
u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 0;
void TIM3_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��


    //���ø�����Ϊ�����������,���TIM3 CH1  ��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //��ʼ��GPIO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���


    TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���


    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����


}



/*�������ƣTIM4_PWMINPUT_INIT(u16 arr,u16 psc)
����      PWM�����ʼ��*/
void TIM4_PWMINPUT_INIT(u16 arr, u16 psc)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM4_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);     //Open TIM4 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //open gpioB clock

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                               //GPIO??
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


    /*�����ж����ȼ�*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM4_ICInitStructure.TIM_ICFilter = 0x0;

    //TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM_PWMIConfig(TIM4, &TIM4_ICInitStructure);     //PWM��������

    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);     //ѡ����Ч�����
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);  //����Ϊ���Ӹ�λģʽ
    TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
    TIM_ITConfig(TIM4, TIM_IT_CC2 | TIM_IT_Update, ENABLE);        //�ж�����
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2 | TIM_IT_Update); //����жϱ�־λ
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_IRQHandler(void)
{

    if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)//����1���������¼�
    {
        duty = TIM_GetCapture1(TIM4); 				//�ɼ�ռ�ձ�
        period	=	TIM_GetCapture2(TIM4);     //�ɼ�����
        CollectFlag = 1;

    }
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2 | TIM_IT_Update); //����жϱ�־λ
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/
