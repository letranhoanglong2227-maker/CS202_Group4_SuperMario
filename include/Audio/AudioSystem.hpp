#pragma once

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Core/GameEventMediator.hpp"

#include <array>
#include <filesystem>
#include <optional>
#include <string>

class AudioSystem {
public:
    static AudioSystem& instance();

    AudioSystem(const AudioSystem&) = delete;
    AudioSystem& operator=(const AudioSystem&) = delete;
    AudioSystem(AudioSystem&&) = delete;
    AudioSystem& operator=(AudioSystem&&) = delete;

    bool playMenuMusic();
    bool playWorldMusic(int worldId);
    void stopMusic();
    void setMusicEnabled(bool enabled);
    [[nodiscard]] bool isMusicEnabled() const noexcept;
    void setMusicVolume(float volume);
    [[nodiscard]] float musicVolume() const noexcept;
    void setEffectsEnabled(bool enabled) noexcept;
    [[nodiscard]] bool areEffectsEnabled() const noexcept;
    bool playCue(AudioCue cue);
private:
    AudioSystem() = default;

    bool playTrack(const std::filesystem::path& relativePath,
                   std::string trackId);
    sf::Music music;
    std::string currentTrack;
    float volume{25.f};
    bool enabled{true};
    bool effectsEnabled{true};
    struct Effect {
        sf::SoundBuffer buffer;
        std::optional<sf::Sound> sound;
        bool loadAttempted{};
    };
    std::array<Effect, 9> effects;
};
