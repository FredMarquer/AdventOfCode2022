#pragma once

#include <assert.h>
#include <iostream>
#include <vector>

#include "Int2.h"

template<class T>
class Array2D
{
private:
	size_t width;
	size_t height;
	T* data;

public:
	Array2D() : width(0), height(0), data(nullptr) {}
	Array2D(size_t width, size_t height);
	Array2D(size_t width, size_t height, const std::vector<T>& vector);
	Array2D(const Array2D& other);
	Array2D(Array2D&& other) noexcept;
	~Array2D() noexcept { delete[] data; }

	Array2D<T>& operator=(const Array2D& other);
	Array2D<T>& operator=(Array2D&& other) noexcept;

	inline T& operator[](size_t index) { return data[index]; }
	inline const T& operator[](size_t index) const { return data[index]; }
	inline T& operator[](Int2 coord);
	inline const T& operator[](Int2 coord) const;
	inline T& getElementAt(size_t x, size_t y);
	inline const T& getElementAt(size_t x, size_t y) const;

	inline size_t getWidth() const { return width; }
	inline size_t getHeight() const { return height; }
	inline size_t getSize() const { return width * height; }

	inline size_t getIndex(size_t x, size_t y) const;
	inline size_t getIndex(Int2 coord) const;
	inline bool isInRange(size_t x, size_t y) const;
	inline bool isInRange(Int2 coord) const;

	void fillColumn(size_t x, const T& value);
	void fillRaw(size_t y, const T& value);
};

template<class T>
Array2D<T>::Array2D(size_t width, size_t height)
	: width(width)
	, height(height)
{
	assert(width > 0);
	assert(height > 0);
	size_t size = width * height;
	data = new T[size]{ T() };
}

template<class T>
Array2D<T>::Array2D(size_t width, size_t height, const std::vector<T>& vector)
	: width(width)
	, height(height)
{
	assert(width > 0);
	assert(height > 0);
	assert(vector.size() > 0);
	assert(width * height == vector.size());
	data = new T[vector.size()];
	memcpy(data, vector.data(), vector.size() * sizeof(T));
}

template<class T>
Array2D<T>::Array2D(const Array2D& other)
	: width(other.width)
	, height(other.height)
{
	int size = width * height;
	data = new T[size];
	memcpy(data, other.data, size * sizeof(T));
}

template<class T>
Array2D<T>::Array2D(Array2D&& other) noexcept
	: width(other.width)
	, height(other.height)
	, data(other.data)
{
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
}

template<class T>
Array2D<T>& Array2D<T>::operator=(const Array2D& other)
{
	int previousSize = getSize();
	int newSize = other.getSize();

	width = other.width;
	height = other.height;

	if (newSize != previousSize) {
		delete[] data;
		if (newSize > 0)
			data = new T[newSize];
	}

	if (newSize > 0)
		memcpy(data, other.data, newSize * sizeof(T));
}

template<class T>
Array2D<T>& Array2D<T>::operator=(Array2D&& other) noexcept
{
	if (this != &other)
	{
		width = other.width;
		height = other.height;
		data = other.data;

		other.width = 0;
		other.height = 0;
		other.data = nullptr;
	}

	return *this;
}

template<class T>
T& Array2D<T>::operator[](Int2 coord)
{
	size_t index = getIndex(coord);
	return data[index];
}

template<class T>
const T& Array2D<T>::operator[](Int2 coord) const
{
	size_t index = getIndex(coord);
	return data[index];
}

template<class T>
T& Array2D<T>::getElementAt(size_t x, size_t y)
{
	size_t index = getIndex(x, y);
	return data[index];
}

template<class T>
const T& Array2D<T>::getElementAt(size_t x, size_t y) const
{
	size_t index = getIndex(x, y);
	return data[index];
}

template<class T>
size_t Array2D<T>::getIndex(size_t x, size_t y) const
{
	assert(isInRange(x, y));
	return x + y * width;
}

template<class T>
size_t Array2D<T>::getIndex(Int2 coord) const
{
	return getIndex(coord.x, coord.y);
}

template<class T>
bool Array2D<T>::isInRange(size_t x, size_t y) const
{
	return
		x >= 0 && x < width&&
		y >= 0 && y < height;
}

template<class T>
bool Array2D<T>::isInRange(Int2 coord) const
{
	return isInRange(coord.x, coord.y);
}

template<class T>
void Array2D<T>::fillColumn(size_t x, const T& value)
{
	int index = getIndex(x, 0);
	int endIndex = getIndex(x, height - 1);
	for (; index <= endIndex; index += width)
		data[index] = value;
}

template<class T>
void Array2D<T>::fillRaw(size_t y, const T& value)
{
	int index = getIndex(0, y);
	int endIndex = getIndex(width - 1, y);
	for (; index <= endIndex; ++index)
		data[index] = value;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Array2D<T>& lhs)
{
	for (size_t y = 0; y < lhs.getHeight(); ++y) {
		for (size_t x = 0; x < lhs.getWidth(); ++x)
			os << lhs.getElementAt(x, y);
		os << '\n';
	}

	return os;
}

