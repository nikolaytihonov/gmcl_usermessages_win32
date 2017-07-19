#include "nekobuf.h"

static int ud_type = 253;
static char* ud_name = "NekoBuf";

void NekoBuf_Create(CLuaState* L,bf_write* buf)
{
	userdata_t* ud = L->NewUserData();

	ud->data = buf;
	ud->type = ud_type;

	L->GetMetaFromRegistry(ud_name);
	L->SetMetaTable(-2);
}

bf_write* NekoBuf_Check(CLuaState* L,int idx)
{
	userdata_t* ud = L->GetUserData(idx);
	if(ud->type==ud_type)
		return (bf_write*)ud->data;
	return NULL;
}

void* MetaCheck(CLuaState* L,int idx,int type)
{
	userdata_t* ud = L->GetUserData(idx);
	if(ud->type==type)
		return ud->data;
	return NULL;
}

int NekoBuf_tostring(CLuaState* L)
{
	L->PushFString("%s: %p",ud_name,NekoBuf_Check(L,1));
	return 1;
}

int NekoBuf_eq(CLuaState* L)
{
	L->PushBool((NekoBuf_Check(L,1) == NekoBuf_Check(L,2)));
	return 1;
}

int NekoBuf_gc(CLuaState* L)
{
	return 0;
}

int NekoBuf_WriteByte(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteByte(L->GetInteger(2) & 0xFF);
	return 0;
}

int NekoBuf_WriteChar(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteChar(L->GetInteger(2) & 0xFF);
	return 0;
}

int NekoBuf_WriteShort(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteShort(L->GetInteger(2) & 0xFFFF);
	return 0;
}

int NekoBuf_WriteWord(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteWord(L->GetInteger(2));
	return 0;
}

int NekoBuf_WriteLong(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteLong(L->GetInteger(2));
	return 0;
}

int NekoBuf_WriteFloat(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteFloat((float)L->GetNumber(2));
	return 0;
}

int NekoBuf_WriteAngle(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	L->CheckType(3,NUMBER);
	buf->WriteBitAngle((float)L->GetNumber(2),L->GetInteger(3));
	return 0;
}

int NekoBuf_WriteAngles(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	QAngle* ang = (QAngle*)MetaCheck(L,2,11); //Angle
	if(ang)
		buf->WriteBitAngles((const QAngle&)ang);
	return 0;
}

int NekoBuf_WriteCoord(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	buf->WriteBitCoord((float)L->GetNumber(2));
	return 0;
}

int NekoBuf_WriteVec3Coord(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	Vector* vec = (Vector*)MetaCheck(L,2,12); //Vector
	if(vec)
		buf->WriteBitVec3Coord((const Vector&)vec);
	return 0;
}

int NekoBuf_WriteVec3Normal(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	Vector* vec = (Vector*)MetaCheck(L,2,12); //Vector
	if(vec)
		buf->WriteBitVec3Normal((const Vector&)vec);
	return 0;
}

int NekoBuf_WriteString(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,STRING);
	buf->WriteString(L->GetString(2));
	return 0;
}

int NekoBuf_WriteEHandle(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	CBasePlayer* pEntity = GetEntityByLuaEntity(L,2);
	CBaseHandle hEntity;
	long EncodedEHandle;

	if(pEntity)
	{
		hEntity = pEntity->GetRefEHandle();
		int iSerialNum = (hEntity.m_Index >> NUM_ENT_ENTRY_BITS)
			& ( (1 << NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS) - 1 );
		EncodedEHandle = (hEntity.m_Index & ENT_ENTRY_MASK) | (iSerialNum << MAX_EDICT_BITS);
	}
	else EncodedEHandle = INVALID_NETWORKED_EHANDLE_VALUE;

	buf->WriteLong(EncodedEHandle);
	return 0;
}

int NekoBuf_WriteBool(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,TBOOLEAN);
	buf->WriteOneBit(L->GetBool(2) ? 1 : 0);
	return 0;
}

int NekoBuf_WriteUBitLong(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	L->CheckType(3,NUMBER);
	buf->WriteUBitLong(L->GetInteger(2),L->GetInteger(3));
	return 0;
}

int NekoBuf_WriteSBitLong(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,NUMBER);
	L->CheckType(3,NUMBER);
	buf->WriteSBitLong(L->GetInteger(2),L->GetInteger(3));
	return 0;
}

int NekoBuf_WriteBits(CLuaState* L)
{
	bf_write* buf = NekoBuf_Check(L,1);
	L->CheckType(2,STRING);
	L->CheckType(3,NUMBER);
	buf->WriteBits(L->GetString(2),L->GetInteger(3));
	return 0;
}

#define REG_FUNC(n)					\
	L->PushCFunction(NekoBuf_##n);	\
	L->SetField(-2,#n)

void NekoBuf_Open(CLuaState* L)
{
	L->CreateMetaTable(ud_name);

	L->PushCFunction(NekoBuf_tostring);
	L->SetField(-2,"__tostring");

	L->PushCFunction(NekoBuf_eq);
	L->SetField(-2,"__eq");

	L->PushCFunction(NekoBuf_gc);
	L->SetField(-2,"__gc");

	REG_FUNC(WriteByte);
	REG_FUNC(WriteChar);
	REG_FUNC(WriteShort);
	REG_FUNC(WriteWord);
	REG_FUNC(WriteLong);
	REG_FUNC(WriteFloat);
	REG_FUNC(WriteAngle);
	REG_FUNC(WriteAngles);
	REG_FUNC(WriteCoord);
	REG_FUNC(WriteVec3Coord);
	REG_FUNC(WriteVec3Normal);
	REG_FUNC(WriteString);
	L->PushCFunction(NekoBuf_WriteShort);
	L->SetField(-2,"WriteEntity");
	REG_FUNC(WriteEHandle);
	REG_FUNC(WriteBool);
	REG_FUNC(WriteUBitLong);
	REG_FUNC(WriteSBitLong);
	REG_FUNC(WriteBits);

	L->Push(-1);
	L->SetField(-2,"__index");
}

void NekoBuf_Close(CLuaState* L)
{
}