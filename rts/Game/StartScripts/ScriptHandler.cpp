// ScriptHandler.cpp: implementation of the CScriptHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "mmgr.h"

#include "ScriptHandler.h"
#include "Game/Game.h"
#include "FileSystem/FileHandler.h"
#include "FileSystem/VFSHandler.h"
#include "LoadScript.h"
#include "CommanderScript.h"
#include "CommanderScript2.h"
#include "AirScript.h"
#include "SkirmishAITestScript.h"
#include "SpawnScript.h"
#include "EmptyScript.h"
#include "TestScript.h"
#include "Platform/SharedLib.h"
#include "ExternalAI/Interface/aidefines.h"
#include "ExternalAI/IAILibraryManager.h"
#include "ExternalAI/SkirmishAIKey.h"
#include "ExternalAI/SkirmishAIData.h"
#include "Exceptions.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CScriptHandler::CScriptHandler() : chosenScript(0)
{
}

/** Load all scripts. */
void CScriptHandler::LoadScripts() {

	loaded_scripts.push_back( new CCommanderScript() );
	loaded_scripts.push_back( new CCommanderScript2() );
	loaded_scripts.push_back( new CAirScript() );
	loaded_scripts.push_back( new CEmptyScript() );
	loaded_scripts.push_back( new CSpawnScript(false) );
	loaded_scripts.push_back( new CSpawnScript(true) );
	loaded_scripts.push_back( new CTestScript() );

	// add the C interface Skirmish AIs
	IAILibraryManager::T_skirmishAIKeys::const_iterator ai, e;
	const IAILibraryManager::T_skirmishAIKeys& skirmishAIKeys = aiLibManager->GetSkirmishAIKeys();
	for(ai=skirmishAIKeys.begin(), e=skirmishAIKeys.end(); ai != e; ++ai) {
		SkirmishAIData aiData;
		aiData.shortName = ai->GetShortName();
		aiData.version   = ai->GetVersion();
		aiData.isLuaAI   = false;
		loaded_scripts.push_back(new CSkirmishAITestScript(aiData));
	}
	// Lua AIs can not be added, as the selection would get invalid when
	// selecting an other mod.

	std::vector<std::string> f = CFileHandler::FindFiles("Saves/", "*.ssf");
	for(std::vector<std::string>::iterator fi = f.begin(), e = f.end(); fi != e; ++fi) {
		loaded_scripts.push_back(new CLoadScript(*fi));
	}
}

/** @return a reference to the only CScriptHandler instance */
CScriptHandler& CScriptHandler::Instance()
{
	static bool created = false;
	static CScriptHandler instance;
	if( !created ) {
		created = true;
		instance.LoadScripts();
	}
	return instance;
}

CScriptHandler::~CScriptHandler()
{
	while(!loaded_scripts.empty()) {
		delete loaded_scripts.back();
		loaded_scripts.pop_back();
	}
}

/** Called by the CScript constructors to add themselves to the CScriptHandler. */
void CScriptHandler::AddScript(std::string name, CScript *s)
{
	scripts[name] = s;
}

std::list<std::string> CScriptHandler::ScriptList()
{
	std::list<std::string> buf;
	for (std::map<std::string,CScript*>::iterator it = scripts.begin(); it != scripts.end(); ++it)
	{
		buf.push_back(it->first);
	}
	return buf;
}

/** Called by the CglList generated by CScriptHandler::GenList on selection of a script. */
void CScriptHandler::SelectScript(std::string s)
{
	CScriptHandler::Instance().chosenName = s;
	if (CScriptHandler::Instance().scripts.find(s) == CScriptHandler::Instance().scripts.end()) {
		throw std::runtime_error("script not found: " + s);
	}
	CScriptHandler::Instance().chosenScript = CScriptHandler::Instance().scripts[s];
	CScriptHandler::Instance().chosenScript->ScriptSelected();
}

