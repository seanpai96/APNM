#include "../include/prototype.h"
#include "ara/nm/NetworkState_Handle0Impl.hpp"
#include "ara/com/com_set_handler.hpp"

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
    cluster.nmNode = vector<NmNode>{ node0, node1 };

    nmConfig.nmCluster = vector<NmCluster>{ cluster };

    nmInstantiation.networkHandle = vector<NmNetworkHandle>{
            {{}, {&connector0}}
    };

    //handle started
    ara::nm::NetworkState_Handle0Impl networkHandle(ara::com::InstanceIdentifier{});
    for (auto &handler: handlers[&networkHandle.NetworkRequestedState]) {
        handler(ara::nm::NetworkStateType::kFullCom);
    }

    this_thread::sleep_for(5000ms);

    for (auto &handler: handlers[&networkHandle.NetworkRequestedState]) {
        handler(ara::nm::NetworkStateType::kNoCom);
    }

    networkHandle.StopOfferService();
}
