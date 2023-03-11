#ifndef ARA_NM_NM_CONFIG_READER_H_
#define ARA_NM_NM_CONFIG_READER_H_

#include <map>
#include <utility>

#include "../../../include/prototype.h"

//The implementation assume that only UdpNmCluster available in NmConfig
class NmConfigReader {
private:
    //map EthernetCommunicationConnector to Pair<cluster index in NmConfig, node index in NmNode>
    std::map<EtheretConmmunicationConnector, pair<int, int>> connectorToIndices;
    NmConfigReader(): connectorToIndices{} {}
    void initializeMaps() {
        for (int i = 0; i < nmConfig.nmCluster.size(); i++) {
            auto &cluster = nmConfig.nmCluster[i];
            for (int j = 0; j < cluster.nmNode.size(); j++) {
                auto *node = (UdpNmNode *)&cluster.nmNode[i];
                connectorToIndices[node -> communicationConnector] = make_pair(i, j);
            }
        }
    }
public:
    static NmConfigReader instance;

    UdpNmNode& operator[](EtheretConmmunicationConnector connector) {
        if (connectorToIndices.empty()) initializeMaps();
        auto indices = connectorToIndices[connector];
        return static_cast<UdpNmNode &>(nmConfig.nmCluster[indices.first].nmNode[indices.second]);
    }

    UdpNmCluster& operator[](UdpNmNode& node) {
        if (connectorToIndices.empty()) initializeMaps();
        auto indices = connectorToIndices[node.communicationConnector];
        return static_cast<UdpNmCluster &>(nmConfig.nmCluster[indices.first]);
    }
};



#endif
