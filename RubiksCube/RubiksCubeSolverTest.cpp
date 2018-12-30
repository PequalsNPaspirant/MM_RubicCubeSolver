#include "stdafx.h"

#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <cassert>
#include <fstream> //for file read/write
#include <sstream> //for stringstream
#include <iomanip> //for std::put_time
using namespace std;

#include "RubiksCubeSolverTest.h"
#include "RubiksCubeModel.h"
#include "RubiksCubeModel_v1.h"
#include "RubiksCubeModel_v2.h"
#include "RubiksCubeSolverUI.h"

namespace mm {

	string getCurrentLocalTimeInNanoSeconds2()
	{
		//This function takes aorund 50 to 70 microseconds to execute
		chrono::system_clock::time_point now = chrono::system_clock::now();

		std::time_t now_t = chrono::system_clock::to_time_t(now);
		std::tm nowTm;
		localtime_s(&nowTm, &now_t);
		std::stringstream bufferStream;
		bufferStream << std::put_time(&nowTm, "%Y-%m-%d_%H-%M-%S");

		long long duration = (chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch())).count() % (long long)1000000000;

		//duration is in nanoseconds, so it can have max 9 digits, 2 commas and 1 dot
		string fractional_seconds(".000,000,000");
		for (int index = 11; duration > 0; --index)
		{
			if (index == 8 || index == 4)
				continue;
			fractional_seconds[index] = '0' + (duration % 10);
			duration /= 10;
		}

