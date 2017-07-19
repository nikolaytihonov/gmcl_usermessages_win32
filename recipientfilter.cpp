#include "recipientfilter.h"

static int ud_type = 254; //RECIPIENTFILTER
static char* ud_name = "RecipientFilter";

int RecipientFilter_Create(CLuaState* L)
{
	userdata_t* ud = L->NewUserData();

	ud->data = new CLocalRecipientFilter;
	ud->type = ud_type;

	L->GetMetaFromRegistry(ud_name);
	L->SetMetaTable(-2);

	return 1;
}

CLocalRecipientFilter* RecipientFilter_Check(CLuaState* L,int idx)
{
	userdata_t* ud = L->GetUserData(idx);
	if(ud->type==ud_type)
		return (CLocalRecipientFilter*)ud->data;
	return NULL;
}

int RecipientFilter_tostring(CLuaState* L)
{
	L->PushFString("%s: %p",ud_name,RecipientFilter_Check(L,1));
	return 1;
}

int RecipientFilter_eq(CLuaState* L)
{
	L->PushBool((RecipientFilter_Check(L,1) == RecipientFilter_Check(L,2)));
	return 1;
}

int RecipientFilter_gc(CLuaState* L)
{
	CLocalRecipientFilter* v1;
	if((v1 = RecipientFilter_Check(L,1)))
		delete v1;
	return 0;
}

int RecipientFilter_MakeReliable(CLuaState* L)
{
	RecipientFilter_Check(L,1)->MakeReliable();
	return 0;
}

int RecipientFilter_AddRecipient(CLuaState* L)
{
	CBasePlayer* pPlayer = GetEntityByLuaEntity(L,2);
	if(pPlayer)
		RecipientFilter_Check(L,1)->AddRecipient(pPlayer);
	return 0;
}

int RecipientFilter_Reset(CLuaState* L)
{
	RecipientFilter_Check(L,1)->Reset();
	return 0;
}

int RecipientFilter_Free(CLuaState* L)
{
	userdata_t* ud = L->GetUserData(1);
	if(ud->data)
	{
		delete (CLocalRecipientFilter*)ud->data;
		ud->data = NULL;
	}
	return 0;
}

void RecipientFilter_Open(CLuaState* L)
{
	L->CreateTable();
	L->PushCFunction(RecipientFilter_Create);
	L->SetField(-2,"Create");
	L->SetGlobal("recipientfilter");

	L->CreateMetaTable(ud_name);

	L->PushCFunction(RecipientFilter_tostring);
	L->SetField(-2,"__tostring");

	L->PushCFunction(RecipientFilter_eq);
	L->SetField(-2,"__eq");

	L->PushCFunction(RecipientFilter_gc);
	L->SetField(-2,"__gc");
	
	L->PushCFunction(RecipientFilter_MakeReliable);
	L->SetField(-2,"MakeReliable");

	L->PushCFunction(RecipientFilter_AddRecipient);
	L->SetField(-2,"AddRecipient");

	L->PushCFunction(RecipientFilter_Reset);
	L->SetField(-2,"Reset");

	L->PushCFunction(RecipientFilter_Free);
	L->SetField(-2,"Free");

	L->Push(-1);
	L->SetField(-2,"__index");
}

void RecipientFilter_Close(CLuaState* L)
{
	L->PushNil();
	L->SetGlobal("recipientfilter");
}