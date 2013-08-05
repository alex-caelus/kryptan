#include "stdafx.h"
#include "CppUnitTest.h"
#include "kryptan_core\PwdFileWorker.h"

// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Kryptan::Core;

namespace UnitTests
{		
	TEST_CLASS(PwdFileWorkerTests)
	{
	public:
		
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Core")
		END_TEST_CLASS_ATTRIBUTE()
		
		TEST_METHOD(ReadFileTest)
		{
			//prepare test
			char* oContent = "Writing this to a file.\n";
			int oLength = strlen(oContent);

			ofstream myfile;
			myfile.open ("test.txt", std::ios::binary);
			myfile.write(oContent, oLength);
			myfile.close();

			//do test
			char* iContent;
			int iLength;
			PwdFileWorker::ReadFile("test.txt", iLength, iContent);

			//assert
			if(oLength != iLength)
			{
				Assert::Fail(L"Content length is not same");
			}
			if(strcmp(oContent, iContent) != 0)
			{
				Assert::Fail(L"File content is not equal to whats written");
			}
		}
		
		TEST_METHOD(ParseFileContentsTest)
		{
			SecureString content(
				"<passwords>\n"
					"<pwd>\n"
						"<description>Google mail</description>\n"
						"<password>qwerty</password>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Hotmail</description>\n"
						"<username>alex.caelus@live.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Stackoverflow</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
				"</passwords>", 0, false);

			auto list = PwdFileWorker::ParseFileContents(content);

			auto labels = new PwdLabelVector();

			//assert passwords

			Assert::AreEqual(4, list->CountPwds());

			Assert::IsNotNull(list->All().at(0));
			Assert::AreEqual("Google mail", list->All().at(0)->GetDescription().getUnsecureString());
			Assert::AreEqual(0, (int)list->All().at(0)->GetUsername().length());
			Assert::AreEqual("qwerty", list->All().at(0)->GetPassword().getUnsecureString());

			Assert::IsNotNull(list->All().at(1));
			Assert::AreEqual("Hotmail", list->All().at(1)->GetDescription().getUnsecureString());
			Assert::AreEqual("alex.caelus@live.com", list->All().at(1)->GetUsername().getUnsecureString());
			Assert::AreEqual("qwerty", list->All().at(1)->GetPassword().getUnsecureString());

			Assert::IsNotNull(list->All().at(2));
			Assert::AreEqual("Stackoverflow", list->All().at(2)->GetDescription().getUnsecureString());
			Assert::AreEqual("alex.caelus@gmail.com", list->All().at(2)->GetUsername().getUnsecureString());
			Assert::AreEqual("qwerty", list->All().at(2)->GetPassword().getUnsecureString());

			Assert::IsNotNull(list->All().at(3));
			Assert::AreEqual("Linkedin", list->All().at(3)->GetDescription().getUnsecureString());
			Assert::AreEqual("alex.caelus@gmail.com", list->All().at(3)->GetUsername().getUnsecureString());
			Assert::AreEqual("qwerty", list->All().at(3)->GetPassword().getUnsecureString());

			//assert labels
			labels->clear();
			labels->push_back(SecureString("online"));
			labels->push_back(SecureString("test"));
			labels->push_back(SecureString("test2"));
			auto filteredPwds = list->Filter(*labels);
			Assert::AreEqual(1, (int)filteredPwds.size());
		}
		
		TEST_METHOD(GetNextTagStartTest)
		{
			char* p1 = "test <tag> test";
			char* p2 = Kryptan::Core::PwdFileWorker::GetNextTagStart(p1);

			Assert::AreEqual(p1+5, p2);
		}
		
		TEST_METHOD(GetTagLengthTest)
		{
			char* p1 = "<tag> test";
			int l = Kryptan::Core::PwdFileWorker::GetTagLength(p1);

			Assert::AreEqual(5, l);
		}

		TEST_METHOD(EscapeTagTest)
		{
			SecureString orig("bla bla < bla &gt;<< bla >> bla ><><>");
			SecureString expected("bla bla &lt; bla &amp;gt;&lt;&lt; bla &gt;&gt; bla &gt;&lt;&gt;&lt;&gt;");

			SecureString result = Kryptan::Core::PwdFileWorker::EscapeTags(orig);

			Assert::AreEqual(expected.getUnsecureString(), result.getUnsecureString());
			Assert::AreEqual(true, expected.equals(result));
		}

		TEST_METHOD(UnescapeTagTest)
		{
			const char* orig = "&gt;bla bla &lt; bla &amp;gt; &lt;&lt; bla &gt;&gt; bla &gt;&lt;&gt;&lt;&gt;&amp;";
			const char* expected = ">bla bla < bla &gt; << bla >> bla ><><>&";

			SecureString result = Kryptan::Core::PwdFileWorker::UnescapeTags(orig, strlen(orig));

			Assert::AreEqual(expected, result.getUnsecureString());
		}
		
		TEST_METHOD(EncryptionTest)
		{
			SecureString original("This is a test", 0, false);
			SecureString key("masterkey");

			int encryptedLength;
			char* encrypted = PwdFileWorker::Encrypt(original, encryptedLength, key);

			SecureString decrypted = PwdFileWorker::Decrypt(encrypted, encryptedLength, key);

			Assert::AreEqual(original.getUnsecureString(), decrypted.getUnsecureString());
		}

	};
}