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

#include "Ymodem.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
*@brief£ºThis function is used to initialize the Ymodem module.
*@param£º
	[pRxBuf], Entry data buffer pointer of Ymodem module.
	[pTxBuf], Exit data buffer pointer of Ymodem module.
	[numBufSize], Length of incoming data buffer.
*@retval£ºNone
*/
static size_t vYmedemReadData(YSINT8_t* pBuff, YUINT8_t numSize, void *pFile)
{
#if (defined(_MSC_VER) || defined(__GUNC__))
	return fread(pBuff, numSize, Y_PACKET_DATA_MAX_SIZE, (FILE*)pFile);
#endif
	return 0;
}


/*!
*@brief£ºThis function is used to initialize the Ymodem module.
*@param£º
	[pRxBuf], Entry data buffer pointer of Ymodem module.
	[pTxBuf], Exit data buffer pointer of Ymodem module.
	[numBufSize], Length of incoming data buffer.
*@retval£ºNone
*/
static size_t vYmedemWriteData(YSINT8_t* pBuff, YUINT8_t numSize, void *pFile)
{
#if (defined(_MSC_VER) || defined(__GUNC__))
	return fwrite(pBuff, numSize, Y_PACKET_DATA_MAX_SIZE, (FILE*)pFile);
#endif
	return 0;
}


/*!
*@brief£ºThis function is called when there is an assertion error in the Ymodem module.
*@param£º
	[file], Error with error.
	[num], The line number of the error.
*@retval£ºNone
*/
static void vYmodemAssertCall(char *pFileName, YUINT16_t numFileLine)
{
#if (Y_DEBUG_ENABLE)
	printf("Error:%s,%d\r\n", pFileName, numFileLine);
#endif
	for( ; ; );
}


/*!
*@brief£ºThis function is used to initialize the Ymodem module.
*@param£º
    [stcYManage]£¬Ymodem manage object.
*@retval£ºNone
*/
static void vYmodemHookBind(void * const pManage, 
	                        void (*pCallback)(void*), 
					        YUINT8_t numToHook)
{
	stcYmodemManage_t* pM = (stcYmodemManage_t*)pManage;
	
	if (pCallback != NULL)
	{
		switch(numToHook)
		{
			case enToHook_CTCH:
				pM->vCommumTxCompleteHook = pCallback;
				break;
			case enToHook_CRCH:
				pM->vCommumRxCompleteHook = pCallback;
				break;
		#if (Y_MANAGE_PACKET_RTX_COMPLETE_HOOK)
			case enToHook_PTCH:
				pM->vPacketTxCompleteHook = pCallback;
				break;
			case enToHook_PRCH:
			    pM->vPacketRxCompleteHook = pCallback;
				break;
		#endif
			default:
				break;
		}
	}
}


/*!
*@brief£ºThis function is used to initialize the Ymodem module.
*@param£º
	[pRxBuf], Entry data buffer pointer of Ymodem module.
	[pTxBuf], Exit data buffer pointer of Ymodem module.
	[numBufSize], Length of incoming data buffer.
*@retval£ºNone
*/
void vYmodemInitialize(stcYmodemManage_t * const stcManage)
{
	vYmodemAssert(stcManage != NULL);
	
	stcManage->vHookBind = vYmodemHookBind;
//	vYmodemAssert((pRxBuf != NULL) && (pTxBuf != NULL));
//	vYmodemAssert(numBufSize >= Y_PACKET_MAX_SIZE);
//	
//	stcYManage->stcState.numModel = 0;
//	stcYManage->stcState.numState = 0;
//	stcYManage->stcState.numSeek  = 0;
//	
//	// rx
//	stcYManage->stcRxPacket.pPacketHeader = pRxBuf;
//	stcYManage->stcRxPacket.pPacketOrder = (pRxBuf + 1);
//	stcYManage->stcRxPacket.pPacketFirstBlock = (pRxBuf + 3);
//	stcYManage->stcRxPacket.pPacketDataBlock = (pRxBuf + Y_PACKET_BLOCK_MAX_SIZE + 3);
//	stcYManage->stcRxPacket.pCRCValue = (pRxBuf + numBufSize - 2);
//	
//	// tx
//	stcYManage->stcTxPacket.pPacketHeader = pTxBuf;
//	stcYManage->stcTxPacket.pPacketOrder = (pTxBuf + 1);
//	stcYManage->stcTxPacket.pPacketFirstBlock = (pTxBuf + 3);
//	stcYManage->stcTxPacket.pPacketDataBlock = (pTxBuf + Y_PACKET_BLOCK_MAX_SIZE + 3);
//	stcYManage->stcTxPacket.pCRCValue = (pTxBuf + numBufSize - 2);
}


/*!
*@brief£ºThis function is used to initialize the Ymodem module.
*@param£º
    [stcYManage]£¬Ymodem manage object.
*@retval£ºNone
*/
void vYmodemCommunReceive(stcYmodemManage_t * const stcYManage)
{
	
}


/*!
*@brief£ºThis function is used to initialize the Ymodem module.
*@param£º
    [stcYManage]£¬Ymodem manage object.
	[pFileInfo], File information object.
*@retval£ºNone
*/
void vYmodemCommunTransmit(stcYmodemManage_t * const stcYManage, 
	                       stcYFI_t pFileInfo)
{
	
}


#ifdef __cplusplus
}
#endif
