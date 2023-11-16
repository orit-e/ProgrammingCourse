/*******************************************************************************
 * 																			   *
 * 					File name:			bit_array_imp.hpp		    		   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Leah								   *
 * 																			   *
 * ****************************************************************************/
#include <algorithm>            // transform(), fill(), equal(), 
#include <functional>           // bit_and<>(), bit_or<>(), bit_xor<>(), 
                                // bit_not<>()
#include <numeric>              // accumulate(), 
#include <stdexcept>            // out_of_range

#include "bit_array.hpp"
#include "helping_classes.hpp"  // CountOnBitsInElement<>(), 
                                // ConvertElementToString<>()

namespace ilrd
{
template<size_t SIZE>    
BitArray<SIZE>::BAOutOfRange::BAOutOfRange()
:out_of_range("Index is larger than num of bits in array")
{
    // Empty on purpose
}

template<size_t SIZE>
BitArray<SIZE>::BitArray() noexcept
: m_bitArray()
{
    ; // Empty on purpose.
}

template<size_t SIZE>
bool BitArray<SIZE>::operator[](size_t index_) const
{
    if(SIZE <= index_)
    {
        throw(BAOutOfRange());
    }
    return (*this).Get(index_);
}

template<size_t SIZE>
typename BitArray<SIZE>::ProxyBool BitArray<SIZE>::operator[](size_t index_)
{
    if(SIZE <= index_)
    {
        throw(BAOutOfRange());
    }

    return BitArray<SIZE>::ProxyBool(*this, index_);
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator|=(
                                        const BitArray<SIZE>& other_) noexcept
{
    std::transform(&m_bitArray[0], &m_bitArray[m_numOfElements], 
                    &other_.m_bitArray[0], &m_bitArray[0], 
                    std::bit_or<bitArrayElementSize_t>());

    return *this;
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator&=(
                                        const BitArray<SIZE>& other_) noexcept
{
    std::transform(&m_bitArray[0], &m_bitArray[m_numOfElements], 
                    &other_.m_bitArray[0], &m_bitArray[0], 
                    std::bit_and<bitArrayElementSize_t>());

    return *this;
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator^=(
                                        const BitArray<SIZE>& other_) noexcept
{
    std::transform(&m_bitArray[0], &m_bitArray[m_numOfElements], 
                    &other_.m_bitArray[0], &m_bitArray[0], 
                    std::bit_xor<bitArrayElementSize_t>());

    return *this;
}

template<size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray<SIZE>& rhs_) const noexcept
{
    return (std::equal(&m_bitArray[0], &m_bitArray[m_numOfElements], 
                    &rhs_.m_bitArray[0]));
}

template<size_t SIZE>
bool BitArray<SIZE>::operator!=(const BitArray<SIZE>& rhs_) const noexcept
{
    return (!(*this == rhs_));
}


template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Set(bool bitState_, size_t index_)
{
    (*this)[index_] = bitState_;

    return *this;
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Set(bool bitState_) noexcept
{
    static bitArrayElementSize_t typeZero = 0;
    bitArrayElementSize_t valToSet = bitState_ ? ~typeZero : typeZero;

    std::fill(&m_bitArray[0], &m_bitArray[m_numOfElements], valToSet);
    if(0 != (SIZE & (bitArraySizeTNumOfBits - 1)))
    {
        static bitArrayElementSize_t SetToDiscard = LeftoverBitsForSetAndFlip();

        m_bitArray[m_numOfElements - 1] &= SetToDiscard;
    }

    return *this;
}

template<size_t SIZE>
bool BitArray<SIZE>::Get(size_t index_) const noexcept
{
    index_ = index_ % SIZE;

    return bool((m_bitArray[index_ / bitArraySizeTNumOfBits] >> 
                    (index_ & (bitArraySizeTNumOfBits - 1))) & 1l);
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Flip(size_t index_)
{
    (*this)[index_] = !(*this)[index_];

    return *this;
}

template<size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Flip() noexcept
{
    std::transform(&m_bitArray[0], &m_bitArray[m_numOfElements], 
                    &m_bitArray[0], 
                    std::bit_not<bitArrayElementSize_t>());
    if(0 != (SIZE & (bitArraySizeTNumOfBits - 1)))
    {
        static bitArrayElementSize_t SetToDiscard = LeftoverBitsForSetAndFlip();

        m_bitArray[m_numOfElements - 1] &= SetToDiscard;
    }
    return *this;
}

template<size_t SIZE>
size_t BitArray<SIZE>::Count() const noexcept
{
    const unsigned char *char_iter = reinterpret_cast<const unsigned char *>(m_bitArray);
    CountOnBitsInElement counter;
    return (std::accumulate(&char_iter[0], 
                            &char_iter[m_numOfElements * sizeOfbitArraySizeT], 
                            0, counter));
}


template<size_t SIZE>
std::string BitArray<SIZE>::ToString() const
{
    const unsigned char *char_iter = reinterpret_cast<const unsigned char *>(m_bitArray);
    std::string tmp;
    ConvertElementToString converter;

    tmp = std::accumulate(&char_iter[0], 
                            &char_iter[m_numOfElements * sizeOfbitArraySizeT], 
                            tmp, converter);
    
    if(0 != (SIZE & (bitArraySizeTNumOfBits - 1)))
    {
        static size_t leftoverBits = bitArraySizeTNumOfBits - 
                                (SIZE & (bitArraySizeTNumOfBits - 1));
        tmp.erase(0, leftoverBits);
    }

    return tmp;
}

template<size_t SIZE>
bitArrayElementSize_t BitArray<SIZE>::LeftoverBitsForSetAndFlip()
{
    size_t leftoverBits = SIZE & (bitArraySizeTNumOfBits - 1);
    bitArrayElementSize_t SetToDiscard = 0;
    
    return (~((~SetToDiscard) << leftoverBits));
}

/******************************************************************************/
// ProxyBool definitions:

template<size_t SIZE>
BitArray<SIZE>::ProxyBool::ProxyBool(BitArray<SIZE>& bitArr_, 
                                        size_t index_) noexcept
: m_index(index_)
, m_bitArrRef(bitArr_)
{
    ; // Empty on purpose.
}

template<size_t SIZE>
typename BitArray<SIZE>::ProxyBool& 
                BitArray<SIZE>::ProxyBool::operator=(bool bitState_) noexcept
{
    size_t ElementToChange = m_index / bitArraySizeTNumOfBits;
    size_t indexInElement = m_index & (bitArraySizeTNumOfBits - 1);

    bitArrayElementSize_t tmpByte = m_bitArrRef.m_bitArray[ElementToChange];
    tmpByte &= ~(1l << indexInElement);
    tmpByte |= (static_cast<bitArrayElementSize_t>(bitState_) << 
                indexInElement);

    m_bitArrRef.m_bitArray[ElementToChange] = tmpByte;
    
    return *this;
}

template<size_t SIZE>
typename BitArray<SIZE>::ProxyBool& BitArray<SIZE>::ProxyBool::operator=(
                            const BitArray<SIZE>::ProxyBool& bitState_) noexcept
{
    bool tmp = bitState_;

    *this = tmp;

    return *this;
}

template<size_t SIZE>
BitArray<SIZE>::ProxyBool::operator bool() const noexcept
{
    return bool((m_bitArrRef.m_bitArray[m_index / bitArraySizeTNumOfBits] >> 
                    (m_index & (bitArraySizeTNumOfBits - 1))) & 1l);
}

} // namespace ilrd