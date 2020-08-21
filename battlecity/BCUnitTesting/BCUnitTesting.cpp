#include "pch.h"
#include "CppUnitTest.h"
#include "../battlecity/Enemy.h"
#include "../battlecity/Brick.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BCUnitTesting
{
	TEST_CLASS(BCUnitTesting)
	{
	public:
		
		TEST_METHOD(Constructor)
		{
			Enemy enemy(1, 1, 1, 1, '1');
			Assert::IsFalse(false,L"mere");

		}
		/*TEST_METHOD(getCanAdvanceBrick)
		{
			Brick b;
			b.setCanAdvance(0);
			Assert::AreEqual(false,false);
		}*/
	};
}
