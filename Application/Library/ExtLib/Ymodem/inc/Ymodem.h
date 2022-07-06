/*
* Ymodem Protocol Stack V1.0.0
* Copyright <2022> <Tyroo>
*
* Redistribution and use in source and binary forms, with or without modification, are 
* permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the 
* following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
* in the documentation and/or other materials provided with the distribution.
* 
* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived 
* from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* https://github.com/Tyroo
*/

#ifndef __YMODEM_H__
#define __YMODEM_H__

#include <stdio.h>

/*! ---------------------------------------------------------
@define block
----------------------------------------------------------*/

/* define config block */
#define Y_DEBUG_ENABLE                         (1)    // Y
#define Y_FILE_CONTINUE_TX_MAX_NUM             (5)    // Y
#define Y_MANAGE_PACKET_RTX_COMPLETE_HOOK      (0)    // Y

#define vYmodemAssert(v)       if (v == 0) vYmodemAssertCall(__FILE__, __LINE__) // Y
#define vYmodemReadDataByte    ((YUINT32_t)&vYmodemReadData)
#define vYmodemWriteDataByte   ((YUINT32_t)&vYmodemWriteData)

/* define read-only config block */
#define Y_PACKET_DATA_MAX_SIZE                 (0x80) // N
#define Y_PACKET_FILE_NAME_MAX_LENGTH          (0x20) // N
#define Y_PACKET_NULL_FRAME                    (0x00) // N
#define Y_PACKET_MAX_SIZE                      (Y_PACKET_BLOCK_MAX_SIZE * 8 + 5) // N

/* define examine block */
#if ((Y_PACKET_FILE_NAME_MAX_LENGTH + Y_PACKET_FILE_SIZE_MAX_LENGTH) > Y_PACKET_DATA_MAX_SIZE)
  #error ("The file name exceeds the maximum packet length.")
#endif

#if (!(defined(vYmodemReadDataByte) && defined(vYmodemWriteDataByte)))
  #error ("The file name exceeds the maximum packet length.")
#endif


/*! ---------------------------------------------------------
@typedef block
----------------------------------------------------------*/

/* Base data type redefinition */
typedef signed   char  YSINT8_t;
typedef unsigned char  YUINT8_t;
typedef unsigned short YUINT16_t;
typedef unsigned int   YUINT32_t;

/* Packet file information type redefinition */
typedef struct _stcYFileInfo
{
	void      *pFile;
	YUINT16_t numFileByteSize;
} stcYFI_t;

typedef struct _stcYTxFileQueue
{
	stcYFI_t  queue[Y_FILE_CONTINUE_TX_MAX_NUM];
	YSINT8_t  strCurTxFileName[Y_PACKET_FILE_NAME_MAX_LENGTH];
	YUINT8_t  numCurTxFileQueueIndex;
	YUINT16_t numCurTxFileSeek;	// Number of byte transferred
	
	void (*vEnqueue)(stcYFI_t stcFileInfo);
	void (*vDequeue)();
	
} stcYTFQ_t;

typedef struct _stcYPacketFileData
{
	YSINT8_t strFileNameData[Y_PACKET_FILE_NAME_MAX_LENGTH];
	YSINT8_t strFileSizeData[Y_PACKET_DATA_MAX_SIZE - Y_PACKET_FILE_NAME_MAX_LENGTH];
} stcTPFD_t;

typedef union _uniYPacketData
{
	YSINT8_t  strData[Y_PACKET_DATA_MAX_SIZE];
	stcTPFD_t strFileData;
} uniYPFD_t;

/* Packet type redefinition */
typedef struct _stcYPacket
{
	YUINT8_t pPacketHeader;
	YUINT8_t pPacketOrder[2];
	uniYPFD_t pPacketData;
	YUINT8_t *pCRCValue;
	
} stcYPacket_t;

typedef struct _stcYStatus
{
	YUINT8_t  numState;
	YUINT8_t  numModel;
	
} stcYStatus_t;

/* Communication status type redefined */
typedef struct _stcYmodemManage
{

	stcYStatus_t stcStatus;
	
	stcYPacket_t stcTxPacket;
	stcYPacket_t stcRxPacket;
	
	stcYTFQ_t stcTxFileQueue;

	void (*vCommumTxCompleteHook)(void *stcManage);
	void (*vCommumRxCompleteHook)(void *stcManage);
	
	void (*vHookBind)(void * const pManage, 
	                  void (*pCallback)(void*), 
					  YUINT8_t numToHook);
	
#if (Y_MANAGE_PACKET_RTX_COMPLETE_HOOK)
	void (*vPacketTxCompleteHook)(void *stcManage, const YUINT8_t * const numProgress);
	void (*vPacketRxCompleteHook)(void *stcManage, const YUINT8_t * const numProgress);
#endif
	
} stcYmodemManage_t;


/*! ---------------------------------------------------------
@enum block
----------------------------------------------------------*/

/* Packet enumeration type definition */
enum enPacketHeaderType { enPHT_SOH = 0x01, enPHT_STX };
enum enStartPacketOrder { enSPON_FON, enSPHT_LON = 0xFF };
enum enToHook { enToHook_CTCH, enToHook_CRCH, enToHook_PTCH, enToHook_PRCH };


/*! ---------------------------------------------------------
@function block
----------------------------------------------------------*/

void vYmodemInitialize(stcYmodemManage_t * const stcManage);

void vYmodemCommunReceive(stcYmodemManage_t * const stcYManage);

void vYmodemCommunTransmit(stcYmodemManage_t * const stcYManage, 
	                       stcYFI_t pFileInfo);


#endif /* __YMODEM_H__ */
