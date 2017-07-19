#ifndef __NEKOBUF_H
#define __NEKOBUF_H

#define GAME_DLL
#include "lua.h"
#include "sdk.h"
#include "state.h"
#include "cbase.h"
#include "bitbuf.h"

void NekoBuf_Open(CLuaState* L);
void NekoBuf_Close(CLuaState* L);

void NekoBuf_Create(CLuaState* L,bf_write* buf);

#endif