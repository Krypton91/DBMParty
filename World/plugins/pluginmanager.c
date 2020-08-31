modded class PluginManager
{	
	override void Init()
	{	
		//----------------------------------------------------------------------
		// Register modules
		//----------------------------------------------------------------------
		//				Module Class Name 						Client	Server
		//----------------------------------------------------------------------
		RegisterPlugin( "PluginPartySettingsClient",						true, 	false );
		super.Init();
	}
}
