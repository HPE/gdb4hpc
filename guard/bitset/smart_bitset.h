/*********************************************************************************\
 * <smart_bitset.h>
 *
 * Copyright 2012-2013,2021 Hewlett Packard Enterprise Development LP.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 *********************************************************************************/
#pragma once

#include "bitset.h"
#include "dense_bitset.h"
#include "sparse_bitset.h"

/*
 * Smart bitset could be a dense bitset or a sparse bitset.
 * It converts a "bitset" between the two formats based on the number of bits.
*/

class SmartBitset : public Bitset {
public:
    virtual std::unique_ptr<Bitset> clone() const override;
    std::unique_ptr<SmartBitset> typedClone() const;

    SmartBitset();
    SmartBitset(int nbits);
    SmartBitset(std::unique_ptr<DenseBitset> &&denseBitset);
    SmartBitset(std::unique_ptr<SparseBitset> &&sparseBitset);

    bool isSparse() const;
    bool isDense() const;
    // return true on success, false on error
    bool convertToDense();

    virtual bool isEmpty() const override;
    virtual bool isFull() const override;

    virtual void set(int bit) override;
    // virtual void unset(int bit) override;
    // virtual bool test(int bit) const override;
    // virtual void invert() override;
    virtual void clear() override;

    virtual int numSet() const override;
    virtual int firstSet() const override;

    virtual size_t sizeInBits() const override;

    // virtual std::string toString() const override;

    // virtual std::string toSet(int offset = 0, int len = -1) const override;

    virtual SerializedPtr serialize() const override;

    virtual Bitset& operator= (const Bitset &other) override;
    SmartBitset& operator= (const SmartBitset &other);
    
    // virtual Bitset& operator&= (const Bitset &other) override;
    virtual Bitset& operator|= (const Bitset &other) override;
    // virtual Bitset& operator-= (const Bitset &other) override;

    virtual bool operator== (const Bitset &other) const override;
    virtual bool operator!= (const Bitset &other) const override;

    // virtual bool hasCommonBits(const Bitset &other) const override;
private:
    std::unique_ptr<DenseBitset> m_dense;
    std::unique_ptr<SparseBitset> m_sparse;
};
