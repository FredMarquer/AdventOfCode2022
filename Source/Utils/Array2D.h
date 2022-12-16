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
	Array2D()
		: width(0)
		, height(0)
		, data(nullptr)
	{}

	Array2D(size_t width, size_t height)
		: width(width)
		, height(height)
	{
		assert(width > 0);
		assert(height > 0);
		size_t size = width * height;
		data = new T[size] { T() };
	}

	Array2D(size_t width, size_t height, const std::vector<T>& vector)
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

	Array2D(const Array2D& other)
		: width(other.width)
		, height(other.height)
	{
		int size = width * height;
		data = new T[size];
		memcpy(data, other.data, size* sizeof(T));
	}

	Array2D(Array2D&& other)
		: width(other.width)
		, height(other.height)
		, data(other.data)
	{
		other.width = 0;
		other.height = 0;
		other.data = nullptr;
	}

	~Array2D()
	{
		delete[] data;
	}

	Array2D<T>& operator=(Array2D&& other) noexcept
	{
		if (this != &other) {
			width = other.width;
			height = other.height;
			data = other.data;

			other.width = 0;
			other.height = 0;
			other.data = nullptr;
		}

		return *this;
	}

	T& operator[](size_t index)
	{
		return data[index];
	}

	const T& operator[](size_t index) const
	{
		return data[index];
	}

	T& operator[](const Int2& coord)
	{
		size_t index = getIndex(coord);
		return data[index];
	}

	const T& operator[](const Int2& coord) const
	{
		size_t index = getIndex(coord);
		return data[index];
	}

	T& getElementAt(size_t x, size_t y)
	{
		size_t index = getIndex(x, y);
		return data[index];
	}

	const T& getElementAt(size_t x, size_t y) const
	{
		size_t index = getIndex(x, y);
		return data[index];
	}

	size_t getWidth() const
	{
		return width;
	}

	size_t getHeight() const
	{
		return height;
	}

	size_t getSize() const
	{
		return width * height;
	}

	size_t getIndex(size_t x, size_t y) const
	{
		assert(isInRange(x, y));
		return x + y * width;
	}

	size_t getIndex(const Int2& coord) const
	{
		return getIndex(coord.x, coord.y);
	}

	bool isInRange(size_t x, size_t y) const
	{
		return
			x >= 0 && x < width&&
			y >= 0 && y < height;
	}

	bool isInRange(const Int2& coord) const
	{
		return isInRange(coord.x, coord.y);
	}
};

template<class T>
std::ostream& operator<<(std::ostream& os, const Array2D<T>& lhs)
{
	for (size_t y = 0; y < lhs.getHeight(); ++y) {
		for (size_t x = 0; x < lhs.getWidth(); ++x) {
			os << lhs.getElementAt(x, y);
		}

		os << std::endl;
	}

	return os;
}

