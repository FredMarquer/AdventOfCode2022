#include "Result.h"

#include <iostream>

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
