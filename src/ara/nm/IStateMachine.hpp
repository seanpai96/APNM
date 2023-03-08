#ifndef ARA_NM_I_STATE_MACHINE_H_
#define ARA_NM_I_STATE_MACHINE_H_

#include <functional>

class IStateMachine {
    virtual void StartInstance(std::function<void(bool)> onStateChangeToNetwork);
    virtual void setRequested(bool requested);
};

#endif
