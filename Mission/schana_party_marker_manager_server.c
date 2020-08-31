class SchanaPartyMarkerManagerServer {
    private ref map<ref string, ref array<ref SchanaPartyMarkerInfo>> markers;
    private bool canSendInfo = true;

    void SchanaPartyMarkerManagerServer () {
        SchanaPartyUtils.LogMessage ("PartyMarker Server Init");
        markers = new ref map<ref string, ref array<ref SchanaPartyMarkerInfo>> ();
        GetRPCManager ().AddRPC ("SchanaModParty", "ServerRegisterMarkersRPC", this, SingleplayerExecutionType.Both);

        GetGame ().GetCallQueue (CALL_CATEGORY_SYSTEM).CallLater (this.SendMarkers, 10000, true);
        GetGame ().GetCallQueue (CALL_CATEGORY_SYSTEM).CallLater (this.ResetSendInfoLock, GetSchanaPartyServerSettings ().GetSendMarkerFrequency () * 1000, true);
    }

    void ~SchanaPartyMarkerManagerServer () {
        GetGame ().GetCallQueue (CALL_CATEGORY_SYSTEM).Remove (this.SendMarkers);
        GetGame ().GetCallQueue (CALL_CATEGORY_SYSTEM).Remove (this.ResetSendInfoLock);
    }

    private void ResetSendInfoLock () {
        canSendInfo = true;
    }

    void ServerRegisterMarkersRPC (CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target) {
        Param1<ref array<ref SchanaPartyMarkerInfo>> data;
        if (!ctx.Read (data))
            return;

        if (SchanaPartyUtils.WillLog (SchanaPartyUtils.DEBUG)) {
            string result;
            JsonSerializer ().WriteToString (data, false, result);
            SchanaPartyUtils.Debug ("ServerRegisterMarkersRPC " + result);
        }

        ServerRegisterMarkers (sender.GetId (), data.param1);
    }

    void ServerRegisterMarkers (string id, ref array<ref SchanaPartyMarkerInfo> playerMarkers) {
        int maxMarkers = GetSchanaPartyServerSettings ().GetMaxMarkers ();
        if (maxMarkers != -1) {
            while (playerMarkers.Count () > maxMarkers) {
                playerMarkers.RemoveOrdered (0);
            }
        }
        markers.Set (id, playerMarkers);

        SendMarkers();
    }

    void SendMarkers () {
        if (canSendInfo) {
            auto id_map = new ref map<ref string, ref PlayerBase> ();

            MissionServer mission = MissionServer.Cast (GetGame ().GetMission ());

            foreach (Man man : mission.m_Players) {
                PlayerBase player = PlayerBase.Cast (man);
                if (player && player.GetIdentity ()) {
                    id_map.Insert (player.GetIdentity ().GetId (), player);
                }
            }

            auto manager = GetSchanaPartyManagerServer ();
            auto parties = manager.GetParties ();

            foreach (auto id, auto party_ids : parties) {
                SchanaPartyUtils.Trace ("SendMarkers Begin " + id);
                SendMarkerInfoToPlayer (id, party_ids, id_map.Get (id));
                SchanaPartyUtils.Trace ("SendMarkers End " + id);
            }

            canSendInfo = false;
        }
    }

    private void SendMarkerInfoToPlayer (string id, ref set<ref string> party_ids, PlayerBase player) {
        auto playerMarkers = new ref array<ref SchanaPartyMarkerInfo>;
        foreach (string party_id : party_ids) {
            if (markers.Contains (party_id)) {
                foreach (auto m : markers.Get (party_id)) {
                    playerMarkers.Insert (m);
                }
            }
        }
        auto info = new ref Param1<ref array<ref SchanaPartyMarkerInfo>> (playerMarkers);

        if (SchanaPartyUtils.WillLog (SchanaPartyUtils.DEBUG)) {
            string result;
            JsonSerializer ().WriteToString (info, false, result);
            SchanaPartyUtils.Debug ("SendMarkers to " + id + " " + result);
        }

        if (player && player.GetIdentity ()) {
            GetRPCManager ().SendRPC ("SchanaModParty", "ClientUpdatePartyMarkersRPC", info, false, player.GetIdentity ());
        } else {
            SchanaPartyUtils.Warn ("SendMarkers failed to " + id);
        }
    }
}

static ref SchanaPartyMarkerManagerServer g_SchanaPartyMarkerManagerServer;
static ref SchanaPartyMarkerManagerServer GetSchanaPartyMarkerManagerServer () {
    if (g_Game.IsServer () && !g_SchanaPartyMarkerManagerServer) {
        g_SchanaPartyMarkerManagerServer = new SchanaPartyMarkerManagerServer;
    }
    return g_SchanaPartyMarkerManagerServer;
}