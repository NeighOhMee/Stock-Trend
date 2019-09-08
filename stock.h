#ifndef __stock_h_
#define __stock_h_
#include <unordered_map>
#include "date.h"
#include <vector>

class StockDay
{
	public:
		StockDay(const std::string &a_close, const std::string &a_volume, const std::string &a_open, 
			const std::string &a_high, const std::string &a_low);
		double getClose() const {return close;}
		double getVolume() const {return volume;}
		double getOpen() const {return open;}
		double getHigh() const {return high;}
		double getLow() const {return low;}
	private:
		double close, volume, open, high, low;
};


typedef std::unordered_map<Date, StockDay, hashFunct> stockMap;

class Stock
{
	public:
		Stock(const std::string &fileName);
		void printData(const std::string &date) const;
		std::string findTrend(const Date &date1, const Date &date2) const;
		void currentTrend(const std::string &startDate) const;
		std::string currentTrendShort(const std::string &startDate) const;
		bool exhibitsBehavior(const std::string &startDate) const;
	private:
		stockMap stockData;
		Date mostCurrentDay;
};

Date convertDateToInt(const std::string &date);
Date convertDateToIntV2(const std::string &date);
double getAverage(int start, int end, const std::vector<double> &data);
#endif