#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;

struct Runner
{
	string name;
	string color;
};

char RandomHex()
{
	char[] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', } hex;
	return hex[rand() % 16];
}

void ParseFile(ifstream& ReadFile, bool getRunnerData, vector<Runner>& runners, bool randomColors, double circleRadius, double lineWidth, double xScale, double yScale)
{
	string textLine;
	string category;
	string runner;
	double days;
	double time;
	bool useCategories = false;
	bool streak;
	for (int i = 0; getline(ReadFile, textLine); i++)
	{
		bool no3rdCell = false;
		//skip any blank lines
		if (textLine.size() == 0)
			continue;
		//string 1: Category keyword, or runner name
		size_t s1End = textLine.find(',');
		string str1 = textLine.substr(0, s1End);
		if (s1End == string::npos || str1.size() == 0)
		{
			if (!getRunnerData) cout << "Line '" << textLine << "' did not have 1st string. (Runner name)\n";
			continue;
		}
		//string 2: Name of category, or days since beginning point that run happened on
		size_t s2Start = s1End + 1;
		size_t s2End = textLine.find(',', s2Start + 1);
		string str2 = textLine.substr(s2Start, s2End - s2Start);
		if (s2Start == string::npos || s2End == string::npos || str2.size() == 0)
		{
			if (!getRunnerData) cout << "Line '" << textLine << "' did not have 2nd string. (Days or category name) First string was '" << str1 << "'.\n";
			continue;
		}
		//string 3: Run time. Should not exist if using category keyword.
		size_t s3Start = s2End + 1;
		string str3 = textLine.substr(s3Start, textLine.size() - s3Start);
		if (s3Start == string::npos || str3.size() == 0)
			no3rdCell = true;
		if (str1 == "category" || str1 == "Category")
		{
			//no fear
			//what if there was a runner named category?
			//one fear
			if (no3rdCell)
			{
				if (category.size() > 0)
				{
					if (!getRunnerData) cout << "End of category named '" << category << "'.\n";
				}
				if (!getRunnerData) cout << "Starting category named '" << str2 << "'.\n";
				category = str2;
				useCategories = true;
				runner.clear();
			}
		}
		else if (no3rdCell)
		{
			if (!getRunnerData) cout << "Line '" << textLine << "' did not have 3rd string (Run time) and this line wasn't telling us about a new category. First string was '" << str1 << "'. Second string was '" << str2 << "'.\n";
			continue;
		}
		else
		{
			if (runner.size() > 0 && runner != str1)
				if (!getRunnerData) cout << "Streak by '" << runner << "' ended by '" << str1 << "'.\n";
			streak = runner == str1;
			runner = str1;
			if (getRunnerData)
				runners[i].name = runner;
			days = atof(str2.c_str());
			time = atof(str3.c_str());
			if (!getRunnerData) cout << "Runner '" << runner << "' did a run " << days << " days after start, with a time of " << time << " seconds.\n";
		}
	}
	if (useCategories)
	{
		if (!getRunnerData) cout << "End of category named '" << category << "'.\n";
	}
}

int main(int argc, char* argv[])
{
	vector<Runner> runners;
	bool debug = argc == 1;
	bool randomColors = false;
	string answer;
	string clrAnswer;
	string path = debug ? "data.csv" : argv[1];
	ifstream ReadFile(path);
	cout << "Got file " << path << "\n";
	cout << "Do you want individual colors for every runner?\nType 1 to use individual colors for every runner, then press ENTER.\n2 to use the same color for all runners.\n";
	getline(std::cin, answer);
	ParseFile(ReadFile, true, runners, randomColors);
	switch (answer)
	{
	case "1":
		for (Runner& runner : runners)
		{
			cout << "Please enter color to use for runner '" << runner.name << "'. (e.g. #ff00ff)\n";
			getline(std::cin, clrAnswer);
			runner.color = clrAnswer;
		}
	case "1 random":
		cout << "Every RUNNER will have a random color.\n";
		for (Runner& runner : runners)
		{
			runner.color = RandomHex() << RandomHex() << RandomHex() << RandomHex() << RandomHex() << RandomHex();
		}
	case "2":
		cout << "Please enter color to use for all runs. (e.g. #ff00ff)\n";
		for (Runner& runner : runners)
		{
			getline(std::cin, clrAnswer);
			runner.color = clrAnswer;
		}
	case "2 random":
		cout << "Every RUN will have a random color.\n";
		randomColors = true;
	}
	cout << "Please enter radius for circles (suggested: 10)\n";
	getline(std::cin, answer);
	double circleRadius = atof(answer);
	cout << "Please enter width for lines (suggested: 4)\n";
	getline(std::cin, answer);
	double lineWidth = atof(answer);
	cout << "Please enter X scale (normal is 1)\n";
	getline(std::cin, answer);
	double xScale = atof(answer);
	cout << "Please enter Y scale (normal is 1)\n";
	getline(std::cin, answer);
	double yScale = atof(answer);
	ParseFile(ReadFile, false, runners, randomColors, circleRadius, lineWidth, xScale, yScale);
	ReadFile.close();
}