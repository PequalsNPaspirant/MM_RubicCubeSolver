//=======================================================================================================//
//   Copyright (c) 2018 Maruti Mhetre                                                                    //
//   All rights reserved.                                                                                //
//=======================================================================================================//
//   Redistribution and use of this software in source and binary forms, with or without modification,   //
//   are permitted for personal, educational or non-commercial purposes provided that the following      //
//   conditions are met:                                                                                 //
//   1. Redistributions of source code must retain the above copyright notice, this list of conditions   //
//      and the following disclaimer.                                                                    //
//   2. Redistributions in binary form must reproduce the above copyright notice, this list of           //
//      conditions and the following disclaimer in the documentation and/or other materials provided     //
//      with the distribution.                                                                           //
//   3. Neither the name of the copyright holder nor the names of its contributors may be used to        //
//      endorse or promote products derived from this software without specific prior written            //
//      permission.                                                                                      //
//=======================================================================================================//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR      //
//   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND    //
//   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR          //
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL   //
//   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   //
//   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  //
//   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT   //
//   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     //
//=======================================================================================================//

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
#include "RubiksCubeSolverUtils.h"

namespace mm {

	int RubiksCubeSolverTest::maxSize = 7;
	vector<int> RubiksCubeSolverTest::scramblingAlgoLengths{ 5, 10, 15, 20, 25, 30, 50, 75, 100, 200 };
	const int RubiksCubeSolverTest::numAlgoOfEachLength = 100;
	vector<string> RubiksCubeSolverTest::genericModels{
		"RubiksCubeModel_v5"
	};

	bool RubiksCubeSolverTest::testRubiksCube(bool animate)
	{
		unique_ptr<RubiksCubeModel> originalModel = refUI_.replaceModelBy("RubiksCubeModel_v1", 3, false);

		unordered_map<int, vector<AlgoPairs>> scrambleAlgosHardcoded;
		vector<testInfoAggregate> testInfoAggregateSetHardcoded(0);
		generateHardcodedTestCases(scrambleAlgosHardcoded, testInfoAggregateSetHardcoded);
		vector<testInfo> testInfoSetHardcoded;
		executeAllTests(scrambleAlgosHardcoded, testInfoAggregateSetHardcoded, testInfoSetHardcoded, animate);
		//refUI_.CreateOkDialog("All " + to_string(testInfoSetHardcoded.size()) + " Hardcoded tests are successfully completed!");

		//vector<AlgoPairs> scrambleAlgosBasic;
		//vector<testInfoAggregate> testInfoAggregateSetBasic;
		//generateBasicTestCases(scrambleAlgosBasic, testInfoAggregateSetBasic);
		//vector<testInfo> testInfoSetBasic;
		//executeAllTests(scrambleAlgosBasic, testInfoAggregateSetBasic, testInfoSetBasic, animate);
		//refUI_.CreateOkDialog("All " + to_string(testInfoSetBasic.size()) + " Basic tests are successfully completed!");

		//vector<AlgoPairs> scrambleAlgosGeneric;
		//vector<testInfoAggregate> testInfoAggregateSetGeneric;
		//generateGenericTestCases(scrambleAlgosGeneric, testInfoAggregateSetGeneric);
		//vector<testInfo> testInfoSetGeneric;
		//executeAllTests(scrambleAlgosGeneric, testInfoAggregateSetGeneric, testInfoSetGeneric, animate);
		//refUI_.CreateOkDialog("All " + to_string(testInfoSetGeneric.size()) + " Generic tests are successfully completed!");

		unordered_map<int, vector<AlgoPairs>> scrambleAlgos(maxSize);
		vector<testInfoAggregate> testInfoAggregateSet(0);
		generateTestCases(scrambleAlgos, testInfoAggregateSet);
		vector<testInfo> testInfoSet;
		executeAllTests(scrambleAlgos, testInfoAggregateSet, testInfoSet, animate);
		refUI_.CreateOkDialog("All " + to_string(testInfoSet.size()) + " tests are successfully completed!");

		ofstream testResultsFile;
		testResultsFile.open("../test/RubiksCubeTestResults_" + getCurrentLocalTimeInNanoSeconds2() + ".csv");
		if (testResultsFile.is_open())
		{
			//writeResultsToCSVFile(testResultsFile, testInfoAggregateSetBasic);
			//writeResultsToCSVFile(testResultsFile, testInfoAggregateSetGeneric);
			//writeResultsToCSVFile(testResultsFile, testInfoSetBasic);
			//writeResultsToCSVFile(testResultsFile, testInfoSetGeneric);
			writeResultsToCSVFile(testResultsFile, testInfoAggregateSet);
			writeResultsToCSVFile(testResultsFile, testInfoSet);

			testResultsFile.close();
		}

		//int numTestCases = testInfoSetBasic.size() + testInfoSetGeneric.size();
		int numTestCases = testInfoSet.size();

		refUI_.CreateOkDialog("All " + to_string(numTestCases) + " tests are successfully completed and written to .csv file!");

		refUI_.replaceModelBy(std::move(originalModel), true);

		return true;
	}

