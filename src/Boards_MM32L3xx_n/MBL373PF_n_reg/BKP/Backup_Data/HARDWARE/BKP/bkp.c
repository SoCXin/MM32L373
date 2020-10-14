#include "bkp.h"
#include "led.h"
#include "delay.h"


/********************************************************************************************************
**������Ϣ ��CheckBackupReg(u16 FirstBackupData)
**�������� : �ж�д�����Ƿ���ȷ
**������� ��FirstBackupData
**������� ��д����index+1��д�ɹ���0
********************************************************************************************************/
u8 CheckBackupReg(void)
{
    if(BKP->DR1 == 0x55)
        return 0;
    else
        return 1;
}

/********************************************************************************************************
**������Ϣ ��WriteToBackupReg()
**�������� : ��BKP���ݼĴ�����д����
**������� ��FirstBackupData
**������� ����
********************************************************************************************************/
void WriteToBackupReg(void)
{
    BKP->DR1 = 0x55;

}
/********************************************************************************************************
**������Ϣ ��BKP_DATA(void)
**�������� : BKP���ݶ�д���ԣ��ж�д�Ͷ��������Ƿ�һ��
**������� ��FirstBackupData
**������� ��i
********************************************************************************************************/
u8 BKP_DATA(void)
{
    u8 i;
    /* Enable PWR and BKP clock */
    RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;
    /* Enable write access to Backup domain */
    PWR->CR |= 0x00000100;

    /* Clear Tamper pin Event(TE) pending flag */
    BKP->CSR |= 0x0001;

    WriteToBackupReg();//��BKP�Ĵ�����д����
    i = CheckBackupReg(); //�ж�����д�����Ƿ���ȷ
    return i;
}