		string retVal(bufferStream.str() + fractional_seconds);
		return std::move(retVal);
	}

	bool RubiksCubeSolverTest::testRubiksCube(bool animate)
	{
		//vector<testInfo> testInfoSet = generateSanityTestInfo();
		//Specific test cases
		vector<testInfo> testInfoSet;
		//testInfoSet.push_back({ "RubiksCubeModel_v2", 3, "X", "X'" });
		testInfoSet.push_back({ "RubiksCubeModel_v2", 3, "D2", "D'2" });

		ofstream testResultsFile;
		testResultsFile.open("../test/RubiksCubeTestResults_" + getCurrentLocalTimeInNanoSeconds2() + ".csv");
		if (testResultsFile.is_open())
		{
			string columns("ModelName,Dimension,Steps,Duration (ns),Scrambling Algorithm,Ideal Solution,Actual Solution");
			testResultsFile.write(columns.c_str(), columns.length());
		}

		int testNum = 0;
		for (const testInfo& info : testInfoSet)
		{
			if(animate)
				refUI_.CreateOkDialog("Test No.: " + to_string(++testNum)
					+ "\nModel Name: " + info.modelName
					+ "\nSize: " + to_string(info.size));
			refUI_.replaceModelBy(info.modelName, info.size);
			refUI_.Reset(); //TODO: replaceModelBy() has a bug. it does not print the cube on screen properly. Reset() is temporary workaround.
			assert(refUI_.isSolved());

			if (animate)
				refUI_.CreateOkDialog("Applying scrambling algo: " + info.scrambleAlgo);
			refUI_.applyAlgorithm(info.scrambleAlgo, animate);
			//assert(!refUI_.isSolved()); //Rubik Cube may or may not be scrmabled depending upon the scrambling algo

			if (!info.solution.empty())
			{
				if (animate)
					refUI_.CreateOkDialog("Applying Ideal solution: " + info.solution);
				refUI_.applyAlgorithm(info.solution, animate);
				assert(refUI_.isSolved());

				if (animate)
					refUI_.CreateOkDialog("Going back to scrambled position: " + info.scrambleAlgo);
				//refUI_.replaceModelBy(modelinfo.modelName, modelinfo.size);
				refUI_.Reset();
				refUI_.applyAlgorithm(info.scrambleAlgo, false);
				refUI_.redrawWindow();
				assert(!refUI_.isSolved());
			}

			if (animate)
				refUI_.CreateOkDialog("Solving again: ");
			int numSteps;
			unsigned long long duration;
			string sol = refUI_.Solve(numSteps, duration, animate);
			assert(refUI_.isSolved());

			//Write results to csv file
			if (testResultsFile.is_open())
			{
				testResultsFile.write("\n", 1);

				string modelName(info.modelName);
				testResultsFile.write(modelName.c_str(), modelName.length());
				testResultsFile.write(",", 1);

				string size(to_string(info.size));
				testResultsFile.write(size.c_str(), size.length());
				testResultsFile.write(",", 1);

				string numStepsStr(to_string(numSteps));
				testResultsFile.write(numStepsStr.c_str(), numStepsStr.length());
				testResultsFile.write(",", 1);

				std::stringstream buffer;
				constexpr int maxPrecision = 4;
				buffer.precision(maxPrecision);
				buffer << std::fixed;
				buffer.imbue(std::locale(""));
				buffer << duration;
				string durationStr = "\"" + buffer.str() + "\"";
				testResultsFile.write(durationStr.c_str(), durationStr.length());
				testResultsFile.write(",", 1);

				testResultsFile.write(info.scrambleAlgo.c_str(), info.scrambleAlgo.length());
				testResultsFile.write(",", 1);

				testResultsFile.write(info.solution.c_str(), info.solution.length());
				testResultsFile.write(",", 1);

				testResultsFile.write(sol.c_str(), sol.length());

				testResultsFile.flush();
			}
		}
		if (testResultsFile.is_open())
			testResultsFile.close();

		refUI_.CreateOkDialog("All tests successfully completed!");

		//Testing
		//scene_.g_cCube.applyAlgorithm("U", true, 20, this);

		string algo;
		

		//algo = "D F' R L' F L D' B' U' F R' F' U L' U2 R L B");
		//algo = "B' L' R' U2 L U' F R F' U B D L' F' L R' F D'");

		//scene_.g_cCube_v2.applyAlgorithm(algo, true, 20, this);

		return true;
	}

	vector<testInfo> RubiksCubeSolverTest::generateSanityTestInfo()
	{		
		struct ModelInfo
		{
			string modelName;
			int size;
		};

		vector<ModelInfo> allModels{
			{ "RubiksCubeModel_v1", 3 },
			{ "RubiksCubeModel_v2", 3 }
		};

		struct AlgoPairs
		{
			string scramble;
			string solution;
		};

		vector<AlgoPairs> scrambleAlgos{
			{ "U", "U'" },
			{ "D", "D'" },
			{ "L", "L'" },
			{ "R", "R'" },
			{ "F", "F'" },
			{ "B", "B'" },
			{ "X", "X'" },
			{ "Y", "Y'" },
			{ "Z", "Z'" },
			{ "U'", "U" },
			{ "D'", "D" },
			{ "L'", "L" },
			{ "R'", "R" },
			{ "F'", "F" },
			{ "B'", "B" },
			{ "X'", "X" },
			{ "Y'", "Y" },
			{ "Z'", "Z" },
			{ "U2", "U'2" },
			{ "D2", "D'2" },
			{ "L2", "L'2" },
			{ "R2", "R'2" },
			{ "F2", "F'2" },
			{ "B2", "B'2" },
			{ "X2", "X'2" },
			{ "Y2", "Y'2" },
			{ "Z2", "Z'2" },
			{ "FB", "B'F'" },
			{ "LR", "R'L'" },
			{ "UD", "D'U'" },
			{ "F'B'", "BF" },
			{ "L'R'", "RL" },
			{ "U'D'", "DU" },
			{ "F2B2", "B'2F'2" },
			{ "L2R2", "R'2L'2" },
			{ "U2D2", "D'2U'2" }
		};

		vector<testInfo> retVal;
		for (ModelInfo& modelinfo : allModels)
		{
			for (const AlgoPairs& algoPair : scrambleAlgos)
			{
				retVal.push_back({ modelinfo.modelName, modelinfo.size, algoPair.scramble, algoPair.solution });
			}
		}

		return retVal;
	}

}