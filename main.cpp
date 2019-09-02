#include <stdio.h>
#include <Python.h>
#include <string>
#include <iostream>
#include <fstream>
#include "stock.h"
#include <algorithm>
#include <assert.h>
#include <chrono>

int main(int argc, char const *argv[])
{	//test comment
	std::string stockSymbol;
	std::ifstream inFile("symbols.txt");
	if (!inFile.is_open())
	{
		std::cerr << "Can't find symbols.txt, check the file directory" << std::endl;
		return 1;
	}
	//std::cout << "Enter the stock symbol you wish to check: ";
	//std::cin >> stockSymbol;
	
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");
	pName = PyUnicode_FromString("webscrape");
	/* Error checking of pName left out */

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	auto start = std::chrono::high_resolution_clock::now();
	while (inFile >> stockSymbol)
	{
		const char *c = stockSymbol.c_str();
		/*
		PyObject *pName, *pModule, *pFunc;
		PyObject *pArgs, *pValue;
		Py_Initialize();
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append(\".\")");
		pName = PyUnicode_FromString("webscrape");
		// Error checking of pName left out 

		pModule = PyImport_Import(pName);
		Py_DECREF(pName);
		*/
		if (pModule != NULL) {
			pFunc = PyObject_GetAttrString(pModule, "createTable");
			/* pFunc is a new reference */

			if (pFunc && PyCallable_Check(pFunc)) {
				pArgs = PyTuple_New(1);
				stockSymbol;
				pValue = PyUnicode_FromString(c);
				if (!pValue) {
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, 0, pValue);

				PyObject_CallObject(pFunc, pArgs);
				std::string command;
				pValue = PyObject_CallObject(pFunc, pArgs);
				Py_DECREF(pArgs);
				if (pValue != NULL) {
					//printf("Result of call: %ld\n", PyInt_AsLong(pValue));
					Py_DECREF(pValue);
				}
				else {
					Py_DECREF(pFunc);
					Py_DECREF(pModule);
					PyErr_Print();
					fprintf(stderr, "Call failed\n");
					return 1;
				}
			}
			else {
				if (PyErr_Occurred())
					PyErr_Print();
				fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
			}
			Py_XDECREF(pFunc);
			//Py_DECREF(pModule); What in the world does this do????????
		}
		else {
			PyErr_Print();
			fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
			std::string command;

			while (1)
			{
				std::cin >> command;
				if (command == "q") break;
			}
			return 1;
		}
		//Py_Finalize();

		Stock apple("output.csv");
		std::cout << apple.exhibitsBehavior("2019/05/16") << std::endl;
		std::string command;
		/*
		while (std::cin >> command)
		{
			if (command == "q") break;
			else if (command == "data")
			{
				std::string date;
				std::cin >> date;
				apple.printData(date);
				std::cout << std::endl;
			}
			
			else if(command == "trend")
			{
				std::string date1, date2;
				std::cin >> date1;
				std::cin >> date2;
				std::cout << apple.findTrend(date1, date2) << std::endl;
				std::cout << std::endl;
			}
			
			else if (command == "shorttrend")
			{
				std::string startDate;
				std::cin >> startDate;
				std::cout << apple.currentTrendShort(startDate) << std::endl;
			}
			else if (command == "longtrend")
			{
				std::string startDate;
				std::cin >> startDate;
				apple.currentTrend(startDate);
			}
			else if (command == "behavior")
			{
				std::string startDate;
				std::cin >> startDate;
				std::cout << apple.exhibitsBehavior(startDate) << std::endl;
			}
			else
			{
				std::cout << "Unknown command" << std::endl;
			}
		}
		*/
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << duration.count();
	while (1)
	{
		std::string comm;
		std::cin >> comm;
		if (comm == "q") break;
	}
	Py_Finalize();
	return 0;
}