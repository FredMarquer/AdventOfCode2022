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
	BinaryHeap(size_t capacity) { reserve(capacity); }
	BinaryHeap(std::vector<T>&& vector) noexcept;

	inline size_t getSize() const { return data.size(); }
	inline bool isEmpty() const { return data.empty(); }
	void push(const T& value);
	T pop();
	inline void reserve(size_t newCapacity) { data.reserve(newCapacity); }
	inline void clear() { data.clear(); }
};

template<class T, class Compare>
BinaryHeap<T, Compare>::BinaryHeap(std::vector<T>&& vector) noexcept
{
	data = std::move(vector);
	std::ranges::make_heap(data, Compare());
}

template<class T, class Compare>
void BinaryHeap<T, Compare>::push(const T& value)
{
	data.push_back(value);
	std::ranges::push_heap(data, Compare());
}

template<class T, class Compare>
T BinaryHeap<T, Compare>::pop()
{
	std::ranges::pop_heap(data, Compare());
	T value = data.back();
	data.pop_back();
	return value;
}
