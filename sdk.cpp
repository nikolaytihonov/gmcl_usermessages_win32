#include "sdk.h"

extern DWORD dwRecipientVTable;
extern IVEngineServer* g_pEngine;

DWORD FindPattern(HMODULE hDll, char* pattern, char* mask)
{
	MODULEINFO mInfo;
	GetModuleInformation(GetCurrentProcess(),hDll,&mInfo,sizeof(MODULEINFO));

	DWORD base = (DWORD)hDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);
	for(DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for(DWORD j = 0; j < patternLength; j++)
			found &= mask[j] == '?' || pattern[j] == *(char*)(base+i+j);
		if(found)
			return base+i;
	}
	return NULL;
}

CBasePlayer* GetEntityByLuaEntity(CLuaState* L,int idx)
{
	int ret;
	edict_t* pEdict;

	L->Push(idx);
	L->GetField(-1,"EntIndex");
	L->Push(-2);
	L->Call(1,1);
	ret = L->GetInteger(-1);
	L->Pop(2);

	if(!(pEdict = g_pEngine->PEntityOfEntIndex(ret)))
		return NULL;
	else if(pEdict->IsFree())
		return NULL;
	return (CBasePlayer*)(pEdict->GetUnknown()->GetBaseEntity());
}

CLocalRecipientFilter::CLocalRecipientFilter()
{
	dwVTable = dwRecipientVTable;
	Reset();
}

CLocalRecipientFilter::~CLocalRecipientFilter()
{
}

void CLocalRecipientFilter::Reset()
{
	m_bReliable = false;
	m_bInitMessage = false;
	m_bUsingPredictionRules = false;
	m_bIgnorePredictionCull = false;
	m_Recipients.RemoveAll();
}

void CLocalRecipientFilter::AddRecipient(const CBasePlayer* pPlayer)
{
	int idx = pPlayer->entindex();
	if(m_Recipients.Find(idx)!=m_Recipients.InvalidIndex())
		return;
	m_Recipients.AddToTail(idx);
}

CLocalUserMessages::CLocalUserMessages()
{
}

CLocalUserMessages::~CLocalUserMessages()
{
}

int CLocalUserMessages::LookupUserMessage(const char* name)
{
	int idx;
	if((idx = m_UserMessages.Find(name))!=m_UserMessages.InvalidIndex())
		return idx;
	return -1;
}