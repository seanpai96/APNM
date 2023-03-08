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
            NMInstanceState GetState() { return state; }

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
            void RestartTimeoutTimer();

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

        void NMInstance::RestartTimeoutTimer(){
            isNmTimeoutTimerRunning = true;
            nmTimeoutTimerTicks = 0;
        }

        void NMInstance::Tick() {
            _ticks++;
            if(state == NMInstanceState::NM_STATE_INIT){
                return;
            }else if(state == NMInstanceState::NM_STATE_BUS_SLEEP){                             //bus sleep
                if(networkState.RequestNetworkState == NetworkStateType::kFullCom){
                    //network requested, enter network mode
                    state = NMInstanceState::NM_STATE_REPEAT_MESSAGE;
                    isNmImmediateCycleTimerRunning = true;
                    isNmTimeoutTimerRunning = true;
                    nmTimeoutTimerTicks = 0;
                    nmImmediateCycleTimerTicks = 0;
                    immediateCycleTimes = cluster.nmImmediateNmTransmissions;
                }
                //bus sleep finish
            }else if(state == NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP){                     //prepare bus sleep
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
                //prepare bus sleep finish
            }else if(state == NMInstanceState::NM_STATE_REPEAT_MESSAGE){                        //repeat message
                if(isNmImmediateCycleTimerRunning){
                    if(immediateCycleTimes <= 0){
                        //immediate cycle times used up, start repeat message timer and message cycle timer
                        isNmImmediateCycleTimerRunning = false;
                        isNmRepeatMessageTimerRunning = true;
                        isNmMsgCycleTimerRunning = true;
                        nmImmediateCycleTimerTicks = 0;
                        nmRepeatMessageTimerTicks = 0;
                        nmMsgCycleTimerTicks = 0;
                    }else{
                        if(nmImmediateCycleTimerTicks >= cluster.nmImmediateNmCycleTime){
                            //send immediate message
                            node.SendNmMessage();
                            RestartTimeoutTimer();          //message sent, restart timeout timer
                            immediateCycleTimes--;
                            nmImmediateCycleTimerTicks = 0;
                        }else{
                            nmImmediateCycleTimerTicks++;
                        }
                    }
                }else if(isNmRepeatMessageTimerRunning){
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
                    }else{
                        nmRepeatMessageTimerTicks++;
                        if(isNmMsgCycleTimerRunning){
                            if(nmMsgCycleTimerTicks >= cluster.nmMsgCycleTime){
                                //send repeat message
                                node.SendNmMessage();  // message sent, restart timeout timer
                                RestartTimeoutTimer();
                                nmMsgCycleTimerTicks = 0;
                            }else{
                                nmMsgCycleTimerTicks++;
                            }
                        }
                    }
                    
                }
                if(isNmTimeoutTimerRunning){
                    if(nmTimeoutTimerTicks >= cluster.nmTimeoutTime){
                        RestartTimeoutTimer();
                    }else{
                        nmTimeoutTimerTicks++;
                    }
                }
                //repeat message finish
            }else if(state == NMInstanceState::NM_STATE_NORMAL_OPERATION){                      //normal operation
                if(networkState.RequestNetworkState == NetworkStateType::kNoCom){
                    //network not requested, enter ready sleep state
                    state = NMInstanceState::NM_STATE_READY_SLEEP;
                    isNmMsgCycleTimerRunning = false;
                    nmMsgCycleTimerTicks = 0;
                }else if(isNmMsgCycleTimerRunning){
                    if(nmMsgCycleTimerTicks >= cluster.nmMsgCycleTime){
                        //send Nm message
                        node.SendNmMessage();  // message sent, restart timeout timer
                        RestartTimeoutTimer();
                        nmMsgCycleTimerTicks = 0;
                    }else{
                        nmMsgCycleTimerTicks++;
                    }
                }
                if(isNmTimeoutTimerRunning){
                    if(nmTimeoutTimerTicks >= cluster.nmTimeoutTime){
                        RestartTimeoutTimer();
                    }else{
                        nmTimeoutTimerTicks++;
                    }
                }
                //normal operation finish
            }else if(state == NMInstanceState::NM_STATE_READY_SLEEP){
                if(networkState.RequestNetworkState == NetworkStateType::kFullCom){
                    //network requested, enter normal operation state
                    state = NMInstanceState::NM_STATE_NORMAL_OPERATION;
                    isNmMsgCycleTimerRunning = true;
                    nmMsgCycleTimerTicks = 0;
                }else if(isNmTimeoutTimerRunning){
                    if(nmTimeoutTimerTicks >= cluster.nmTimeoutTime){
                        //timeout, enter prepare bus sleep state
                        state = NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP;
                        isNmTimeoutTimerRunning = false;
                        nmTimeoutTimerTicks = 0;
                        isNmWaitBusSleepTimerRunning = true;
                        nmWaitBusSleepTimerTicks = 0;
                    }else{
                        nmTimeoutTimerTicks++;
                    }
                }
                //ready sleep finish
            }else if(state == NMInstanceState::NM_STATE_PREPARE_BUS_SLEEP){                     //prepare bus sleep
                if(networkState.RequestNetworkState == NetworkStateType::kFullCom){
                    //network requested, enter repeat message state
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
                //prepare bus sleep finish
            }
        }
    }  // namespace nm
}  // namespace ara
