#include "Toolbox.hpp"

using namespace geode::prelude;

void Toolbox::recordClick() {
    auto now = std::chrono::steady_clock::now();
    m_clickTimestamps.push_back(now);
    m_totalClicks++;
}

int Toolbox::getCPS() {
    auto now = std::chrono::steady_clock::now();
    while (!m_clickTimestamps.empty() &&
           std::chrono::duration_cast<std::chrono::milliseconds>(now - m_clickTimestamps.front()).count() > 1000) {
        m_clickTimestamps.pop_front();
    }
    return static_cast<int>(m_clickTimestamps.size());
}

bool Toolbox::isNoclipEnabled() {
    return Mod::get()->getSettingValue<bool>("noclip");
}

bool Toolbox::isHitboxesEnabled() {
    return Mod::get()->getSettingValue<bool>("show-hitboxes");
}

bool Toolbox::isSpeedhackEnabled() {
    return Mod::get()->getSettingValue<bool>("speedhack-enabled");
}

float Toolbox::getSpeedhackRate() {
    return static_cast<float>(Mod::get()->getSettingValue<double>("speedhack-rate"));
}

bool Toolbox::isSafeModeEnabled() {
    return Mod::get()->getSettingValue<bool>("safe-mode");
}

bool Toolbox::isPracticeMusicSyncEnabled() {
    return Mod::get()->getSettingValue<bool>("practice-music-sync");
}

bool Toolbox::areCheatsActive() const {
    auto mod = Mod::get();
    bool noclip = mod->getSettingValue<bool>("noclip");
    bool speedhack = mod->getSettingValue<bool>("speedhack-enabled");
    float speed = static_cast<float>(mod->getSettingValue<double>("speedhack-rate"));

    return noclip || (speedhack && std::abs(speed - 1.0f) > 0.01f);
}
