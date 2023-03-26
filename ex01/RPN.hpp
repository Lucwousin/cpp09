/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   RPN.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lsinke <lsinke@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/26 01:26:28 by lsinke        #+#    #+#                 */
/*   Updated: 2023/03/26 01:26:28 by lsinke        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <string>

class RPN {
	public:
		RPN();
		RPN(const RPN &other);
		~RPN();
		RPN &operator=(const RPN &other);

		void solve(const std::string &input);
};

#endif //RPN_HPP
