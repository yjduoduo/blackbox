/*
*********************************************************************************************************
*                                                
*                            对象:	CTimeDef 
*
*                        (c) Copyright 2010-2016, 蒋雪梅
*                              All Rights Reserved
*                                    Ver1.0
* File : CTimeDef .H
* By   : 
*********************************************************************************************************
*/

#ifndef CTime0Def_H
#define CTime0Def_H
 
#include "includeall.h"

#ifdef  CTime0Def_C
#define Ex_CTime0Def 
#else
#define Ex_CTime0Def  extern 
#endif

/*******************************************************************/
/******************       全局常量定义?    *********************/
/*******************************************************************/
//constant
#define  cTime0_2ms     0    //同步
#define  cTime0_5ms     1    //
#define  cTime0_600us   2    //传递参数   
#define  cTime0_8ms     3    //

#define  cCMaxTime0Def   4    //


#define  cSystemFrequency   100
#define  cSystemTimeBit     2500

#define  cTime0V_2ms     cSystemTimeBit*20     
#define  cTime0V_5ms     cSystemTimeBit*50       
#define  cTime0V_600us   cSystemTimeBit*6          
#define  cTime0V_8ms     cSystemTimeBit*80 
#define  cTime0V_20ms     cSystemTimeBit*200 


#define  cTimeUnit   100 


#define  cTime600us   (600/cTimeUnit)  //数据反馈输出使用
#define  cTime1ms     (1000/cTimeUnit) 
#define  cTime2ms     (2*cTime1ms)
#define  cTime9ms     (9*cTime1ms)
#define  cTime10ms    (10*cTime1ms)
#define  cTime14ms    (14*cTime1ms)
#define  cTime18ms    (18*cTime1ms)
#define  cTime22ms    (22*cTime1ms)

#define  cTime50ms    (50*cTime1ms)
#define  cTime80ms    (80*cTime1ms)
#define  cTime380ms   (380*cTime1ms)
#define  cTime800ms   (800*cTime1ms)


#define  cTime2ms_Sys0      (cTime2ms)
#define  cTime2ms_Data0     (cTime2ms_Sys0+cTime600us)
#define  cTime2ms_Data1     (cTime2ms_Data0+cTime600us)
#define  cTime2ms_Data2     (cTime2ms_Data1+cTime600us)
#define  cTime2ms_Data3     (cTime2ms_Data2+cTime600us)
#define  cTime2ms_Data4     (cTime2ms_Data3+cTime600us)
#define  cTime2ms_Data5     (cTime2ms_Data4+cTime600us)
#define  cTime2ms_Data6     (cTime2ms_Data5+cTime600us)
#define  cTime2ms_Data7     (cTime2ms_Data6+cTime600us)
#define  cTime2ms_Sys1      (cTime2ms_Data7+cTime600us)
#define  cTime2ms_SysE      (cTime2ms_Sys1+cTime600us)

/*******************************************************************/
/******************       数据结构定义?    *********************/
/*******************************************************************/ 
typedef union type_WaitToRun{
    uint32 Word;
    struct
    {
        volatile uint32 b2ms:1;//wait 2ms arrived,must be volatile
        uint32 b20ms:1;//keyboard
        uint32 b100ms:1;//lcd display
        uint32 b500ms:1;//rtc clock
    }Flag;


}WaitToRun;
 
 	

/*******************************************************************/
/****************     公有成员变量,函数及常量     ******************/
/*******************************************************************/
//  #define  cTime2ms_Sys0     (cTime2ms)
  
extern  uint32   Get_CTime0Def(void);
extern  void     Set_CTime0Def(uint32 vTimeId); 
extern  uint32   Sel_CTime0Def(uint8 vTimer);
extern  uint32   Get_Timer_100us(void);
extern  uint32   Get_Timer_200us(void);
extern  uint32   Get_Timer_600us(void);
extern  uint32   Get_Timer_2ms(void);
extern  uint32   Get_Timer_5ms(void);
extern  uint32   Get_Timer_8ms(void);
extern  uint32   Get_Timer_10ms(void);
extern  uint32   Get_Timer_20ms(void);
extern  uint32   Get_Timer_1s(void);
//两次下降沿时长
void AddTwoFallEdgeCounter(void);
void ClrTwoFallEdgeCounter(void);
uint32 GetTwoFallEdgeCounter(void);
//上升沿时长
void AddRisedgeCounter(void);
void ClrRisedgeCounter(void);
uint32 GetRisedgeCounter(void);
//等待2ms //2ms wait state
void AddWait2msCounter(void);
void ClrWait2msCounter(void);
uint32 GetWait2msCounter(void);
uint8 Is2msArrived(void);
void Enable2ms(void);
void Disable2ms(void);
uint8 Get2msState(void);


//2ms arrived flag
void Set2msArrived(void);
uint32 Get2msArrived(void);
void Clr2msArrived(void);


 /*******************************************************************/
/********************* 私有成员变量,函数及常量    *******************/
/*******************************************************************/
//Private:
//#define CTimeDef_C
#ifdef  CTimeDef_C    

uint32  vCurTime0Def=0;
volatile uint32 vTimer0Counter=0;//两次下降沿长度，同步获取
volatile uint32 vTimerRisingCounter=0;//记录上升沿长度，命令获取
volatile uint32 vTimer0Wait2msCounter=0;//等待2ms,等待发送数据
volatile uint8 vWait2msState=0;//enable or disable wait 2ms
static  uint32  m_Time0Def[cCMaxTime0Def]={cTime0V_2ms,cTime0V_5ms,cTime0V_600us,cTime0V_8ms};

volatile WaitToRun vWaittoRun;
#endif   
#endif


