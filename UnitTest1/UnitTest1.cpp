#include "pch.h"
#include "CppUnitTest.h"
#include "../5.3a/5.3a.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			char f[6] = "test";
			int t = BinSearch(f, "sas", 3, 4);
			Assert::AreEqual(t, -1);
		}
	};
}
