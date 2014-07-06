// stdafx.cpp : source file that includes just the standard includes
//	upgrader.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void HexToint(char *pBuffer, int len, int& iIndex, int* pOut)
{
	UCHAR   chrTemp[20];
	int iIndexTemp = iIndex;
	for (int i = iIndexTemp; i < (len + iIndexTemp); i++ )
	{
		chrTemp[i - iIndexTemp] = (UCHAR)(pBuffer[i]);
		pOut[i - iIndexTemp] = (int)chrTemp[i - iIndexTemp];
		//m_npBufferIndex++;
		iIndex++;
	}
	
}

void HexToULONG(char *pBuffer, int Len, int& iIndex, ULONG *pOut)
{
	ULONG uUlong[10];
	int i;
	int iIndexTemp = iIndex;
	memset(uUlong, 0, Len);
	*pOut = 0x0;

	for ( i = iIndexTemp; i < (Len + iIndexTemp); i++ )
	{
		uUlong[i - iIndexTemp] = (ULONG)(pBuffer[i]);
		//m_npBufferIndex++;
		iIndex++;
	}

	// 将单字节的char转换成Len个字节的ULONG（占32位）
	for ( i = 0; i < Len; i++ )
	{
		*pOut |= uUlong[i] << ((Len - i - 1)* 8 );		
	}
}

void HexToChar(char *pBuffer, int Len, int& iIndex, char *pOut)
{
	int iIndexTemp = iIndex;
	memset( pOut, NULL, Len );
	for (int i = iIndexTemp; i < (Len + iIndexTemp); i++ )
	{
		pOut[i - iIndexTemp] = (char)(pBuffer[i]);
		//m_npBufferIndex++;
		iIndex++;
	}
}

void Ip2String(char *outputStr, char *inputIp)
{
	char address[4],
		output[17];

	memset(output, 0, sizeof(output));
	memcpy(address, inputIp, sizeof(address));

	sprintf(outputStr, "%u.%u.%u.%u", 
		(unsigned char) address[0],
		(unsigned char) address[1],
		(unsigned char) address[2],
		(unsigned char) address[3]);
	//	*outputStr = output;
}

BOOL IsHexDigit(char cChar)
{
	if(	(cChar >= '0' && cChar <= '9') ||
		(cChar >= 'A' && cChar <= 'F') ||
		(cChar >= 'a' && cChar <= 'f') ) 
		return TRUE;
	else
		return FALSE;
}
