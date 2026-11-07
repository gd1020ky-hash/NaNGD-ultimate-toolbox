#include <Geode/Geode.hpp>
#include "Toolbox.hpp"
#include "ToolboxPopup.hpp"

using namespace geode::prelude;

static void applySpeedhack() {
    bool enabled = Toolbox::get().isSpeedhackEnabled();
    float rate = enabled ? Toolbox::get().getSpeedhackRate() : 1.0f;

    if (auto director = CCDirector::sharedDirector()) {
        if (auto scheduler = director->getScheduler()) {
            scheduler->setTimeScale(rate);
        }
    }

    if (Mod::get()->getSettingValue<bool>("speedhack-audio-sync")) {
        if (auto fmod = FMODAudioEngine::sharedEngine()) {
            if (fmod->m_currentSoundChannel) {
                fmod->m_currentSoundChannel->setPitch(rate);
            }
        }
    }
}

$on_mod(Loaded) {
    log::info("Ultimate Toolbox initialized successfully.");

    listenForSettingChanges("safe-mode", [](bool enabled) {
        log::info("Safe mode changed: {}", enabled);
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
