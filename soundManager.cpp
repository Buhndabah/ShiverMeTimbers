#include <string>
#include <iostream>
#include "soundManager.h"
#include "clock.h"

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

SoundManager::~SoundManager() {
    std::cout << "Cleaning up sounds" << std::endl;
    Mix_HaltMusic();
    Mix_FreeMusic(music);
    for(unsigned int i=0; i< sounds.size(); ++i)
    {
        Mix_FreeChunk(sounds[i]);
    }
    Mix_CloseAudio();
}

SoundManager::SoundManager() :
    volume(SDL_MIX_MAXVOLUME),
    currentSound(-1),
    music(NULL),
    audioBuffers(4096),
    sounds(),
    channels(),
    throwWaitTime(500),
    throwLastPlayed(0),
    stepWaitTime(500),
    stepLastPlayed(0)
{
        if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 4, audioBuffers))
        {
            std::cerr << "audio open" << std::endl;
            throw std::string("Unable to open audio");
        }
        music = Mix_LoadMUS("resources/sounds/freezeezy.wav");
        if(!music) {
            std::cerr << "freez" << std::endl; 
            throw std::string(Mix_GetError());
        }
        startMusic();
        Mix_Chunk *check=NULL;
        sounds.push_back( check = Mix_LoadWAV("resources/sounds/snowstep.wav"));
        if(!check) {
            std::cerr << "step" << std::endl;
            throw std::string(Mix_GetError());
        }
        sounds.push_back( check = Mix_LoadWAV("resources/sounds/snowthrow.wav"));
        if(!check) {
            std::cerr << "throw" << std::endl;
            throw std::string(Mix_GetError());
        }
        for(unsigned int i=0; i< sounds.size(); ++i) channels.push_back(i);
}

void SoundManager::toggleMusic() {
    if(Mix_PausedMusic()){
        Mix_ResumeMusic();
    }
}

void SoundManager::operator[](int index) {
    switch(index) {
        case 0:
            if(Clock::getInstance().getTotalTicks() - stepLastPlayed < stepWaitTime) return;
            else stepLastPlayed = Clock::getInstance().getTotalTicks();
            break;
        case 1:
            if(Clock::getInstance().getTotalTicks() - throwLastPlayed < throwWaitTime) return;
            else throwLastPlayed = Clock::getInstance().getTotalTicks();
            break;
        default:
            throw std::string("invalid index into sound manager");
            break;
    }
    if(currentSound >= 0) Mix_HaltChannel(currentSound);
    currentSound = index;
    Mix_VolumeChunk(sounds[index],volume);
    channels[index] = Mix_PlayChannel(channels[index], sounds[index], 0);
}

void SoundManager::startMusic() {
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(music, -1);
}

void SoundManager::stopMusic() {
    Mix_HaltMusic();
    Mix_FreeMusic(music);
} 
