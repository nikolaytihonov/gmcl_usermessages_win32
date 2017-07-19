#ifndef __SDK_H
#define __SDK_H

#define GAME_DLL
#include <Windows.h>
#include <Psapi.h>
#include "utlvector.h"
#include "utldict.h"
#include "cbase.h"

//Used to retrieve recipient vtable
//#define AWARDACHIEVEMENT_SIG "\x55\x8B\xEC\x83\xEC\x20\x56\x8B\xF1\x8D\x4D\xE0\xE8\x00\x00\x00\x00\x56\x8D\x4D\xE0\xC7\x45\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8D\x45\xE0\x68\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\x8B\x4D\x08\x51\xE8\x00\x00\x00\x00\x8B\x55\x0C\x52\xE8\x00\x00\x00\x00\x83\xC4\x10\xE8\x00\x00\x00\x00\x8D\x4D\xE0\xE8\x00\x00\x00\x00\x5E\x8B\xE5\x5D\xC2\x08\x00"
//#define AWARDACHIEVEMENT_MASK "xxxxxxxxxxxxx????xxxxxx?????x????xxxx????xx????xxxxx????xxxxx????xxxx????xxxx????xxxxxxx"
#define AWARDACHIEVEMENT_SIG "\x55\x8B\xEC\x83\xEC\x20\x56\x8B\xF1\x8D\x4D\xE0\xE8\x00\x00\x00\x00\x56\x8D\x4D\xE0\xC7\x45\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8D\x45\xE0\x68\x00\x00\x00\x00\x50\xE8\x00\x00\x00\x00\xFF\x75\x08\xE8\x00\x00\x00\x00\xFF\x75\x0C\xE8\x00\x00\x00\x00\x83\xC4\x10\xE8\x00\x00\x00\x00\x8D\x4D\xE0\xE8\x00\x00\x00\x00\x5E\x8B\xE5\x5D\xC2\x08\x00"
#define AWARDACHIEVEMENT_MASK "xxxxxxxxxxxxx????xxxxxx?????x????xxxx????xx????xxxx????xxxx????xxxx????xxxx????xxxxxxx"
#define AWARDACHIEVEMENT_OFFSET 0x18

//#define USERMESSAGEBEGIN_SIG "\x55\x8B\xEC\x8B\x0D\x00\x00\x00\x00\x56\x8B\x75\x0C\x56\xE8\x00\x00\x00\x00\x83\xF8\xFF\x75\x0C\x56\x68\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x8B\x11\x8B\x92\x00\x00\x00\x00\x50\x8B\x45\x08\x50\xFF\xD2\xA3\x00\x00\x00\x00\x5E\x5D\xC3"
//#define USERMESSAGEBEGIN_MASK "xxxxx????xxxxxx????xxxxxxx????xx????xx????xxxx????xxxxxxxx????xxx"
//#define USERMESSAGEBEGIN_OFFSET 0x5
#define USERMESSAGEBEGIN_SIG "\x55\x8B\xEC\xFF\x75\x0C\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xD0\x83\xFA\xFF\x75\x0E\xFF\x75\x0C\x68\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x8B\x0D\x00\x00\x00\x00\x52\xFF\x75\x08\x8B\x01\xFF\x90\x00\x00\x00\x00\xA3\x00\x00\x00\x00\x5D\xC3"
#define USERMESSAGEBEGIN_MASK "xxxxxxxx????x????xxxxxxxxxxx????xx????xx????xxxxxxxx????x????xx"
#define USERMESSAGEBEGIN_OFFSET 0x8

class CLocalRecipientFilter
{
public:
	CLocalRecipientFilter();
	~CLocalRecipientFilter();

	void Reset();
	inline void MakeReliable();
	void AddRecipient(const CBasePlayer* pPlayer);
private:
	DWORD dwVTable;
	bool m_bReliable;
	bool m_bInitMessage;
	CUtlVector<int>	m_Recipients;
	bool m_bUsingPredictionRules;
	bool m_bIgnorePredictionCull;
};

void CLocalRecipientFilter::MakeReliable()
{
	m_bReliable = true;
}

typedef void (*UserMsgCallback_t)(bf_read&);

typedef struct {
	int size;
	const char* name;
	CUtlVector<UserMsgCallback_t> clienthooks;
} usermessage_t;

class CLocalUserMessages
{
public:
	CLocalUserMessages();
	~CLocalUserMessages();

	int LookupUserMessage(const char* name);
	inline usermessage_t* GetUserMessage(int idx);
	inline const char* GetUserMessageName(int idx);
	inline int GetTotal();
	inline bool IsValidIndex(int idx);
private:
	DWORD dwVTable;
	CUtlDict<usermessage_t*,int> m_UserMessages;
};

usermessage_t* CLocalUserMessages::GetUserMessage(int idx)
{
	if(idx>=0&&idx<GetTotal())
		return m_UserMessages[idx];
	return NULL;
}

int CLocalUserMessages::GetTotal()
{
	return m_UserMessages.Count();
}

const char* CLocalUserMessages::GetUserMessageName(int idx)
{
	if(idx>=0&&idx<GetTotal())
		return m_UserMessages.GetElementName(idx);
	return "<wrong idx>";
}

bool CLocalUserMessages::IsValidIndex(int idx)
{
	return m_UserMessages.IsValidIndex(idx);
}

DWORD FindPattern(HMODULE hDll, char* pattern, char* mask);
CBasePlayer* GetEntityByLuaEntity(CLuaState* L,int idx);

#endif