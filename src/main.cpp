#include "../include/prototype.h"
#include "ara/nm/NetworkState_Handle0Impl.hpp"
#include "ara/com/com_set_handler.hpp"

#include <chrono>
#include <iostream>

int main() {
    EtheretConmmunicationConnector connector0;
    UdpNmNode node0{};
    node0.nmNodeId = 0;
    node0.machine = {};
    node0.allNmMessagesKeepAwake = true;
    node0.communicationConnector = &connector0;
    node0.nmMsgCycleOffset = 1;


    EtheretConmmunicationConnector connector1;
    UdpNmNode node1{};
    node1.nmNodeId = 1;
    node1.machine = {};
    node1.allNmMessagesKeepAwake = true;
    node1.communicationConnector = &connector1;
    node1.nmMsgCycleOffset = 1;

    UdpNmCluster cluster{};
    cluster.nmCbvPosition = 0;
    cluster.nmImmediateNmCycleTime = 3;
    cluster.nmImmediateNmTransmissions = 0;
    cluster.nmMsgCycleTime = 5;
    cluster.nmNetworkTimeout = 20;
    cluster.nmNidPosition = 1;
    cluster.nmPncParticipation = false;
    cluster.nmRepeatMessageTime = 10;
    cluster.nmUserDataLength = 0;
    cluster.nmUserDataOffset = 3;
    cluster.nmWaitBusSleepTime = 20;
    cluster.pncClusterVectorLength = 0;

    //set muticast IP address
    cluster.networkConfiguration.ipv4MulticastipAaddress = "224.0.0.6";


    //cluster.nmNode = vector<NmNode *>{ &node0, &node1 };
    cluster.nmNode.push_back(&node0);
    cluster.nmNode.push_back(&node1);

    nmConfig.nmCluster = vector<NmCluster *>{ &cluster };

    nmInstantiation.networkHandle = vector<NmNetworkHandle>{
            {{}, {&connector0}},
            {{}, {&connector1}}
    };

    //handle started
    ara::nm::NetworkState_HandleImpl networkHandle(ara::com::InstanceIdentifier{}, 0);
    for (auto &handler: handlers[&networkHandle.NetworkRequestedState]) {
//        handler(ara::nm::NetworkStateType::kFullCom);
    }

    int i;
    std::cin >> i;

    for (auto &handler: handlers[&networkHandle.NetworkRequestedState]) {
        handler(ara::nm::NetworkStateType::kNoCom);
    }

    networkHandle.StopOfferService();
}