	void RubiksCubeSolverTest::generateHardcodedTestCases(unordered_map<int, vector<AlgoPairs>>& scrambleAlgosHardcoded, vector<testInfoAggregate>& testInfoAggregateSetHardcoded)
	{
		//Specific test cases to test
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v2", 3, "X", "X'" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v2", 3, "D2", "D'2" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v3", 2, "F", "" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v3", 2, "D'", "" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v4", 3, "D", "" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v4", 3, "D'", "" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v4", 3, "D2", "" });
		//testInfoSetHardcoded.push_back({ "RubiksCubeModel_v4", 3, "LRDFL'", "" });

		//scrambleAlgosHardcoded.push_back({"U", "U'"});
		//testInfoAggregateSetHardcoded.push_back({"RubiksCubeModel_v5", 4});


	}

	void RubiksCubeSolverTest::generateBasicTestCases(vector<AlgoPairs>& scrambleAlgosBasic, vector<testInfoAggregate>& testInfoAggregateSetBasic)
	{
		//50 hardcoded scrambling algos
		scrambleAlgosBasic = vector<AlgoPairs>{
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

			{ "U'2",  "U2" },
			{ "D'2",  "D2" },
			{ "L'2",  "L2" },
			{ "R'2",  "R2" },
			{ "F'2",  "F2" },
			{ "B'2",  "B2" },
			{ "X'2",  "X2" },
			{ "Y'2",  "Y2" },
			{ "Z'2",  "Z2" },

			{ "FB", "B'F'" },
			{ "LR", "R'L'" },
			{ "UD", "D'U'" },
			{ "F'B'", "BF" },
			{ "L'R'", "RL" },
			{ "U'D'", "DU" },

			{ "F2B2", "B'2F'2" },
			{ "L2R2", "R'2L'2" },
			{ "U2D2", "D'2U'2" },

			{ "FLU", "U'L'F'" },
			{ "F'L'U'", "ULF" },
			{ "BRD", "D'R'B'" },
			{ "B'R'D'", "DRB" },
			{ "FLUBRD", "D'R'B'U'L'F'" },
		};

		//Add 10 x 100 = 1000 random srambling algos independent of Model
		bool includeNonStandardRotations = false;
		unique_ptr<RubiksCubeModel> model = RubiksCubeModelFactory::getRubiksCubeModel("RubiksCubeModel_v2", 3);
		
		for (int len : scramblingAlgoLengths)
			for (int i = 0; i < numAlgoOfEachLength; ++i)
				scrambleAlgosBasic.push_back({ model->getScramblingAlgo(len, includeNonStandardRotations), "" });

		//vector<testInfoAggregate> testInfoAggregateSet{
		//	{ "RubiksCubeModel_v3", 2 },
		//	{ "RubiksCubeModel_v4", 2 },
		//	{ "RubiksCubeModel_v5", 2 },

		//	{ "RubiksCubeModel_v1", 3 },
		//	{ "RubiksCubeModel_v2", 3 },
		//	{ "RubiksCubeModel_v3", 3 },
		//	{ "RubiksCubeModel_v4", 3 },
		//	{ "RubiksCubeModel_v5", 3 },

		//	{ "RubiksCubeModel_v4", 4 },
		//	//{ "RubiksCubeModel_v5", 4 }
		//};

		//Fill up size == 2 models
		testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v3", 2 });
		testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v4", 2 });
		for (int i = 0; i < genericModels.size(); ++i)
			testInfoAggregateSetBasic.push_back({ genericModels[i], 2 });

		//Fill up size == 3 models
		testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v1", 3 });
		testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v2", 3 });
		testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v3", 3 });
		testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v4", 3 });
		for (int i = 0; i < genericModels.size(); ++i)
			testInfoAggregateSetBasic.push_back({ genericModels[i], 3 });

		//testInfoAggregateSetBasic.push_back({ "RubiksCubeModel_v4", 4 });
		for (unsigned int size = 4; size <= maxSize; ++size)
			for (int i = 0; i < genericModels.size(); ++i)
				testInfoAggregateSetBasic.push_back({ genericModels[i], size });

		//for (testInfoAggregate& modelinfo : testInfoAggregateSetBasic)
		//{
		//	modelinfo.nsAggregateDuration_ = 0;
		//	modelinfo.numTestCases_ = scrambleAlgos.size();
		//}
	
		////All above 50 + 1000 scrambling algos are tested on every model
		//unsigned int testNum = 0;
		//for (const AlgoPairs& algoPair : scrambleAlgos)
		//{
		//	for (testInfoAggregate& modelinfo : testInfoAggregateSetBasic)
		//	{
		//		testInfoSetBasic.emplace_back(modelinfo.modelName_, modelinfo.size_, algoPair.scramble, algoPair.solution);
		//		executeTest(*testInfoSetBasic.rbegin(), animate, ++testNum);
		//		modelinfo.nsAggregateDuration_ += testInfoSetBasic.rbegin()->nsDuration_;
		//	}
		//}
	}

	void RubiksCubeSolverTest::generateGenericTestCases(vector<AlgoPairs>& scrambleAlgosGeneric, vector<testInfoAggregate>& testInfoAggregateSetGeneric)
	{
		bool includeNonStandardRotations = true;
		unique_ptr<RubiksCubeModel> model = RubiksCubeModelFactory::getRubiksCubeModel("RubiksCubeModel_v5", 3);

		for (int len : scramblingAlgoLengths)
			for (int i = 0; i < numAlgoOfEachLength; ++i)
				scrambleAlgosGeneric.push_back({ model->getScramblingAlgo(len, includeNonStandardRotations), "" });

		for (unsigned int size = 4; size <= maxSize; ++size)
			for (int i = 0; i < genericModels.size(); ++i)
				testInfoAggregateSetGeneric.push_back({ genericModels[i], size });

		////numModex x 10 x 100 = numModex x 1000  Model specific scrambling algos
		//bool includeNonStandardRotations = true;
		//unsigned int testNum = 0;
		//for (testInfoAggregate& modelinfo : testInfoAggregateSetGeneric)
		//{
		//	modelinfo.nsAggregateDuration_ = 0;
		//	modelinfo.numTestCases_ = 0;
		//	for (int len : scramblingAlgoLengths)
		//	{
		//		for (int i = 0; i < numAlgoOfEachLength; ++i)
		//		{
		//			testInfoSetGeneric.emplace_back(modelinfo.modelName_, modelinfo.size_, model->getScramblingAlgo(len, includeNonStandardRotations), "");
		//			executeTest(*testInfoSetGeneric.rbegin(), animate, ++testNum);
		//			modelinfo.nsAggregateDuration_ += testInfoSetGeneric.rbegin()->nsDuration_;
		//			modelinfo.numTestCases_ += 1;
		//		}
		//	}
		//}
	}

	void RubiksCubeSolverTest::generateTestCases(unordered_map<int, vector<AlgoPairs>>& scrambleAlgos, vector<testInfoAggregate>& testInfoAggregateSet)
	{
		//50 hardcoded scrambling algos
		vector<AlgoPairs> scrambleAlgos_size2{
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

			{ "U'2",  "U2" },
			{ "D'2",  "D2" },
			{ "L'2",  "L2" },
			{ "R'2",  "R2" },
			{ "F'2",  "F2" },
			{ "B'2",  "B2" },
			{ "X'2",  "X2" },
			{ "Y'2",  "Y2" },
			{ "Z'2",  "Z2" },

			{ "FB", "B'F'" },
			{ "LR", "R'L'" },
			{ "UD", "D'U'" },
			{ "F'B'", "BF" },
			{ "L'R'", "RL" },
			{ "U'D'", "DU" },

			{ "F2B2", "B'2F'2" },
			{ "L2R2", "R'2L'2" },
			{ "U2D2", "D'2U'2" },

			{ "FLU", "U'L'F'" },
			{ "F'L'U'", "ULF" },
			{ "BRD", "D'R'B'" },
			{ "B'R'D'", "DRB" },
			{ "FLUBRD", "D'R'B'U'L'F'" },
		};

		//scrambleAlgos.resize(maxSize - 2);
		scrambleAlgos[2] = std::move(scrambleAlgos_size2);

		bool includeNonStandardRotations = false;
		for (unsigned int size = 2; size <= maxSize; ++size)
		{
			if(size >= 4)
				includeNonStandardRotations = true;
			unique_ptr<RubiksCubeModel> model = RubiksCubeModelFactory::getRubiksCubeModel("RubiksCubeModel_v5", size);

			//Add 10 x 100 = 1000 random srambling algos
			for (int len : scramblingAlgoLengths)
				for (int i = 0; i < numAlgoOfEachLength; ++i)
					scrambleAlgos[size].push_back({ model->getScramblingAlgo(len, includeNonStandardRotations), "" });
		}

		//Fill up size == 2 models
		//testInfoAggregateSet.push_back({ "RubiksCubeModel_v3", 2 });
		//testInfoAggregateSet.push_back({ "RubiksCubeModel_v4", 2 });
		for (int i = 0; i < genericModels.size(); ++i)
			testInfoAggregateSet.push_back({ genericModels[i], 2 });

		//Fill up size == 3 models
		//testInfoAggregateSet.push_back({ "RubiksCubeModel_v1", 3 });
		//testInfoAggregateSet.push_back({ "RubiksCubeModel_v2", 3 });
		//testInfoAggregateSet.push_back({ "RubiksCubeModel_v3", 3 });
		//testInfoAggregateSet.push_back({ "RubiksCubeModel_v4", 3 });
		for (int i = 0; i < genericModels.size(); ++i)
			testInfoAggregateSet.push_back({ genericModels[i], 3 });

		for (unsigned int size = 4; size <= maxSize; ++size)
			for (int i = 0; i < genericModels.size(); ++i)
				testInfoAggregateSet.push_back({ genericModels[i], size });
	}

	bool RubiksCubeSolverTest::executeAllTests(unordered_map<int, vector<AlgoPairs>>& scrambleAlgos, vector<testInfoAggregate>& testInfoAggregateSet,
		vector<testInfo>& testInfoSet, bool animate)
	{
		for (testInfoAggregate& modelinfo : testInfoAggregateSet)
		{
			modelinfo.nsAggregateDuration_ = 0;
			modelinfo.numTestCases_ = scrambleAlgos[modelinfo.size_].size();
		}

		unsigned int testNum = 0;
		for(int size = 2; size <= maxSize; ++size)
		{
			const vector<AlgoPairs>& algos = scrambleAlgos[size];
			for(int i = 0; i < algos.size(); ++i)
			{
				for(int j = 0; j < testInfoAggregateSet.size(); ++j)
				{
					if (testInfoAggregateSet[j].size_ >= size)
					{
						testInfoSet.emplace_back(testInfoAggregateSet[j].modelName_, testInfoAggregateSet[j].size_, algos[i].scramble, algos[i].solution);
						executeTest(*testInfoSet.rbegin(), animate, ++testNum);
						testInfoAggregateSet[j].nsAggregateDuration_ += testInfoSet.rbegin()->nsDuration_;
					}
				}
			}
		}

		return true;
	}

	void RubiksCubeSolverTest::executeTest(testInfo& info, bool animate, unsigned int testNum)
	{			
		if (animate)
			refUI_.CreateOkDialog("Test No.: " + to_string(testNum)
				+ "\nModel Name: " + info.modelName_
				+ "\nSize: " + to_string(info.size_));
		unique_ptr<RubiksCubeModel> oldModel = refUI_.replaceModelBy(info.modelName_, info.size_, animate);
		oldModel.reset(nullptr);
		refUI_.Reset(animate); //TODO: replaceModelBy() has a bug. it does not print the cube on screen properly. Reset() is temporary workaround.
		RubiksCubeSolverUtils::RunTimeAssert(refUI_.isSolved());

		if (animate)
			refUI_.CreateOkDialog("Applying scrambling algo: " + info.scrambleAlgo_);
		refUI_.applyAlgorithm(info.scrambleAlgo_, animate);
		//RubiksCubeSolverUtils::RunTimeAssert(!refUI_.isSolved()); //Rubik Cube may or may not be scrmabled depending upon the scrambling algo

		if (!info.idealSolution_.empty())
		{
			if (animate)
				refUI_.CreateOkDialog("Applying Ideal solution: " + info.idealSolution_);
			refUI_.applyAlgorithm(info.idealSolution_, animate);
			RubiksCubeSolverUtils::RunTimeAssert(refUI_.isSolved());

			if (animate)
				refUI_.CreateOkDialog("Going back to scrambled position: " + info.scrambleAlgo_);
			//refUI_.replaceModelBy(modelinfo.modelName, modelinfo.size);
			refUI_.Reset(animate);
			refUI_.applyAlgorithm(info.scrambleAlgo_, false);
			if (animate)
				refUI_.redrawWindow();
			//RubiksCubeSolverUtils::RunTimeAssert(!refUI_.isSolved()); //Rubik Cube may or may not be scrmabled depending upon the scrambling algo
		}

		if (animate)
			refUI_.CreateOkDialog("Solving again: ");
		//int numSteps;
		//unsigned long long duration;
		info.actualSolution_ = refUI_.Solve(info.numSteps_, info.nsDuration_, animate);
		RubiksCubeSolverUtils::RunTimeAssert(refUI_.isSolved());
	}

	void RubiksCubeSolverTest::writeResultsToCSVFile(ofstream& testResultsFile, const vector<testInfo>& testInfoSet)
	{
		if (!testResultsFile.is_open())
			return;

		string columns("ModelName,Size,Steps,Duration (ns),Scrambling Algorithm,Ideal Solution,Actual Solution");
		testResultsFile.write(columns.c_str(), columns.length());

		for (const testInfo& info : testInfoSet)
		{
			testResultsFile.write("\n", 1);

			string modelName(info.modelName_);
			testResultsFile.write(modelName.c_str(), modelName.length());
			
			testResultsFile.write(",", 1);
			string size(to_string(info.size_));
			testResultsFile.write(size.c_str(), size.length());
			
			testResultsFile.write(",", 1);
			string numStepsStr(to_string(info.numSteps_));
			testResultsFile.write(numStepsStr.c_str(), numStepsStr.length());
			
			testResultsFile.write(",", 1);
			std::stringstream buffer;
			constexpr int maxPrecision = 4;
			buffer.precision(maxPrecision);
			buffer << std::fixed;
			buffer.imbue(std::locale(""));
			buffer << info.nsDuration_;
			string durationStr = "\"" + buffer.str() + "\"";
			testResultsFile.write(durationStr.c_str(), durationStr.length());
			
			testResultsFile.write(",", 1);
			testResultsFile.write(info.scrambleAlgo_.c_str(), info.scrambleAlgo_.length());
			
			testResultsFile.write(",", 1);
			testResultsFile.write(info.idealSolution_.c_str(), info.idealSolution_.length());
			
			testResultsFile.write(",", 1);
			testResultsFile.write(info.actualSolution_.c_str(), info.actualSolution_.length());

			testResultsFile.flush();
		}
		testResultsFile.write("\n\n", 2);
	}

	void RubiksCubeSolverTest::writeResultsToCSVFile(ofstream& testResultsFile, const vector<testInfoAggregate>& testInfoAggregateSet)
	{
		if (!testResultsFile.is_open())
			return;

		string columns("ModelName,Size,Total test cases,Aggregate Duration (ns)");
		testResultsFile.write(columns.c_str(), columns.length());
		for (const testInfoAggregate& modelinfo : testInfoAggregateSet)
		{
			testResultsFile.write("\n", 1);

			string modelName(modelinfo.modelName_);
			testResultsFile.write(modelName.c_str(), modelName.length());
			
			testResultsFile.write(",", 1);
			string size(to_string(modelinfo.size_));
			testResultsFile.write(size.c_str(), size.length());
			
			testResultsFile.write(",", 1);
			string numTestCases(to_string(modelinfo.numTestCases_));
			testResultsFile.write(numTestCases.c_str(), numTestCases.length());
			
			testResultsFile.write(",", 1);
			std::stringstream buffer;
			constexpr int maxPrecision = 4;
			buffer.precision(maxPrecision);
			buffer << std::fixed;
			buffer.imbue(std::locale(""));
			buffer << modelinfo.nsAggregateDuration_;
			string durationStr = "\"" + buffer.str() + "\"";
			testResultsFile.write(durationStr.c_str(), durationStr.length());

			testResultsFile.flush();
		}
		testResultsFile.write("\n\n", 2);
	}

	string RubiksCubeSolverTest::getCurrentLocalTimeInNanoSeconds2()
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


}