#pragma once

namespace wwhrt {

// This file should not be modified.

class EventPrinter : public Subscriber {
  public:
    void onAdd(const CryptoAdd& add) final {
        std::cout << add.seqNum << " " << add.symbol << " add " << add.orderId
                  << ": " << (add.side == Side::Bid ? "buy " : "sell ")
                  << add.size << " @ " << add.price << "\n";
    }
    void onUpdate(const CryptoUpdate& update) final {
        std::cout << update.seqNum << " " << update.symbol << " update "
                  << update.orderId << ": "
                  << (update.side == Side::Bid ? "buy " : "sell ")
                  << update.size << " @ " << update.price << "\n";
    }
    void onDelete(const CryptoDelete& delete_) final {
        std::cout << delete_.seqNum << " " << delete_.symbol << " delete "
                  << delete_.orderId << "\n";
    }
};

class BookPrinter : public Subscriber {
  public:
    BookPrinter(BookBuilder* bb) : bookBuilder(bb) {}
    void onAdd(const CryptoAdd& add) final { onAny(add.symbol, add.seqNum); }
    virtual void onUpdate(const CryptoUpdate& update) final {
        onAny(update.symbol, update.seqNum);
    }
    virtual void onDelete(const CryptoDelete& delete_) final {
        onAny(delete_.symbol, delete_.seqNum);
    }
    void onAny(Symbol symbol, uint32_t seqNum) {
        std::vector<BookBuilder::Order> bestBids =
                bookBuilder->getBestBids(symbol);
        std::vector<BookBuilder::Order> bestOffers =
                bookBuilder->getBestOffers(symbol);
        double bidSize = 0.0, askSize = 0.0;
        for (auto& order : bestBids) {
            bidSize += order.size;
        }
        for (auto& order : bestOffers) {
            askSize += order.size;
        }
        std::cout << seqNum << " " << symbol << " best bid/ask: " << bidSize
                  << " @ "
                  << (bestBids.empty()
                              ? std::numeric_limits<double>::quiet_NaN()
                              : bestBids[0].price)
                  << " / " << askSize << " @ "
                  << (bestOffers.empty()
                              ? std::numeric_limits<double>::quiet_NaN()
                              : bestOffers[0].price)
                  << "\n";
    }

  private:
    BookBuilder* bookBuilder;
};

} // namespace wwhrt
