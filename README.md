# gmcl_usermessages_win32

SourceSDK usermessages in Lua!

## usermessages
usermessages.LookupUserMessage("name") --Lookup message id by name<br>
usermessages.GetUserMessages() --Return a table with registered usermessages<br>
usermessages.GetUserMessageName(idx) --Return usermessage name by id<br>
usermessages.GetTotal() --Return total number of usermessages<br>

## recipientfilter

Container that's contain players and other stuff

rf = recipientfilter.Create() --Returns new userdata<br>
rf:MakeReliable() --Don't forget to do it before you use it<br>
rf:AddRecipient(Entity(1)) --Add player to filter<br>
rf:Free() --When you done (usermessages.MessageEnd()) don't forget to free it!<br>

## bf_write (NekoBuf) and usermessages usage

bf = usermessages.UserMessageBegin(rf,usermessage_id) --Begin usermessage with our filter, returns bf_write userdata<br>

bf_write metamethods:<br>
WriteByte<br>
WriteChar<br>
WriteShort<br>
WriteWord<br>
WriteLong<br>
WriteFloat<br>
WriteAngle<br>
WriteAngles<br>
WriteCoord<br>
WriteVec3Coord<br>
WriteVec3Normal<br>
WriteString<br>
WriteEntity<br>
WriteEHandle<br>
WriteBool<br>
WriteUBitLong<br>
WriteSBitLong<br>
WriteBits<br>

Also for defination look in nekobuf.cpp<br>

When you done, YOU MUST END THIS! (or u get engine error and server crash)<br>
usermessages.MessageEnd()

## Warning

When you're using usermessages, be carefully! Any error in code stop script and MessageEnd wasn't called, this can crash your server

## Example
for LuaCmd also SendLua
```
rf = recipientfilter.Create()
rf:MakeReliable()
rf:AddRecipient(Entity(1))

bf = usermessages.UserMessageBegin(rf,usermessages.LookupUserMessage("LuaCmd"))
bf:WriteString("print(\"Hello World!\")")
usermessages.MessageEnd()
rf:Free()
```
