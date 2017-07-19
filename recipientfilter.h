#ifndef __RECIPIENTFILTER_H
#define __RECIPIENTFILTER_H

#include "state.h"
#include "lua.h"
#include "sdk.h"

void RecipientFilter_Open(CLuaState* L);
void RecipientFilter_Close(CLuaState* L);

int RecipientFilter_Create(CLuaState* L);
CLocalRecipientFilter* RecipientFilter_Check(CLuaState* L,int idx);

#endif