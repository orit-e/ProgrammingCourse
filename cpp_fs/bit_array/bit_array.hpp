/*******************************************************************************
 * 																			   *
 * 					File name:			bit_array.hpp			    		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Leah								   *
 * 																			   *
 * ****************************************************************************/

#ifndef ILRD_RD9315_BIT_ARRAY_HPP
#define ILRD_RD9315_BIT_ARRAY_HPP

// standard includes
#include <iosfwd>       // size_t
#include <stdexcept>    // out_of_range

// local includes
#include "../utils/utils.hpp"

namespace ilrd
{

typedef size_t bitArrayElementSize_t;
const size_t sizeOfbitArraySizeT = sizeof(bitArrayElementSize_t);
const size_t bitArraySizeTNumOfBits = sizeOfbitArraySizeT * 8;

template<size_t SIZE>
class BitArray
{
private:
    class ProxyBool;
public:
    BitArray() noexcept;
    // compiler generated:
    // ~BitArray() noexcept;
    // BitArray(const BitArray& other_);
    // BitArray& operator=(const BitArray& other_);

    bool operator[](size_t index_) const; // may throw BAOutOfRange
    ProxyBool operator[](size_t index_); // may throw BAOutOfRange
    
    BitArray& operator|=(const BitArray& other_) noexcept;
    BitArray& operator&=(const BitArray& other_) noexcept;
    BitArray& operator^=(const BitArray& other_) noexcept;

    //BitArray& operator>>=(size_t shift_);
    //BitArray& operator<<=(size_t shift_);

    bool operator==(const BitArray& rhs_) const noexcept;
    bool operator!=(const BitArray& rhs_) const noexcept;

    BitArray& Set(bool bitState_, size_t index_); // may throw BAOutOfRange
    BitArray& Set(bool bitState_) noexcept;
    bool Get(size_t index_) const noexcept;
    BitArray& Flip(size_t index_); // may throw BAOutOfRange
    BitArray& Flip() noexcept;
    size_t Count() const noexcept; // noexcept?
    std::string ToString() const; //?

    class BAOutOfRange : public std::out_of_range
    {
    public:
        BAOutOfRange();
    };

private:
    class ProxyBool
    {
    public:
        explicit ProxyBool(BitArray& bitArr_, size_t index_) noexcept;
        ProxyBool& operator=(const ProxyBool& other_) noexcept;
        // compiler generated:
        // ~Proxy() noexcept;

        ProxyBool& operator=(bool bitState_) noexcept;
        operator bool() const noexcept;

    private:
        size_t m_index;
        BitArray& m_bitArrRef;
    };
    
    static const size_t m_numOfElements = SIZE / bitArraySizeTNumOfBits + 
                                        ((0 == SIZE % bitArraySizeTNumOfBits) ? 
                                            0 : 1);
    bitArrayElementSize_t m_bitArray[m_numOfElements];

    bitArrayElementSize_t LeftoverBitsForSetAndFlip();
};


// class iterator...
// class const iterator...



} // namespace ilrd

#include "bit_array_imp.hpp"

#endif // ILRD_RD9315_BIT_ARRAY_HPP
