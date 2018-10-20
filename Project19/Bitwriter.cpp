#include "Bitwriter.h"
#include <string>
#include <fstream>
#include <bitset>
#include <iostream>

Bitwriter::Bitwriter(const std::string& filename) : file_(new std::ofstream(filename, std::ios::binary))
{
}

Bitwriter::~Bitwriter()
{
	flush();
	delete file_;
}

int Bitwriter::putBit(const unsigned int bit)
{
	buffer_ = (buffer_ << 1) | (bit & 1);
	position_++;
	if (position_ > 7)
	{
		file_->write(reinterpret_cast<char*>(&buffer_), sizeof(unsigned char));
		position_ = 0;
		buffer_ = 0;
	}
	return 0;
}

int Bitwriter::putByte(unsigned char byte)
{
	flush();
	file_->write(reinterpret_cast<char*>(&byte), sizeof(unsigned char));
	return 0;
}

int Bitwriter::putLength(uint32_t total) const
{
	file_->write(reinterpret_cast<char*>(&total), sizeof(uint32_t));
	return 0;
}

void Bitwriter::flush()
{
	if (buffer_ || position_)
	{
		buffer_ <<= (8 - position_);
		file_->write(reinterpret_cast<char*>(&buffer_), sizeof(unsigned char));
		position_ = 0;
		buffer_ = 0;
	}
}
