#include "NetworkState_Handle0Skeleton.hpp"
#include "../com/com_set_handler.hpp"

//com impelementaitons are minimized, most of them are ignored
//These content should be generated, but sadly we don't have a generator
using ara::nm::skeleton::NetworkState_Handle0Skeleton;
//constructors
NetworkState_Handle0Skeleton::NetworkState_Handle0Skeleton(
    ara::com::InstanceIdentifier instanceId,
    ara::com::MethodCallProcessingMode mode
) { }

NetworkState_Handle0Skeleton::NetworkState_Handle0Skeleton(
    ara::com::InstanceIdentifierContainer instanceIds,
    ara::com::MethodCallProcessingMode mode
) { }

NetworkState_Handle0Skeleton::NetworkState_Handle0Skeleton(
    ara::core::InstanceSpecifier instanceSpec,
    ara::com::MethodCallProcessingMode mode
) { }

ara::core::Result<NetworkState_Handle0Skeleton> NetworkState_Handle0Skeleton::Create(
    const ara::com::InstanceIdentifier &instanceID,
    ara::com::MethodCallProcessingMode mode
) noexcept {
    return ara::core::Result<NetworkState_Handle0Skeleton>(
            NetworkState_Handle0Skeleton(instanceID, mode)
    );
}

ara::core::Result<NetworkState_Handle0Skeleton> NetworkState_Handle0Skeleton::Create(
    const ara::com::InstanceIdentifierContainer &instanceIDs,
    ara::com::MethodCallProcessingMode mode
) noexcept {
    return ara::core::Result<NetworkState_Handle0Skeleton>(
            NetworkState_Handle0Skeleton(instanceIDs, mode)
    );
}

ara::core::Result<NetworkState_Handle0Skeleton> NetworkState_Handle0Skeleton::Create(
    const ara::core::InstanceSpecifier &instanceSpec,
    ara::com::MethodCallProcessingMode mode
) noexcept {
    return ara::core::Result<NetworkState_Handle0Skeleton>(
            NetworkState_Handle0Skeleton(instanceSpec, mode)
    );
}

ara::nm::skeleton::NetworkState_Handle0Skeleton::~NetworkState_Handle0Skeleton() { }

ara::core::Result<void> NetworkState_Handle0Skeleton::OfferService() {
    //By AUTOSAR_EXP_ARAComAPI 5.4.8.3, OfferService should return error if there's any field without SetHandler
    //Here the check is ignored, since it's an ara::com feature, rather than ara::nm
    return ara::core::Result<void>();
}

void NetworkState_Handle0Skeleton::StopOfferService() { }

ara::core::Future<bool> NetworkState_Handle0Skeleton::ProcessNextMethodCall() {
    auto promise = ara::core::Promise<bool>();
    //always return false here, we only use kEvent mode
    promise.set_value(false);
    return promise.get_future();
}

using ara::nm::skeleton::fields::NetworkCurrentState;
ara::core::Result<void> NetworkCurrentState::Update(
    const NetworkCurrentState::FieldType& data
) {
    //though a service should update the value via this method, there's no way we can obtain value from ara::com
    //if knowing the current value is necessary, the implementation should keep a relica itself
    //AUTOSAR_EXP_ARAComAPI 5.4.8.5

    //since we don't have a real ara::com, a plain return is provided
    return ara::core::Result<void>();
}

ara::core::Result<void> NetworkCurrentState::RegisterGetHandler(
    std::function<ara::core::Future<NetworkCurrentState::FieldType>()> getHandler
) {
    //left empty
    //this feature will not be used by the implementation of this skeleton
    return ara::core::Result<void>();
}

using ara::nm::skeleton::fields::NetworkRequestedState;
ara::core::Result<void> NetworkRequestedState::Update(
    const NetworkRequestedState::FieldType& data
) {
    //though a service should update the value via this method, there's no way we can obtain value from ara::com
    //if knowing the current value is necessary, the implementation should keep a relica itself
    //AUTOSAR_EXP_ARAComAPI 5.4.8.5

    //since we don't have a real ara::com, a plain return is provided
    return ara::core::Result<void>();
}

ara::core::Result<void> NetworkRequestedState::RegisterGetHandler(
    std::function<ara::core::Future<NetworkRequestedState::FieldType>()> getHandler
) {
    //left empty
    //this feature will not be used by the implementation of this skeleton
    return ara::core::Result<void>();
}

ara::core::Result<void> NetworkRequestedState::RegisterSetHandler(
    std::function<ara::core::Future<NetworkRequestedState::FieldType>(const NetworkRequestedState::FieldType& data)> setHandler
) {
    //we need to implement this since SetHandler is mandatory, and we're going to use Set Handler in skeleton implementation
    //AUTOSAR_EXP_ARAComAPI 5.4.8.2
    //here we just put the handler into a global map, the real ara::com implementation should replace this
    if (handlers.find(this) == handlers.end()) {
        handlers[this] = {};
    }
    handlers[this].emplace_back(setHandler);
    return ara::core::Result<void>();
}