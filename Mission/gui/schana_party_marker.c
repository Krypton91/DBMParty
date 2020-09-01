class SchanaPartyMarkerMenu extends UIScriptedMenu {
    static string SCHANA_PARTY_MARKER_DELETE = "SCHANA_PARTY_MARKER_DELETE";

    private Widget m_SchanaPartyMarkerRoot;
    private TextWidget m_SchanaPartyMarkerNametag;
    private TextWidget m_SchanaPartyMarkerDistance;
    private ImageWidget m_SchanaPartyMarkerIcon;

    private string m_SchanaPartyMarkerName = "";
    private vector m_SchanaPartyMarkerPosition = "0 0 0";
    private bool isDefaultNameTag;
    //Tactical Pings
    void SchanaPartyMarkerMenu (string name, vector position) {
        m_SchanaPartyMarkerRoot = GetGame ().GetWorkspace ().CreateWidgets ("SchanaModParty/GUI/Layouts/marker.layout");
        m_SchanaPartyMarkerNametag = TextWidget.Cast (m_SchanaPartyMarkerRoot.FindAnyWidget ("nametag"));
        m_SchanaPartyMarkerDistance = TextWidget.Cast (m_SchanaPartyMarkerRoot.FindAnyWidget ("distance"));
        m_SchanaPartyMarkerIcon = ImageWidget.Cast (m_SchanaPartyMarkerRoot.FindAnyWidget ("icon"));
        m_SchanaPartyMarkerRoot.Show (false);

        m_SchanaPartyMarkerName = name;
        m_SchanaPartyMarkerPosition = position;
        isDefaultNameTag = true;
        GetGame ().GetCallQueue (CALL_CATEGORY_GUI).CallLater (this.SchanaUpdate, 16, true);
    }

    void ~SchanaPartyMarkerMenu () {
        GetGame ().GetCallQueue (CALL_CATEGORY_GUI).Remove (this.SchanaUpdate);
        if (m_SchanaPartyMarkerRoot != null) {
            m_SchanaPartyMarkerRoot.Show (false);
            m_SchanaPartyMarkerRoot.Unlink ();
        }
    }

    void SchanaUpdate () {
        if (SchanaPartyMarkerShouldShow ()) {
            SchanaPartyMarkerUpdate ();
        } else if (m_SchanaPartyMarkerRoot != null) {
            m_SchanaPartyMarkerRoot.Show (false);
            delete this;
        }
    }
    private bool SchanaPartyMarkerShouldShow () {
        if (m_SchanaPartyMarkerRoot == null) {
            return false;
        }
        if (!GetGame ().GetPlayer ()) {
            return false;
        }
        if (m_SchanaPartyMarkerName == SCHANA_PARTY_MARKER_DELETE) {
            return false;
        }
        return true;
    }

    void SchanaPartySetRemoveFlag () {
        m_SchanaPartyMarkerName = SCHANA_PARTY_MARKER_DELETE;
    }

    void SchanaPartyMarkerUpdate () 
    {
        float x, y, distance;
        vector position = m_SchanaPartyMarkerPosition;
        vector screenPosition = GetGame ().GetScreenPos (position + "0 0.2 0");

        x = Math.Round (screenPosition[0]);
        y = Math.Round (screenPosition[1]);
        m_SchanaPartyMarkerRoot.SetPos (x, y);

        distance = Math.Round (vector.Distance (position, GetGame ().GetPlayer ().GetPosition ()));
        string distanceString = "";
        int MaxDistance = PluginPartySettingsClient().RangeOfMarker;
        if(distance > MaxDistance && MaxDistance != 0)
            return;
        if(PluginPartySettingsClient().TacticalPing == 1)
            return;
        m_SchanaPartyMarkerNametag.SetText (m_SchanaPartyMarkerName);
        m_SchanaPartyMarkerDistance.SetText (distanceString);

        m_SchanaPartyMarkerRoot.Show (SchanaPartyMarkerVisibleOnScreen ());
    }

    private bool SchanaPartyMarkerVisibleOnScreen () {
        vector position = m_SchanaPartyMarkerPosition;
        vector screenPositionRelative = GetGame ().GetScreenPosRelative (position);

        if (screenPositionRelative[0] >= 1 || screenPositionRelative[0] == 0 || screenPositionRelative[1] >= 1 || screenPositionRelative[1] == 0) {
            return false;
        } else if (screenPositionRelative[2] < 0) {
            return false;
        }
        return true;
    }
}