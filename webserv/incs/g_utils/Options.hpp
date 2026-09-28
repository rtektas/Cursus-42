/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Options.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 08:36:21 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/06 19:17:00 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <utils/Logs.hpp>
#include <utils/utils.hpp>
#include <iostream>
#include <string>
#include <unistd.h>

// Command-line options structure
struct Options
{
	std::string		config_file;		// Path to the configuration file
	bool			show_help;			// Flag to show help message
	bool			verbose_logging;	// Enable verbose logging

	// Constructor with default values
	Options() : config_file(""), show_help(false), verbose_logging(false) {}
};

namespace OptionsParser
{
	using io::out;
	// Display usage information
	void	print_usage(const char *program_name)
	{
		out	<< "Usage: " << program_name << " [options] [config_file]" << "\n"
			<< "Options:" << "\n"
			<< "  -c <file>   Specify configuration file"	<< "\n"
			<< "  -v          Enable verbose logging"		<< "\n"
			<< "  -h          Display this help message"	<< "\n"
			<< "\n"
			<< "Config file can be specified either with "
			<< "-c flag or as a position argument." 		<< "\n"
			<< "If not specified, default configuration"
			<< "will be used."								<< "\n";
	}

	// Parse command line arguments into Options structure
	Options parse(int ac, char **av)
	{
		Options	opt;
		int		opt_val;

		// Parse command line options:
		while ((opt_val = getopt(ac, av, "c:vh")) != -1)
		{
			switch (opt_val)
			{
				case 'c':
					opt.config_file = optarg;
					break;
				case 'v':
					opt.verbose_logging = true;
					break;
				case 'h':
					opt.show_help = true;
					break;
				default:
					// Invalid option
					opt.show_help = true;
					return (opt);
			}
		}

		// Check for positional arguments (config file without -c flag)
		if (optind < ac)
		{
			// If config file was already set with -c, this is an error
			if (opt.config_file.empty())
			{
				std::cerr	<< "Error: Config file specified both with -c flag and as positional argument" << "\n";
				opt.show_help = true;
				return (opt);
			}

			// If more than one positional argument, this is an error
			if (optind + 1 < ac)
			{
				std::cerr	<< "Error: To many arguments" << "\n";
				opt.show_help = true;
				return (opt);
			}

			// Set config file from positional argument
			opt.config_file = av[optind];
		}

		return (opt);
	}
}

#endif
