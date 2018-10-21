#pragma once
#include <vector>
#include <iostream>
#include <algorithm>

// Simple Heap container that holds data. When you add data to it, it says cool. That's it. :)

template<typename Data, typename Priority>
class HeapH
{
public:
	void push(Priority priority, Data data);
	Data pop(Priority priority);
	bool is_empty(){ return data_.empty(); }

	int size(){ return data_.size(); }
	void display();
	void HeapSort();
private:
	std::vector<Data> data_;
	void bubbleUp(int i, Priority priority);
	void bubbleDown(int i, Priority priority);
	int parent(int i) const;
	int leftChild(int i) const;
	int rightChild(int i) const;
};

template <typename Data, typename Priority>
void HeapH<Data, Priority>::push(Priority priority, Data data)
{
	data_.push_back(data);
	bubbleUp((data_.size() - 1), priority);
	std::cout << "cool, data added. done" << std::endl;
}

template <typename Data, typename Priority>
void HeapH<Data, Priority>::display()
{
	std::cout << "List:" << std::endl;
	for (const auto i : data_)
	{
		std::cout << i << std::endl;
	}
}

template <typename Data, typename Priority>
void HeapH<Data, Priority>::HeapSort()
{
	std::sort(data_.begin(), data_.end(), Priority());
}

template <typename Data, typename Priority>
Data HeapH<Data, Priority>::pop(Priority priority)
{
	auto temp = data_.back();
	data_.back() = data_[0];
	auto pop = data_.back();
	data_.pop_back();
	if (!is_empty())
	{
		data_[0] = temp;
		bubbleDown(0, priority);
	}
	return pop;
}

template <typename Data, typename Priority>
void HeapH<Data, Priority>::bubbleUp(int i, Priority priority)
{
	if (i && priority(data_[i], data_[parent(i)]))
	{
		std::swap(data_[i], data_[parent(i)]);
		bubbleUp(parent(i), priority);
	}
}

template <typename Data, typename Priority>
void HeapH<Data, Priority>::bubbleDown(int i, Priority priority)
{
	auto left = leftChild(i);
	auto right = rightChild(i);
	const int size = data_.size();

	auto largest = i;

	if (left < size && (priority(data_[left], data_[i])))
	{
		largest = left;
	}

	if (right < size && (priority(data_[right], data_[largest])))
	{
		largest = right;
	}

	if (largest != i)
	{
		std::swap(data_[i], data_[largest]);
		bubbleDown(largest, priority);
	}
}

template <typename Data, typename Priority>
int HeapH<Data, Priority>::parent(const int i) const
{
	return (i - 1) / 2;
}

template <typename Data, typename Priority>
int HeapH<Data, Priority>::leftChild(const int i) const
{
	return (2 * i + 1);
}

template <typename Data, typename Priority>
int HeapH<Data, Priority>::rightChild(const int i) const
{
	return (2 * i + 2);
}
