#pragma once

#include <Geode/Geode.hpp>
#include <deque>
#include <chrono>

class Toolbox {
private:
    std::deque<std::chrono::steady_clock::time_point> m_clickTimestamps;
    int m_totalClicks = 0;
    bool m_cheatsUsedThisAttempt = false;

    Toolbox() = default;

public:
    static Toolbox& get() {
        static Toolbox instance;
        return instance;
    }

    void recordClick();
    int getCPS();
    int getTotalClicks() const { return m_totalClicks; }
    void resetClicks() { m_totalClicks = 0; m_clickTimestamps.clear(); }

    void markCheatUsed() { m_cheatsUsedThisAttempt = true; }
    void resetAttemptState() { m_cheatsUsedThisAttempt = false; }
    bool areCheatsActive() const;
    bool hasCheatedThisAttempt() const { return m_cheatsUsedThisAttempt; }

    bool isNoclipEnabled();
    bool isHitboxesEnabled();
    bool isSpeedhackEnabled();
    float getSpeedhackRate();
    bool isSafeModeEnabled();
    bool isPracticeMusicSyncEnabled();
};
