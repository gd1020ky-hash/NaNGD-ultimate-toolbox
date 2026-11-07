#include "HUDOverlay.hpp"
#include "Toolbox.hpp"
#include "FrameAnalyzer.hpp"
#include <algorithm>

using namespace geode::prelude;

bool HUDOverlay::init() {
    if (!CCNode::init()) return false;

    this->setID("toolbox-hud-overlay"_spr);
    this->setZOrder(999);

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_statusLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_statusLabel->setAnchorPoint({1.0f, 1.0f});
    m_statusLabel->setPosition({winSize.width - 10.f, winSize.height - 10.f});
    m_statusLabel->setScale(0.6f);
    this->addChild(m_statusLabel);

    m_fpsLabel = CCLabelBMFont::create("FPS: 60", "bigFont.fnt");
    m_fpsLabel->setAnchorPoint({0.0f, 1.0f});
    m_fpsLabel->setPosition({10.f, winSize.height - 10.f});
    m_fpsLabel->setScale(0.4f);
    this->addChild(m_fpsLabel);

    m_cpsLabel = CCLabelBMFont::create("CPS: 0", "bigFont.fnt");
    m_cpsLabel->setAnchorPoint({0.0f, 1.0f});
    m_cpsLabel->setPosition({10.f, winSize.height - 28.f});
    m_cpsLabel->setScale(0.4f);
    this->addChild(m_cpsLabel);

    m_percentLabel = CCLabelBMFont::create("0.00%", "bigFont.fnt");
    m_percentLabel->setAnchorPoint({0.5f, 1.0f});
    m_percentLabel->setPosition({winSize.width / 2.0f, winSize.height - 10.f});
    m_percentLabel->setScale(0.5f);
    this->addChild(m_percentLabel);

    m_frameAnalysisLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_frameAnalysisLabel->setAnchorPoint({0.0f, 0.0f});
    m_frameAnalysisLabel->setPosition({10.f, 25.f});
    m_frameAnalysisLabel->setScale(0.55f);
    m_frameAnalysisLabel->setColor({180, 240, 255});
    this->addChild(m_frameAnalysisLabel);

    m_frameWindowLabel = CCLabelBMFont::create("", "chatFont.fnt");
    m_frameWindowLabel->setAnchorPoint({0.0f, 0.0f});
    m_frameWindowLabel->setPosition({10.f, 10.f});
    m_frameWindowLabel->setScale(0.55f);
    m_frameWindowLabel->setColor({255, 230, 140});
    this->addChild(m_frameWindowLabel);

    this->scheduleUpdate();
    return true;
}

void HUDOverlay::update(float dt) {
    auto mod = Mod::get();
    if (!mod->getSettingValue<bool>("show-hud")) {
        this->setVisible(false);
        return;
    }
    this->setVisible(true);

    m_frameCount++;
    m_fpsAccumulator += dt;
    if (m_fpsAccumulator >= 0.5f) {
        m_currentFPS = static_cast<int>(std::round(m_frameCount / m_fpsAccumulator));
        m_frameCount = 0;
        m_fpsAccumulator = 0.0f;
    }

    if (mod->getSettingValue<bool>("hud-show-fps")) {
        m_fpsLabel->setVisible(true);
        m_fpsLabel->setString(fmt::format("FPS: {}", m_currentFPS).c_str());
    } else {
        m_fpsLabel->setVisible(false);
    }

    if (mod->getSettingValue<bool>("hud-show-cps")) {
        m_cpsLabel->setVisible(true);
        m_cpsLabel->setString(fmt::format("CPS: {}", Toolbox::get().getCPS()).c_str());
    } else {
        m_cpsLabel->setVisible(false);
    }

    auto playLayer = PlayLayer::get();
    if (playLayer && mod->getSettingValue<bool>("hud-accurate-percent")) {
        m_percentLabel->setVisible(true);
        float percent = playLayer->getCurrentPercent();
        m_percentLabel->setString(fmt::format("{:.2f}%", percent).c_str());
    } else {
        m_percentLabel->setVisible(false);
    }

    if (mod->getSettingValue<bool>("frame-analysis-enabled")) {
        m_frameAnalysisLabel->setVisible(true);
        auto const& metrics = FrameAnalyzer::get().getMetrics();
        m_frameAnalysisLabel->setString(
            fmt::format("Frame: {:.2f}ms (Jitter: ±{:.2f}ms) | 1% Low: {:.0f} FPS",
                metrics.currentFrametimeMs,
                metrics.jitterMs,
                metrics.onePercentLowFps
            ).c_str()
        );
    } else {
        m_frameAnalysisLabel->setVisible(false);
    }

    if (mod->getSettingValue<bool>("frame-window-counter")) {
        m_frameWindowLabel->setVisible(true);
        auto const& click = FrameAnalyzer::get().getLastClickInfo();
        uint64_t currentTick = FrameAnalyzer::get().getCurrentPhysicsTick();
        
        m_frameWindowLabel->setString(
            fmt::format("Tick: #{} | Last Click: F#{} (Delta: {}f, Offset: +{:.2f}ms)",
                currentTick,
                click.physicsTick,
                click.framesSinceLastClick,
                click.subframeOffsetMs
            ).c_str()
        );
    } else {
        m_frameWindowLabel->setVisible(false);
    }

    if (Toolbox::get().areCheatsActive()) {
        m_statusLabel->setVisible(true);
        if (Toolbox::get().isSafeModeEnabled()) {
            m_statusLabel->setString("SAFE MODE (Progress Disabled)");
            m_statusLabel->setColor({255, 100, 100});
        } else {
            m_statusLabel->setString("CHEATS ACTIVE");
            m_statusLabel->setColor({255, 180, 0});
        }
    } else {
        m_statusLabel->setVisible(false);
    }
}

HUDOverlay* HUDOverlay::create() {
    auto ret = new HUDOverlay();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
