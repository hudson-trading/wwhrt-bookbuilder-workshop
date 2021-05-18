#pragma once

namespace wwhrt {

// This file should not be modified.

class BookMeasurer : public Subscriber {
  public:
    BookMeasurer(BookBuilder* bb) : bookBuilder(bb) {}

    void onAdd(const CryptoAdd& add) final { onAny(add.symbol); }
    virtual void onUpdate(const CryptoUpdate& update) final {
        onAny(update.symbol);
    }
    virtual void onDelete(const CryptoDelete& delete_) final {
        onAny(delete_.symbol);
    }

    // On any update, get the orders at the BBO. This will be used for
    // gauging performance in real-world usage
    void onAny(Symbol symbol) {
        std::vector<BookBuilder::Order> bestBids =
                bookBuilder->getBestBids(symbol);
        std::vector<BookBuilder::Order> bestOffers =
                bookBuilder->getBestOffers(symbol);

        // Ask GCC not to optimize-away the above calls
        asm volatile("" ::"g"(bestBids), "g"(bestOffers) : "memory");
    }

  private:
    BookBuilder* bookBuilder;
};

} // namespace wwhrt
