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
#define Y_PACKET_BLOCK_MAX_SIZE                (0x80)
#define Y_PACKET_FILE_NAME_MAX_LENGTH          (0x20)
#define Y_PACKET_FILE_SIZE_MAX_LENGTH          (0x0A)
#define Y_PACKET_NULL_FRAME                    (0x00)
#define Y_PACKET_MAX_SIZE                      (Y_PACKET_BLOCK_MAX_SIZE * 8 + 5)

#define Y_MANAGE_USE_PACKET_TX_COMPLETE_HOOK   (0)

#define vYmodemAssert(v)    if(v == 0) vYmodemError(__FILE__, __LINE__)

/* define examine block */
#if ((Y_PACKET_FILE_NAME_MAX_LENGTH + Y_PACKET_FILE_SIZE_MAX_LENGTH) > Y_PACKET_BLOCK_MAX_SIZE)
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
typedef struct _stcYPacketFileInfo
{
	FILE     *pFile;
	YSINT8_t strFileName[Y_PACKET_FILE_NAME_MAX_LENGTH];
	YSINT8_t strFileSize[Y_PACKET_FILE_SIZE_MAX_LENGTH];
} stcYPFI_t;

/* Packet type redefinition */
typedef struct _stcYPacket
{
	stcYPFI_t stcPacketFileInfo;
	
	YSINT8_t *pPacketHeader;
	YSINT8_t *pPacketOrder;
	
	YSINT8_t *pPacketFirstBlock;
	YSINT8_t *pPacketDataBlock;
	
	YSINT8_t *pCRCValue;
	
} stcYPacket_t;

typedef struct _stcYmodemStatus
{
	YUINT8_t  numState;
	YUINT8_t  numModel;
	YUINT16_t numSeek;	// Number of byte transferred

} stcYmodemStatus_t;

/* Communication status type redefined */
typedef struct _stcYmodemManage
{

	stcYmodemStatus_t stcState;

	stcYPacket_t stcTxPacket;
	stcYPacket_t stcRxPacket;
	
	void (*FirstPacketRxCompleteHook)(void* stcManage, stcYPFI_t* stcFileInfo);
	void (*DataPacketRxCompleteHook)(void* stcManage);
	
	void (*FirstPacketTxCompleteHook)(void* stcManage, stcYPFI_t* stcFileInfo);
	void (*DataPacketTxCompleteHook)(void* stcManage);
	
	#if (Y_MANAGE_USE_PACKET_TX_COMPLETE_HOOK)
	  void (*DataPacketRxCompleteBeforeHook)(void* stcManage, YUINT8_t numProgress);
	  void (*DataPacketTxCompleteBeforeHook)(void* stcManage, YUINT8_t numProgress);
	#endif
	
	void (*HookBind)(void* pCallback);
	
} stcYmodemManage_t;


/*! ---------------------------------------------------------
@enum block
----------------------------------------------------------*/

/* Packet enumeration type definition */
enum enPacketHeaderType { enPHT_SOH = 0x01, enPHT_STX };
enum enStartPacketOrder { enSPON_FON, enSPHT_LON = 0xFF };


/*! ---------------------------------------------------------
@function block
----------------------------------------------------------*/

static void vYmodemError(char* file, YUINT16_t num);

void vYmodemInitialize(stcYmodemManage_t* stcYManage,
					   YSINT8_t * const pRxBuf, 
					   YSINT8_t * const pTxBuf,
					   const YUINT16_t numBufSize);



#endif /* __YMODEM_H__ */
