#include "Audio/AudioSystem.hpp"

#include "Core/AssetLocator.hpp"

#include <algorithm>
#include <utility>

namespace {
std::filesystem::path cueAsset(AudioCue cue) {
    switch (cue) {
    case AudioCue::MenuMove:
    case AudioCue::MenuConfirm:
        return "assets/audio/buttonSound.wav";
    case AudioCue::Jump:
        return "assets/audio/Jump.wav";
    case AudioCue::Coin:
        return "assets/audio/coin.wav";
    case AudioCue::BrickBreak:
        return "assets/audio/break brick.wav";
    case AudioCue::PowerUp:
        return "assets/audio/eat mushroom.mp3";
    case AudioCue::Shoot:
        return "assets/audio/shoot.mp3";
    case AudioCue::PlayerDeath:
        return "assets/audio/Game Over.mp3";
    case AudioCue::LevelComplete:
        return "assets/audio/buttonSound.wav";
    }
    return {};
}
}

AudioSystem& AudioSystem::instance() {
    static AudioSystem service;
    return service;
}

bool AudioSystem::playMenuMusic() {
    return playTrack("assets/audio/Background Music.wav", "menu");
}

bool AudioSystem::playWorldMusic(int worldId) {
    const int world = std::clamp(worldId, 1, 3);
    return playTrack(
        "assets/audio/Level " + std::to_string(world) + ".mp3",
        "world-" + std::to_string(world));
}

void AudioSystem::stopMusic() {
    music.stop();
    currentTrack.clear();
}

void AudioSystem::setMusicEnabled(bool musicEnabled) {
    enabled = musicEnabled;
    if (!enabled) music.stop();
    else if (!currentTrack.empty()) music.play();
}

bool AudioSystem::isMusicEnabled() const noexcept { return enabled; }

void AudioSystem::setMusicVolume(float selectedVolume) {
    volume = std::clamp(selectedVolume, 0.f, 100.f);
    music.setVolume(volume);
}

float AudioSystem::musicVolume() const noexcept { return volume; }

void AudioSystem::setEffectsEnabled(bool selected) noexcept {
    effectsEnabled = selected;
    if (!effectsEnabled) {
        for (auto& effect : effects) {
            if (effect.sound) effect.sound->stop();
        }
    }
}

bool AudioSystem::areEffectsEnabled() const noexcept {
    return effectsEnabled;
}

bool AudioSystem::playCue(AudioCue cue) {
    if (!effectsEnabled) return true;
    Effect& effect = effects[static_cast<std::size_t>(cue)];
    if (!effect.sound && !effect.loadAttempted) {
        effect.loadAttempted = true;
        const auto path = AssetLocator::find(cueAsset(cue));
        if (!path || !effect.buffer.loadFromFile(*path)) return false;
        effect.sound.emplace(effect.buffer);
        effect.sound->setVolume(volume);
    }
    if (!effect.sound) return false;
    effect.sound->stop();
    effect.sound->play();
    return true;
}

bool AudioSystem::playTrack(const std::filesystem::path& relativePath,
                            std::string trackId) {
    if (currentTrack == trackId) {
        if (enabled && music.getStatus() != sf::SoundSource::Status::Playing)
            music.play();
        return true;
    }
    const auto path = AssetLocator::find(relativePath);
    if (!path || !music.openFromFile(*path)) return false;
    currentTrack = std::move(trackId);
    music.setLooping(true);
    music.setVolume(volume);
    if (enabled) music.play();
    return true;
}
