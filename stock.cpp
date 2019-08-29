#include "stock.h"
#include <string>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <algorithm>

Stock::Stock(const std::string &fileName)
{	//opens the csv file and creates a hash table of each day to the data
	//of that day(close, open ect..)
	std::ifstream file(fileName);
	//std::cout << fileName << std::endl;
	assert(file.is_open());
	std::string date;
	std::string close, volume, open, high, low;
	bool found = false;
	while (file.good())
	{
		std::getline(file, date, ',');
		//checks to see if the first entry in the line is a date, if not ignore it
		//std::cout << date << std::endl;
		if (date.find("/") == -1)
		{
			std::getline(file, date);
			continue;
		}
		std::getline(file, open, ',');
		std::getline(file, high, ',');
		std::getline(file, low, ',');
		std::getline(file, close, ',');
		std::getline(file, volume, '\n');

		/*
		std::getline(file, close, ',');
		std::getline(file, volume, ',');
		std::getline(file, open, ',');
		std::getline(file, high, ',');
		std::getline(file, low, '\n');
		*/
		//removes the "" from each string
		date.erase(std::remove(date.begin(), date.end(), '"'), date.end());
		close.erase(std::remove(close.begin(), close.end(), '"'), close.end());
		volume.erase(std::remove(volume.begin(), volume.end(), '"'), volume.end());
		open.erase(std::remove(open.begin(), open.end(), '"'), open.end());
		high.erase(std::remove(high.begin(), high.end(), '"'), high.end());
		low.erase(std::remove(low.begin(), low.end(), '"'), low.end());

		//Date day = convertDateToInt(date);
		Date day = convertDateToIntV2(date);
		if (!found)
		{
			mostCurrentDay = day;
			//std::cout << "MOST CURRENT" << day << std::endl;
			found = true;
		}
		//ignores the day if it is already in the hashtable
		if (stockData.find(day) != stockData.end())
		{
			std::cerr << date << " is already acounted for" << std::endl;
			continue;
		}
		stockData.insert(std::pair<Date, StockDay>(day, StockDay(close, volume, open, high, low)));
	}
	file.close();
	//std::cout << "constructed" << std::endl;
	//std::cout << mostCurrentDay << std::endl;
}

void Stock::printData(const std::string &date) const
{	//prints the stock data for the date
	Date day = convertDateToInt(date);
	stockMap::const_iterator map_it = stockData.find(day);
	if (map_it == stockData.end())
	{
		std::cerr << "No records available for " << date << std::endl;
		return;
	}
	const StockDay *pt = &map_it->second;
	std::cout << "Close: " << pt->getClose() << "\nVolume: " << pt->getVolume() << "\nOpen: " << pt->getOpen() << "\nHigh: "
		<< pt->getHigh() << "\nLow: " << pt->getLow() << std::endl;
}

std::string Stock::findTrend(const Date &date1, const Date &date2) const
{
	stockMap::const_iterator map_it1 = stockData.find(date1);
	stockMap::const_iterator map_it2 = stockData.find(date2);
	if (map_it1 == stockData.end() || map_it2 == stockData.end())
	{
		std::cerr << "Data for one of the entered dates could not be found" << std::endl;
		return "null";
	}
	const StockDay *day1 = &map_it1->second;
	const StockDay *day2 = &map_it2->second;
	//higher highs and higher lows signify an uptrend
	if (day2->getHigh() > day1->getHigh() && day2->getLow() > day1->getLow())
	{
		return "uptrend";
	}
	else if (day2->getHigh() < day1->getHigh() && day2->getLow() < day1->getLow())
	{
		return "downtrend";
	}
	else
	{
		return "unknown";
	}
}

void Stock::currentTrend(const std::string &date) const
{	//calculates the of the current stock
	Date day = convertDateToInt(date);
	stockMap::const_iterator map_it = stockData.find(day);
	std::vector<double> closes;

	while (day < mostCurrentDay)
	{
		//std::cout << day << std::endl;
		if (map_it != stockData.end()) closes.push_back(map_it->second.getClose());
		++day;
		map_it = stockData.find(day);
	}
	std::cout << "done" << std::endl;
	//get an average for each week
	assert(closes.size() >= 7);
	int startI = 0;
	int endI = 4;
	std::vector<double> averages;
	while (1)
	{
		averages.push_back(getAverage(startI, endI, closes));
		//stops the loop once the end of the vector is reached
		if (endI == closes.size() - 1) break;
		startI = endI + 1;
		if (endI + 5 >= closes.size())
		{
			endI = closes.size() - 1;
		}
		else endI += 5;
	}
	int weeks = 0;
	std::string trend = "unknown";
	for (int i = averages.size() - 1; i != 0; --i)
	{
		std::cout << averages[i] << std::endl;
		if (averages[i] > averages[i - 1])
		{
			if (trend == "uptrend") weeks++;
			else if (trend == "downtrend")
			{
				std::cout << "downtrend for " << weeks << "weeks" << std::endl;
				return;
			}
			else
			{
				trend = "uptrend";
				weeks++;
			}
		}
		else if (averages[i] < averages[i - 1])
		{
			if (trend == "uptrend")
			{
				std::cout << "uptrend for " << weeks << " weeks" << std::endl;
				return;
			}
			else if (trend == "downtrend") weeks++;
			else
			{
				trend = "downtrend";
				weeks++;
			}
		}
		else
		{	//sideways
			weeks++;
		}
	}
	std::cout << trend << " for " << weeks << " weeks" << std::endl;
	return;
	/*
	double lastHigh, lastLow, currentHigh, currentLow = 0;
	std::string currentTrend = "unknown";
	std::string previousDay = date;
	std::string currentDay = date; //change to grab the day after
	for(int i-0; i<6; ++i) //TODO: change to loop over each date in order
	{
		if(findTrend(currentDay, previousDay) == "down") // && and trend of last two days is uptrend
		{	//a high new peak is found
			//currentHigh =
			if(currentHigh > lastHigh)
			{	//still an uptrend if this peak is higher than the last one
				currentTrend = "uptrend"
			}
			else if(currentHigh < lastHigh)
			{	//b

			}
		}
	}
	*/
}


