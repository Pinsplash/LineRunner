#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;

void ParseFile(ifstream& ReadFile)
{
	string textLine;
	string category;
	string runner;
	double days;
	double time;
	bool useCategories = false;
	bool streak;
	while (getline(ReadFile, textLine))
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
			cout << "Line '" << textLine << "' did not have 1st string. (Runner name)\n";
			continue;
		}
		//string 2: Name of category, or days since beginning point that run happened on
		size_t s2Start = s1End + 1;
		size_t s2End = textLine.find(',', s2Start + 1);
		string str2 = textLine.substr(s2Start, s2End - s2Start);
		if (s2Start == string::npos || s2End == string::npos || str2.size() == 0)
		{
			cout << "Line '" << textLine << "' did not have 2nd string. (Days or category name) First string was '" << str1 << "'.\n";
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
					cout << "End of category named '" << category << "'.\n";
				}
				cout << "Starting category named '" << str2 << "'.\n";
				category = str2;
				useCategories = true;
				runner.clear();
			}
		}
		else if (no3rdCell)
		{
			cout << "Line '" << textLine << "' did not have 3rd string (Run time) and this line wasn't telling us about a new category. First string was '" << str1 << "'. Second string was '" << str2 << "'.\n";
			continue;
		}
		else
		{
			if (runner.size() > 0 && runner != str1 && streak)
				cout << "Streak by '" << runner << "' ended by '" << str1 << "'.\n";
			streak = runner == str1;
			runner = str1;
			days = atof(str2.c_str());
			time = atof(str3.c_str());
			cout << "Runner '" << runner << "' did a run " << days << " days after start, with a time of " << time << " seconds.\n";
		}
	}
}

int main(int argc, char* argv[])
{
	bool debug = argc == 1;
	string path = debug ? "data.csv" : argv[1];
	cout << path << "\n";
	ifstream ReadFile(path);
	ParseFile(ReadFile);
	ReadFile.close();
}