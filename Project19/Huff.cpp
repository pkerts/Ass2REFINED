#include "Heap.h"
#include "Bitwriter.h"
#include <fstream>
#include <climits>
#include <string>
using namespace std;

struct Node
{
	unsigned int frequency;
	unsigned char data;
	Node* left;
	Node* right;
};

struct Priority
{
	bool operator()(Node &lhs, Node &rhs) const
	{
		return lhs.frequency < rhs.frequency;
	}
};

unsigned int frequency_table[UCHAR_MAX + 1]{ 0 };
void buildFrequencyTable(ifstream& file)
{
	ifstream::pos_type position = 0;
	while (file.peek() != -1) // Use of peek() to read text file without extracting and removing the characters
	{
		const unsigned char unsigned_ch = file.peek();
		frequency_table[(int)unsigned_ch]++;
		position += 1;
		file.seekg(position);
	}
	file.seekg(0);
}

struct CodedSymbol
{
	unsigned char length;
	unsigned long long bit_pattern;
};
CodedSymbol coded_symbols_array[256]{ {0, 0} };
void createCodedSymbols(Node* root, unsigned char length, const unsigned long long bit_pattern)
{
	if (root == nullptr)
	{
		return;
	}

	if (!(root->left) && !(root->right))
	{
		CodedSymbol cs{ length, bit_pattern };
		coded_symbols_array[(int)root->data] = cs;
	}

	length++;
	if (root->left)
		createCodedSymbols(root->left, length, bit_pattern << 1);
	if (root->right)
		createCodedSymbols(root->right, length, (bit_pattern << 1) | 1);
}

void clean(Node* root)
{
	if (root->left)
	{
		clean(root->left);
	}
	if (root->right)
	{
		clean(root->right);
	}
	delete root;
}

int main(int argc, char** argv) // The compiled code will take an argument
{
	// BUILD FREQUENCY TABLE
	string filename = argv[1];
	ifstream in(filename);
	buildFrequencyTable(in);

	// BUILD PRIORITY QUEUE
	HeapH<Node, Priority> heap;
	for (auto i = 0; i < UCHAR_MAX+1; i++)
	{
		const auto frequency = frequency_table[i];
		if (frequency)
		{
			const auto data = (unsigned char)i;
			const Node n{frequency, data, nullptr, nullptr};
			heap.push(Priority(), n);
		}
	}

	// BUILD HUFFMAN TREE
	while (heap.size() > 1)
	{
		const auto left = new Node{ heap.pop(Priority()) };
		const auto right = new Node{ heap.pop(Priority()) };
		const auto frequency = left->frequency + right->frequency;
		const Node n{ frequency, '\0', left, right};
		heap.push(Priority(), n);
	}
	const auto root = new Node{ heap.pop(Priority()) };

	createCodedSymbols(root, 0, 0);

	// COMPRESSED FILE OUTPUT
	filename.append(".huff");
	Bitwriter writer(filename);

	// Variable-length records
	for (auto& i : coded_symbols_array)
	{
		writer.putByte(i.length);
		for (auto position = (i.length - 1); position > -1; position--)
		{
			unsigned long long mask = 1;
			mask <<= (position);
			if (i.bit_pattern & mask)
			{
				writer.putBit(1);
			}
			else
			{
				writer.putBit(0);
			}
		}
		writer.flush();
	}
	writer.flush();

	// Number of symbols in the file
	in.seekg(0, ios_base::end);
	const uint32_t file_length = in.tellg();
	in.seekg(0);
	writer.putLength(file_length);

	// Coded symbols
	auto bits_written = 0;
	for (unsigned int file_pos = 0; file_pos < file_length; file_pos++)
	{
		in.seekg(file_pos);
		const auto ch = in.peek();
		const unsigned int bit_length = coded_symbols_array[(unsigned int)ch].length;
		const auto bit_pattern = coded_symbols_array[(int)ch].bit_pattern;
		for (int bit_pos = (bit_length - 1); bit_pos > -1; bit_pos--)
		{
			unsigned long long mask = 1;
			mask <<= (bit_pos);
			if (bit_pattern & mask)
			{
				writer.putBit(1);
			}
			else
			{
				writer.putBit(0);
			}
			bits_written++;
		}
		bits_written = 0;
	}

	// CLEANUP
	clean(root);
	return 0;
}