std::string Stock::currentTrendShort(const std::string &startDate) const
{	//finds the short term trend(current day not included)

	//finds a day where the stock market was open for current and previous
	std::cout << mostCurrentDay << std::endl;
	Date currentDay = mostCurrentDay;
	do
	{
		--currentDay;
	} while (stockData.find(currentDay) == stockData.end());

	std::cout << currentDay << std::endl;
	Date previousDay = currentDay;
	do
	{
		--previousDay;
	} while (stockData.find(previousDay) == stockData.end());

	//start day is the earliest day from the stock market
	Date startDay = convertDateToInt(startDate);
	std::string currentTrend = "none";
	std::string newTrend;

	//looks for the trend by comparing the trend of the previous and current day
	while (previousDay >= startDay)
	{
		newTrend = findTrend(previousDay, currentDay);
		if (currentTrend == "none" || currentTrend == "unknown")
		{
			currentTrend = newTrend;
		}
		else if (currentTrend == "uptrend" && newTrend == "downtrend") return currentTrend;
		else if (currentTrend == "downtrend" && newTrend == "uptrend") return currentTrend;

		do
		{
			--currentDay;
		} while (stockData.find(currentDay) == stockData.end());

		do
		{
			--previousDay;
			if (previousDay < startDay) return currentTrend;
		} while (stockData.find(previousDay) == stockData.end());
	}
	return currentTrend;
}

bool Stock::exhibitsBehavior(const std::string &startDate) const
{	//determines if the stock exhibits (.....) the wanted behavior 
	//TODO: check for uptrend in the longterm
	if (currentTrendShort(startDate) == "downtrend")
	{
		std::cout << "down" << std::endl;
		Date currentDay = mostCurrentDay;
		Date previousDay = currentDay;
		do
		{
			--previousDay;
		} while (stockData.find(previousDay) == stockData.end());
		stockMap::const_iterator map_it1 = stockData.find(currentDay);
		stockMap::const_iterator map_it2 = stockData.find(previousDay);
		const StockDay *curD = &map_it1->second;
		const StockDay *prevD = &map_it2->second;
		std::cout << curD->getClose() << std::endl;
		std::cout << prevD->getHigh() << std::endl;
		if (curD->getClose() > prevD->getHigh()) return true;
	}
	return false;
}




StockDay::StockDay(const std::string &a_close, const std::string &a_volume, const std::string &a_open,
	const std::string &a_high, const std::string &a_low)
{

	close = std::stod(a_close);
	volume = std::stod(a_volume);
	open = std::stod(a_open);
	high = std::stod(a_high);
	low = std::stod(a_low);

}




Date convertDateToInt(const std::string &date)
{	//takes a date string a returns a date object of the coresponding date
	int startIndex, endIndex;
	int dates[3];
	startIndex = 0;
	//splits the year/month/year into ints in an array
	for (int i = 0; i < 3; ++i)
	{
		endIndex = date.find("/", startIndex);
		dates[i] = std::stoi(date.substr(startIndex, endIndex - startIndex));
		startIndex = endIndex + 1;
	}
	Date convertedDate(dates[0], dates[1], dates[2]);
	return convertedDate;
}

Date convertDateToIntV2(const std::string &date)
{	//takes a date string a returns a date object of the coresponding date
	int startIndex, endIndex;
	int dates[3];
	startIndex = 0;
	//splits the year/month/year into ints in an array
	for (int i = 0; i < 3; ++i)
	{
		endIndex = date.find("/", startIndex);
		dates[i] = std::stoi(date.substr(startIndex, endIndex - startIndex));
		startIndex = endIndex + 1;
	}
	Date convertedDate(dates[2], dates[0], dates[1]);
	return convertedDate;
}

double getAverage(int start, int end, const std::vector<double> &data)
{	//calculates the average of the values from the start index to the end included
	int size = end - start + 1;
	double average = 0;
	for (int i = start; i <= end; ++i)
	{
		average += data[i];
	}
	average /= size;
	return average;
}