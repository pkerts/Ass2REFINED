#pragma once
#include <fstream>
#include <string>

class Bitwriter
{
public:
	explicit Bitwriter(const std::string& filename);
	~Bitwriter();

	int putBit(unsigned int bit);
	int putByte(unsigned char byte);
	int putLength(uint32_t total) const;
	void flush();
private:
	unsigned char buffer_{0};
	int position_{0};
	std::ofstream* file_;
};
