#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <string_view>
#include <cmath>

namespace wwhrt {

// Event structures used by the BookBuilder implementation
// This file should not be modified.

using Symbol = std::array<char, 8>;

#pragma pack(push, 1)
enum MsgType : uint16_t {
    Unset,

    Add,
    Update,
    Delete,
};

struct BaseEvent {
    MsgType msgType;
    uint16_t msgSize;
    uint32_t seqNum;
};

template <class Msg> struct Event : BaseEvent {
    Event() : BaseEvent{Msg::MSGTYPE, sizeof(Msg), 0} {}
};

enum Side : uint8_t {
    Bid,
    Ask,
};

template <class Msg> struct CryptoOrder : Event<Msg> {
    CryptoOrder() {}

    Symbol symbol;
    double size;
    double price;
    uint64_t orderId;
    Side side;

    std::array<char, 3> filler = {};
};

struct CryptoAdd : public CryptoOrder<CryptoAdd> {
    static constexpr MsgType MSGTYPE = MsgType::Add;
};
struct CryptoUpdate : public CryptoOrder<CryptoUpdate> {
    static constexpr MsgType MSGTYPE = MsgType::Update;
};
struct CryptoDelete : public CryptoOrder<CryptoDelete> {
    static constexpr MsgType MSGTYPE = MsgType::Delete;
};
#pragma pack(pop)

} // namespace wwhrt

inline std::ostream& operator<<(std::ostream& os, const wwhrt::Symbol& symbol) {
    return os << std::string(symbol.data(), symbol.size()).c_str();
}

namespace std {
template <> struct hash<wwhrt::Symbol> {
    std::size_t operator()(const wwhrt::Symbol& sym) const {
        return std::hash<std::uint64_t>{}(
                *reinterpret_cast<const std::uint64_t*>(&sym));
    }
};
} // namespace std
