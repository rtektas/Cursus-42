/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 23:36:40 by isadbaib          #+#    #+#             */
/*   Updated: 2026/04/27 12:37:40 by isadbaib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READFILE_HPP
#define READFILE_HPP

#include <string>
#include <fstream>
#include <g_utils/Types.hpp>

class ReadFile
{
	private :
		bool          error_;
		std::ifstream file_;
		VEC_STR       data_;
		STR           dataGross_;
	public :
		  ReadFile();
		  ReadFile(STR const &namefile);
	    ~ReadFile();

		  void    create(std::string const &nameFile);
	    STR     getData();
		  STR     getDataStringFilter(bool lowercase);
		  VEC_STR	getDataVector(bool lowercase);
		  VEC_STR getDataVectorFilter(bool lowercase);
		  bool    getError();

};

#endif

