// Copyright (c) 2020 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AVALANCHE_PEERMANAGER_H
#define BITCOIN_AVALANCHE_PEERMANAGER_H

#include <atomic>
#include <cstdint>
#include <vector>

static constexpr size_t NO_PEER = ~size_t(0);

struct Slot {
    uint64_t start;
    uint64_t stop;

    Slot(uint64_t startIn, uint64_t stopIn) : start(startIn), stop(stopIn) {}

    bool contains(uint64_t slot) const { return start <= slot && slot < stop; }
    bool precedes(uint64_t slot) const { return slot >= stop; }
    bool follows(uint64_t slot) const { return start > slot; }
};

class PeerManager {
    std::vector<Slot> slots;
    uint64_t slotCount = 0;
    uint64_t fragmentation = 0;

public:
    void addPeer(uint64_t score);
    void rescorePeer(size_t i, uint64_t score);
    void removePeer(size_t i) { rescorePeer(i, 0); }

    size_t selectPeer() const;

    // Accssors.
    uint64_t getSlotCount() const { return slotCount; }
    uint64_t getFragmentation() const { return fragmentation; }
};

/**
 * This is an internal method that is exposed for testing purposes.
 */
size_t selectPeerImpl(const std::vector<Slot> &slots, const uint64_t slot,
                      const uint64_t max);

#endif // BITCOIN_AVALANCHE_PEERMANAGER_H
