#ifndef ARA_NM_NETWORK_STATE_HANDLE_0_IMPL_H_
#define ARA_NM_NETWORK_STATE_HANDLE_0_IMPL_H_

#include "NetworkState_Handle0Skeleton.hpp"
#include "../com/fakecom.hpp"
#include "../core/result.h"
#include "IStateMachine.hpp"

#include <functional>

struct Machine;

namespace ara {
    namespace nm {
        //You can see this Skeleton/Impl style in AUTOSAR_EXP_ARAComAPI 
        //Listing 5.13, Listing 5.15, Listing 5.16, Listing 5.17, Listing 5.19

        //Also in AUTOSAR_EXP_ARAComAPI 5.4.3 says that
        //  As you see in the example code of the RadarServiceSkeleton above, 
        //  the skeleton class, from which the service implementer has to subclass his service implementation, 
        //  provides....
        //clearly shows that this Skeleton/Impl style is required.

        //This class locates in ara::nm namespace since [SWS_ANM_91000] states that 
        //ara::nm is the namespace of NetworkState (the interface of our service)
        using skeleton::NetworkState_Handle0Skeleton;
        class NetworkState_Handle0Impl: public NetworkState_Handle0Skeleton {
            friend IStateMachine;
            public:
            //Here 3 types of constructors are all provided, but mode is not settable (we'll only use kEventSingleThread)
            //we're not going to deal with kEvent and KPoll
            NetworkState_Handle0Impl(ara::com::InstanceIdentifier instanceIdentifier);
            NetworkState_Handle0Impl(ara::com::InstanceIdentifierContainer instanceIds);
            NetworkState_Handle0Impl(ara::core::InstanceSpecifier instanceSpec);
            ~NetworkState_Handle0Impl();

            //the implementation also prohibit copy constructor and copy assignment
            NetworkState_Handle0Impl(const NetworkState_Handle0Impl& other) = delete;
            NetworkState_Handle0Impl& operator=(const NetworkState_Handle0Impl& other) = delete;

            void updateNetworkCurrentState();

            private:
            void initialize();
            IStateMachine createMachine(EtheretConmmunicationConnector *connector, std::function<void(bool)> &onStateChangeToNetwork);
            int getEthernetConnectorNumber();
        };
    }
}

struct Machine {
    IStateMachine stateMachine;
    ara::nm::NetworkState_Handle0Impl *handle;

    bool machineInNetworkMode = false;
    std::function<void(bool)> machineStateChangeCallback = [this](bool isToNetworkMode) {
        this -> machineInNetworkMode = isToNetworkMode;
        this -> handle -> updateNetworkCurrentState();
    };
};

#endif