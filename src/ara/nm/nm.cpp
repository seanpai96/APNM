#include "nm.hpp"

#include <bits/stdc++.h>

#include "CallbackTimer.cpp"

namespace ara {
    namespace nm {
        class NMInstance {
            NMNetworkHandle &handle;
            UdpNmNode &node;
            UdpNmCluster &cluster;
            CallbackTimer timer;

           public:
            NMInstance(NMNetworkHandle &handle, UdpNmNode &node, UdpNmCluster &cluster);
            void StartInstance();
            void StopInstance();
            void Tick();

           private:
            int _ticks = 0;
        };

        NMInstance::NMInstance(NMNetworkHandle &handle, UdpNmNode &node,
                               UdpNmCluster &cluster)
            : handle(handle), node(node), cluster(cluster) {}

        void NMInstance::StartInstance() {
            timer.start(1000, [this]() { this->Tick(); });
        }

        void NMInstance::StopInstance() { timer.stop(); }

        void NMInstance::Tick() {
        }
    }  // namespace nm
}  // namespace ara
