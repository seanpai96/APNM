#ifndef ARA_NM_NETWORK_STATE_HANDLE_0_SKELETON_H_
#define ARA_NM_NETWORK_STATE_HANDLE_0_SKELETON_H_

#include "../com/fakecom.hpp"
#include "../core/result.h"
#include "../core/instance_specifier.h"
#include "../core/future.h"
#include "../core/future.h"
#include "../core/vector.h"
#include "NetworkStateType.hpp"
#include "../../../include/prototype.h"

#include <functional>

namespace ara {
    namespace nm {
        namespace skeleton {
            //Adapted from AUTOSAR_EXP_ARAComAPI Listing 5.12
            //The content of these classes should be generated, but sadly we don't have a generator
            namespace fields {
                class NetworkCurrentState {
                public:
                    using FieldType = NetworkStateType;

                    ara::core::Result<void> Update(const FieldType& data);
                    ara::core::Result<void> RegisterGetHandler(
                            std::function<ara::core::Future<FieldType>()> getHandler
                    );
                    //RegisterSetHandler isn't available here since NetworkCurrentState has no setter
                };

                class NetworkRequestedState {
                public:
                    using FieldType = NetworkStateType;

                    ara::core::Result<void> Update(const FieldType& data);
                    ara::core::Result<void> RegisterGetHandler(
                            std::function<ara::core::Future<FieldType>()> getHandler
                    );
                    ara::core::Result<void> RegisterSetHandler(
                            std::function<ara::core::Future<FieldType>(const FieldType& data)> setHandler
                    );
                };
            }

            class NetworkState_Handle0Skeleton {
            public:
                //constructors
                NetworkState_Handle0Skeleton(
                    ara::com::InstanceIdentifier instanceId,
                    ara::com::MethodCallProcessingMode mode = 
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle0Skeleton> Create(
                    const ara::com::InstanceIdentifier &instanceID,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;


                NetworkState_Handle0Skeleton(
                    ara::com::InstanceIdentifierContainer instanceIds,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle0Skeleton> Create(
                    const ara::com::InstanceIdentifierContainer &instanceIDs,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;


                NetworkState_Handle0Skeleton(
                    ara::core::InstanceSpecifier instanceSpec,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle0Skeleton> Create(
                    const ara::core::InstanceSpecifier &instanceSpec,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;

                //prohibited constructor/operator
                NetworkState_Handle0Skeleton(const NetworkState_Handle0Skeleton& other) = delete;
                NetworkState_Handle0Skeleton& operator=(const NetworkState_Handle0Skeleton& other) = delete;

                //destructor
                ~NetworkState_Handle0Skeleton();

                ara::core::Result<void> OfferService();
                void StopOfferService();
                ara::core::Future<bool> ProcessNextMethodCall();

                fields::NetworkCurrentState NetworkCurrentState;
                fields::NetworkRequestedState NetworkRequestedState;
            protected:
                //This file should be generated, so corresponding NmNetworkHandle is hard-coded here
                NmNetworkHandle &handle = nmInstantiation.networkHandle[0];
            };
        }
    }
}

#endif