#include "FrameAnalyzer.hpp"

using namespace geode::prelude;

void FrameAnalyzer::resetAttempt() {
    m_physicsTickCount = 0;
    m_lastClickPhysicsTick = 0;
    m_lastClickInfo = {};
    m_frametimeHistory.clear();
    m_lastFrameTime = std::chrono::high_resolution_clock::now();
}

void FrameAnalyzer::onPhysicsStep() {
    m_physicsTickCount++;
}

void FrameAnalyzer::onRenderFrameStart() {
    auto now = std::chrono::high_resolution_clock::now();
    m_currentFrameStart = now;

    if (m_lastFrameTime.time_since_epoch().count() == 0) {
        m_lastFrameTime = now;
        return;
    }

    double frameDurationMs = std::chrono::duration<double, std::milli>(now - m_lastFrameTime).count();
    m_lastFrameTime = now;

    m_frametimeHistory.push_back(frameDurationMs);
    if (m_frametimeHistory.size() > m_maxHistory) {
        m_frametimeHistory.pop_front();
    }

    if (m_frametimeHistory.empty()) return;

    m_cachedMetrics.currentFrametimeMs = frameDurationMs;
    double sum = std::accumulate(m_frametimeHistory.begin(), m_frametimeHistory.end(), 0.0);
    double avg = sum / m_frametimeHistory.size();
    m_cachedMetrics.avgFrametimeMs = avg;

    double varianceSum = 0.0;
    for (double ft : m_frametimeHistory) {
        varianceSum += std::pow(ft - avg, 2);
    }
    m_cachedMetrics.jitterMs = std::sqrt(varianceSum / m_frametimeHistory.size());

    std::vector<double> sortedTimes(m_frametimeHistory.begin(), m_frametimeHistory.end());
    std::sort(sortedTimes.begin(), sortedTimes.end());
    
    size_t index99th = static_cast<size_t>(std::ceil(sortedTimes.size() * 0.99)) - 1;
    double worstFrametime = sortedTimes[std::min(index99th, sortedTimes.size() - 1)];
    m_cachedMetrics.onePercentLowFps = worstFrametime > 0.0 ? (1000.0 / worstFrametime) : 0.0;
}

void FrameAnalyzer::recordClick() {
    auto now = std::chrono::high_resolution_clock::now();
    double subframeMs = std::chrono::duration<double, std::milli>(now - m_currentFrameStart).count();

    m_lastClickInfo.physicsTick = m_physicsTickCount;
    m_lastClickInfo.framesSinceLastClick = (m_lastClickPhysicsTick > 0) ? (m_physicsTickCount - m_lastClickPhysicsTick) : 0;
    m_lastClickInfo.subframeOffsetMs = subframeMs;

    m_lastClickPhysicsTick = m_physicsTickCount;
}
