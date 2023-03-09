#include "NetworkState_Handle1Impl.hpp"

using ara::nm::NetworkState_Handle1Impl;

NetworkState_Handle1Impl::NetworkState_Handle1Impl(
    ara::com::InstanceIdentifier instanceIdentifier
): NetworkState_Handle1Skeleton(instanceIdentifier, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

NetworkState_Handle1Impl::NetworkState_Handle1Impl(
    ara::com::InstanceIdentifierContainer instanceIds
): NetworkState_Handle1Skeleton(instanceIds, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

NetworkState_Handle1Impl::NetworkState_Handle1Impl(
    ara::core::InstanceSpecifier instanceSpec
): NetworkState_Handle1Skeleton(instanceSpec, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

ara::core::Vector<Machine> machineThreads;

NetworkState_Handle1Impl::~NetworkState_Handle1Impl() {
    for (auto &machineThread: machineThreads) {
        machineThread.machine.StopInstance();
    }
}

ara::core::Future<ara::nm::NetworkStateType> networkRequestedStateSetHandler(ara::nm::NetworkStateType newValue) {
    for (auto &machineThread: machineThreads) {
        machineThread.machine.setRequested(newValue == ara::nm::NetworkStateType::kFullCom);
    }
    //no need to check, the validity is guaranteed by enum NetworkStateType
    auto promise = ara::core::Promise<ara::nm::NetworkStateType>();
    promise.set_value(newValue);
    return promise.get_future();
}

IStateMachine createMachine() {

}

int NetworkState_Handle1Impl::getEthernetConnectorNumber() {

}

void NetworkState_Handle1Impl::initialize() {
    NetworkRequestedState.RegisterSetHandler(networkRequestedStateSetHandler);
    for (int i = 0; i < this->getEthernetConnectorNumber(); i++) {
        machineThreads.emplace_back(Machine{
                .machine = createMachine(),
                .handle = this
        });
    }

    for (auto &machineThread: machineThreads) {
        machineThread.machine.StartInstance(machineThread.machineStateChangeCallback);
    }

    OfferService();
}

void NetworkState_Handle1Impl::updateNetworkCurrentState() {
    auto targetState = NetworkStateType::kFullCom;
    for (auto &machineThread: machineThreads) {
        if (!machineThread.machineInNetworkMode) {
            targetState = NetworkStateType::kNoCom;
            break;
        }
    }
    NetworkCurrentState.Update(targetState);
}