#include "QrBox.h"
#include <string.h> 
#include <sstream>
#include <qrencode.h>
#include "../kryptan_core/SecureString.h"


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SOCKET_ERROR -1
#endif

using namespace Kryptan;
using namespace std;

string getMyIP()
{
	char szBuffer[1024];
	string myIPs;

#ifdef WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return "";
#endif


	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return "";
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL)
	{
#ifdef WIN32
		WSACleanup();
#endif
		return "";
	}

	for (int i = 0; host->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
		if (i > 0)
		{
			myIPs += ",";
		}
		myIPs += inet_ntoa(addr);
	}


#ifdef WIN32
	WSACleanup();
#endif
	return myIPs;
}

/*
* The following is extracted and modified from the libqrencode library (qrenc.c)
* It's released under the GNU licence.
*/

#ifndef _WIN32
#define lstrcatW(o,i) strcat((char*)o, (char*)i)
#endif
static wchar_t* writeUTF8(QRcode *qrcode)
{
	wchar_t* output = new wchar_t[qrcode->width * qrcode->width];
	output[0] = 0;
	int x, y;
	int realwidth;

	realwidth = qrcode->width + 4;

	//put 1 characters as top margin
	for (int i = 0; i < realwidth; i++)
	{
		lstrcatW(output, L"█");
	}

	/* data */
	for (y = 0; y < qrcode->width; y += 2) {
		unsigned char *row1, *row2;
		row1 = qrcode->data + y*qrcode->width;
		row2 = row1 + qrcode->width;

		//put 2 characters as left margin
		lstrcatW(output, L"██");

		for (x = 0; x < qrcode->width; x++) {
			if (row1[x] & 1) {
				if (y < qrcode->width - 1 && row2[x] & 1) {
					lstrcatW(output, L" ");
				}
				else {
					lstrcatW(output, L"▄"); //lower half block
				}
			}
			else {
				if (y < qrcode->width - 1 && row2[x] & 1) {
					lstrcatW(output, L"▀"); //upper half block
				}
				else {
					lstrcatW(output, L"█"); //full block
				}
			}
		}

		//put 2 characters as right margin
		lstrcatW(output, L"██");
	}

	//put 1 characters as bottom margin
	for (int i = 0; i < realwidth; i++)
	{
		lstrcatW(output, L"█");
	}

	return output;
}

class QrBoxImplementation : public DialogBase
{
public:
	QrBoxImplementation(std::string title, int height, int width, int starty, int startx, bool modal, BorderStyle border, int padding)
		: DialogBase(title, height, width, starty, startx, modal, border, padding)
	{

	}

	void Show(wchar_t* qrMessage)
	{
		DialogBase::Show();
		//print message
		WINDOW* w = GetWindowPtr();

		//debug
		//qrMessage[20] = 0;

		waddwstr(w, qrMessage);
		wrefresh(w);

		//hide cursor
		curs_set(0);
	}
};

QRIpPortKeyBox::QRIpPortKeyBox(std::string title)
{
	this->title = title;
	box = NULL;
}

QRIpPortKeyBox::~QRIpPortKeyBox()
{
	delete box;
}

void QRIpPortKeyBox::Show(int port, Kryptan::Core::SecureString key, bool waitForInput)
{
	int height;
	int width;
	int starty;
	int startx;
	bool modal = false;
	BorderStyle border = BorderStyle::Plain;
	int padding = 0;

	string localIps = getMyIP();
	if (localIps.empty())
		throw runtime_error("Could not get local ip address!");

	string toConvert = localIps + ":" + to_string(port) + ":" + key.getUnsecureString();

	key.UnsecuredStringFinished();

	QRcode* code = QRcode_encodeString(toConvert.c_str(), 0, QR_ECLEVEL_L, QRencodeMode::QR_MODE_8, true);

	width = code->width + 4;
	height = (int)((width / 2.0) + 0.5);

	wchar_t* qrcode = writeUTF8(code);

	QRcode_free(code);

	starty = getmaxy(stdscr) / 2 - height / 2;
	startx = getmaxx(stdscr) / 2 - width / 2;

	delete box;
	box = new QrBoxImplementation(title, height, width, starty, startx, modal, border, padding);
	((QrBoxImplementation*)box)->Show(qrcode);

	delete qrcode;
}

void QRIpPortKeyBox::Hide()
{
	if (box)
	{
		delete box;
		box = 0;
	}
}
