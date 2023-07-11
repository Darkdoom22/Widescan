// ReSharper disable CppClangTidyClangDiagnosticCastFunctionType
// ReSharper disable CppInconsistentNaming

#pragma once
#include <Windows.h>
#include <lua.h>
#include "lauxlib.h"

namespace LuaCoreWrapper
{
	const HMODULE LuaCore = GetModuleHandleW(L"LuaCore.dll");

	using fnLuaL_Register = void(__cdecl*)(lua_State*, const char* libName, const luaL_Reg*);
	const fnLuaL_Register oLuaL_Register = reinterpret_cast<fnLuaL_Register>(
		GetProcAddress(LuaCore, "luaL_register"));

	using fnLua_PushNumber = void(__cdecl*)(lua_State*, lua_Number);
	const fnLua_PushNumber oLua_PushNumber = reinterpret_cast<fnLua_PushNumber>(
		GetProcAddress(LuaCore, "lua_pushnumber"));

	using fnLuaL_PushBoolean = void(__cdecl*)(lua_State*, int);
	const fnLuaL_PushBoolean oLua_PushBoolean = reinterpret_cast<fnLuaL_PushBoolean>(
		GetProcAddress(LuaCore, "lua_pushboolean"));

	using fnLuaL_CheckInteger = lua_Integer(__cdecl*)(lua_State*, int);
	const fnLuaL_CheckInteger oLuaL_CheckInteger = reinterpret_cast<fnLuaL_CheckInteger>(
		GetProcAddress(LuaCore, "luaL_checkinteger"));

	using fnLuaL_CheckNumber = lua_Number(__cdecl*)(lua_State*, int);
	const fnLuaL_CheckNumber oLuaL_CheckNumber = reinterpret_cast<fnLuaL_CheckNumber>(
		GetProcAddress(LuaCore, "luaL_checknumber"));

	using fnLuaL_CheckLString = const char* (__cdecl*)(lua_State*, int, size_t*);
	const fnLuaL_CheckLString oLuaL_CheckLString = reinterpret_cast<fnLuaL_CheckLString>(
		GetProcAddress(LuaCore, "luaL_checklstring"));

	using fnLua_SetTop = void(__cdecl*)(lua_State*, int);
	const fnLua_SetTop oLua_SetTop = reinterpret_cast<fnLua_SetTop>(
		GetProcAddress(LuaCore, "lua_settop"));

	using fnLua_GetField = void(__cdecl*)(lua_State*, int, const char*);
	const fnLua_GetField oLua_GetField = reinterpret_cast<fnLua_GetField>(
		GetProcAddress(LuaCore, "lua_getfield"));

	using fnLua_CreateTable = void(__cdecl*)(lua_State*, int, int);
	const fnLua_CreateTable oLua_CreateTable = reinterpret_cast<fnLua_CreateTable>(
		GetProcAddress(LuaCore, "lua_createtable"));

	using fnLua_GetTop = int(__cdecl*)(lua_State*);
	const fnLua_GetTop oLua_GetTop = reinterpret_cast<fnLua_GetTop>(
		GetProcAddress(LuaCore, "lua_gettop"));

	using fnLua_SetField = void(__cdecl*)(lua_State*, int, const char*);
	const fnLua_SetField oLua_SetField = reinterpret_cast<fnLua_SetField>(
		GetProcAddress(LuaCore, "lua_setfield"));

	using fnLua_RawSetI = void(__cdecl*)(lua_State*, int, int);
	const fnLua_RawSetI oLua_RawSetI = reinterpret_cast<fnLua_RawSetI>(
		GetProcAddress(LuaCore, "lua_rawseti"));

	using fnLuaL_LoadFile = int(__cdecl*)(lua_State*, const char*);
	const fnLuaL_LoadFile oLuaL_LoadFile = reinterpret_cast<fnLuaL_LoadFile>(
		GetProcAddress(LuaCore, "luaL_loadfile"));

	using fnLua_PCall = int(__cdecl*)(lua_State*, int, int, int);
	const fnLua_PCall oLua_PCall = reinterpret_cast<fnLua_PCall>(
		GetProcAddress(LuaCore, "lua_pcall"));

	using fnLua_SetGlobal = void(__cdecl*)(lua_State*, const char*);
	const fnLua_SetGlobal oLua_SetGlobal = reinterpret_cast<fnLua_SetGlobal>(
		GetProcAddress(LuaCore, "lua_setglobal"));

	using fnLua_NewMetaTable = int(__cdecl*)(lua_State*, const char*);
	const fnLua_NewMetaTable oLuaL_NewMetaTable = reinterpret_cast<fnLua_NewMetaTable>(
		GetProcAddress(LuaCore, "luaL_newmetatable"));

	using fnLua_SetMetaTable = void(__cdecl*)(lua_State*, int);
	const fnLua_SetMetaTable oLua_SetMetaTable = reinterpret_cast<fnLua_SetMetaTable>(
		GetProcAddress(LuaCore, "lua_setmetatable"));

	using fnLua_PushValue = void(__cdecl*)(lua_State*, int);
	const fnLua_PushValue oLua_PushValue = reinterpret_cast<fnLua_PushValue>(
		GetProcAddress(LuaCore, "lua_pushvalue"));

	using fnLuaL_Error = int(__cdecl*)(lua_State*, const char*, ...);
	const fnLuaL_Error oLuaL_Error = reinterpret_cast<fnLuaL_Error>(
		GetProcAddress(LuaCore, "luaL_error"));

#define oLua_NewTable(L) oLua_CreateTable(L, 0, 0)
#define oLua_Pop(L, n) oLua_SetTop(L, -(n)-1)
}