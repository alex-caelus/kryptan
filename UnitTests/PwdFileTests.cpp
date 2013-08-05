#include "stdafx.h"
#include "CppUnitTest.h"
#include "kryptan_core\PwdFile.h"

// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Kryptan::Core;

namespace UnitTests
{	
	TEST_CLASS(PwdFileTests)
	{
	public:
		
		BEGIN_TEST_CLASS_ATTRIBUTE()
			TEST_CLASS_ATTRIBUTE(L"Category", L"Core")
		END_TEST_CLASS_ATTRIBUTE()

				
		TEST_METHOD(CreateAndParseTest)
		{
			PwdFile* file = new PwdFile("test.pwd");
			file->CreateNew();
			PwdList* list = file->GetPasswordList();
			
			Pwd* p1 = list->CreatePwd(SecureString("test1"), SecureString("user1"), SecureString("pass1"));
			Pwd* p2 = list->CreatePwd(SecureString("test2"), SecureString("user2"), SecureString("pass2"));
			Pwd* p3 = list->CreatePwd(SecureString("test3"), SecureString("user3"), SecureString("pass3"));
			
			list->AddPwdToLabel(p1, SecureString("label1"));
			list->AddPwdToLabel(p2, SecureString("label1"));
			list->AddPwdToLabel(p3, SecureString("label1"));
			list->AddPwdToLabel(p2, SecureString("label2"));
			list->AddPwdToLabel(p3, SecureString("label2"));
			list->AddPwdToLabel(p3, SecureString("label3"));

			file->Save(SecureString("key"));
		}

		TEST_METHOD(SaveTest)
		{
			Assert::Fail(L"Test not implemented");
		}
		
		TEST_METHOD(GettersTest)
		{
			Assert::Fail(L"Test not implemented");
		}
	};
}