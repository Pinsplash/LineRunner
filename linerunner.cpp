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

struct Run
{
	string name;
	string color;
	double days = 0;
	double time = 0;
};

struct Category
{
	string name;
	string color = "none";
	vector<Run> runs;
};

enum ColorMode
{
	COLOR_BY_RUNNER,
	COLOR_BY_RUNNER_RANDOM,
	COLOR_MONO,
	COLOR_MONO_RANDOM,
	COLOR_BY_CATEGORY,
	COLOR_BY_CATEGORY_RANDOM
};

struct GraphProperties
{
	ColorMode colormode = COLOR_BY_RUNNER;
	double circleRadius;
	double lineWidth;
	double xScale;
	double yScale;
	double width;
	double height;
	double lowestTime;
};

string RandomHex()
{
	string hex = "0123456789abcdef";
	string s(1, hex[rand() % 16]);
	return s;
}

string RandomColor()
{
	string start = "#";
	return start.append(RandomHex()).append(RandomHex()).append(RandomHex()).append(RandomHex()).append(RandomHex()).append(RandomHex());
}

string GetRunnerColor(vector<Runner>& runners, string name)
{
	for (Runner& runner : runners)
	{
		if (!strcmp(runner.name.c_str(), name.c_str()))
		{
			return runner.color;
		}
	}
	return "";
}

void ParseFile(ifstream& ReadFile, vector<Runner>& runners, vector<Category>& categories, GraphProperties& props)
{
	string textLine;
	string category;
	string name;
	double days;
	double time;
	double biggestTime = 0;
	double smallestTime = FLT_MAX;
	bool useCategories = false;
	while (getline(ReadFile, textLine))
	{
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
		bool no3rdCell = false;
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
				name.clear();
				Category newCat;
				newCat.name = category;
				categories.push_back(newCat);
				continue;
			}
		}
		else if (no3rdCell)
		{
			cout << "Line '" << textLine << "' did not have 3rd string (Run time) and this line wasn't telling us about a new category. First string was '" << str1 << "'. Second string was '" << str2 << "'.\n";
			continue;
		}

		name = str1;

		days = atof(str2.c_str());
		if (days > props.width)
			props.width = days;

		time = atof(str3.c_str());
		if (time > biggestTime)
			biggestTime = time;
		if (time < smallestTime)
			smallestTime = time;

		//if this runner's name is new to us, add to the list
		bool bNewRunner = true;
		int k = -1;
		for (Runner& runner : runners)
		{
			k++;
			if (!strcmp(runner.name.c_str(), name.c_str()))
			{
				bNewRunner = false;
				break;
			}
		}
		if (bNewRunner)
		{
			Runner pNewRunner;
			pNewRunner.name = name;
			runners.push_back(pNewRunner);
			k = (int)runners.size() - 1;
		}
		Run newRun;
		newRun.name = runners[k].name;
		newRun.color = runners[k].color;
		newRun.days = days;
		newRun.time = time;
		//find category to add us to
		for (Category& cat : categories)
		{
			if (cat.name == category)
			{
				cat.runs.push_back(newRun);
			}
		}
		cout << "Runner '" << newRun.name << "' did a run " << newRun.days << " days after start, with a time of " << newRun.time << " seconds.\n";
	}
	props.height = biggestTime - smallestTime;
	props.lowestTime = smallestTime;
}

