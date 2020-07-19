#pragma once

//! Display int value at compile time
//! When trying to build (or static analysi) the compiler will emit a warning
//! containing the message
template <int value>
class StaticMessage {
private: // This should fail
    StaticMessage() = default;
};
