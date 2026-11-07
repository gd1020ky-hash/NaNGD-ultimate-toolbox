#include <Geode/Geode.hpp>
#include "Toolbox.hpp"
#include "ToolboxPopup.hpp"

using namespace geode::prelude;

// Updates the game timeScale and syncs the music pitch smoothly
static void applySpeedhack() {
    bool enabled = Toolbox::get().isSpeedhackEnabled();
    float rate = enabled ? Toolbox::get().getSpeedhackRate() : 1.0f;

    // Apply speedhack to Cocos2d's scheduler
    if (auto director = CCDirector::sharedDirector()) {
        if (auto scheduler = director->getScheduler()) {
            scheduler->setTimeScale(rate);
        }
    }

    // Keep music pitch locked in tempo with speedhack rate
    if (Mod::get()->getSettingValue<bool>("speedhack-audio-sync")) {
        if (auto fmod = FMODAudioEngine::sharedEngine()) {
            if (fmod->m_backgroundMusicChannel) {
                fmod->m_backgroundMusicChannel->setPitch(rate);
            }
        }
    }
}

// Mod entrypoint: runs once when Geode loads the mod
$on_mod(Loaded) {
    log::info("NaNGD Ultimate Toolbox loaded! Have fun!");

    // Keep settings in sync dynamically
    listenForSettingChanges("safe-mode", [](bool enabled) {
        log::info("Safe mode is now: {}", enabled ? "ENABLED" : "DISABLED");
    });

    listenForSettingChanges("speedhack-enabled", [](bool) {
        applySpeedhack();
    });

    listenForSettingChanges("speedhack-rate", [](double) {
        applySpeedhack();
    });

    listenForSettingChanges("speedhack-audio-sync", [](bool) {
        applySpeedhack();
    });
}