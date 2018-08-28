#include "CppUnitTest.h"
#include "EncounterTracker.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(EncounterTrackerTest)
	{
		EncounterTracker tracker;

		TEST_METHOD(setMinStepsTest)
		{
			int steps = 25;
			tracker.setMinSteps(steps);

			Assert::AreEqual(steps, tracker.getMinSteps());
		}

		TEST_METHOD(setMaxStepsTest)
		{
			int steps = 55;
			tracker.setMaxSteps(steps);

			Assert::AreEqual(steps, tracker.getMaxSteps());
		}

		TEST_METHOD(setMinAndMaxStepsTest)
		{
			int minSteps = 30;
			tracker.setMinSteps(minSteps);

			int maxSteps = 20;
			tracker.setMaxSteps(maxSteps); //minsteps will be brought down to max step amount

			Assert::AreEqual(maxSteps, tracker.getMinSteps());
		}

		TEST_METHOD(setMaxAndMinStepsTest)
		{
			int maxSteps = 10;
			tracker.setMaxSteps(maxSteps); 

			int minSteps = 30;
			tracker.setMinSteps(minSteps); //maxsteps will be brought up to min step amount

			Assert::AreEqual(minSteps, tracker.getMaxSteps());
		}

		TEST_METHOD(takeStepHorzTest)
		{
			tracker.setMinSteps(10); 
			tracker.setMaxSteps(40);

			tracker.takeStep(Axis::HORIZONTAL);

			Assert::AreEqual(1, tracker.getPeaceSteps()); //horizontal steps are worth 1 peacestep
		}

		TEST_METHOD(takeStepVertTest)
		{
			tracker.setMinSteps(10);
			tracker.setMaxSteps(40);

			tracker.takeStep(Axis::VERTICAL);

			Assert::AreEqual(2, tracker.getPeaceSteps()); //vertical steps are worth 2 peacesteps
		}

		TEST_METHOD(resetStepsTest)
		{
			tracker.setMinSteps(3);
			tracker.setMaxSteps(7);

			tracker.takeStep(Axis::HORIZONTAL);
			tracker.resetSteps();

			Assert::AreEqual(0, tracker.getPeaceSteps()); 
		}

		TEST_METHOD(encounterTest)
		{
			tracker.setMinSteps(1);
			tracker.setMaxSteps(20);
			tracker.setEncounterChance(100);//%100 chance of encounter

			tracker.takeStep(Axis::HORIZONTAL);

			Assert::AreEqual(true, tracker.didEncounterOccur());
		}

		TEST_METHOD(resetEncounterTest)
		{
			tracker.setMinSteps(1);
			tracker.setMaxSteps(20);
			tracker.setEncounterChance(100);//%100 chance of encounter

			tracker.takeStep(Axis::VERTICAL);
			tracker.resetSteps();

			Assert::AreEqual(false, tracker.didEncounterOccur());
		}
	};
}