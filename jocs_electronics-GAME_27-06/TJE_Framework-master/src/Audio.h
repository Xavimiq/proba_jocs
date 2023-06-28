#include "extra/bass.h"
#include "utils.h"
#include <map>

 
class Audio
{
public:
    // Handler for BASS_SampleLoad
    HSAMPLE sample;

    Audio();    // Sample = 0 here
    ~Audio();   // Free sample (BASS_SampleFree)

    // Play audio and return the channel
    HCHANNEL play(float volume);

    // Manager stuff
    // The map!
    static std::map<std::string, Audio*> sAudiosLoaded;

    // Initialize BASS
    static bool Init();
    // Get from manager map
    static Audio* Get(const char* filename);
    // Play Manager API
    static HCHANNEL Play(const char* filename, float volume, bool loop);
    static HCHANNEL Play3D(const char* filename, Vector3 position, float volume, bool loop);
    // Stop sounds
    static bool Stop(HCHANNEL channel);
};


