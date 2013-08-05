#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

namespace Kryptan
{
	namespace Core
	{
		class KryptanBaseException : std::runtime_error
		{
		public:
			KryptanBaseException(const std::string& msg)
				: runtime_error(msg)
			{
			}
		};

		class KryptanFileNotExistException : KryptanBaseException
		{
		public:
			KryptanFileNotExistException(const std::string& msg)
				: KryptanBaseException(msg)
			{
			}
		};
	}
}

#endif