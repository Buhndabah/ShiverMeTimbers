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
    audioRate(22050),
    audioChannels(4),
    audioBuffers(4096),
    sounds(),
    channels(),
    throwWaitTime(500),
    throwLastPlayed(0),
    stepWaitTime(500),
    stepLastPlayed(0)
{
        if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, audioBuffers))
        {
            throw std::string("Unable to open audio");
        }
        music = Mix_LoadMUS("resources/sounds/freezeezy.wav");
        //music = Mix_LoadMUS("resources/sounds/trololo.mp3");
        //if(!music) throw std::string("Couldn't load trololo.mp3 ")+Mix_GetError();
        if(!music) throw std::string("Couldn't load explosion.wav")+Mix_GetError();
        startMusic();
        sounds.push_back( Mix_LoadWAV("resources/sounds/snowstep.wav"));
        sounds.push_back( Mix_LoadWAV("resources/sounds/snowthrow.wav"));
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
