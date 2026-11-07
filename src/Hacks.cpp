#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <filesystem>

#include "Toolbox.hpp"
#include "FrameAnalyzer.hpp"
#include "HUDOverlay.hpp"
#include "ToolboxPopup.hpp"

using namespace geode::prelude;

// ============================================================================
// 1. PlayLayer Hooks (Noclip, Hitboxes, Practice Sync, Keybinds, Safe Mode)
// ============================================================================
class $modify(ToolboxPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        Toolbox::get().resetClicks();
        Toolbox::get().resetAttemptState();
        FrameAnalyzer::get().resetAttempt();

        auto hud = HUDOverlay::create();
        this->addChild(hud);

        if (Mod::get()->getSettingValue<bool>("hide-playlayer-ui")) {
            if (auto pUI = this->getChildByID("percentage-label")) pUI->setVisible(false);
            if (auto pBar = this->getChildByID("progress-bar")) pBar->setVisible(false);
        }

        return true;
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        FrameAnalyzer::get().resetAttempt();
    }

    void update(float dt) {
        FrameAnalyzer::get().onRenderFrameStart();
        FrameAnalyzer::get().onPhysicsStep();

        if (Toolbox::get().isSpeedhackEnabled()) {
            Toolbox::get().markCheatUsed();
        }

        if (this->m_debugDrawNode) {
            this->m_debugDrawNode->setVisible(Toolbox::get().isHitboxesEnabled());
        }

        PlayLayer::update(dt);
    }

    void togglePracticeMode(bool practice) {
        PlayLayer::togglePracticeMode(practice);

        if (practice && Toolbox::get().isPracticeMusicSyncEnabled() && this->m_level) {
            GameSoundManager::sharedManager()->stopBackgroundMusic();
            GameSoundManager::sharedManager()->playBackgroundMusic(
                this->m_level->getAudioFileName(),
                false,
                true
            );
        }
    }

    void keyDown(enumKeyCodes key) {
        if (key == KEY_Tab || key == KEY_C) {
            ToolboxPopup::create()->show();
            return;
        }
        PlayLayer::keyDown(key);
    }

    void destroyPlayer(PlayerObject* player, GameObject* hazard) {
        if (Toolbox::get().isNoclipEnabled()) {
            Toolbox::get().markCheatUsed();
            return;
        }

        PlayLayer::destroyPlayer(player, hazard);

        if (Mod::get()->getSettingValue<bool>("instant-respawn")) {
            this->resetLevel();
        }
    }

    void levelComplete() {
        if (Toolbox::get().isSafeModeEnabled() && (Toolbox::get().areCheatsActive() || Toolbox::get().hasCheatedThisAttempt())) {
            log::warn("Safe Mode Active: High score submission suppressed.");
            Notification::create("Safe Mode: Completion not submitted!", NotificationIcon::Warning)->show();
            this->resetLevel();
            return;
        }

        PlayLayer::levelComplete();
    }
};

// ============================================================================
// 2. PlayerObject Hooks (Input Timing, NanGD Bell Sound, CPS & Rainbow Trail)
// ============================================================================
class $modify(ToolboxPlayerObject, PlayerObject) {
    struct Fields {
        float m_rainbowHue = 0.0f;
    };

    void pushButton(PlayerButton btn) {
        PlayerObject::pushButton(btn);
        
        Toolbox::get().recordClick();
        FrameAnalyzer::get().recordClick();

        if (Mod::get()->getSettingValue<bool>("bell-click-sound")) {
            auto soundPath = Mod::get()->getResourcesDir() / "bell.wav";
            if (!std::filesystem::exists(soundPath)) {
                soundPath = Mod::get()->getResourcesDir() / "bell.ogg";
            }
            if (std::filesystem::exists(soundPath)) {
                FMODAudioEngine::sharedEngine()->playEffect(soundPath.string(), 1.0f, 1.0f, 0.8f);
            }
        }
    }

    void update(float dt) {
        PlayerObject::update(dt);

        if (Mod::get()->getSettingValue<bool>("rainbow-trail")) {
            m_fields->m_rainbowHue += dt * 180.f;
            if (m_fields->m_rainbowHue >= 360.f) m_fields->m_rainbowHue -= 360.f;

            float c = 1.0f;
            float x = c * (1.0f - std::abs(std::fmod(m_fields->m_rainbowHue / 60.0f, 2.0f) - 1.0f));
            float r = 0, g = 0, b = 0;

            if (m_fields->m_rainbowHue < 60)      { r = c; g = x; b = 0; }
            else if (m_fields->m_rainbowHue < 120){ r = x; g = c; b = 0; }
            else if (m_fields->m_rainbowHue < 180){ r = 0; g = c; b = x; }
            else if (m_fields->m_rainbowHue < 240){ r = 0; g = x; b = c; }
            else if (m_fields->m_rainbowHue < 300){ r = x; g = 0; b = c; }
            else                                  { r = c; g = 0; b = x; }

            ccColor3B rainbowColor = {
                static_cast<GLubyte>(r * 255),
                static_cast<GLubyte>(g * 255),
                static_cast<GLubyte>(b * 255)
            };

            if (m_motionStreak) m_motionStreak->setColor(rainbowColor);
            if (m_waveTrail) m_waveTrail->setColor(rainbowColor);
        }
    }
};

// ============================================================================
// 3. PauseLayer Hooks (Confirm Exit, Hotkey, Pause Menu Button)
// ============================================================================
class $modify(ToolboxPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("right-button-menu");
        if (!menu) menu = this->getChildByID("center-button-menu");

        if (menu) {
            auto btnSprite = ButtonSprite::create("Toolbox", "goldFont.fnt", "GJ_button_05.png", 0.7f);
            auto btn = CCMenuItemSpriteExtra::create(
                btnSprite,
                this,
                menu_selector(ToolboxPauseLayer::onOpenToolbox)
            );
            btn->setID("pause-toolbox-btn"_spr);
            menu->addChild(btn);
            menu->updateLayout();
        }
    }

    void keyDown(enumKeyCodes key) {
        if (key == KEY_Tab || key == KEY_C) {
            ToolboxPopup::create()->show();
            return;
        }
        PauseLayer::keyDown(key);
    }

    void onQuit(CCObject* sender) {
        auto playLayer = PlayLayer::get();
        bool confirmExit = Mod::get()->getSettingValue<bool>("confirm-exit");

        if (confirmExit && playLayer && playLayer->getCurrentPercent() >= 75.0f) {
            float percent = playLayer->getCurrentPercent();
            geode::createQuickPopup(
                "Confirm Exit",
                fmt::format("You are at <cr>{:.1f}%</c>!\nAre you sure you want to quit?", percent),
                "Cancel", "Quit",
                [this, sender](auto*, bool btn2) {
                    if (btn2) {
                        PauseLayer::onQuit(sender);
                    }
                }
            );
            return;
        }

        PauseLayer::onQuit(sender);
    }

    void onOpenToolbox(CCObject*) {
        ToolboxPopup::create()->show();
    }
};

// ============================================================================
// 4. Main Menu Integration
// ============================================================================
class $modify(ToolboxMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu) {
            auto sprite = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
            sprite->setScale(0.85f);

            auto btn = CCMenuItemSpriteExtra::create(
                sprite,
                this,
                menu_selector(ToolboxMenuLayer::onOpenToolbox)
            );
            btn->setID("main-menu-toolbox-btn"_spr);
            bottomMenu->addChild(btn);
            bottomMenu->updateLayout();
        }

        return true;
    }

    void onOpenToolbox(CCObject*) {
        ToolboxPopup::create()->show();
    }
};
