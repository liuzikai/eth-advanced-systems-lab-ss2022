#ifndef TEAM02_INSTRUMENTED_INDEX_H
#define TEAM02_INSTRUMENTED_INDEX_H

#include "common.h"
#include <functional>

struct InstrumentedIndex {
public:
    InstrumentedIndex();

    explicit InstrumentedIndex(index_t v);

    bool operator<(const InstrumentedIndex& other) const;

    bool operator<=(const InstrumentedIndex& other) const;

    bool operator>(const InstrumentedIndex& other) const;

    bool operator>=(const InstrumentedIndex& other) const;

    bool operator==(const InstrumentedIndex& other) const;

    bool operator!=(const InstrumentedIndex& other) const;

    InstrumentedIndex operator+(const InstrumentedIndex& other) const;
    
    InstrumentedIndex operator*(const InstrumentedIndex& other) const;

    InstrumentedIndex operator+(const bool& other) const;

    InstrumentedIndex operator-(const InstrumentedIndex& other) const;

    InstrumentedIndex operator%(const InstrumentedIndex& other) const;

    InstrumentedIndex operator&(const InstrumentedIndex& other) const;

    InstrumentedIndex operator>>(const InstrumentedIndex& other) const;

    InstrumentedIndex& operator++();

    InstrumentedIndex operator++(int);

    explicit operator index_t() const;

    explicit operator int() const;

//private:
    index_t val;
};

namespace std {
template <> struct hash<InstrumentedIndex>
{
    size_t operator()(const InstrumentedIndex & x) const
    {
        return hash<index_t>()(x.val);
    }
};
}

namespace AVX2{
    void increment_op_count_by(uint64_t count);
}

namespace OpCounter {
    uint64_t GetOpCount();
    void ResetOpCount();
}



#endif //TEAM02_INSTRUMENTED_INDEX_H