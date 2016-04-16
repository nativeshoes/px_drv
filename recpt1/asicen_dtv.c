#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <ctype.h>
#include <libgen.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/ioctl.h>
#include "asicen_dtv.h"
#include "recpt1.h"

#define		GET_DRV_SUPPORT	_IOR(0x8D, 0x81, int *)
#define		GET_RANDOM_KEY	_IOR(0x8D, 0x82, int *)
#define		GEN_ENC_SEED	_IOW(0x8D, 0x83, int *)
#define		DECRYP_MULTI_TS	_IOR(0x8D, 0x85, int *)
#define     SET_BCAS_COMMAND  _IOW(0x8D, 0x87, int *)
#define     GET_BCAS_COMMAND  _IOR(0x8D, 0x88, int *)

#define BCAS_Reset			0
#define BCAS_IDLE			1
#define BCAS_StartWtCmd	2
#define BCAS_WaitRdCmd		3
#define BCAS_SendChain		4
#define BCAS_CmdOK			5

typedef struct _AUSBDTV_GEN_ENCSEED_STRUCTURE 
{
    unsigned char *APEncSeed;
    unsigned char APEncSeedLen;
    unsigned char *PCKey;
    unsigned char PCKeyLen;
}AUSBDTV_GEN_ENCSEED_STRUCTURE, *PAUSBDTV_GEN_ENCSEED_STRUCTURE;

typedef struct _AUSBDTV_DECRYP_MULTI_TS_STRUCTURE 
{
    unsigned char* TSData_In;
    unsigned char* TSData_Out;
    int FrameNum;
}AUSBDTV_DECRYP_MULTI_TS_STRUCTURE, *PAUSBDTV_DECRYP_MULTI_TS_STRUCTURE;

typedef struct _AUSBDTV_BCAS_CMD_RW
{
	unsigned char Status;
	unsigned char ValidByteCnt;
	unsigned char Data[320];
} AUSBDTV_BCAS_CMD_RW, *PAUSBDTV_BCAS_CMD_RW;

