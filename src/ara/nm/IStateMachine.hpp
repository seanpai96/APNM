#ifndef ARA_NM_I_STATE_MACHINE_H_
#define ARA_NM_I_STATE_MACHINE_H_

#include <functional>

class IStateMachine {
public:
    virtual void StartInstance();
    virtual void StopInstance();
    virtual void setRequested(bool requested);
};

#endif
