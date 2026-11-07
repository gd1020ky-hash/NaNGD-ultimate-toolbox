#pragma once

#include <Geode/Geode.hpp>
#include <deque>
#include <vector>
#include <chrono>
#include <numeric>
#include <cmath>
#include <algorithm>

struct FrameMetrics {
    double currentFrametimeMs = 0.0;
    double avgFrametimeMs = 0.0;
    double jitterMs = 0.0;
    double onePercentLowFps = 0.0;
};

struct ClickTimingInfo {
    uint64_t physicsTick = 0;
    uint64_t framesSinceLastClick = 0;
    double subframeOffsetMs = 0.0;
};

class FrameAnalyzer {
private:
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    std::chrono::high_resolution_clock::time_point m_currentFrameStart;
    std::deque<double> m_frametimeHistory;
    const size_t m_maxHistory = 120;

    uint64_t m_physicsTickCount = 0;
    uint64_t m_lastClickPhysicsTick = 0;
    ClickTimingInfo m_lastClickInfo;

    FrameMetrics m_cachedMetrics;

    FrameAnalyzer() = default;

public:
    static FrameAnalyzer& get() {
        static FrameAnalyzer instance;
        return instance;
    }

    void onRenderFrameStart();
    void onPhysicsStep();
    void recordClick();
    void resetAttempt();

    FrameMetrics const& getMetrics() const { return m_cachedMetrics; }
    ClickTimingInfo const& getLastClickInfo() const { return m_lastClickInfo; }
    uint64_t getCurrentPhysicsTick() const { return m_physicsTickCount; }
};
