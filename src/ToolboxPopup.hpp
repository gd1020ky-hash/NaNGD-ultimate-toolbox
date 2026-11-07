#pragma once

#include <Geode/Geode.hpp>

class ToolboxPopup : public geode::Popup<> {
protected:
    bool setup() override;
    void onToggleClicked(cocos2d::CCObject* sender);
    void onOpenSettings(cocos2d::CCObject* sender);

public:
    static ToolboxPopup* create();
};
