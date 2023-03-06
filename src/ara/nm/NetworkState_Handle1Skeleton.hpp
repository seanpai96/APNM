#include "com/fakecom.hpp"
#include "core/result.h"
#include "core/instance_specifier.h"
#include "core/future.h"
#include "NetworkStateType.hpp"

#include <vector>

namespace ara {
    namespace nm {
        namespace skeleton {
            //Adapted from AUTOSAR_EXP_ARAComAPI Listing 5.12
            //The content of this class should be generated, but sadly we don't have a generator
            class NetworkState_Handle1Sketelon {
                public:
                //constructors
                NetworkState_Handle1Sketelon(
                    ara::com::InstanceIdentifier instanceId,
                    ara::com::MethodCallProcessingMode mode = 
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle1Sketelon> Create(
                    const ara::com::InstanceIdentifier &instanceID,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;


                NetworkState_Handle1Sketelon(
                    ara::com::InstanceIdentifierContainer instanceIds,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle1Sketelon> Create(
                    const ara::com::InstanceIdentifierContainer &instanceIDs,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;


                NetworkState_Handle1Sketelon(
                    ara::core::InstanceSpecifier instanceSpec,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                );

                static ara::core::Result<NetworkState_Handle1Sketelon> Create(
                    const ara::core::InstanceSpecifier &instanceSpec,
                    ara::com::MethodCallProcessingMode mode =
                        ara::com::MethodCallProcessingMode::kEvent
                ) noexcept;

                //prohibited constructor/operator
                NetworkState_Handle1Sketelon(const NetworkState_Handle1Sketelon& other) = delete;
                NetworkState_Handle1Sketelon& operator=(const NetworkState_Handle1Sketelon& other) = delete;

                //destructor
                ~NetworkState_Handle1Sketelon();

                ara::core::Result<void> OfferService();
                void StopOfferService();
                ara::core::Future<bool> ProcessNextMethodCall();

                fields::NetworkCurrentState NetworkCurrentState;
                fields::NetworkRequestedState NetworkRequestedState;
            };

            namespace fields {
                class NetworkCurrentState {
                    public:
                    using FieldType = NetworkStateType;

                    ara::core::Result<void> Update(const FieldType& data);
                    ara::core::Result<void> RegisterGetHandler(
                        std::function<ara::core::Future<FieldType>()> getHandler
                    );
                    //RegisterSetHalder isn't available here since NetworkCurrentState has no setter
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
                    std::vector<std::function<ara::core::Future<FieldType>(const FieldType* data)>> setHandlers;
                };
            }
        }
    }
}