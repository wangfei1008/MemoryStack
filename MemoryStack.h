// MemoryStack.h: interface for the MemoryStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMORYSTACK_H__CD5A4E40_400B_4097_B6C4_09A92F076B5B__INCLUDED_)
#define AFX_MEMORYSTACK_H__CD5A4E40_400B_4097_B6C4_09A92F076B5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef  WX_API_DLLEXP
#define WX_API_DLLEXP extern "C" __declspec(dllimport)
#else
#define WX_API_DLLEXP extern "C" __declspec(dllexport)
#endif

typedef struct MemStack* pMStack;
typedef struct MemStack  MStack;


WX_API_DLLEXP pMStack InitMemStack(unsigned long size);

WX_API_DLLEXP void ClearMemStack(pMStack* pstack);

WX_API_DLLEXP void PushMemStack(pMStack pstack, unsigned long len, byte* buffer);

WX_API_DLLEXP unsigned long PopMemStack(pMStack pstack, unsigned long len, byte* buffer);

WX_API_DLLEXP unsigned long ReadMemStack(pMStack pstack, unsigned long len, byte* buffer);

WX_API_DLLEXP unsigned long MemStackSize(pMStack pstack);

#endif // !defined(AFX_MEMORYSTACK_H__CD5A4E40_400B_4097_B6C4_09A92F076B5B__INCLUDED_)
