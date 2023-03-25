/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   BitcoinExchange.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: lucas <lucas@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/25 19:55:07 by lucas         #+#    #+#                 */
/*   Updated: 2023/03/25 19:55:07 by lucas         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <fstream>
# include <string>
# include <map>

class BitcoinExchange {
	private:
		BitcoinExchange(); // Not implemented, need db

		typedef std::greater_equal<std::string> BtcComp;
		typedef std::map<std::string, float, BtcComp> BtcDatabase;

		BtcDatabase _database;
	public:
		BitcoinExchange(const char *db);
		BitcoinExchange(const BitcoinExchange &other);
		~BitcoinExchange();

		BitcoinExchange &operator=(const BitcoinExchange &other);

		void	applyExchangeRate(const char *file);
};


#endif //BITCOINEXCHANGE_HPP
