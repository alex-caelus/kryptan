#ifndef QRBOX_H
#define QRBOX_H

#include <string>
#include "MessageBoxes.h"
#include "../kryptan_core/SecureString.h"

namespace Kryptan
{
	class QRIpPortKeyBox
	{
	public:
		QRIpPortKeyBox(std::string title);

		void Show(int port, Kryptan::Core::SecureString key, bool waitForInput=true);
		void Hide();

		~QRIpPortKeyBox();

	private:

		std::string title;
		DialogBase* box;
	};
};

#endif