#define MINIAUDIO_IMPLEMENTATION
#include "audioManager.h"
#include <iostream>

AudioManager::AudioManager() {
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine." << std::endl;
    }
}

AudioManager::~AudioManager() {
    for (auto& pair : sounds) {
        ma_sound_uninit(&pair.second);
    }
    ma_engine_uninit(&engine);
}

bool AudioManager::loadSound(const std::string& name, const std::string& filePath) {
    ma_sound sound;
    if (ma_sound_init_from_file(&engine, filePath.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS) {
        std::cerr << "Failed to load sound: " << filePath << std::endl;
        return false;
    }
    sounds[name] = sound;
    return true;
}

void AudioManager::playSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        std::cout << "Play: " << name << std::endl;
        ma_sound_start(&it->second);
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void AudioManager::stopSound(const std::string& name) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        ma_sound_stop(&it->second);
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void AudioManager::stopAllSounds() {
    for (auto& pair : sounds) {
        ma_sound_stop(&pair.second);
    }
}
