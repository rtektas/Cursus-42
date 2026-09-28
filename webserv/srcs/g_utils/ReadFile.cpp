/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReadFile.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isadbaib <isadbaib@student.42belgium.be>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 23:37:41 by isadbaib          #+#    #+#             */
/*   Updated: 2026/05/17 12:58:46 by camy             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/g_utils/ReadFile.hpp"
#include <g_utils/Logs.hpp>
#include <g_utils/Types.hpp>
#include <g_utils/Convertion.hpp>
#include <string>
#include <cstddef>
#include <vector>
#include <sstream>

ReadFile::~ReadFile()
{
	data_.clear();
}

ReadFile::ReadFile(): error_(true)
{}

ReadFile::ReadFile(std::string const &nameFile): error_(true)
{
    std::ostringstream	oss;
    std::string line;

    file_.open(nameFile.c_str(), std::ifstream::in);
	if (nameFile.empty() || !file_.is_open())
	{
		if (nameFile.empty())
			Logs::error("problem dans le fichier.","");
		else
			Logs::error("problem adresse du fichier","");
		return ;
	}
	oss << file_.rdbuf();
	if (oss.str().empty())
    {
        file_.close();
        return ;
    }
    dataGross_ = oss.str();
    file_.close();
	error_ = false;
}

void ReadFile::create(std::string const &nameFile)
{
	file_.open(nameFile.c_str());
	std::ostringstream	oss;

	if (nameFile.empty() || !file_.is_open())
	{
		error_ = true;
		if (nameFile.empty())
			Logs::error("problem dans le fichier.","");
		else
			Logs::error("problem adresse du fichier","");
		return ;
	}
	oss << file_.rdbuf();
	dataGross_ = oss.str();
	file_.close();
}

std::string ReadFile::getData()
{
	return (dataGross_);
}

std::vector<std::string> ReadFile::getDataVector(bool lowercase)
{
	STR s;
	std::istringstream iss;

	if (data_.size() == 0)
	{
		iss.str(dataGross_);
		while (std::getline(iss, s))
			if (lowercase)
                data_.push_back(Convertion::toLower(s));	
            else
                data_.push_back(s);	
            
	}
	return (data_);
}

std::vector<std::string> ReadFile::getDataVectorFilter(bool lowercase)
{
	using std::string;
	using std::vector;
	using std::stringstream;
	stringstream	sso;
	VEC_STR			filter;
	STR				tmp;
	STR				tmp2;
	size_t			i;

	data_ = getDataVector(lowercase);
	if (data_.size() <= 1)
	{
		error_ = true;
		Logs::error("fichier vide","");
		return (filter);
	}
	i = -1;
	while (++i < data_.size())
	{
		tmp = data_[i].substr(0, data_[i].find("#"));
		sso << tmp;
		while (sso >> tmp2)
			filter.push_back(tmp2);
		sso.clear();
	}
	return (filter);
}

STR ReadFile::getDataStringFilter(bool lowercase)
{
	SIZET	i;
	VEC_STR	s;
	STR		res;

	i = -1;
	s = getDataVectorFilter(lowercase);
	while (++i < s.size())
		res.append(STR(s[i] + "\n"));
	return (res);
}
bool ReadFile::getError()
{
	return (error_);
}

