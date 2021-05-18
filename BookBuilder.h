#pragma once

#include "Allocator.h"
#include "Events.h"
#include "Reader.h"

#include <list>
#include <unordered_map>
#include <utility>
#include <vector>

namespace wwhrt {

// BookBuilder Class
//
// This class is responsible for processing events that represent
// updates to the order book, in order to maintain the book state.
// The book state consists of all the active orders on the book, as
// defined by the Order struct below. This class must be able to
// return all orders at the Best Bid (side=Bid, highest price) and
// Best Offer (side=Ask, lowest price), also known as the BBO.
//
// Please modify the below class implementation.
class BookBuilder : public Subscriber {
  public:
    struct Order {
        double price;
        double size;
        uint64_t id;
        Side side;

        bool operator==(const Order& ord) const { return id == ord.id; }
        bool operator<(const Order& ord) const { return id < ord.id; }
    };

    struct OrderHasher {
        size_t operator()(const Order& ord) const {
            return std::hash<uint64_t>()(ord.id);
        }
    };
    BookBuilder() = default;
    std::vector<Order> getBestBids(Symbol symbol);
    std::vector<Order> getBestOffers(Symbol symbol);

    void onAdd(const CryptoAdd& add) final;
    void onUpdate(const CryptoUpdate& update) final;
    void onDelete(const CryptoDelete& delete_) final;

  private:
    // Change me!
    std::unordered_map<Symbol, std::list<Order, Allocator<Order>>> orders;
};

} // namespace wwhrt
