
#include "Events.h"
#include "Reader.h"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace wwhrt {

int32_t Reader::breakOnSeqNum = -1;

Reader::Reader(const char* filePath) : fp{std::fopen(filePath, "r")} {
    if (fp == nullptr) {
        std::string err = "Error while opening file ";
        err += filePath;
        err += ": ";
        err += std::strerror(errno);
        throw std::runtime_error(err);
    }
}

Reader::~Reader() {
    std::fclose(fp);
}

void Reader::run() {
    static constexpr size_t ReadSize = 64 * 1024;
    std::byte buf[ReadSize];

    size_t msgOffset = 0;
    while (true) {
        const size_t bytesThisRead = ReadSize - msgOffset;
        const size_t bytesRead =
                std::fread(buf + msgOffset, 1, bytesThisRead, fp);
        const std::byte* endptr = buf + msgOffset + bytesRead;
        auto current = reinterpret_cast<const BaseEvent*>(buf);
        auto bPtr = reinterpret_cast<const std::byte*>(current);
        while (bytesRead > 0 && bPtr + sizeof(uint16_t) <= endptr &&
               bPtr + current->msgSize <= endptr) {
            if (breakOnSeqNum >= 0 &&
                current->seqNum == (uint32_t) breakOnSeqNum) {
                asm("int3");
            }
            switch (current->msgType) {
                case MsgType::Add:
                    for (Subscriber* sub : subscribers) {
                        sub->onAdd(*static_cast<const CryptoAdd*>(current));
                    }
                    break;
                case MsgType::Delete:
                    for (Subscriber* sub : subscribers) {
                        sub->onDelete(
                                *static_cast<const CryptoDelete*>(current));
                    }
                    break;
                case MsgType::Update:
                    for (Subscriber* sub : subscribers) {
                        sub->onUpdate(
                                *static_cast<const CryptoUpdate*>(current));
                    }
                    break;
                default:
                    std::cout << "Encountered unexpected msgType "
                              << static_cast<uint16_t>(current->msgType)
                              << " msgSize " << current->msgSize << std::endl;
                    throw std::runtime_error("");
            }
            current = reinterpret_cast<const BaseEvent*>(
                    reinterpret_cast<const std::byte*>(current) +
                    current->msgSize);
            bPtr = reinterpret_cast<const std::byte*>(current);
        }
        if (auto bPtr = reinterpret_cast<const std::byte*>(current);
            bPtr < endptr) {
            // Preserve partial message at the end of the buffer
            msgOffset = endptr - bPtr;
            if (msgOffset > 2) {
                if (msgOffset > current->msgSize) {
                    msgOffset = 0;
                }
            }
            std::memcpy(buf, bPtr, msgOffset);
        } else {
            msgOffset = 0;
        }
        if (bytesRead < bytesThisRead) {
            if (std::feof(fp)) {
                break;
            } else {
                throw std::runtime_error("Enountered error reading from file");
            }
        }
    }
}

} // namespace wwhrt
