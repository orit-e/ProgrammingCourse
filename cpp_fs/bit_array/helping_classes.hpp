/*******************************************************************************
 * 																			   *
 * 					File name:			helping_classes.hpp			    	   *
 * 					Written by:			Orit								   *
 * 					Reviewed by:		Leah								   *
 * 																			   *
 * ****************************************************************************/
#ifndef ILRD_RD9315_HELPING_CLASSES_HPP
#define ILRD_RD9315_HELPING_CLASSES_HPP

#include <iosfwd>

namespace ilrd
{
class CountOnBitsInElement
{
public:
    size_t operator()(size_t init, const unsigned char byte_);
};

class ConvertElementToString
{
public:
    std::string& operator()(std::string& init, const unsigned char byte_);
};


} // namespace ilrd

#endif // ILRD_RD9315_HELPING_CLASSES_HPP