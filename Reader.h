#pragma once

#include "Events.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

namespace wwhrt {

// This file should not be modified.

class Subscriber {
  public:
    virtual ~Subscriber() {}
    virtual void onAdd(const CryptoAdd&) {}
    virtual void onUpdate(const CryptoUpdate&) {}
    virtual void onDelete(const CryptoDelete&) {}
};

// The reader class is used for generating event callbacks from events
// stored in a flat file
class Reader {
  public:
    Reader(const char* filePath);
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;
    Reader(Reader&&) = delete;
    Reader& operator=(Reader&&) = delete;

    ~Reader();

    void addSubscriber(Subscriber* subscriber) {
        subscribers.push_back(subscriber);
    }

    void run();

    static int32_t breakOnSeqNum;

  private:
    FILE* fp;
    std::vector<Subscriber*> subscribers;
};

} // namespace wwhrt
