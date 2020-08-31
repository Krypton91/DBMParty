////////////////////////////////////////////////////////////////////
//DeRap: SchanaModParty\config.bin
//Produced from mikero's Dos Tools Dll version 7.64
//https://mikero.bytex.digital/Downloads
//'now' is Mon Aug 24 02:26:31 2020 : 'file' last modified on Sun Jul 19 06:28:53 2020
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class SchanaModParty
	{
		requiredAddons[] = {};
		units[] = {};
		weapons[] = {};
	};
};
class CfgMods
{
	class SchanaModParty
	{
		name = "Deutsche Bohrmaschine Party";
		action = "https://github.com/schana/dayz-mod-party";
		author = "schana";
		type = "mod";
		inputs = "SchanaModParty/Data/Inputs.xml";
		dependencies[] = {"Game","World", "Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"SchanaModParty/Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"SchanaModParty/World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"SchanaModParty/Mission"};
			};
		};
	};
};
