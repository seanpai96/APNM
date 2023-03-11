#ifndef ARA_COM_SET_HANDLER_H_
#define ARA_COM_SET_HANDLER_H_

#include <map>
#include "../core/vector.h"
#include "../core/future.h"
#include "../nm/NetworkState_Handle0Skeleton.hpp"
#include "../nm/NetworkStateType.hpp"
using ara::nm::skeleton::fields::NetworkRequestedState;
using ara::nm::NetworkStateType;

map<NetworkRequestedState *,
    ara::core::Vector<std::function<ara::core::Future<NetworkRequestedState::FieldType>(const NetworkRequestedState::FieldType& data)>>> handlers{};

#endif