//========= Gen Key ============
static const unsigned int  Box0[256] = {
    0x63636363U, 0x7c7c7c7cU, 0x77777777U, 0x7b7b7b7bU,
    0xf2f2f2f2U, 0x6b6b6b6bU, 0x6f6f6f6fU, 0xc5c5c5c5U,
    0x30303030U, 0x01010101U, 0x67676767U, 0x2b2b2b2bU,
    0xfefefefeU, 0xd7d7d7d7U, 0xababababU, 0x76767676U,
    0xcacacacaU, 0x82828282U, 0xc9c9c9c9U, 0x7d7d7d7dU,
    0xfafafafaU, 0x59595959U, 0x47474747U, 0xf0f0f0f0U,
    0xadadadadU, 0xd4d4d4d4U, 0xa2a2a2a2U, 0xafafafafU,
    0x9c9c9c9cU, 0xa4a4a4a4U, 0x72727272U, 0xc0c0c0c0U,
    0xb7b7b7b7U, 0xfdfdfdfdU, 0x93939393U, 0x26262626U,
    0x36363636U, 0x3f3f3f3fU, 0xf7f7f7f7U, 0xccccccccU,
    0x34343434U, 0xa5a5a5a5U, 0xe5e5e5e5U, 0xf1f1f1f1U,
    0x71717171U, 0xd8d8d8d8U, 0x31313131U, 0x15151515U,
    0x04040404U, 0xc7c7c7c7U, 0x23232323U, 0xc3c3c3c3U,
    0x18181818U, 0x96969696U, 0x05050505U, 0x9a9a9a9aU,
    0x07070707U, 0x12121212U, 0x80808080U, 0xe2e2e2e2U,
    0xebebebebU, 0x27272727U, 0xb2b2b2b2U, 0x75757575U,
    0x09090909U, 0x83838383U, 0x2c2c2c2cU, 0x1a1a1a1aU,
    0x1b1b1b1bU, 0x6e6e6e6eU, 0x5a5a5a5aU, 0xa0a0a0a0U,
    0x52525252U, 0x3b3b3b3bU, 0xd6d6d6d6U, 0xb3b3b3b3U,
    0x29292929U, 0xe3e3e3e3U, 0x2f2f2f2fU, 0x84848484U,
    0x53535353U, 0xd1d1d1d1U, 0x00000000U, 0xededededU,
    0x20202020U, 0xfcfcfcfcU, 0xb1b1b1b1U, 0x5b5b5b5bU,
    0x6a6a6a6aU, 0xcbcbcbcbU, 0xbebebebeU, 0x39393939U,
    0x4a4a4a4aU, 0x4c4c4c4cU, 0x58585858U, 0xcfcfcfcfU,
    0xd0d0d0d0U, 0xefefefefU, 0xaaaaaaaaU, 0xfbfbfbfbU,
    0x43434343U, 0x4d4d4d4dU, 0x33333333U, 0x85858585U,
    0x45454545U, 0xf9f9f9f9U, 0x02020202U, 0x7f7f7f7fU,
    0x50505050U, 0x3c3c3c3cU, 0x9f9f9f9fU, 0xa8a8a8a8U,
    0x51515151U, 0xa3a3a3a3U, 0x40404040U, 0x8f8f8f8fU,
    0x92929292U, 0x9d9d9d9dU, 0x38383838U, 0xf5f5f5f5U,
    0xbcbcbcbcU, 0xb6b6b6b6U, 0xdadadadaU, 0x21212121U,
    0x10101010U, 0xffffffffU, 0xf3f3f3f3U, 0xd2d2d2d2U,
    0xcdcdcdcdU, 0x0c0c0c0cU, 0x13131313U, 0xececececU,
    0x5f5f5f5fU, 0x97979797U, 0x44444444U, 0x17171717U,
    0xc4c4c4c4U, 0xa7a7a7a7U, 0x7e7e7e7eU, 0x3d3d3d3dU,
    0x64646464U, 0x5d5d5d5dU, 0x19191919U, 0x73737373U,
    0x60606060U, 0x81818181U, 0x4f4f4f4fU, 0xdcdcdcdcU,
    0x22222222U, 0x2a2a2a2aU, 0x90909090U, 0x88888888U,
    0x46464646U, 0xeeeeeeeeU, 0xb8b8b8b8U, 0x14141414U,
    0xdedededeU, 0x5e5e5e5eU, 0x0b0b0b0bU, 0xdbdbdbdbU,
    0xe0e0e0e0U, 0x32323232U, 0x3a3a3a3aU, 0x0a0a0a0aU,
    0x49494949U, 0x06060606U, 0x24242424U, 0x5c5c5c5cU,
    0xc2c2c2c2U, 0xd3d3d3d3U, 0xacacacacU, 0x62626262U,
    0x91919191U, 0x95959595U, 0xe4e4e4e4U, 0x79797979U,
    0xe7e7e7e7U, 0xc8c8c8c8U, 0x37373737U, 0x6d6d6d6dU,
    0x8d8d8d8dU, 0xd5d5d5d5U, 0x4e4e4e4eU, 0xa9a9a9a9U,
    0x6c6c6c6cU, 0x56565656U, 0xf4f4f4f4U, 0xeaeaeaeaU,
    0x65656565U, 0x7a7a7a7aU, 0xaeaeaeaeU, 0x08080808U,
    0xbabababaU, 0x78787878U, 0x25252525U, 0x2e2e2e2eU,
    0x1c1c1c1cU, 0xa6a6a6a6U, 0xb4b4b4b4U, 0xc6c6c6c6U,
    0xe8e8e8e8U, 0xddddddddU, 0x74747474U, 0x1f1f1f1fU,
    0x4b4b4b4bU, 0xbdbdbdbdU, 0x8b8b8b8bU, 0x8a8a8a8aU,
    0x70707070U, 0x3e3e3e3eU, 0xb5b5b5b5U, 0x66666666U,
    0x48484848U, 0x03030303U, 0xf6f6f6f6U, 0x0e0e0e0eU,
    0x61616161U, 0x35353535U, 0x57575757U, 0xb9b9b9b9U,
    0x86868686U, 0xc1c1c1c1U, 0x1d1d1d1dU, 0x9e9e9e9eU,
    0xe1e1e1e1U, 0xf8f8f8f8U, 0x98989898U, 0x11111111U,
    0x69696969U, 0xd9d9d9d9U, 0x8e8e8e8eU, 0x94949494U,
    0x9b9b9b9bU, 0x1e1e1e1eU, 0x87878787U, 0xe9e9e9e9U,
    0xcecececeU, 0x55555555U, 0x28282828U, 0xdfdfdfdfU,
    0x8c8c8c8cU, 0xa1a1a1a1U, 0x89898989U, 0x0d0d0d0dU,
    0xbfbfbfbfU, 0xe6e6e6e6U, 0x42424242U, 0x68686868U,
    0x41414141U, 0x99999999U, 0x2d2d2d2dU, 0x0f0f0f0fU,
    0xb0b0b0b0U, 0x54545454U, 0xbbbbbbbbU, 0x16161616U,
};
static const unsigned int  Box1[] = {
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x1B000000, 0x36000000
};

typedef unsigned int	u32;

#define GETU32PT(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))

void Gen_Identify_Key(unsigned char* OutputKey, unsigned char* InputKey, unsigned char* RandomKey) 
{
	int i = 0,j=0;
	unsigned int  temp;
	unsigned int Output[8],*finalOut=(unsigned int *)OutputKey;
	unsigned char InputKeyTmp[16];

	for(i=0;i<16;i++)
		InputKeyTmp[i]=InputKey[i]^RandomKey[i];

	i = 0;
	Output[0] = GETU32PT(InputKeyTmp     );
	Output[1] = GETU32PT(InputKeyTmp +  4);
	Output[2] = GETU32PT(InputKeyTmp +  8);
	Output[3] = GETU32PT(InputKeyTmp + 12);
	for (;;) 
	{
		temp  = Output[3];
		Output[4] = Output[0] ^
				(Box0[(temp >> 16) & 0xff] & 0xff000000) ^
				(Box0[(temp >>  8) & 0xff] & 0x00ff0000) ^
				(Box0[(temp      ) & 0xff] & 0x0000ff00) ^
				(Box0[(temp >> 24)       ] & 0x000000ff) ^
				Box1[i];
		Output[5] = Output[1] ^ Output[4];
		Output[6] = Output[2] ^ Output[5];
		Output[7] = Output[3] ^ Output[6];
		if (++i == 10) 
		{
			for(j=0;j<4;j++)
				finalOut[j]=Output[j+4];
			return ;
		}

		for(j=0;j<4;j++)
			Output[j]=Output[j+4];
	}
}

