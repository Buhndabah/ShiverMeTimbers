#include <vector>
#include <SDL.h>
#include <SDL/SDL_mixer.h>

class SoundManager {
public:
    static SoundManager& getInstance();
    ~SoundManager();

    void startMusic();
    void stopMusic();
    void toggleMusic();
    void operator[](int);

private:
    int volume;
    int currentSound;
    Mix_Music *music;

    int audioBuffers;
    std::vector<Mix_Chunk*> sounds;
    std::vector<int> channels;
    SoundManager();
    SoundManager(const SoundManager&);
    SoundManager& operator=(const SoundManager&);
    Uint32 throwWaitTime;
    Uint32 throwLastPlayed;
    Uint32 stepWaitTime;
    Uint32 stepLastPlayed;
};
