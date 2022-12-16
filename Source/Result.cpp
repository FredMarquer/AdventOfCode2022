#include "Result.h"

#include <assert.h>
#include <iostream>
#include <variant>

const Result Result::Invalid = Result();

bool Result::operator==(const Result& rhs) const
{
	return data == rhs.data;
}

bool Result::operator!=(const Result& rhs) const
{
	return !(*this == rhs);
}

std::ostream& operator<<(std::ostream& stream, const Result& result)
{
	if (result.isInteger())
		stream << result.getInteger();
	else if (result.isString())
		stream << result.getString();
	else {
		assert(!result.isValid());
		stream << "Result::Invalid";
	}

	return stream;
}
