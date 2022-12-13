#pragma once

#include <algorithm>
#include <vector>

template<class T, class Compare>
class BinaryHeap
{
private:
	std::vector<T> data;

public:
	BinaryHeap() {}

	BinaryHeap(std::vector<T>&& vector)
	{
		data = vector;
		std::make_heap(data.begin(), data.end(), Compare());
	}

	size_t size()
	{
		return data.size();
	}

	bool empty()
	{
		return data.empty();
	}

	void push(const T& value)
	{
		data.push_back(value);
		std::push_heap(data.begin(), data.end(), Compare());
	}

	T pop()
	{
		std::pop_heap(data.begin(), data.end(), Compare());
		T value = data.back();
		data.pop_back();
		return value;
	}

	void clear()
	{
		data.clear();
	}
};

