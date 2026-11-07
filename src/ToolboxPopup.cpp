#include "ToolboxPopup.hpp"
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

bool ToolboxPopup::setup() {
    this->setTitle("Ultimate Toolbox");

    auto winSize = m_size;

    auto addRow = [this](float yOffset, char const* name, std::string const& settingKey) {
        auto mod = Mod::get();
        bool currentVal = mod->getSettingValue<bool>(settingKey);

        auto onSprite = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
        auto offSprite = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
        onSprite->setScale(0.65f);
        offSprite->setScale(0.65f);

        auto toggler = CCMenuItemToggler::create(
            offSprite,
            onSprite,
            this,
            menu_selector(ToolboxPopup::onToggleClicked)
        );
        toggler->setUserObject(CCString::create(settingKey));
        toggler->toggle(currentVal);
        toggler->setPosition(30.f, yOffset);
        m_buttonMenu->addChild(toggler);

        auto label = CCLabelBMFont::create(name, "bigFont.fnt");
        label->setScale(0.35f);
        label->setAnchorPoint({0.0f, 0.5f});
        label->setPosition(48.f, yOffset);
        m_mainLayer->addChild(label);
    };

    float startY = winSize.height - 48.f;
    float rowHeight = 24.f;

    addRow(startY, "Safe Mode (Protect Stats)", "safe-mode");
    addRow(startY - rowHeight * 1, "Noclip (Invincibility)", "noclip");
    addRow(startY - rowHeight * 2, "Show Hitboxes", "show-hitboxes");
    addRow(startY - rowHeight * 3, "Practice Music Sync", "practice-music-sync");
    addRow(startY - rowHeight * 4, "Speedhack (TimeScale)", "speedhack-enabled");
    addRow(startY - rowHeight * 5, "NanGD Bell Click Sound", "bell-click-sound");
    addRow(startY - rowHeight * 6, "Confirm Exit on High %", "confirm-exit");
    addRow(startY - rowHeight * 7, "High Accuracy Frame Analysis", "frame-analysis-enabled");
    addRow(startY - rowHeight * 8, "Frame Window Counter", "frame-window-counter");
    addRow(startY - rowHeight * 9, "Instant Respawn", "instant-respawn");
    addRow(startY - rowHeight * 10, "In-Game HUD Overlay", "show-hud");

    auto settingsSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    settingsSprite->setScale(0.70f);
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        settingsSprite,
        this,
        menu_selector(ToolboxPopup::onOpenSettings)
    );
    settingsBtn->setPosition(winSize.width - 25.f, 25.f);
    m_buttonMenu->addChild(settingsBtn);

    return true;
}

void ToolboxPopup::onToggleClicked(CCObject* sender) {
    auto toggler = static_cast<CCMenuItemToggler*>(sender);
    auto keyStr = static_cast<CCString*>(toggler->getUserObject())->getCString();
    bool newState = toggler->isToggled();

    Mod::get()->setSettingValue(keyStr, newState);
    Notification::create(
        fmt::format("{}: {}", keyStr, newState ? "ON" : "OFF"),
        NotificationIcon::Info
    )->show();
}

void ToolboxPopup::onOpenSettings(CCObject*) {
    geode::openSettingsPopup(Mod::get());
}

ToolboxPopup* ToolboxPopup::create() {
    auto ret = new ToolboxPopup();
    if (ret && ret->initAnchored(440.f, 360.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
