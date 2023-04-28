#include "NetworkState_Handle0Impl.hpp"
#include "nm.hpp"
#include "NmConfigReader.hpp"

#include <iostream>

using ara::nm::NetworkState_HandleImpl;

<template int HANDLE>
NetworkState_HandleImpl<HANDLE>::NetworkState_HandleImpl(
    ara::com::InstanceIdentifier instanceIdentifier
): NetworkState_Handle0Skeleton(instanceIdentifier, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

<template int HANDLE>
NetworkState_HandleImpl<HANDLE>::NetworkState_HandleImpl(
    ara::com::InstanceIdentifierContainer instanceIds
): NetworkState_Handle0Skeleton(instanceIds, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

<template int HANDLE>
NetworkState_HandleImpl<HANDLE>::NetworkState_HandleImpl(
    ara::core::InstanceSpecifier instanceSpec
): NetworkState_Handle0Skeleton(instanceSpec, ara::com::MethodCallProcessingMode::kEventSingleThread) {
    initialize();
}

ara::core::Vector<Machine *> machines;

<template int HANDLE>
NetworkState_HandleImpl<HANDLE>::~NetworkState_HandleImpl() {
    for (auto &machineThread: machines) {
        machineThread -> stateMachine -> StopInstance();
        delete machineThread -> stateMachine;
    }
}

ara::core::Future<ara::nm::NetworkStateType> networkRequestedStateSetHandler(ara::nm::NetworkStateType newValue) {
    for (auto &machineThread: machines) {
        //[SWS_ANM_00084], here we notify every network so they know whether they're requested
        machineThread -> stateMachine -> setRequested(newValue == ara::nm::NetworkStateType::kFullCom);
    }
    //no need to check, the validity is guaranteed by enum NetworkStateType
    auto promise = ara::core::Promise<ara::nm::NetworkStateType>();
    promise.set_value(newValue);
    return promise.get_future();
}

<template int HANDLE>
IStateMachine *NetworkState_HandleImpl<HANDLE>::createMachine(EtheretConmmunicationConnector *connector, std::function<void(bool)> &onStateChangeToNetwork) {
    auto node = configReader[connector];
    auto cluster = configReader[node];
    return new ara::nm::NMInstance(node, cluster, onStateChangeToNetwork);
}

<template int HANDLE>
int NetworkState_HandleImpl<HANDLE>::getEthernetConnectorNumber() {
    return handle.vlan.size();
}

<template int HANDLE>
void NetworkState_HandleImpl<HANDLE>::initialize() {
    NetworkRequestedState.RegisterSetHandler(networkRequestedStateSetHandler);
    for (int i = 0; i < this->getEthernetConnectorNumber(); i++) {
        Machine *machine = new Machine();
        machines.emplace_back(machine);
        auto &last = machines.back();
        last -> handle = this;
        last -> stateMachine = createMachine(handle.vlan[i], last -> machineStateChangeCallback);
    }

    for (auto &machine: machines) {
        machine -> stateMachine -> StartInstance();
    }

    OfferService();
}

<template int HANDLE>
void NetworkState_HandleImpl<HANDLE>::updateNetworkCurrentState() {
    //[SWS_ANM_00083], NetworkCurrentState should be kFullCom only if all associated networks are in Network Mode
    auto targetState = NetworkStateType::kFullCom;
    for (auto &machineThread: machines) {
        std::cout << &machineThread << " in network mode in update: " << machineThread -> machineInNetworkMode << std::endl;
        if (!machineThread -> machineInNetworkMode) {
            targetState = NetworkStateType::kNoCom;
            break;
        }
    }
    NetworkCurrentState.Update(targetState);
}
