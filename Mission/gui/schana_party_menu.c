class SchanaPartyMenu extends UIScriptedMenu {
	private bool m_SchanaPartyIsInitialized;
	private bool m_SchanaPartyMenuIsOpen;
	private TextListboxWidget m_SchanaPartyPlayerList;
	private TextListboxWidget m_SchanaPartyPartyList;
	private ButtonWidget m_SchanaPartyButtonAdd;
	private ButtonWidget m_SchanaPartyButtonRemove;
	private EditBoxWidget m_playersearchBox;
	private EditBoxWidget m_RangeOfMarker;
	private ButtonWidget m_SetBTN;
	int m_offlineModeValue;
	int m_RangeOfPartyMarkerValue;
	int m_DisablePingValue;
	int m_PartyNotifyValue;
	int m_markerOperacityValue;
	int m_nameTagStyleValue;
	//CheckBoxen
	private CheckBoxWidget m_OfflineMode;
	private CheckBoxWidget m_PartyNotify;
	private CheckBoxWidget m_DisablePing;

	//SLide Wiget
	private SliderWidget m_markerOperacity;
	private XComboBoxWidget m_nameTagStyle;


	//SearchFunction:
	const float m_FilterUpdate = 0.35;
	float m_FilterUpdateCurTick;
	private string 	m_searchFilter;
	bool isSearching;
	bool shouldUpdateSettings;

	private ref map<string, string> member_sorting_map;

	void SchanaPartyMenu () {
		member_sorting_map = new map<string, string> ();
	}

	override Widget Init () 
	{
		if (!m_SchanaPartyIsInitialized) 
		{
			layoutRoot = GetGame ().GetWorkspace ().CreateWidgets ("SchanaModParty/GUI/Layouts/DBM.layout");
			m_SchanaPartyPlayerList = TextListboxWidget.Cast (layoutRoot.FindAnyWidget ("Players"));
			m_SchanaPartyPartyList = TextListboxWidget.Cast (layoutRoot.FindAnyWidget ("Party"));
			m_SchanaPartyButtonAdd = ButtonWidget.Cast (layoutRoot.FindAnyWidget ("ButtonAdd"));
			m_SchanaPartyButtonRemove = ButtonWidget.Cast (layoutRoot.FindAnyWidget ("ButtonRemove"));
			m_playersearchBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("EditSearch"));
			m_RangeOfMarker = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("RangeOfMarkersystem"));
			m_PartyNotify = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("PartyNotify"));
			m_DisablePing = CheckBoxWidget.Cast(layoutRoot.FindAnyWidget("DisablePing"));
			m_SetBTN = ButtonWidget.Cast(layoutRoot.FindAnyWidget("ButtonSet"));
			m_markerOperacity = SliderWidget.Cast(layoutRoot.FindAnyWidget("SliderWidget0"));
			m_nameTagStyle = XComboBoxWidget.Cast(layoutRoot.FindAnyWidget("SelectNametagStyle"));

			m_SchanaPartyIsInitialized = true;
		}

		SchanaPartyUpdateLists();
		return layoutRoot;
	}
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		if(w == m_markerOperacity)
		{
			if(finished)
			{
				CheckSettings();
			}
			return true;
		}
		if(w == m_nameTagStyle)
		{
			if(finished)
			{
				CheckSettings();
			}
			return true;
		}
		if(w == m_playersearchBox)
		{
			//isSearching = true;
			//if(finished)
			//{
				SearchPlayer();
				//PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
				//player.MessageStatus("Event Search triggert!");
			//}
			return true;
		}
		return false;
	}
	void ~SchanaPartyMenu () {
		GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.SchanaPartyUpdateLists);
		GetGame ().GetUIManager ().Back ();
		g_Game.GetUIManager ().ShowCursor (true);
		g_Game.GetUIManager ().ShowUICursor (false);
		GetGame ().GetInput ().ResetGameFocus ();
		isSearching = false;
		if (layoutRoot) {
			layoutRoot.Unlink ();
		}
	}

	override void OnShow () {
		super.OnShow ();
		LoadSettings();
		shouldUpdateSettings = true;
		GetGame ().GetCallQueue (CALL_CATEGORY_GUI).CallLater (this.SchanaPartyUpdateLists, 500, true);
	}

	override void OnHide () {
		super.OnHide ();
		GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.SchanaPartyUpdateLists);
		shouldUpdateSettings = false;
		g_Game.GetUIManager ().ShowCursor (true);
		g_Game.GetUIManager ().ShowUICursor (false);
		GetGame ().GetInput ().ResetGameFocus ();
	}

	override bool OnClick (Widget w, int x, int y, int button) 
	{
		int selectedRow;
		Param1<string> id;
		switch (w) 
		{
			case m_SchanaPartyButtonAdd:
				selectedRow = m_SchanaPartyPlayerList.GetSelectedRow ();
				if (selectedRow == -1) {
					break;
				}
				m_SchanaPartyPlayerList.GetItemData (selectedRow, 0, id);
				SchanaPartyUtils.LogMessage ("Menu add " + id.param1);
				GetSchanaPartyManagerClient ().AddPlayerToParty (id.param1);
				m_playersearchBox.SetText("");
				SchanaPartyUpdateLists ();
				break;

			case m_SchanaPartyButtonRemove:
				selectedRow = m_SchanaPartyPartyList.GetSelectedRow ();
				if (selectedRow == -1) {
					break;
				}
				m_SchanaPartyPartyList.GetItemData (selectedRow, 0, id);
				SchanaPartyUtils.LogMessage ("Menu remove " + id.param1);
				GetSchanaPartyManagerClient ().RemovePlayerFromParty (id.param1);
				SchanaPartyUpdateLists ();
				m_SchanaPartyPartyList.SelectRow (selectedRow - 1);
				break;
			case m_PartyNotify:
				PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
				player.MessageStatus("Cooming soon! :) PartySystem V2 by krypton91");
				break;
			case m_DisablePing:
				break;	
			case m_SetBTN:
				break;
		}
		//Print("DEBUG:::: Class schanaPartyMenu.c INT VALUES: RangeofMarker: " + m_RangeOfPartyMarkerValue + "Offline Mode Value =  " + m_offlineModeValue + "Diable ping = " + m_DisablePingValue);
		//Upate the DBM Settings file.
		CheckSettings();
		return super.OnClick (w, x, y, button);
	}

	void CheckSettings()
	{
		if(m_PartyNotify.IsChecked())
		{
			m_PartyNotifyValue = 1;
		}
		else
		{
			m_PartyNotifyValue = 0;
		}
		if(m_DisablePing.IsChecked())
		{
			m_DisablePingValue = 1;
		}
		else
		{
			m_DisablePingValue = 0;
		}
		if(IsNumberOnly(m_RangeOfMarker.GetText()))
		{
						//executen
			m_RangeOfPartyMarkerValue = m_RangeOfMarker.GetText().ToInt();
			//PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			//Function Handled by DBM Notify
			//player.DBMNotify("Test", "Test1","This is the Message field.","SchanaModParty/GUI/Layouts/DBMLOgo48.edds", 20);
		}
		else
		{
			m_RangeOfMarker.SetText("");
		}
		//float DayzEngineIsGay = m_markerOperacity.GetCurrent();

		m_markerOperacityValue = m_markerOperacity.GetCurrent();
		m_nameTagStyleValue = m_nameTagStyle.GetCurrentItem();
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		SchanaPartyNametagsMenu.CheckSettings();

		//After Set All Update..
		PluginPartySettingsClient().UpdateFile(m_RangeOfPartyMarkerValue, m_DisablePingValue, m_PartyNotifyValue, m_markerOperacityValue, m_nameTagStyleValue);

	}
	bool IsNumberOnly(string index)
	{
		if(index.Contains("1") || index.Contains("2") || index.Contains("3") || index.Contains("4") || index.Contains("5") || index.Contains("6") || index.Contains("7") || index.Contains("8") || index.Contains("9") || index.Contains("0"))
			return true;
		return false;	
	}

	void LoadSettings()
    {
		PluginPartySettingsClient().ReadFile();

		PluginPartySettingsClient settings = new PluginPartySettingsClient;
		if(settings.TacticalPing == 1)
			m_DisablePing.SetChecked(true);
		else
			m_DisablePing.SetChecked(false);
		
		if(settings.PartyNotify == 1)
			m_PartyNotify.SetChecked(true);
		else
			m_PartyNotify.SetChecked(false);

		//Range
		int range = settings.RangeOfMarker;
		if(range != 0)
			m_RangeOfMarker.SetText(range.ToString());
		
		m_markerOperacity.SetCurrent(settings.MarkerOperacity);
		m_nameTagStyle.SetCurrentItem(settings.NameTagStyle);
		//Print("Settings to load values: " + "TacticalPing system: " + settings.TacticalPing);
		//Print("Settings to load values: " + "PartyNotify: " + settings.PartyNotify);
		//Print("Settings to load values: " + "RangeOfMarker: " + settings.RangeOfMarker);
    }
	void SchanaPartyUpdateLists () 
	{
		if (m_SchanaPartyIsInitialized) 
		{
			SchanaPartyUpdatePartyList ();
			SchanaPartyUpdatePlayerList ();
			SchanaPartyUpdatePartyStatus ();
		}
	}

	void SchanaPartyUpdatePartyList () 
	{
		member_sorting_map.Clear ();
		string member_name;
		int insert_row = 0;

		auto members = GetSchanaPartySettings ().GetMembers ();
		foreach (auto id : members) {
			member_name = GetSchanaPartySettings ().GetName (id);
			member_name.ToLower ();
			member_sorting_map.Insert (member_name + id, id);
		}

		auto sorted_keys = member_sorting_map.GetKeyArray ();
		sorted_keys.Sort ();
		foreach (auto sorted_key : sorted_keys) {
			string member_id = member_sorting_map.Get (sorted_key);
			member_name = GetSchanaPartySettings ().GetName (member_id);
			if (insert_row < m_SchanaPartyPartyList.GetNumItems ()) {
				m_SchanaPartyPartyList.SetItem (insert_row, member_name, new Param1<string> (member_id), 0);
				
			} else {
				m_SchanaPartyPartyList.AddItem (member_name, new Param1<string> (member_id), 0);
			}
			insert_row++;
		}

		while (insert_row < m_SchanaPartyPartyList.GetNumItems ()) {
			m_SchanaPartyPartyList.SelectRow (-1);
			m_SchanaPartyPartyList.RemoveRow (insert_row);
		}

		if (m_SchanaPartyPartyList.GetSelectedRow () >= insert_row) {
			m_SchanaPartyPartyList.SelectRow (insert_row - 1);
		}
	}
	void SearchPlayer()
	{
		string searchText = m_playersearchBox.GetText();
		if(searchText == ""){
			SchanaPartyUpdatePlayerList();
			return;
		}
		searchText.ToLower();
		m_SchanaPartyPlayerList.ClearItems();	
		member_sorting_map.Clear ();
		auto onlinePlayers = GetSchanaPartyManagerClient().GetOnlinePlayers();
		foreach (auto id, auto player_name : onlinePlayers) 
		{
			//player_name.ToLower();
			string searchedPlayer = player_name;
			searchedPlayer.ToLower();
			if(searchedPlayer.Contains(searchText))
			{
				m_SchanaPartyPlayerList.AddItem (player_name, new Param1<string> (id), 0);
			}
		}
	}

	void SchanaPartyUpdatePlayerList ()
	{
		//Buged with Selection Removed here and added back to the Event.
		/*if(m_playersearchBox.GetText() != "")
		{
			SearchPlayer();
			return;
		}
		*/
		//Player is searching...
		if(m_playersearchBox.GetText() != "")
			return;
		member_sorting_map.Clear ();
		int insert_row = 0;
		auto onlinePlayers = GetSchanaPartyManagerClient ().GetOnlinePlayers ();
		foreach (auto id, auto player_name : onlinePlayers) {
			player_name.ToLower ();
			member_sorting_map.Insert (player_name + id, id);
		}

		auto sorted_keys = member_sorting_map.GetKeyArray ();
		sorted_keys.Sort ();
		foreach (auto sorted_key : sorted_keys) 
		{
			string player_id = member_sorting_map.Get (sorted_key);
			if (insert_row < m_SchanaPartyPlayerList.GetNumItems ()) 
			{
				m_SchanaPartyPlayerList.SetItem (insert_row, onlinePlayers.Get (player_id), new Param1<string> (player_id), 0);
			} 
			else 
			{
				m_SchanaPartyPlayerList.AddItem (onlinePlayers.Get (player_id), new Param1<string> (player_id), 0);
			}
			insert_row++;
		}
		while (insert_row < m_SchanaPartyPlayerList.GetNumItems ()) {
			m_SchanaPartyPlayerList.RemoveRow (insert_row);
		}

		if (m_SchanaPartyPlayerList.GetSelectedRow () >= insert_row) {
			m_SchanaPartyPlayerList.SelectRow (insert_row - 1);
		}
	}

	void SchanaPartyUpdatePartyStatus () {
		Param1<string> id;
		for (int i = 0; i < m_SchanaPartyPartyList.GetNumItems (); ++i) {
			m_SchanaPartyPartyList.GetItemData (i, 0, id);

			if (GetSchanaPartyManagerClient ().IsPartyMemberOnline (id.param1)) {
				// Green A400
				m_SchanaPartyPartyList.SetItemColor (i, 0, 0xFF00E676);
			} else if (GetSchanaPartyManagerClient ().IsPartyMemberOnlineButNotMutual (id.param1)) {
				// Light Blue A400
				m_SchanaPartyPartyList.SetItemColor (i, 0, 0xFF00B0FF);
			} else {
				// Gray 400
				m_SchanaPartyPartyList.SetItemColor (i, 0, 0xFFBDBDBD);
			}
		}
	}

	bool SchanaPartyMenuIsOpen () {
		return m_SchanaPartyMenuIsOpen;
	}

	void SchanaPartyMenuSetOpen (bool open) {
		m_SchanaPartyMenuIsOpen = open;
	}
}