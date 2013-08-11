#ifndef PROMTS_H
#define PROMTS_H

#include "DialogBase.h"
#include "../kryptan_core/core.h"

namespace Kryptan
{
	class PromptPass : DialogBase
	{
	public:
		PromptPass(std::string title, std::string message, bool modal = true);

		Core::SecureString Prompt();
	private:
		std::string message;
	};
};


#endif