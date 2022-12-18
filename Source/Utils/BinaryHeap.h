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
	BinaryHeap(std::vector<T>&& vector);

	inline size_t getSize() { return data.size(); }
	inline bool isEmpty() { return data.empty(); }
	void push(const T& value);
	T pop();
	inline void reserve(size_t newCapacity) { data.reserve(newCapacity); }
	inline void clear() { data.clear(); }
};

template<class T, class Compare>
BinaryHeap<T, Compare>::BinaryHeap(std::vector<T>&& vector)
{
	data = vector;
	std::make_heap(data.begin(), data.end(), Compare());
}

template<class T, class Compare>
void BinaryHeap<T, Compare>::push(const T& value)
{
	data.push_back(value);
	std::push_heap(data.begin(), data.end(), Compare());
}

template<class T, class Compare>
T BinaryHeap<T, Compare>::pop()
{
	std::pop_heap(data.begin(), data.end(), Compare());
	T value = data.back();
	data.pop_back();
	return value;
}
