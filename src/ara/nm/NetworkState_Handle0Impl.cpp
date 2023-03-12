#include "NetworkState_Handle0Impl.hpp"
#include "nm.hpp"
#include "NmConfigReader.hpp"

using ara::nm::NetworkState_Handle0Impl;

NetworkState_Handle0Impl::NetworkState_Handle0Impl(
    ara::com::InstanceIdentifier instanceIdentifier
): NetworkState_Handle0Skeleton(instanceIdentifier, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

NetworkState_Handle0Impl::NetworkState_Handle0Impl(
    ara::com::InstanceIdentifierContainer instanceIds
): NetworkState_Handle0Skeleton(instanceIds, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

NetworkState_Handle0Impl::NetworkState_Handle0Impl(
    ara::core::InstanceSpecifier instanceSpec
): NetworkState_Handle0Skeleton(instanceSpec, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

ara::core::Vector<Machine> machines;

NetworkState_Handle0Impl::~NetworkState_Handle0Impl() {
    for (auto &machineThread: machines) {
        machineThread.stateMachine.StopInstance();
    }
}

ara::core::Future<ara::nm::NetworkStateType> networkRequestedStateSetHandler(ara::nm::NetworkStateType newValue) {
    for (auto &machineThread: machines) {
        //[SWS_ANM_00084], here we notify every network so they know whether they're requested
        machineThread.stateMachine.setRequested(newValue == ara::nm::NetworkStateType::kFullCom);
    }
    //no need to check, the validity is guaranteed by enum NetworkStateType
    auto promise = ara::core::Promise<ara::nm::NetworkStateType>();
    promise.set_value(newValue);
    return promise.get_future();
}

IStateMachine NetworkState_Handle0Impl::createMachine(EtheretConmmunicationConnector *connector, std::function<void(bool)> &onStateChangeToNetwork) {
    auto &node = NmConfigReader::instance[connector];
    auto &cluster = NmConfigReader::instance[node];
    return ara::nm::NMInstance(node, cluster, onStateChangeToNetwork);
}

int NetworkState_Handle0Impl::getEthernetConnectorNumber() {
    return handle.vlan.size();
}

void NetworkState_Handle0Impl::initialize() {
    NetworkRequestedState.RegisterSetHandler(networkRequestedStateSetHandler);
    for (int i = 0; i < this->getEthernetConnectorNumber(); i++) {
        machines.emplace_back(Machine());
        auto &last = machines.back();
        last.handle = this;
        last.stateMachine = createMachine(handle.vlan[i], last.machineStateChangeCallback);
    }

    for (auto &machine: machines) {
        machine.stateMachine.StartInstance();
    }

    OfferService();
}

void NetworkState_Handle0Impl::updateNetworkCurrentState() {
    //[SWS_ANM_00083], NetworkCurrentState should be kFullCom only if all associated networks are in Network Mode
    auto targetState = NetworkStateType::kFullCom;
    for (auto &machineThread: machines) {
        if (!machineThread.machineInNetworkMode) {
            targetState = NetworkStateType::kNoCom;
            break;
        }
    }
    NetworkCurrentState.Update(targetState);
}
