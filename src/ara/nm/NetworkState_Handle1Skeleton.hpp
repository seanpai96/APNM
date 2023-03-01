#include "com/fakecom.hpp"
#include "core/result.h"
#include "core/instance_specifier.h"
#include "future.h"

namespace ara {
    namespace nm {
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
        };
    }
}