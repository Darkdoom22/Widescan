#include <Windows.h>
#include <cstdint>

#include "Scanner.h"
#include "LuaCoreWrapper.h"

//struct internal to GlobalNowZone struct 
struct WidescanInfo
{
	uint32_t Status;
	float X;
	float Z;
	float Y;
	uint16_t Index;
	uint16_t Level;
	uint32_t _unk1;
	//theres another uint32_t here, not sure what it is, maybe related
};

const auto gcTrackingStartSetPattern = "\xa1\x00\x00\x00\x00\x85\xc0\x74\x00\x6a\x00\x6a\x00\x68\xf5\x00\x00\x00\xe8";
const auto gcTrackingStartSetMask = "x????xxx?xxxxxxxxxx";

const auto gcTrackingStopSetPattern = "\xa1\x00\x00\x00\x00\x85\xc0\x74\x00\x6a\x00\x6a\x00\x68\xf6\x00\x00\x00\xe8";
const auto gcTrackingStopSetMask = "x????xxx?xxxxxxxxxx";

const auto gcTrackingPosGetPattern = "\xa1\x00\x00\x00\x00\x85\xc0\x74\x00\xf7\x80\x00\x00\x00\x00\xff\xff\x00\x00";
const auto gcTrackingPosMask = "x????xxx?xx????xxxx";

using fnTrackingPosGet = WidescanInfo*(__stdcall*)();
using fnTrackingStartSet = bool(__cdecl*)(uint32_t);
using fnTrackingStopSet = bool(__cdecl*)();

//returns a pointer to current widescan info
const auto oTrackingPosGet = reinterpret_cast<fnTrackingPosGet>(FindPattern::ScanModIn(
	const_cast<char*>(gcTrackingPosGetPattern),
	const_cast<char*>(gcTrackingPosMask),
	"FFXiMain.dll"));

//outgoing 0xF5 packet handler
const auto oTrackingStartSet = reinterpret_cast<fnTrackingStartSet>(
	FindPattern::ScanModIn(
		const_cast<char*>(gcTrackingStartSetPattern), 
		const_cast<char*>(gcTrackingStartSetMask),
		"FFXiMain.dll"));

//outgoing 0xF6 packet handler
const auto oTrackingStopSet = reinterpret_cast<fnTrackingStopSet>(
	FindPattern::ScanModIn(
		const_cast<char*>(gcTrackingStopSetPattern),
		const_cast<char*>(gcTrackingStopSetMask),
		"FFXiMain.dll"));

static int GetWidescanInfo(lua_State* L)
{
	if (LuaCoreWrapper::oLua_GetTop(L) != 0)
	{
		return LuaCoreWrapper::oLuaL_Error(L, "GetWidescanInfo takes no arguments");
	}

	if (!oTrackingPosGet)
	{
		return LuaCoreWrapper::oLuaL_Error(L, "GetWidescanInfo failed to find function oTrackingPosGet, the signature may have changed");
	}

	const auto widescanInfo = oTrackingPosGet();//only returns a pointer when there is a valid track
	if (widescanInfo)
	{
		LuaCoreWrapper::oLua_NewTable(L);

		LuaCoreWrapper::oLua_PushNumber(L, widescanInfo->Status);
		LuaCoreWrapper::oLua_SetField(L, -2, "Status");

		LuaCoreWrapper::oLua_PushNumber(L, widescanInfo->X);
		LuaCoreWrapper::oLua_SetField(L, -2, "X");

		LuaCoreWrapper::oLua_PushNumber(L, widescanInfo->Z);
		LuaCoreWrapper::oLua_SetField(L, -2, "Z");

		LuaCoreWrapper::oLua_PushNumber(L, widescanInfo->Y);
		LuaCoreWrapper::oLua_SetField(L, -2, "Y");

		LuaCoreWrapper::oLua_PushNumber(L, widescanInfo->Index);
		LuaCoreWrapper::oLua_SetField(L, -2, "Index");

		LuaCoreWrapper::oLua_PushNumber(L, widescanInfo->Level);
		LuaCoreWrapper::oLua_SetField(L, -2, "Level");

		return 1;
	}

	return 0;
}

static int TrackingStartSet(lua_State* L)
{
	if(LuaCoreWrapper::oLua_GetTop(L) != 1)
	{
		return LuaCoreWrapper::oLuaL_Error(L, "TrackingStartSet takes only 1 argument");
	}

	if (!oTrackingStartSet)
	{
		return LuaCoreWrapper::oLuaL_Error(L, "TrackingStartSet failed to find function oTrackingStartSet, the signature may have changed");
	}

	const auto trackingStartSet = oTrackingStartSet(LuaCoreWrapper::oLuaL_CheckInteger(L, 1));
	LuaCoreWrapper::oLua_PushBoolean(L, trackingStartSet);
	return 1;
}

static int TrackingStopSet(lua_State* L)
{
	if(LuaCoreWrapper::oLua_GetTop(L) != 0)
	{
		return LuaCoreWrapper::oLuaL_Error(L, "TrackingStopSet takes no arguments");
	}

	if (!oTrackingStopSet)
	{
		return LuaCoreWrapper::oLuaL_Error(L, "TrackingStopSet failed to find function oTrackingStopSet, the signature may have changed");
	}

	const auto trackingStopSet = oTrackingStopSet();
	LuaCoreWrapper::oLua_PushBoolean(L, trackingStopSet);
	return 1;
}

extern "C" int __declspec(dllexport) luaopen_Widescan(lua_State* L)
{
	constexpr struct luaL_Reg funcs[] = {

		{"TrackingStartSet", TrackingStartSet},
		{"TrackingStopSet", TrackingStopSet},
		{"GetWidescanInfo", GetWidescanInfo},
		{nullptr, nullptr},
	};

	LuaCoreWrapper::oLuaL_Register(L, "Widescan", funcs);
	return 1;
}



