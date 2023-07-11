#include <Windows.h>
#include <cstdint>

#include "Scanner.h"
#include "LuaCoreWrapper.h"

const auto gcTrackingStartSetPattern = "\xa1\x00\x00\x00\x00\x85\xc0\x74\x00\x6a\x00\x6a\x00\x68\xf5\x00\x00\x00\xe8";
const auto gcTrackingStartSetMask = "x????xxx?xxxxxxxxxx";

const auto gcTrackingStopSetPattern = "\xa1\x00\x00\x00\x00\x85\xc0\x74\x00\x6a\x00\x6a\x00\x68\xf6\x00\x00\x00\xe8";
const auto gcTrackingStopSetMask = "x????xxx?xxxxxxxxxx";

const auto pGlobalNowZonePattern = "\x8D\x14\x80\x8D\x04\x50\x8B\x15\x00\x00\x00\x00\x8D\xBC\x82\x60\x98";
const auto pGlobalNowZoneMask = "xxxxxxxx????xxxxx";
constexpr auto pGlobalNowZoneOffset = 0x8;

const auto WidescanInfoOffsetPattern = "\xb0\x01\x66\x89\xB2\x00\x00\x00\x00\x5e\xc3";
const auto WidescanInfoOffsetMask = "xxxxx????xx";

const auto WidescanInfoOffset = *reinterpret_cast<int32_t*>(FindPattern::ScanModIn(
	const_cast<char*>(WidescanInfoOffsetPattern),
	const_cast<char*>(WidescanInfoOffsetMask),
	"FFXiMain.dll") + 5);

using fnTrackingStartSet = bool(__cdecl*)(uint32_t);
using fnTrackingStopSet = bool(__cdecl*)();

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

auto pGlobalNowZone = reinterpret_cast<uint32_t>(FindPattern::ScanModIn(
	const_cast<char*>(pGlobalNowZonePattern),
	const_cast<char*>(pGlobalNowZoneMask),
	"FFXiMain.dll") + pGlobalNowZoneOffset);

//struct internal to GlobalNowZone struct 
struct WidescanInfo
{
	uint32_t Status;
	float X;
	float Z;
	float Y;
	uint16_t Index;
	uint16_t Level;
	//theres another uint32_t here, not sure what it is, maybe related
};

static int GetWidescanInfo(lua_State* L)
{
	if(pGlobalNowZone)
	{
		const auto pNowZone = *reinterpret_cast<uintptr_t*>(pGlobalNowZone);
		if(pNowZone)
		{
			const auto nowZone = *reinterpret_cast<uintptr_t*>(pNowZone);
			const auto widescanInfo = reinterpret_cast<WidescanInfo*>((nowZone + WidescanInfoOffset) - 0x10);

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

	}
	return 0;
}

static int TrackingStartSet(lua_State* L)
{
	if (!oTrackingStartSet)
	{
		return 0;
	}

	const auto trackingStartSet = oTrackingStartSet(LuaCoreWrapper::oLuaL_CheckInteger(L, 1));
	LuaCoreWrapper::oLua_PushBoolean(L, trackingStartSet);
	return 1;
}

static int TrackingStopSet(lua_State* L)
{
	if (!oTrackingStopSet)
	{
		return 0;
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



