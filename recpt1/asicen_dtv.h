#ifndef _ASICEN_DTV_H_
#define _ASICEN_DTV_H_

typedef int HANDLE;

int DTV_SetEncrypKey(unsigned char* APEncSeed, unsigned char APEncSeedLen,unsigned char* PCKey, unsigned char PCKeyLen,HANDLE hDeviceHandle);
int DTV_GetDecryptData(unsigned char* TSData_In, int FrameNum,unsigned char* TSData_Out,HANDLE hDeviceHandle);
int DTV_SCardTransmit(unsigned char *pbSendBuffer,int SendLength,unsigned char *pbRecvBuffer,int *RecvLength,HANDLE hDeviceHandle);

#endif