int DTV_Get_Device_Support(unsigned char* pFeature,int InputSz,HANDLE hDeviceHandle)
{
	int   success;
	unsigned char u8Data=0;
	unsigned long ReturnVariable=0;
	
	success = ioctl(hDeviceHandle, GET_DRV_SUPPORT,pFeature);
 	return success;
}

int DTV_Get_RandomKey(unsigned char* pRandomKey,int InputSz,HANDLE hDeviceHandle)
{
	int   success;
	unsigned char u8Data=0;
	unsigned long ReturnVariable=0;
	
	success = ioctl(hDeviceHandle, GET_RANDOM_KEY,pRandomKey);
 	return success;
}

int DTV_SetEncrypKey(unsigned char* APEncSeed, unsigned char APEncSeedLen,	unsigned char* PCKey, unsigned char PCKeyLen,HANDLE hDeviceHandle)
{
	int   success;
	AUSBDTV_GEN_ENCSEED_STRUCTURE DTV_GEN_ENCSEED_Data;
	unsigned long ReturnVariable=0;
	unsigned char u8CmdRtData[64];
	unsigned char bAPKeyIdenfy[4];
	unsigned char pRandomKey[16];
	unsigned char OutputKey[16];
	
	if(DTV_Get_Device_Support(bAPKeyIdenfy,4,hDeviceHandle))
		return FALSE;
	
	if(bAPKeyIdenfy[0]!=0)
	{
		if(DTV_Get_RandomKey(pRandomKey,16,hDeviceHandle))
			return FALSE;
		Gen_Identify_Key(OutputKey, APEncSeed, pRandomKey);
		
		DTV_GEN_ENCSEED_Data.APEncSeed=OutputKey;
		DTV_GEN_ENCSEED_Data.APEncSeedLen=APEncSeedLen;
		DTV_GEN_ENCSEED_Data.PCKey=PCKey;
		DTV_GEN_ENCSEED_Data.PCKeyLen=PCKeyLen;

		success = ioctl(hDeviceHandle, GEN_ENC_SEED,&DTV_GEN_ENCSEED_Data);
	}
	else
	{
		DTV_GEN_ENCSEED_Data.APEncSeed=APEncSeed;
		DTV_GEN_ENCSEED_Data.APEncSeedLen=APEncSeedLen;
		DTV_GEN_ENCSEED_Data.PCKey=PCKey;
		DTV_GEN_ENCSEED_Data.PCKeyLen=PCKeyLen;

		success = ioctl(hDeviceHandle, GEN_ENC_SEED,&DTV_GEN_ENCSEED_Data);
	}
	
	return success;
}

int DTV_GetDecryptData(unsigned char* TSData_In, int FrameNum,unsigned char* TSData_Out,HANDLE hDeviceHandle)
{
	int   success;
	AUSBDTV_DECRYP_MULTI_TS_STRUCTURE DTV_DECRYP_MULTI_Data;
	unsigned long ReturnVariable=0;
	
	
	DTV_DECRYP_MULTI_Data.TSData_In=TSData_In;
	DTV_DECRYP_MULTI_Data.TSData_Out=TSData_Out;
	DTV_DECRYP_MULTI_Data.FrameNum=FrameNum;

	success = ioctl(hDeviceHandle, DECRYP_MULTI_TS,&DTV_DECRYP_MULTI_Data);
	return success;
}

int DTV_SCardTransmit(unsigned char *pbSendBuffer,int SendLength,unsigned char *pbRecvBuffer,int *RecvLength,HANDLE hDeviceHandle)
{
	AUSBDTV_BCAS_CMD_RW bcas_cmd;
	int   success;
	unsigned char timeout_counter;

	memcpy(bcas_cmd.Data,pbSendBuffer,SendLength);
	bcas_cmd.ValidByteCnt=SendLength;
	timeout_counter=0;
	success =ioctl (hDeviceHandle, SET_BCAS_COMMAND,&bcas_cmd);

	success =ioctl (hDeviceHandle, GET_BCAS_COMMAND,&bcas_cmd);
	timeout_counter=0;
	while(bcas_cmd.Status!=BCAS_IDLE)
	{
		timeout_counter++;
		if(timeout_counter>20)
			break;
		success =ioctl (hDeviceHandle, GET_BCAS_COMMAND,&bcas_cmd);
		usleep(50000);
	}

    if(timeout_counter<20)
    {
		memcpy(pbRecvBuffer,bcas_cmd.Data,bcas_cmd.ValidByteCnt);
		*RecvLength=bcas_cmd.ValidByteCnt;
   	}

	return success;
}
