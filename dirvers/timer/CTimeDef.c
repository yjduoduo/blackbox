/*
*********************************************************************************************************
*                                                
*                              ¶ÔÏó:	CTime0Def
*
*                        (c) Copyright 2000-2006, jxm
*                              All Rights Reserved
*                                    Ver1.5
* File : CTime0Def.C
* By   : jxm
*********************************************************************************************************
*/
#define    CTimeDef_C	  
#include  "CTimeDef.h" 
#include  "type.h"
#include  "LPC17xx.H" 

#define cFreqNum 40000/*40000*/

uint32  Get_CTime0Def(void)
{
	return vCurTime0Def;
}

void  Set_CTime0Def(uint32 vTimeId) 
{
	vCurTime0Def=m_Time0Def[vTimeId&3];
}

uint32  Sel_CTime0Def(uint8 vTimer)
{
	return m_Time0Def[vTimer&3];
}
uint32 Get_Timer_100us(void)
{
    return SystemFrequency/cFreqNum;	//100us
}

uint32 Get_Timer_200us(void)
{
    return 2*SystemFrequency/cFreqNum;	//100us
}
uint32 Get_Timer_600us(void)
{
    return 6*(SystemFrequency/cFreqNum);	//600us
}
uint32 Get_Timer_2ms(void)
{
    return 20*(SystemFrequency/cFreqNum);	//2ms Ä¬ÈÏ
}
uint32 Get_Timer_5ms(void)
{
    return 50*(SystemFrequency/cFreqNum);	//5ms
}
uint32 Get_Timer_8ms(void)
{
    return 80*(SystemFrequency/cFreqNum);	//8ms
}
uint32 Get_Timer_10ms(void)
{
    return 100*(SystemFrequency/cFreqNum);	//10ms
}
uint32 Get_Timer_20ms(void)
{
	return 200*(SystemFrequency/40000);	//20ms
}
uint32 Get_Timer_1s(void)
{
    return 200*50*(SystemFrequency/40000);	//20ms
}

void AddTwoFallEdgeCounter(void)
{
		vTimer0Counter++;
}
void ClrTwoFallEdgeCounter(void)
{
		vTimer0Counter = 0;
}
uint32 GetTwoFallEdgeCounter(void)
{
		return vTimer0Counter;
}
void AddRisedgeCounter(void)
{
    vTimerRisingCounter++;
}
void ClrRisedgeCounter(void)
{
    vTimerRisingCounter=0;
}
uint32 GetRisedgeCounter(void)
{
    return vTimerRisingCounter;
}
//wait 2ms,to send data
void AddWait2msCounter(void)
{
    vTimer0Wait2msCounter++;
}
void ClrWait2msCounter(void)
{
    vTimer0Wait2msCounter=0;
}
uint32 GetWait2msCounter(void)
{
    return vTimer0Wait2msCounter;
}

uint8 Is2msArrived(void)
{
    return (vTimer0Wait2msCounter>=cTime2ms);
}

//2ms wait state
void Enable2ms(void)
{
    vWait2msState=1;
}

void Disable2ms(void)
{
    vWait2msState=0;
}

uint8 Get2msState(void)
{
    return vWait2msState;
}

void Set2msArrived(void)
{
    vWaittoRun.Flag.b2ms=1;
}
uint32 Get2msArrived(void)
{
    return vWaittoRun.Flag.b2ms;
}
void Clr2msArrived(void)
{
    vWaittoRun.Flag.b2ms=0;
}


//
//uint32 GetTimer3Counter(void)
//{
//        return vTimer3Counter;
//}
//void AddTimer3Counter(void)
//{
//        vTimer3Counter++;
//}
//void ClrTimer3Counter(void)
//{
//        vTimer3Counter = 0;
//}
