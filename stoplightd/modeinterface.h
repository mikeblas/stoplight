#pragma once

#include <daemonize/syslog.hpp>

#include "lights.h"


enum ModeID {
    NO_MODE = -1,
    SELECTOR_MODE = 0,
    REGULAR_MODE = 1,
    REMO_MODE = 2,
    FLICKER_MODE = 3,
    CLOCK_MODE = 4,
};

class ModeInterface
{

protected:
    Lights& lights;
    daemonize::logger& log;
    bool quitting = false;

public:
    ModeInterface(Lights& lights, daemonize::logger& log)
       : lights(lights), log(log)
    {
    }

    virtual std::string GetModeName() = 0;
    virtual ModeID GetModeID() = 0;

    virtual ~ModeInterface() = default;

    virtual void operator()() = 0;
    virtual void Shutdown() = 0;

    // -1 if this mode wants to keep running
    // otherwise, a mode number to switch to that mode
    virtual ModeID NewMode()
    {
        return NO_MODE;
    }

    // this mode has quit running (exited)
    bool IsQuitting() const
    {
        return quitting;
    }

    enum ButtonResult {
        NOTHING,
        CHANGE_MODE,
        QUIT_MODE,
    };

    virtual ButtonResult OnAButtonPressed()  { return ButtonResult::NOTHING; }
    virtual ButtonResult OnAButtonReleased() { return ButtonResult::NOTHING; }
    virtual ButtonResult OnBButtonPressed()  { return ButtonResult::NOTHING; }
    virtual ButtonResult OnBButtonReleased() { return ButtonResult::NOTHING; }
    virtual ButtonResult OnCButtonPressed()  { return ButtonResult::NOTHING; }
    virtual ButtonResult OnCButtonReleased() { return ButtonResult::NOTHING; }
    virtual ButtonResult OnDButtonPressed()  { return ButtonResult::NOTHING; }
    virtual ButtonResult OnDButtonReleased() { return ButtonResult::NOTHING; }
};

