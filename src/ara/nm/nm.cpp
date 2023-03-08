#include "nm.hpp"

#include <bits/stdc++.h>

#include "CallbackTimer.cpp"
#include "NetworkStateType.hpp"

namespace ara {
    namespace nm {
        class NMInstance {
            NMNetworkHandle &handle;
            NMNetworkState &networkState;          //assume
            UdpNmNode &node;
            UdpNmCluster &cluster;
            CallbackTimer timer;
            NMInstanceState state = NMInstanceState::NM_STATE_INIT;
            std::function<void(bool)> onStateChangeToNetwork;

           public:
            NMInstance(NMNetworkHandle &handle, UdpNmNode &node, UdpNmCluster &cluster, NMNetworkState &networkState, std::function<void(bool)> onStateChangeToNetwork);
            void StartInstance();
            void StopInstance();
            void Tick();
            void Reset();

           private:
            int _ticks = 0;
            bool isNmTimeoutTimerRunning = false;
            bool isNmRepeatMessageTimerRunning = false;
            bool isNmMsgCycleTimerRunning = false;
            bool isNmImmediateCycleTimerRunning = false;
            bool isNmWaitBusSleepTimerRunning = false;
            int nmTimeoutTimerTicks = 0;
            int nmRepeatMessageTimerTicks = 0;
            int nmMsgCycleTimerTicks = 0;
            int nmImmediateCycleTimerTicks = 0;
            int immediateCycleTimes = 0;
            int nmWaitBusSleepTimerTicks = 0;

        };

        NMInstance::NMInstance(NMNetworkHandle &handle, UdpNmNode &node,
                               UdpNmCluster &cluster, NMNetworkState &networkState, std::function<void(bool)> onStateChangeToNetwork)
            : handle(handle), node(node), cluster(cluster), networkState(networkState), onStateChangeToNetwork(onStateChangeToNetwork){}

        void NMInstance::StartInstance() {
            this->Reset();
            state = NMInstanceState::NM_STATE_BUS_SLEEP;
            timer.start(1000, [this]() { this->Tick(); });
        }

        void NMInstance::StopInstance() {
            timer.stop(); 
            state = NMInstanceState::NM_STATE_INIT;    
        }

        void NMInstance::Reset(){
            isNmTimeoutTimerRunning = false;
            isNmRepeatMessageTimerRunning = false;
            isNmMsgCycleTimerRunning = false;
            isNmImmediateCycleTimerRunning = false;
            isNmWaitBusSleepTimerRunning = false;
            nmTimeoutTimerTicks = 0;
            nmRepeatMessageTimerTicks = 0;
            nmMsgCycleTimerTicks = 0;
            nmImmediateCycleTimerTicks = 0;
            nmWaitBusSleepTimerTicks = 0;
        }

        void NMInstance::Tick() {
            _ticks++;
            if(state == NMInstanceState::NM_STATE_INIT){
                return;
            }else if(state == NMInstanceState::NM_STATE_BUS_SLEEP){
                if(networkState.RequestNetworkState == NetworkStateType::kFullCom){
                    //network requested, enter network mode
                    state = NMInstanceState::NM_STATE_REPEAT_MESSAGE;
                    isNmImmediateCycleTimerRunning = true;
                    isNmTimeoutTimerRunning = true;
                    nmTimeoutTimerTicks = 0;
                    nmImmediateCycleTimerTicks = 0;
                    immediateCycleTimes = cluster.nmImmediateNmTransmissions;
                }
            }else if(state == NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP){
                if(networkState.RequestNetworkState == NetworkStateType::kFullCom){
                    //network requested, enter network mode
                    state = NMInstanceState::NM_STATE_REPEAT_MESSAGE;
                    isNmImmediateCycleTimerRunning = true;
                    isNmTimeoutTimerRunning = true;
                    nmTimeoutTimerTicks = 0;
                    nmImmediateCycleTimerTicks = 0;
                    immediateCycleTimes = cluster.nmImmediateNmTransmissions;
                }else if(isNmWaitBusSleepTimerRunning){
                    if(nmWaitBusSleepTimerTicks >= cluster.nmWaitBusSleepTime){
                        //wait bus sleep timeout, enter bus sleep mode
                        state = NMInstanceState::NM_STATE_BUS_SLEEP;
                        isNmWaitBusSleepTimerRunning = false;
                        nmWaitBusSleepTimerTicks = 0;
                    }else{
                        nmWaitBusSleepTimerTicks++;
                    }
                }
            }else if(state == NMInstanceState::NM_STATE_REPEAT_MESSAGE){
                if(isNmImmediateCycleTimerRunning){
                    if(immediateCycleTimes <= 0){
                        //immediate cycle times used up, start repeat message timer
                        isNmImmediateCycleTimerRunning = false;
                        isNmRepeatMessageTimerRunning = true;
                        nmImmediateCycleTimerTicks = 0;
                        nmRepeatMessageTimerTicks = 0;
                    }else{
                        if(nmImmediateCycleTimerTicks >= cluster.nmImmediateNmCycleTime){
                            //send immediate message
                            node.SendImmediateMessage();
                            immediateCycleTimes--;
                            nmImmediateCycleTimerTicks = 0;
                        }else{
                            nmImmediateCycleTimerTicks++;
                        }
                    }
                }if(isNmRepeatMessageTimerRunning){
                    if(nmRepeatMessageTimerTicks >= cluster.nmRepeatMessageTime){
                        //repeat message timeout, check request network state
                        isNmRepeatMessageTimerRunning = false;
                        nmRepeatMessageTimerTicks = 0;
                        if(networkState.RequestNetworkState == NetworkStateType::kNoCom){
                            //network not requested, enter ready sleep state
                            state = NMInstanceState::NM_STATE_READY_SLEEP;
                            isNmMsgCycleTimerRunning = false;
                            nmMsgCycleTimerTicks = 0;
                        }else if(networkState.RequestNetworkState == NetworkStateType::kFullCom){
                            //network requested, enter normal operation state
                            state = NMInstanceState::NM_STATE_NORMAL_OPERATION;

                        }
                    }else if(nmMsgCycleTimerTicks >= cluster.nmMsgCycleTime){
                        //send repeat message
                        node.SendRepeatMessage();
                        nmMsgCycleTimerTicks = 0;
                    }else{
                        nmMsgCycleTimerTicks++;
                    }
                }
            }
        }
    }  // namespace nm
}  // namespace ara