int main(int argc, char* argv[])
{
	vector<Runner> runners;
	vector<Category> categories;
	GraphProperties props;
	bool debug = argc == 1;
	string answer;
	string clrAnswer;
	string path = debug ? "data.csv" : argv[1];
	ifstream ReadFile(path);
	string base_filename = path.substr(path.find_last_of("/\\") + 1);
	string::size_type const p(base_filename.find_last_of('.'));
	string file_without_extension = base_filename.substr(0, p);
	ofstream writingFile;
	writingFile.open(file_without_extension + ".svg");
	ParseFile(ReadFile, runners, categories, props);

	cout << "\nGot file " << path << "\n";
	cout << "How to color lines and dots?\nType 1 to use individual colors for every runner, then press ENTER.\n2 to use the same color for all runners.\n3 to use different colors for every category\n";
	getline(cin, answer);
	if (answer == "1")
	{
		props.colormode = COLOR_BY_RUNNER;
		for (Runner& runner : runners)
		{
			cout << "Please enter color to use for runner '" << runner.name << "'. (e.g. #ff00ff)\n";
			getline(cin, clrAnswer);
			runner.color = clrAnswer;
		}
	}
	else if (answer == "1 random")
	{
		props.colormode = COLOR_BY_RUNNER_RANDOM;
		cout << "Every RUNNER will have a random color.\n";
		for (Runner& runner : runners)
			runner.color = RandomColor();
	}
	else if (answer == "2")
	{
		props.colormode = COLOR_MONO;
		cout << "Please enter color to use for all runs. (e.g. #ff00ff)\n";
		getline(cin, clrAnswer);
		for (Runner& runner : runners)
			runner.color = clrAnswer;
	}
	else if (answer == "2 random")
	{
		props.colormode = COLOR_MONO_RANDOM;
		cout << "Every RUN will have a random color.\n";
	}
	else if (answer == "3")
	{
		props.colormode = COLOR_BY_CATEGORY;
		for (Category& category : categories)
		{
			cout << "Please enter color to use for category '" << category.name << "'. (e.g. #ff00ff)\n";
			getline(cin, clrAnswer);
			category.color = clrAnswer;
		}
	}
	else if (answer == "3 random")
	{
		props.colormode = COLOR_BY_CATEGORY_RANDOM;
		cout << "Every CATEGORY will have a random color.\n";
		for (Category& category : categories)
			category.color = RandomColor();
	}
	else
	{
		cout << "Didn't recognize answer.\n";
		cin.get();
		return 0;
	}
	cout << "Please enter radius for circles (suggested: 10)\n";
	getline(cin, answer);
	props.circleRadius = atof(answer.c_str());
	cout << "Please enter width for lines (suggested: 4)\n";
	getline(cin, answer);
	props.lineWidth = atof(answer.c_str());
	cout << "Please enter X scale (normal is 1)\n";
	getline(cin, answer);
	props.xScale = atof(answer.c_str());
	cout << "Please enter Y scale (normal is 1)\n";
	getline(cin, answer);
	props.yScale = atof(answer.c_str());

	cout << "Starting writing to " << file_without_extension << ".cfg\n";
	writingFile << "<?xml version=\"1.0\" encoding=\"UTF - 8\" standalone=\"no\"?>\n";
	writingFile << "<svg width=\"" << props.width * props.xScale << "mm\" height=\"" << props.height * props.yScale << "mm\" viewBox=\"0 0 " << props.width * props.xScale << " " << props.height * props.yScale << "\" version=\"1.1\" id=\"svg1\" > \n";
	for (Category& category : categories)
	{
		writingFile << "<g inkscape:groupmode=\"layer\" id=\"" << category.name << "\">\n";
		string color;
		switch (props.colormode)
		{
		case COLOR_MONO_RANDOM:
			color = RandomColor();
			break;
		case COLOR_BY_CATEGORY:
		case COLOR_BY_CATEGORY_RANDOM:
			color = category.color;
			break;
		default://mono, runner, and runner random already decided colors
			color = GetRunnerColor(runners, category.runs[0].name);
			break;
		}
		string name = category.runs[0].name;
		double days = category.runs[0].days;
		double time = category.runs[0].time;
		writingFile << "<circle style=\"fill:" << color << "\" id=\"" << name << "\" cx=\"" << days * props.xScale << "\" cy=\"" << (time - props.lowestTime) * props.yScale << "\" r=\"" << props.circleRadius << "\"/>\n";
		for (int i = 1; i < category.runs.size(); i++)
		{
			Run& run = category.runs[i];
			writingFile << "<path style=\"stroke-width:" << props.lineWidth << ";stroke:" << color << ";stroke-opacity:1\" d=\"M "
				<< days * props.xScale <<		"," << (time - props.lowestTime) * props.yScale << " "
				<< run.days * props.xScale << "," << (run.time - props.lowestTime) * props.yScale << "\" id=\"" << name << "\"/>\n";
			switch (props.colormode)
			{
			case COLOR_MONO_RANDOM:
				color = RandomColor();
				break;
			case COLOR_MONO:
			case COLOR_BY_RUNNER:
			case COLOR_BY_RUNNER_RANDOM:
				color = GetRunnerColor(runners, run.name);
				break;
			default://category and category random don't change color here
				break;
			}
			name = run.name;
			days = run.days;
			time = run.time;
			writingFile << "<circle style=\"fill:" << color << "\" id=\"" << category.name << "-" << name << "\" cx=\"" << run.days * props.xScale << "\" cy=\"" << (run.time - props.lowestTime) * props.yScale << "\" r=\"" << props.circleRadius << "\"/>\n";
		}
		writingFile << "</g>\n";
	}
	writingFile << "</svg>\n";
	cout << "Finished writing to " << file_without_extension << ".cfg\n";
	writingFile.close();
	ReadFile.close();

	cout << "Done. Press ENTER or the X button to close.\n";
	cin.get();
}