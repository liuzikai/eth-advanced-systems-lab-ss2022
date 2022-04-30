#include "common.h"
#include "instrumented_index.h"
#include <iostream>

// This is probably the only way to do this since there is no
// restriction on how we create a InstrumentedIndex, might be a calloc, ugh..
uint64_t op_count = 0;

InstrumentedIndex::InstrumentedIndex() {
    val = 0;
}

InstrumentedIndex::InstrumentedIndex(index_t v) : val(v) {}

InstrumentedIndex::InstrumentedIndex(int v) : val(v) {}

bool InstrumentedIndex::operator<(const InstrumentedIndex& other) const {
    op_count++;
    return val < other.val;
}

bool InstrumentedIndex::operator==(const InstrumentedIndex& other) const {
    op_count++;
    return val == other.val;
}



InstrumentedIndex InstrumentedIndex::operator+(const InstrumentedIndex& other) const {
    op_count++;
    return InstrumentedIndex(val + other.val);
}

InstrumentedIndex InstrumentedIndex::operator+(const int& other) const {
    op_count++;
    return InstrumentedIndex(val + other);
}

InstrumentedIndex InstrumentedIndex::operator-(const InstrumentedIndex& other) const {
    op_count++;
    return InstrumentedIndex(val - other.val);
}

InstrumentedIndex InstrumentedIndex::operator-(const int& other) const {
    op_count++;
    return InstrumentedIndex(val - other);
}

InstrumentedIndex& InstrumentedIndex::operator++() {
    op_count++;
    val++;
    return *this;
}

InstrumentedIndex InstrumentedIndex::operator++(int) {
    op_count++;
    InstrumentedIndex before_val = *this;
    val++;
    return before_val;
}

InstrumentedIndex::operator index_t() const {
    // This is needed when index is used to access and index.
    return val;
}

namespace OpCounter {
    uint64_t GetOpCount() {
        return op_count;
    }

    void ResetOpCount() {
        op_count = 0;
    }
}