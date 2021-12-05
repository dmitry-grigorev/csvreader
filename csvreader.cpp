// csvreader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef pair<vector<string>, vector<vector<double>>> dataframe;

class CSVReader
{
public:
	CSVReader() = default;
private:
	vector<string> readnames(ifstream &in, const char delimiter)
	{
		vector<string> names;
		string line;
		getline(in, line);
		auto lineit = line.begin(), lineend = line.end();
		auto namebegin = lineit;
		while (lineit != lineend)
		{
			if (*lineit == delimiter)
			{
				names.push_back(string(namebegin, lineit));
				namebegin = ++lineit;
			}
			++lineit;
		}
		names.push_back(string(namebegin, lineit));

		return names;
	}

	vector<vector<double>> readdata(ifstream &in, const unsigned int datawidth, const char delimiter)
	{
		vector<vector<double>> data;
		vector<double> currline(datawidth);
		string line;
		string::iterator lineit, lineend, databegin;
		unsigned int currlinenumber = 1, currlinesize = 0;
		while (getline(in, line))
		{
			lineit = line.begin();
			databegin = lineit;
			lineend = line.end();
			try
			{
				while (lineit != lineend)
				{
					if (*lineit == delimiter )
					{
						currline.at(currlinesize++) = stod(string(databegin, lineit));
						databegin = ++lineit;
					}
					++lineit;
				}
				currline.at(currlinesize++) = stod(string(databegin, lineit));
			}
			catch (out_of_range)
			{
				throw length_error(string("Line ") + to_string(currlinenumber) + string(" size is greater than the number of features"));
			}
			if (currlinesize < datawidth)
			{
				throw length_error(string("Line ") + to_string(currlinenumber) + string(" size is less than the number of features"));
			}
			data.push_back(currline);
			++currlinenumber;
			currlinesize = 0;
		}

		return data;
	}
public:
	dataframe read(const char *filename, const char delimiter)
	{
		dataframe df;
		ifstream in(filename);
		df.first = readnames(in, delimiter);
		try
		{
			df.second = readdata(in, df.first.size(), delimiter);
		}
		catch (length_error &e)
		{
			cout << e.what() << endl;
			in.close();
		}
		in.close();
		return df;
	}
};

int main()
{
	CSVReader csvreader;
	dataframe df = csvreader.read("successful_example.csv", ',');

	for (auto namesit = df.first.begin(), namesend = df.first.end(); namesit != namesend; ++namesit)
		cout << *namesit << ' ';
	cout << endl;
	for (auto rowsit = df.second.begin(), rowsend = df.second.end(); rowsit != rowsend; ++rowsit)
	{
		for (auto colit = (*rowsit).begin(), colend = (*rowsit).end(); colit != colend; ++colit)
			cout << *colit << ' ';
		cout << endl;
	}
	cout << endl;
	return 0;
}

