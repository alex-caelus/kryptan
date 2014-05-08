#include "QrBox.h"
#include <string.h> 
#include <sstream>
#include <qrencode.h>
#include "../kryptan_core/SecureString/SecureString.h"


#ifdef _WIN32
#include <windows.h>
#else
#include <system_error>
#include <wchar.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#endif

using namespace Kryptan;
using namespace std;

string getMyIP()
{
	string myIPs;
#ifdef _WIN32
	char szBuffer[1024];

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	if (::WSAStartup(wVersionRequested, &wsaData) != 0)
		return "";


	if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
	{
		WSACleanup();
		return "";
	}

	struct hostent *host = gethostbyname(szBuffer);
	if (host == NULL)
	{
		WSACleanup();
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

	WSACleanup();
#else
	struct ifaddrs * ifAddrStruct=NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;

	if(getifaddrs(&ifAddrStruct) != 0)
        throw std::system_error(errno, std::generic_category());

    int i=0;
	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) 
	{
		string sIp;
        if(ifa->ifa_addr == NULL)
            continue;

		if (ifa->ifa_addr->sa_family == AF_INET)
		{   
			// check it is IP4
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			sIp = addressBuffer;
		}
		else if (ifa->ifa_addr->sa_family == AF_INET6) 
		{   
			// check it is IP6
			// is a valid IP6 Address
			tmpAddrPtr = &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
			char addressBuffer[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
			sIp = addressBuffer;
		}
		else
		{
			continue;
		}
		if(sIp == "127.0.0.1" || sIp == "::1")
        {
            //we dont care about localhost addresses
            continue;
        }
        
        if (i > 0)
		{
			myIPs += ",";
		}
		myIPs += sIp;
        i++;
	}
	if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
#endif
	return myIPs;
}

/*
* The following is extracted and modified from the libqrencode library (qrenc.c)
* It's released under the GNU licence.
*/

#ifndef _WIN32
#define lstrcatW(o,i) wcscat(o, i)
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

	string toConvert = localIps + "," + to_string(port) + "," + key.getUnsecureString();

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
