/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BitcoinExchange.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucas <lucas@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/25 19:55:10 by lucas         #+#    #+#                 */
/*   Updated: 2023/03/25 19:55:10 by lucas         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <BitcoinExchange.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>

static bool printError(const std::string &line) {
	std::cerr << "Error: bad input: " << line << '\n';
	return false;
}

static bool validateDate(std::string &date) {
	if (date.length() != 10)
		return printError(date);

	std::istringstream str(date);
	int val;
	str >> val;
	if (str.fail() || val < 0 || val > 9999 || str.peek() != '-')
		return printError(date);
	str.ignore();
	str >> val;
	if (str.fail() || val < 1 || val > 12 || str.peek() != '-')
		return printError(date);
	str.ignore();
	str >> val;
	if (str.fail() || val < 1 || val > 31 || !str.eof())
		return printError(date);
	return true;
}

static bool parseLine(std::ifstream &in, const char *delims, bool checkVal, std::string &date, float &val) {
	std::string line;
	std::getline(in, line);

	if (in.fail())
		return false;
	// find end of date
	size_t i = line.find_first_of(delims);
	if (i == std::string::npos)
		return printError(line);
	// get date
	date = line.substr(0, i);
	if (!validateDate(date))
		return false;
	// skip spaces and delim
	i = line.find_first_not_of(delims, i);
	// get value
	const char *floatP = line.c_str() + i;
	char *endP;
	val = std::strtof(floatP, &endP);
	if (val < 0 || (checkVal && (val == 0 || val > 1000)))
		return printError(std::string(floatP));
	return true;
}

BitcoinExchange::BitcoinExchange(const char *db) {
	std::ifstream db_in;
	db_in.exceptions(std::ifstream::badbit);
	db_in.open(db);
	if (db_in.fail())
		throw std::ios_base::failure("Failed to open file");

	std::string header;
	std::getline(db_in, header);
	if (header != "date,exchange_rate")
		std::cerr << "Invalid file header...\n";

	while (!db_in.eof()) {
		std::string date; float val;

		if (!parseLine(db_in, ",", false, date, val))
			continue;
		_database.insert(std::make_pair(date, val));
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
	_database = other._database;
}

// Destructor
BitcoinExchange::~BitcoinExchange() {}

// Operators
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other) {
		this->_database = other._database;
	}
	return *this;
}

static std::string getValueString(float val) {
	std::ostringstream stream;
	stream << std::fixed << val;

	std::string str = stream.str();
	str.erase(str.find_last_not_of("0.") + 1);
	return (str);
}

void BitcoinExchange::applyExchangeRate(const char *file) {
	std::ifstream input;
	input.exceptions(std::ifstream::badbit);
	input.open(file);
	if (input.fail())
		throw std::ifstream::failure("Failed to open file");

	std::string header;
	std::getline(input, header);
	if (header != "date | value")
		printError(header);
	while (!input.eof()) {
		std::string date; float val;
		if (!parseLine(input, " |", true, date, val))
			continue;

		BtcDatabase::iterator rate = _database.upper_bound(date);
		if (rate == _database.end()) {
			std::cerr << "Error: No date before : " << date << "\n";
			continue;
		}

		const float newval = val * rate->second;
		std::cout << date << " => " << getValueString(val)
				  << " = " << getValueString(newval) << "\n";
	}
}
