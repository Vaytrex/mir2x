/*
 * =====================================================================================
 *
 *       Filename: serverluamodule.cpp
 *        Created: 12/19/2017 20:16:03
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include "monoserver.hpp"
#include "serverluamodule.hpp"

ServerLuaModule::ServerLuaModule()
    : LuaModule()
{
    m_LuaState.set_function("getTime", []() -> int
    {
        extern MonoServer *g_MonoServer;
        return (int)(g_MonoServer->getCurrTick());
    });

    m_LuaState.script(R"#(math.randomseed(getTime()))#");
}

void ServerLuaModule::addLog(int nLogType, const char *szLogInfo)
{
    if(!szLogInfo){
        szLogInfo = "";
    }

    // any time if you call addLog() in LUA
    // then this will get printed in the server GUI console

    extern MonoServer *g_MonoServer;
    switch(nLogType){
        case 0  : g_MonoServer->addLog(LOGTYPE_INFO   , "%s", szLogInfo); return;
        case 1  : g_MonoServer->addLog(LOGTYPE_WARNING, "%s", szLogInfo); return;
        case 2  : g_MonoServer->addLog(LOGTYPE_FATAL  , "%s", szLogInfo); return;
        default : g_MonoServer->addLog(LOGTYPE_DEBUG  , "%s", szLogInfo); return;
    }
}
