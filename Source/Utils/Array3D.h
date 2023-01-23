#pragma once

#include <assert.h>
#include <vector>

#include "Int3.h"

template<class T>
class Array3D
{
private:
	size_t width;
	size_t height;
	size_t depth;
	T* data;

public:
	Array3D() : width(0), height(0), depth(0), data(nullptr) {}
	Array3D(size_t width, size_t height, size_t depth);
	Array3D(size_t width, size_t height, size_t depth, const std::vector<T>& vector);
	Array3D(const Array3D& other);
	Array3D(Array3D&& other) noexcept;
	~Array3D() noexcept { delete[] data; }

	Array3D<T>& operator=(const Array3D& other);
	Array3D<T>& operator=(Array3D&& other) noexcept;

	inline T& operator[](size_t index) { return data[index]; }
	inline const T& operator[](size_t index) const { return data[index]; }
	inline T& operator[](Int3 coord);
	inline const T& operator[](Int3 coord) const;
	inline T& at(size_t x, size_t y, size_t z);
	inline const T& at(size_t x, size_t y, size_t z) const;

	inline size_t getWidth() const { return width; }
	inline size_t getHeight() const { return height; }
	inline size_t getDepth() const { return depth; }
	inline size_t getSize() const { return width * height * depth; }

	inline size_t getIndex(size_t x, size_t y, size_t z) const;
	inline size_t getIndex(Int3 coord) const;
	inline bool isInBounds(size_t x, size_t y, size_t z) const;
	inline bool isInBounds(Int3 coord) const;
};

template<class T>
Array3D<T>::Array3D(size_t width, size_t height, size_t depth)
	: width(width)
	, height(height)
	, depth(depth)
{
	assert(width > 0);
	assert(height > 0);
	assert(depth > 0);
	size_t size = width * height * depth;
	data = new T[size]{ T() };
}

template<class T>
Array3D<T>::Array3D(size_t width, size_t height, size_t depth, const std::vector<T>& vector)
	: width(width)
	, height(height)
	, depth(depth)
{
	assert(width > 0);
	assert(height > 0);
	assert(depth > 0);
	assert(vector.size() > 0);
	assert(width * height * depth == vector.size());
	data = new T[vector.size()];
	memcpy(data, vector.data(), vector.size() * sizeof(T));
}

template<class T>
Array3D<T>::Array3D(const Array3D& other)
	: width(other.width)
	, height(other.height)
	, depth(other.depth)
{
	size_t size = width * height * depth;
	data = new T[size];
	memcpy(data, other.data, size * sizeof(T));
}

template<class T>
Array3D<T>::Array3D(Array3D&& other) noexcept
	: width(other.width)
	, height(other.height)
	, depth(other.depth)
	, data(other.data)
{
	other.width = 0;
	other.height = 0;
	other.depth = 0;
	other.data = nullptr;
}

template<class T>
Array3D<T>& Array3D<T>::operator=(const Array3D& other)
{
	int previousSize = getSize();
	int newSize = other.getSize();

	width = other.width;
	height = other.height;
	depth = other.depth;

	if (newSize != previousSize) {
		delete[] data;
		if (newSize > 0)
			data = new T[newSize];
	}

	if (newSize > 0)
		memcpy(data, other.data, newSize * sizeof(T));
}

template<class T>
Array3D<T>& Array3D<T>::operator=(Array3D&& other) noexcept
{
	if (this != &other)
	{
		width = other.width;
		height = other.height;
		depth = other.depth;
		data = other.data;

		other.width = 0;
		other.height = 0;
		other.depth = 0;
		other.data = nullptr;
	}

	return *this;
}

template<class T>
T& Array3D<T>::operator[](Int3 coord)
{
	size_t index = getIndex(coord);
	return data[index];
}

template<class T>
const T& Array3D<T>::operator[](Int3 coord) const
{
	size_t index = getIndex(coord);
	return data[index];
}

template<class T>
T& Array3D<T>::at(size_t x, size_t y, size_t z)
{
	size_t index = getIndex(x, y, z);
	return data[index];
}

template<class T>
const T& Array3D<T>::at(size_t x, size_t y, size_t z) const
{
	size_t index = getIndex(x, y, z);
	return data[index];
}

template<class T>
size_t Array3D<T>::getIndex(size_t x, size_t y, size_t z) const
{
	assert(isInBounds(x, y, z));
	return x + y * width + z * width * height;
}

template<class T>
size_t Array3D<T>::getIndex(Int3 coord) const
{
	return getIndex(coord.x, coord.y, coord.z);
}

template<class T>
bool Array3D<T>::isInBounds(size_t x, size_t y, size_t z) const
{
	return
		x >= 0 && x < width &&
		y >= 0 && y < height &&
		z >= 0 && z < depth;
}

template<class T>
bool Array3D<T>::isInBounds(Int3 coord) const
{
	return isInBounds(coord.x, coord.y, coord.z);
}

