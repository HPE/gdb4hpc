// Copyright 2023 Hewlett Packard Enterprise Development LP.
#pragma once

#include <vector>

// the mdspan represents an multi-dimensional indexing into a single array of values.
// it doesn't cover the full spectrum of cases covered by the future std::mdspan, but
// it does at least cover the index reordered cases we can glean from gdb visible
// data
class mdspan {
public:
    mdspan() = default;
    mdspan(const std::vector<size_t>& sizes, const std::vector<size_t>& strides);

    // the offset in each direction is the external index of the first element
    // in each dimension
    mdspan(const std::vector<size_t>& sizes, const std::vector<size_t>& strides,
           const std::vector<int>& offsets);

    // return true if the combinations of sizes, strides, and offsets makes sense.
    bool isValid() const;

    size_t dim() const { return m_sizes.size(); }

    const std::vector<size_t>& sizes() const { return m_sizes; }
    const std::vector<size_t>& strides() const { return m_strides; }
    const std::vector<int>& offsets() const { return m_offsets; }

    size_t size(int n) { return m_sizes[n]; }
    size_t stride(int n) { return m_strides[n]; }
    int offset(int n) { return m_offsets[n]; }

    void setSizes(const std::vector<size_t>& sizes,
                  const std::vector<size_t>& strides);

private:  
    std::vector<size_t> m_sizes;
    std::vector<size_t> m_strides;
    std::vector<int> m_offsets;
};
