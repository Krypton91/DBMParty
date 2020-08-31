class SchanaModPartyServerSettings {
    private static string DIR = "$profile:DeutscheBohrmaschine";
    private static string PATH =  DIR + "\\server-config.json";

    private static const int DEFAULT_LOG_FREQUENCY = 10;
    private static const int DEFAULT_VERBOSITY = 1;
    private static const int DEFAULT_MAX_PARTY_SIZE = -1;
    private static const int DEFAULT_SEND_INFO_FREQUENCY = 2;
    private static const int DEFAULT_SEND_MARKER_FREQUENCY = 2;
    private static const int DEFAULT_MAX_MARKERS = 10;

    private int logPartiesFrequencySeconds = DEFAULT_LOG_FREQUENCY;
    private int verbosity = DEFAULT_VERBOSITY;
    private int maxPartySize = DEFAULT_MAX_PARTY_SIZE;
    private int sendInfoFrequencySeconds = DEFAULT_SEND_INFO_FREQUENCY;
    private int sendMarkerFrequencySeconds = DEFAULT_SEND_MARKER_FREQUENCY;
    private int maxMarkers = DEFAULT_MAX_MARKERS;

    int GetLogFrequency () {
        return logPartiesFrequencySeconds;
    }

    int GetVerbosity () {
        return verbosity;
    }

    int GetMaxPartySize () {
        return maxPartySize;
    }

    int GetSendInfoFrequency () {
        return sendInfoFrequencySeconds;
    }

    int GetSendMarkerFrequency () {
        return sendMarkerFrequencySeconds;
    }

    int GetMaxMarkers () {
        return maxMarkers;
    }

    void Save () {
        if (GetGame ().IsServer ()) {
            if (!FileExist (DIR)) {
                MakeDirectory (DIR);
            }
            JsonFileLoader<SchanaModPartyServerSettings>.JsonSaveFile (PATH, this);
        }
    }

    static ref SchanaModPartyServerSettings Get () {

        auto settings = new ref SchanaModPartyServerSettings ();

        if (FileExist (PATH)) {
            JsonFileLoader<SchanaModPartyServerSettings>.JsonLoadFile (PATH, settings);
        }

        LoadDefaultsIfMissing (settings);

        settings.Save ();

        return settings;
    }

    static void LoadDefaultsIfMissing (SchanaModPartyServerSettings settings) {
        if (settings.logPartiesFrequencySeconds == 0) {
            settings.logPartiesFrequencySeconds = DEFAULT_LOG_FREQUENCY;
        }
        if (settings.verbosity == 0) {
            settings.verbosity = DEFAULT_VERBOSITY;
        }
        if (settings.maxPartySize == 0) {
            settings.maxPartySize = DEFAULT_MAX_PARTY_SIZE;
        }
        if (settings.sendInfoFrequencySeconds == 0) {
            settings.sendInfoFrequencySeconds = DEFAULT_SEND_INFO_FREQUENCY;
        }
        if (settings.sendMarkerFrequencySeconds == 0) {
            settings.sendMarkerFrequencySeconds = DEFAULT_SEND_MARKER_FREQUENCY;
        }
        if (settings.maxMarkers == 0) {
            settings.maxMarkers = DEFAULT_MAX_MARKERS;
        }
    }
}

static ref SchanaModPartyServerSettings g_SchanaPartyServerSettings;
static ref SchanaModPartyServerSettings GetSchanaPartyServerSettings () {
    if (g_Game.IsServer () && !g_SchanaPartyServerSettings) {
        g_SchanaPartyServerSettings = SchanaModPartyServerSettings.Get ();
    }
    return g_SchanaPartyServerSettings;
}