#pragma once

#include <Geode/Geode.hpp>

class HUDOverlay : public cocos2d::CCNode {
private:
    cocos2d::CCLabelBMFont* m_statusLabel = nullptr;
    cocos2d::CCLabelBMFont* m_fpsLabel = nullptr;
    cocos2d::CCLabelBMFont* m_cpsLabel = nullptr;
    cocos2d::CCLabelBMFont* m_percentLabel = nullptr;
    cocos2d::CCLabelBMFont* m_frameAnalysisLabel = nullptr;
    cocos2d::CCLabelBMFont* m_frameWindowLabel = nullptr;

    float m_fpsAccumulator = 0.0f;
    int m_frameCount = 0;
    int m_currentFPS = 0;

    bool init() override;
    void update(float dt) override;

public:
    static HUDOverlay* create();
};
