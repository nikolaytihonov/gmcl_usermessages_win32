#define GAME_DLL
#include <Windows.h>
#include "cbase.h"
#include "interface.h"
#include "state.h"
#include "lua.h"
#include "sdk.h"
#include "recipientfilter.h"
#include "nekobuf.h"

IVEngineServer* g_pEngine = NULL;
CLocalUserMessages* g_pLocalUserMessages;
DWORD dwRecipientVTable;

int LookupUserMessage(CLuaState* L)
{
	L->CheckType(1,STRING);
	L->PushInteger(g_pLocalUserMessages->LookupUserMessage(L->GetString(1)));
	return 1;
}

int GetUserMessages(CLuaState* L)
{
	usermessage_t* umsg;

	L->CreateTable();
	for(int i = 0; i < g_pLocalUserMessages->GetTotal(); i++)
	{
		umsg = g_pLocalUserMessages->GetUserMessage(i);
		if(!umsg || !g_pLocalUserMessages->IsValidIndex(i))
			continue;
		L->PushInteger(i);
		L->CreateTable();

		L->PushString(g_pLocalUserMessages->GetUserMessageName(i));
		L->SetField(-2,"name");

		L->PushInteger(umsg->size);
		L->SetField(-2,"size");

		L->SetTable(-3);
	}
	return 1;
}

int GetUserMessageName(CLuaState* L)
{
	L->CheckType(1,NUMBER);
	L->PushString(g_pLocalUserMessages->GetUserMessageName(L->GetInteger(1)));
	return 1;
}

int GetTotal(CLuaState* L)
{
	L->PushInteger(g_pLocalUserMessages->GetTotal());
	return 1;
}

int UserMessageBegin(CLuaState* L)
{
	CLocalRecipientFilter* filter = RecipientFilter_Check(L,1);
	bf_write* buf;
	L->CheckType(2,NUMBER);
	if(!filter) L->PushNil();
	else
	{
		buf = g_pEngine->UserMessageBegin((IRecipientFilter*)filter,L->GetInteger(2));
		NekoBuf_Create(L,buf);
	}
	return 1;
}

int MessageEnd(CLuaState* L)
{
	g_pEngine->MessageEnd();
	return 0;
}

GMOD_MODULE_OPEN()
{
	HMODULE hServer,hEngine;
	CreateInterfaceFn EngineFactory;
	if(!(hServer = GetModuleHandle("server.dll"))
		|| !(hEngine = GetModuleHandle("engine.dll")))
		return 1;
	if(!(EngineFactory = (CreateInterfaceFn)GetProcAddress(hEngine,"CreateInterface")))
		return 2;
	if(!(g_pEngine = (IVEngineServer*)EngineFactory("VEngineServer021",NULL)))
		return 3;

	DWORD dwAwardAchievement = FindPattern(hServer,AWARDACHIEVEMENT_SIG,
		AWARDACHIEVEMENT_MASK);
	dwRecipientVTable = *(PDWORD)((char*)dwAwardAchievement+AWARDACHIEVEMENT_OFFSET);

	Msg("dwAwardAchievement %p dwRecipientVTable %p\n",dwAwardAchievement,dwRecipientVTable);
	
	DWORD dwUserMessageBegin = FindPattern(hServer,USERMESSAGEBEGIN_SIG,
		USERMESSAGEBEGIN_MASK);
	g_pLocalUserMessages = *(CLocalUserMessages**)((char*)dwUserMessageBegin+USERMESSAGEBEGIN_OFFSET);

	Msg("dwUserMessageBegin %p g_pLocalUserMessages %p\n",dwUserMessageBegin,g_pLocalUserMessages);

	L->CreateTable();
	L->PushCFunction(LookupUserMessage);
	L->SetField(-2,"LookupUserMessage");

	L->PushCFunction(GetUserMessages);
	L->SetField(-2,"GetUserMessages");

	L->PushCFunction(GetUserMessageName);
	L->SetField(-2,"GetUserMessageName");

	L->PushCFunction(GetTotal);
	L->SetField(-2,"GetTotal");

	L->PushCFunction(UserMessageBegin);
	L->SetField(-2,"UserMessageBegin");

	L->PushCFunction(MessageEnd);
	L->SetField(-2,"MessageEnd");

	L->SetGlobal("usermessages");

	RecipientFilter_Open(L);
	NekoBuf_Open(L);
	return 0;
}

GMOD_MODULE_CLOSE()
{
	L->PushNil();
	L->SetGlobal("usermessages");
	RecipientFilter_Close(L);
	NekoBuf_Close(L);
	return 0;
}