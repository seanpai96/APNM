#include <vector>
using namespace std;

class AdaptivePlatformServiceInstance{

};
/*              Physical Area               */

class EthernetPhysicalChannel{

};

class PhysicalChannel{

};

class EtheretConmmunicationConnector{

};

class MachineDesign{

};

/*              Pnc Area                */

class PncMappingIdent{                          //this is only a referrable identifier

};

class PncMapping{
    public:
        PncMappingIdent ident;
        PhysicalChannel physicalChannel;        //map pnc to physical channel
        unsigned int pncIdentifier;
        bool pncWakeupEnable;
        AdaptivePlatformServiceInstance serviceInstance;
        string identifier;                      //pnc name
};



/*              Virtual Area                */

class CommunicationCluster{
    
};

class NmNode{                                   //this is abstract alse, as NmCluster
    public:
        MachineDesign machine;                  //reference to the machine which contain this node
        int nmNodeId;                   
};

class UdpNmNode : public NmNode{
    public:
        bool allNmMessagesKeepAwake;            //Specifies if Nm drops irrelevant NM PDUs
        EtheretConmmunicationConnector communicationConnector;
        time_t nmMsgCycleOffset;                //Node specific time offset in the periodic transmission node
};

class NmCluster{                                //this class is abstract
    public:
        vector<NmNode> nmNode;                  //collections of mnNodes in this cluster
        CommunicationCluster communicationCluster;
        bool nmPncParticipation = false;        //currently mo use
        unsigned int pncClusterVectorLength;    //currently no use
};

class UdpNmNetworkConfiguration{                //Configuration of a UDP port and UDP multicast IP address of the Nm communication on a VLAN
    public:
        string ipv4MulticastipAaddress;
        string ipv6MulticastipAaddress;
        unsigned int priority;                  //defines VLAN frmae priority
        unsigned int udpPort;
};

class UdpNmCluster : public NmCluster{
    public:
        UdpNmNetworkConfiguration networkConfiguration;
        int nmCbvPosition;                      //Defines the position of the control bit vector within the Nm Pdu (Byte positon).
        time_t nmMsgCycleTime;                  //Period of a NmPdu in seconds
        time_t nmNetworkTimeout;                //Network Timeout for NmPdus in seconds
        int nmNidPosition;                      //Defines the byte position of the source node identifier within the NmPdu
        time_t nmRepeatMessageTime;             //Timeout for Repeat Message State in seconds
        int nmUserDataLength;                   //Defines the length in bytes of the user data contained in the Nm message
        unsigned int nmUserDataOffset;          //Specifies the offset (in bytes) of the user data information in the NM message
        time_t nmWaitBusSleepTime;              //Timeout for bus calm down phase in seconds
        vector<EthernetPhysicalChannel> vlan;           //Reference to vlan
};

class NmConfig{
    public:
        vector<NmCluster> nmCluster;            //collection of mnClusters
};


class NmNetworkHandle{
    public:
        PncMappingIdent partialNetwork;         //reference to a pnc that included in this handle
        EtheretConmmunicationConnector vlan;    //reference to a vlan that included in this handle
};

class NmInstantiation{
    public:
        NmNetworkHandle networkHandle;          //supported handles used to control PNC
};

/*              Declaration             */

NmInstantiation nmInstantiation;                
NmConfig nmConfig;
