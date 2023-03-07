#include "../com/fakecom.hpp"
#include "../core/result.h"
#include "../core/instance_specifier.h"
#include "../core/future.h"
#include "../core/future.h"
#include "../core/vector.h"
#include "NetworkStateType.hpp"

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

                private:
                    ara::core::Vector<std::function<ara::core::Future<FieldType>(const FieldType& data)>> setHandlers;
                };
            }

            class NetworkState_Handle1Skeleton {
            public:
                //constructors
                NetworkState_Handle1Skeleton(
                    ara::com::InstanceIdentifier instanceId,
                    ara::com::MethodCallProcessingMode mode = 
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle1Skeleton> Create(
                    const ara::com::InstanceIdentifier &instanceID,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;


                NetworkState_Handle1Skeleton(
                    ara::com::InstanceIdentifierContainer instanceIds,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle1Skeleton> Create(
                    const ara::com::InstanceIdentifierContainer &instanceIDs,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;


                NetworkState_Handle1Skeleton(
                    ara::core::InstanceSpecifier instanceSpec,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle1Skeleton> Create(
                    const ara::core::InstanceSpecifier &instanceSpec,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;

                //prohibited constructor/operator
                NetworkState_Handle1Skeleton(const NetworkState_Handle1Skeleton& other) = delete;
                NetworkState_Handle1Skeleton& operator=(const NetworkState_Handle1Skeleton& other) = delete;

                //destructor
                ~NetworkState_Handle1Skeleton();

                ara::core::Result<void> OfferService();
                void StopOfferService();
                ara::core::Future<bool> ProcessNextMethodCall();

                fields::NetworkCurrentState NetworkCurrentState;
                fields::NetworkRequestedState NetworkRequestedState;
            };
        }
    }
}