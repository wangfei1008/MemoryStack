//////////////////////////////////////////////////////////////////////
// MemoryStack.cpp: implementation of the MemoryStack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemoryStack.h"
#include "syslog/syslog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct MemStack
{
	unsigned long memmax;        //最大存储内存
	unsigned long memlen;        //内存当前长度
    CRITICAL_SECTION lock;       //互斥锁 
	
	byte* buffer;                //存储内存	
}MemStack;

pMStack InitMemStack(unsigned long size)
{	
//	logsetparm(LOG_NOPRINTF);
	logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "init memory...\n");
	pMStack pstack = NULL;
	
	if(NULL == (pstack = (pMStack)malloc( sizeof(MStack) )))
		return NULL;

	pstack->memmax = size;
	pstack->memlen = 0;

	if (NULL == (pstack->buffer = (byte*)malloc(size)))
		return NULL;
	memset(pstack->buffer, '\0', size);

	InitializeCriticalSection(&pstack->lock);
	logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "init memory success:%d, max size:%l\n", pstack, size);
	return pstack;	
}

void ClearMemStack(pMStack* pstack)
{
	CRITICAL_SECTION* pLock;

	if ((*pstack) != NULL)
	{
		logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "clear memory:%d\n", *pstack);
		pLock = &(*pstack)->lock;
		EnterCriticalSection(pLock);
		if ((*pstack)->buffer != NULL)//////2.29
		{
			free((*pstack)->buffer);
			(*pstack)->buffer = NULL;
		}
		LeaveCriticalSection(pLock);
		DeleteCriticalSection(pLock); 
		pLock = NULL;
		free(*pstack);
		*pstack = NULL;
		loginfo("clear memory success\n");
		logrelease;
	}
}

void PushMemStack(pMStack pstack, unsigned long len, byte* buffer)
{
	if (pstack == NULL || len <= 0 || buffer == NULL)
		return;

	EnterCriticalSection(&pstack->lock);
	logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "PushMemStack0:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
	if ((len + pstack->memlen) < pstack->memmax )//添加内存数据不能使内存满
	{
		memcpy(pstack->buffer + pstack->memlen, buffer, len);		
		pstack->memlen += len;
	}
	else
	{
		if (len > pstack->memmax)
			len = pstack->memmax;
		TRACE("Push memory more maxsize\n");
		memcpy(pstack->buffer, pstack->buffer + len + pstack->memlen - pstack->memmax, pstack->memmax - len);
		memcpy(pstack->buffer + pstack->memmax - len, buffer, len);
		pstack->memlen = pstack->memmax;
	}
	logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "PushMemStack1:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
	LeaveCriticalSection(&pstack->lock);
}

unsigned long PopMemStack(pMStack pstack, unsigned long len, byte* buffer)
{
	if (pstack == NULL || len <= 0 || buffer == NULL)
		return -1;

	 EnterCriticalSection(&pstack->lock);
	 logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "PopMemStack0:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
	 unsigned long length = 0;
	 if(len > pstack->memlen)//取得长度大于内存实际存储长度
	 {
		 memcpy(buffer, pstack->buffer, pstack->memlen);
		 length = pstack->memlen;
		 pstack->memlen = 0;
		 memset(pstack->buffer, '\0', pstack->memmax);
		 logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "PopMemStack1:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
		 LeaveCriticalSection(&pstack->lock);

		 return length;
	 }
	 else
	 {
		 memcpy(buffer, pstack->buffer, len);
		 memcpy(pstack->buffer, pstack->buffer + len, pstack->memlen - len);
		 pstack->memlen -= len;
		 memset(pstack->buffer + pstack->memlen, '\0', pstack->memmax - pstack->memlen);
		 logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "PopMemStack2:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
		 LeaveCriticalSection(&pstack->lock);

		 return len;
	 }
}

unsigned long ReadMemStack(pMStack pstack, unsigned long len, byte* buffer)
{
	if (pstack == NULL || len <= 0 || buffer == NULL)
		return -1;
	 
	 EnterCriticalSection(&pstack->lock);
	 logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "ReadMemStack0:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);

	 if(len > pstack->memlen)//取得长度大于内存实际存储长度
	 {
		 memcpy(buffer, pstack->buffer, pstack->memlen);
		 logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "ReadMemStack1:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
		 LeaveCriticalSection(&pstack->lock);
		 return pstack->memlen;
	 }
	 else
	 {
		 memcpy(buffer, pstack->buffer, len);
		 logwrite(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, "ReadMemStack2:pstack%d, len%d, buffer%d, pstackbuffer%d, pstacklen%d\n", pstack, len, buffer, pstack->buffer, pstack->memlen);
		 LeaveCriticalSection(&pstack->lock);
		 return len;
	 }
}

unsigned long MemStackSize(pMStack pstack)
{
	return pstack->memlen;
}