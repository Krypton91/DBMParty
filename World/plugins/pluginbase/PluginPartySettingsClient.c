class PluginPartySettingsClient extends PluginBase
{
    protected const static string  DIR = "$profile:DeutscheBohrmaschine";
    protected const static string  PATH = DIR + "\\ClientSettings.DBM";
    FileHandle  m_FileHandler;
    static int RangeOfMarker;
    static int TacticalPing;
    static int PartyNotify;
    static int MarkerOperacity;
    static int NameTagStyle;
    bool isInited = false;
    
    void PluginPartySettingsClient()
    {
        if(GetGame().IsClient() && !isInited)
        {
            if(!FileExist(DIR))
                MakeDirectory(DIR);
            if(FileExist(PATH))
            {
                ReadFile();
            }
            else
            {
                CreateFile();
            }
            isInited = true;
        }
    }
    void ~PluginPartySettingsClient()
    {
        //CloseFile(m_FileHandler);
        isInited = false;
    }
    void CreateFile()
    {
        //Print("DBMPARTY -> Creating new SettingsDatabase");
        m_FileHandler = OpenFile(PATH, FileMode.WRITE);
        if(m_FileHandler != 0)
        {
            FPrintln(m_FileHandler, "<RangeOfPartMarker> 0");
            FPrintln(m_FileHandler, "<TacticalPing> 0");
            FPrintln(m_FileHandler, "<PartyNotify> 0");
            FPrintln(m_FileHandler, "<MarkerOperacity> 1");
            FPrintln(m_FileHandler, "<NameTagStyle> 0");
            FPrintln(m_FileHandler, "<FileEnd>");
        }
        CloseFile(m_FileHandler);
    }
    void ReadFile()
    {
        //Print("[DBMPARTY] File Read Start.");

		FileHandle file_index = OpenFile(PATH, FileMode.READ);
		
		if ( file_index == 0 )
		{
			//Print("[DBMPARTY]-> FATAL ERROR -> UserSettings was not found!");
			return;
		}

		int variableCounter = 0;
		
		string line_content = "";
		while (variableCounter <= 100 && !line_content.Contains("<FileEnd>"))
		{
			bool validEntry = false;

			line_content = "";
			int char_count = FGets( file_index,  line_content );
			line_content = FileReadHelper.TrimComment(line_content);
			
			if (line_content.Contains("<RangeOfPartMarker>"))
			{
				line_content.Replace("<RangeOfPartMarker>", "");
				line_content = FileReadHelper.TrimComment(line_content);

				RangeOfMarker = line_content.ToInt();
				validEntry = true;
			}

			if (line_content.Contains("<TacticalPing>"))
			{
				line_content.Replace("<TacticalPing>", "");
				line_content = FileReadHelper.TrimComment(line_content);

				TacticalPing = line_content.ToInt();
				validEntry = true;
			}

			if (line_content.Contains("<PartyNotify>"))
			{
				line_content.Replace("<PartyNotify>", "");
				line_content = FileReadHelper.TrimComment(line_content);

				PartyNotify = line_content.ToInt();
				validEntry = true;
			}
            if (line_content.Contains("<PartyNotify>"))
			{
				line_content.Replace("<PartyNotify>", "");
				line_content = FileReadHelper.TrimComment(line_content);

				PartyNotify = line_content.ToInt();
				validEntry = true;
			}
            if (line_content.Contains("<MarkerOperacity>"))
			{
				line_content.Replace("<MarkerOperacity>", "");
				line_content = FileReadHelper.TrimComment(line_content);

				MarkerOperacity = line_content.ToInt();
				validEntry = true;
			}
            if (line_content.Contains("<NameTagStyle>"))
			{
				line_content.Replace("<NameTagStyle>", "");
				line_content = FileReadHelper.TrimComment(line_content);

				NameTagStyle = line_content.ToInt();
				validEntry = true;
			}
        }
		CloseFile(file_index);
	}
    /*
    void ReadFile()
    {
        //Print(":::::::DBM PartySystem DEBUG START:::::::");
        //Print("Reading Client settings....");
        string line_content = "";
        m_FileHandler = OpenFile(m_Settings_Path + "ClientSettings.DBM", FileMode.READ);
        FileReadHelper.SearchForNextTermInFile(m_FileHandler, "<IsOffline>", "<RangeOfPartMarker>");
        line_content.Replace("<IsOffline>", "");
		line_content = FileReadHelper.TrimComment(line_content);
        ClientIsOffline = line_content.ToInt();
        Print("IsOffline =:" + line_content.ToInt() + ":");

        line_content = "";
        //RangeOfMarker
        FileReadHelper.SearchForNextTermInFile(m_FileHandler, "<RangeOfPartMarker>", "<TacticalPing>");
        line_content.Replace("<RangeOfPartMarker>", "");
		line_content = FileReadHelper.TrimComment(line_content);
        RangeOfMarker = line_content.ToInt();
        Print("RangeOfPartMarker =:" + line_content.ToInt() + ":");

        line_content = "";
        //RangeOfMarker
        FileReadHelper.SearchForNextTermInFile(m_FileHandler, "<TacticalPing>", "");
        line_content.Replace("<TacticalPing>", "");
		line_content = FileReadHelper.TrimComment(line_content);
        TacticalPing = line_content.ToInt();
        //Print("TacticalPing =:" + TacticalPing + ":");

        line_content = "";
        //PartyNotify
        FileReadHelper.SearchForNextTermInFile(m_FileHandler, "<PartyNotify>", "");
        line_content.Replace("<PartyNotify>", "");
		line_content = FileReadHelper.TrimComment(line_content);
        PartyNotify = line_content.ToInt();
        //Print("PartyNotify =:" + PartyNotify + ":");
        //Print(":::::::DBM PartySystem DEBUG END:::::::");
        //Print("File sucessfully read:" + " IsOffline=" + ClientIsOffline + " RangeOfPartyMarker=" + RangeOfMarker + "TacticalPing=" + TacticalPing + "PartyNotify=" + PartyNotify);
        CloseFile(m_FileHandler);
    }
    */
    
    //This update the File to the current usere Settings.
    void UpdateFile(int dbm_RangeOfPartyMarker, int dbm_TacticalPing, int dbm_PartyNotify, int dbm_MarkerOperacity, int dbm_NameTagStyle)
    {
        Print("DBMPARTY -> File Update Called.");
        if(FileExist(PATH))
            DeleteFile(PATH);
        m_FileHandler = OpenFile(PATH, FileMode.WRITE);
        if(m_FileHandler != 0)
        {
            FPrintln(m_FileHandler, "<RangeOfPartMarker> " + dbm_RangeOfPartyMarker.ToString());
            FPrintln(m_FileHandler, "<TacticalPing> " + dbm_TacticalPing.ToString());
            FPrintln(m_FileHandler, "<PartyNotify> " + dbm_PartyNotify.ToString());
            FPrintln(m_FileHandler, "<MarkerOperacity> " + dbm_MarkerOperacity.ToString());
            FPrintln(m_FileHandler, "<NameTagStyle> " + dbm_NameTagStyle.ToString());
            FPrintln(m_FileHandler, "<FileEnd>");

            //Update vars
            RangeOfMarker = dbm_RangeOfPartyMarker;
            TacticalPing = dbm_TacticalPing;
            PartyNotify = dbm_PartyNotify;
            MarkerOperacity = dbm_MarkerOperacity;
            NameTagStyle = dbm_NameTagStyle;

        }
        CloseFile(m_FileHandler);
        //ReadFile();
    }
};