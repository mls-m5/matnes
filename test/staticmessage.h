#pragma once

//! Display int value at compile time
template <int value>
class StaticMessage {
private: // This should fail
    StaticMessage() = default;
};
