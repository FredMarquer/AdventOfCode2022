#include "Result.h"

#include <iostream>

bool Result::operator==(const Result& rhs) const
{
	if (!stringValue.empty())
		return stringValue == rhs.stringValue;
	else if (!rhs.stringValue.empty())
		return false;
	else
		return intValue == rhs.intValue;
}

bool Result::operator!=(const Result& rhs) const
{
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& stream, const Result& result)
{
	if (result.stringValue.empty()) {
		stream << result.intValue;
	}
	else {
		stream << result.stringValue;
	}

	return stream;
}
