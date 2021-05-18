#include "BookBuilder.h"

#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <utility>

namespace wwhrt {

std::vector<BookBuilder::Order> BookBuilder::getBestBids(Symbol symbol) {
    std::vector<Order> bestBids;
    for (const Order& order : orders[symbol]) {
        if (order.side == Bid) {
            if (bestBids.empty() || order.price > bestBids[0].price) {
                bestBids.clear();
                bestBids.push_back(order);
            } else if (bestBids[0].price == order.price) {
                bestBids.push_back(order);
            }
        }
    }
    return bestBids;
}

std::vector<BookBuilder::Order> BookBuilder::getBestOffers(Symbol symbol) {
    std::vector<Order> bestOffers;
    for (const Order& order : orders[symbol]) {
        if (order.side == Ask) {
            if (bestOffers.empty() || order.price < bestOffers[0].price) {
                bestOffers.clear();
                bestOffers.push_back(order);
            } else if (bestOffers[0].price == order.price) {
                bestOffers.push_back(order);
            }
        }
    }
    return bestOffers;
}

void BookBuilder::onAdd(const CryptoAdd& add) {
    orders[add.symbol].push_back(Order{.price = add.price,
                                       .size = add.size,
                                       .id = add.orderId,
                                       .side = add.side});
}

void BookBuilder::onUpdate(const CryptoUpdate& update) {
    auto& symbolOrders = orders[update.symbol];
    for (auto it = symbolOrders.begin(); it != symbolOrders.end(); ++it) {
        if (it->id == update.orderId) {
            it->price = update.price;
            it->size = update.size;
            it->side = update.side;
            return;
        }
    }
    // assert(false); // Could not find existing order to update!
}

void BookBuilder::onDelete(const CryptoDelete& delete_) {
    auto& symbolOrders = orders[delete_.symbol];
    for (auto it = symbolOrders.begin(); it != symbolOrders.end(); ++it) {
        if (it->id == delete_.orderId) {
            symbolOrders.erase(it);
            return;
        }
    }
    // assert(false); // Could not find existing order to delete!
}

} // namespace wwhrt
