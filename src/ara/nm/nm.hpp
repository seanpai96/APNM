#ifndef ARA_NM_NM_H_
#define ARA_NM_NM_H_


#include <cstdint>

namespace ara{
    namespace nm{
        enum class NMInstanceState: uint8_t{
            NM_STATE_INIT,
            NM_STATE_BUS_SLEEP,
            NM_STATE_PREPARE_BUS_SLEEP,
            NM_STATE_REPEAT_MESSAGE,
            NM_STATE_NORMAL_OPERATION,
            NM_STATE_READY_SLEEP
        };
    }
}

#endif