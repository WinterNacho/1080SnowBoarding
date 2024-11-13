#pragma once

#include "miniaudio.h"
#include <string>
#include <unordered_map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool loadSound(const std::string& name, const std::string& filePath);
    void playSound(const std::string& name);
    void stopSound(const std::string& name);
    void stopAllSounds();

private:
    ma_engine engine;
    std::unordered_map<std::string, ma_sound> sounds;
};
