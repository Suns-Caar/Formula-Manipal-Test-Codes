#include "xcanps.h"
#include "xparameters.h"
#include "xil_printf.h"
#include "xil_exception.h"
#include "xsdps.h"
#include "stdlib.h"
static XSdPs ps7_EMMC;
XSdPs_Config * EMMC_Config;
u8 WR_Buf[50][10];
u8 Emmc_ExtCsd[1024];
#define CAN_DEVICE_ID	XPAR_XCANPS_0_DEVICE_ID
#define TEST_BTR_SYNCJUMPWIDTH		3
#define TEST_BTR_SECOND_TIMESEGMENT	2
#define TEST_BTR_FIRST_TIMESEGMENT	15
#define TEST_BRPR_BAUD_PRESCALAR	29
#define TEST_MESSAGE_ID			2000
#define FRAME_DATA_LENGTH 8
#define XCANPS_MAX_FRAME_SIZE_IN_WORDS (XCANPS_MAX_FRAME_SIZE/sizeof(u32))
int CanPsPolledExample(u16 DeviceId);
static int RecvFrame(XCanPs *InstancePtr);
static int SendFrame(XCanPs *InstancePtr);

int msg=-1;
static u32 RxFrame[8];
static u32 TxFrame[XCANPS_MAX_FRAME_SIZE_IN_WORDS];
static XCanPs Can;
#ifndef TESTAPP_GEN
XCanPs *CanInstPtr = &Can;
int main()
{
	u32 i,j;
	int Status;
	xil_printf("CAN example starting now");
	Status = CanPsPolledExample(CAN_DEVICE_ID);
	return XST_SUCCESS;
}
#endif
int CanPsPolledExample(u16 DeviceId)
{
	int Status,j;
	EMMC_Config= XSdPs_LookupConfig(XPAR_PS7_SD_1_DEVICE_ID);
	XSdPs_CfgInitialize(&ps7_EMMC, EMMC_Config, EMMC_Config->BaseAddress);
	Status=XSdPs_MmcCardInitialize(&ps7_EMMC);
	Status=XSdPs_Change_ClkFreq(&ps7_EMMC,50000000);
	Status=XSdPs_Select_Card(&ps7_EMMC);
	Status=XSdPs_SetBlkSize(&ps7_EMMC,XSDPS_BLK_SIZE_512_MASK);
	Status=XSdPs_Get_Mmc_ExtCsd(&ps7_EMMC,Emmc_ExtCsd);
	XCanPs_Config *ConfigPtr;
	ConfigPtr = XCanPs_LookupConfig(DeviceId);
	if (CanInstPtr == NULL) {
		return XST_FAILURE;
	}
	Status = XCanPs_CfgInitialize(CanInstPtr,
					ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE_CONFIG);
	while(XCanPs_GetMode(CanInstPtr) != XCANPS_MODE_CONFIG);
	XCanPs_SetBaudRatePrescaler(CanInstPtr, TEST_BRPR_BAUD_PRESCALAR);
	XCanPs_SetBitTiming(CanInstPtr, TEST_BTR_SYNCJUMPWIDTH,
				TEST_BTR_SECOND_TIMESEGMENT,

				TEST_BTR_FIRST_TIMESEGMENT);
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE_NORMAL);
	while(XCanPs_GetMode(CanInstPtr)!= XCANPS_MODE_NORMAL){
		if(XCanPs_GetMode(CanInstPtr)== XCANPS_MODE_NORMAL){
		break;}}
	print("------------CAN Reception starting now--------------");
	xil_printf("\n ID  DLC  Data" );
//	sleep(2);

	Status=SendFrame(CanInstPtr);
	while(1){
		Status=SendFrame(CanInstPtr);

		if(XCanPs_IsRxEmpty(CanInstPtr) != TRUE){
				Status=RecvFrame(CanInstPtr);
	}
//		Status = XSdPs_WritePolled(&ps7_EMMC, 0x05, 2, WR_Buf);
	}

	return Status;
}
static int RecvFrame(XCanPs *InstancePtr)
{
	u32 *FramePtr;
	int Status;
	u8 Index;
	u16 ID;
	u8 a,b,c,d,DLC;
	u8 d1,d2,d3,d4,d5,d6,d7,d8;
	while(XCanPs_IsRxEmpty(CanInstPtr));

	Status = XCanPs_Recv(InstancePtr, RxFrame);
	msg++;


//		print("\n Message received");
		FramePtr = (u32*)(&RxFrame[0]);
		ID=(u16)(FramePtr[0]>>21);
		DLC=(u8)(FramePtr[1]>>24);
		DLC=DLC/((u8)16);
		d1=(u8)(FramePtr[2]);
		d2=(FramePtr[2]>>8);
		FramePtr[2]=FramePtr[2]>>8;
		d3=FramePtr[2]>>8;
		FramePtr[2]=FramePtr[2]>>8;
		d4=(u8)(FramePtr[2]>>8);
		d5=(u8)(FramePtr[3]);
		d6=(FramePtr[3]>>8);
		FramePtr[3]=FramePtr[3]>>8;
		d7=FramePtr[3]>>8;
		FramePtr[3]=FramePtr[3]>>8;
		d8=(u8)(FramePtr[3]>>8);

		xil_printf("\n%d  %d  %d %d %d %d %d %d %d %d %d",ID,DLC,d1,d2,d3,d4,d5,d6,d7,d8,msg);
//
		WR_Buf[msg][0]=ID;
		WR_Buf[msg][1]=DLC;
		WR_Buf[msg][2]=d1;
		WR_Buf[msg][3]=d2;
		WR_Buf[msg][4]=d3;
		WR_Buf[msg][5]=d4;
		WR_Buf[msg][6]=d5;
		WR_Buf[msg][7]=d6;
		WR_Buf[msg][8]=d7;
		WR_Buf[msg][9]=d8;
//		XCanPs_IntrClear(InstancePtr, XCANPS_IXR_RXNEMP_MASK);
		if(ID == 1){
			Status = XSdPs_WritePolled(&ps7_EMMC, 0x05, 2, WR_Buf);
			xil_printf("emmc written");
		}

//		xil_printf("\n %d", sizeof(WR_Buf));

		return Status;
}
static int SendFrame(XCanPs *InstancePtr)
{
	u8 *FramePtr;
	int Index;
	int Status;

	TxFrame[0] = (u32)XCanPs_CreateIdValue((u32)TEST_MESSAGE_ID, 0, 0, 0, 0);
	TxFrame[1] = (u32)XCanPs_CreateDlcValue((u32)FRAME_DATA_LENGTH);

	FramePtr = (u8 *)(&TxFrame[2]);
	for (Index = 0; Index < FRAME_DATA_LENGTH; Index++) {
		*FramePtr++ = (u8)Index;
	}
	while (XCanPs_IsTxFifoFull(InstancePtr) == TRUE);
	for(int i =0;i<5;i++){
	Status = XCanPs_Send(InstancePtr, TxFrame);
	}

	return Status;
}
