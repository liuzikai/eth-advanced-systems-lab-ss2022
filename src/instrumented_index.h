#ifndef TEAM02_INSTRUMENTED_INDEX_H
#define TEAM02_INSTRUMENTED_INDEX_H

#include "common.h"

class InstrumentedIndex {
public:
    InstrumentedIndex();

    explicit InstrumentedIndex(index_t v);

    InstrumentedIndex(int v);

    bool operator<(const InstrumentedIndex& other) const;

    bool operator==(const InstrumentedIndex& other) const;

    // TODO: This is not working for some reason....
    bool operator!=(const InstrumentedIndex& other) const;

    InstrumentedIndex operator+(const InstrumentedIndex& other) const;

    InstrumentedIndex operator+(const int& other) const;

    InstrumentedIndex operator-(const InstrumentedIndex& other) const;

    InstrumentedIndex operator-(const int& other) const;

    InstrumentedIndex& operator++();

    InstrumentedIndex operator++(int);

    operator index_t() const;

private:
    index_t val;
};

namespace OpCounter {
    uint64_t GetOpCount();
    void ResetOpCount();
}



#endif //TEAM02_INSTRUMENTED_INDEX_H