/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alephoen <alephoen@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 17:08:31 by alephoen          #+#    #+#             */
/*   Updated: 2026/04/04 19:18:02 by alephoen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <a_config_parser/ServersConf.hpp>
#include <a_config_parser/ConfParser.hpp>
#include <g_utils/Logs.hpp>

#include <e_server/Server.hpp>
#include <g_utils/Signals.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

void	validate_conf(const ServersConf &servers_conf);

static void print_banner();

int main(int ac, char **av)
{
	// std::cout << "Hello from webserv!" << std::endl;
	print_banner();
	if (ac > 2)
	{
		Logs::error("webserv accept no more than one argument.", "");
		return (EXIT_FAILURE);
	}
	try
	{
		// show help message if requested
		if (ac > 1 && std::string(av[1]) == "-h")
		{
			std::cout << "Display help...\n";
			return (EXIT_SUCCESS);
		}

		// set up signal handlers first
		Signals::init_handlers();

		// Get servers config after parsing (this is owned by mate)
		ServersConf	servers_conf = ConfParser().getServersConf(av[1]);

		validate_conf(servers_conf);

		// Alex continue here...
		// RouteConf	route_conf = RouteConf(servers_conf);
		RouteConf	route_conf(servers_conf);
		Server		Server(route_conf);
		Server.run();

	}
	catch (const std::exception &e)
	{
		Logs::error("webserv: " + std::string(e.what()));
		return (EXIT_FAILURE);
	}
	catch (...)
	{
		Logs::error("webserv: Unkown exception occured");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

template<typename T>
std::string to_string98(const T& value);

void	validate_conf(const ServersConf &servers_conf)
{
	SIZET	servers_size = servers_conf.getServers().size();
	if (servers_size == 0)
		Logs::fatal("server configuration not found.");

	const	VEC_SERVERS_CONF&	servers = servers_conf.getServers();

	SIZET i = -1;
	while (++i < servers_size)
	{
		if (servers[i].getListeners().size() == 0)
			Logs::fatal("server " + to_string98(i + 1) + ": missing listen directive");

		if (servers[i].getRoot() == "")
			Logs::fatal("server " + to_string98(i + 1) + ": missing root directive");

		const VEC_LOCATION_CONF&	locations = servers[i].getLocation();
		SIZET locations_size = locations.size();
		if (locations_size == 0)
			Logs::warn("server " + to_string98(i + 1) + ": no location defined.");
	}
}

// banner.cpp — drop this call into your main() right at the start
// Usage: print_banner();

// #include <iostream>
// 
// static void print_banner()
// {
    // // Colors
    // const char* RESET   = "\033[0m";
    // const char* BOLD    = "\033[1m";
    // const char* RED     = "\033[38;5;196m";
    // const char* ORANGE  = "\033[38;5;208m";
    // const char* YELLOW  = "\033[38;5;220m";
    // const char* CYAN    = "\033[38;5;51m";
    // const char* WHITE   = "\033[38;5;255m";
    // const char* GRAY    = "\033[38;5;240m";
    // const char* PURPLE  = "\033[38;5;135m";
// 
    // std::cout
        // << "\n"
        // << BOLD << RED
        // << "  ██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗\n"
        // << ORANGE
        // << "  ██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║\n"
        // << YELLOW
        // << "  ██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║\n"
        // << CYAN
        // << "  ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝\n"
        // << PURPLE
        // << "  ╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ \n"
        // << WHITE
        // << "   ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  \n"
        // << RESET
        // << "\n"
        // << GRAY   << "  ─────────────────────────────────────────────────────────────\n" << RESET
        // << WHITE  << "  " << BOLD << "42 School" << RESET << WHITE << " │ HTTP/1.1 Web Server" << RESET
        // << GRAY   << " │ " << RESET
        // << CYAN   << "C++98" << RESET << "\n"
        // << GRAY   << "  ─────────────────────────────────────────────────────────────\n" << RESET
        // << "\n";
// }

// banner.cpp — drop this call into your main() right at the start
// Usage: print_banner();


// banner.cpp — drop this call into your main() right at the start
// Usage: print_banner();

#include <iostream>

static void print_banner()
{
    const char* RESET   = "\033[0m";
    const char* BOLD    = "\033[1m";
    const char* RED     = "\033[38;5;196m";
    const char* ORANGE  = "\033[38;5;208m";
    const char* YELLOW  = "\033[38;5;220m";
    const char* CYAN    = "\033[38;5;51m";
    const char* WHITE   = "\033[38;5;255m";
    const char* GRAY    = "\033[38;5;240m";
    const char* PURPLE  = "\033[38;5;135m";
    const char* GOLD    = "\033[38;5;214m";

    std::cout
        << "\n"

        // WEBSERV banner
        << BOLD << RED    << "  ██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗\n" << RESET
        << BOLD << ORANGE << "  ██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║\n" << RESET
        << BOLD << YELLOW << "  ██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║\n" << RESET
        << BOLD << CYAN   << "  ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝\n" << RESET
        << BOLD << PURPLE << "  ╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ \n" << RESET
        << BOLD << WHITE  << "   ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝ \n" << RESET

        << "\n"

        // Phoenix
        << BOLD << YELLOW << "                   \U0001F525     _,     ,_     \U0001F525\n" << RESET
        << BOLD << GOLD   << "                        .'/  ,_   \\'.\n" << RESET
        << BOLD << ORANGE << "                       |  \\__( >__/  |\n" << RESET
        << BOLD << RED    << "                       \\             /\n" << RESET
        << BOLD << ORANGE << "                        '-..__ __..-'\n" << RESET
        << BOLD << YELLOW << "                             /_\\ \n" << RESET
        << "\n"
        << BOLD << RED    << "                   \U0001F525 " << GOLD << " PHOENIX WEBSERV" << RED << "  \U0001F525\n" << RESET

        << "\n"
        << GRAY   << "  ─────────────────────────────────────────────────────────────\n" << RESET
        << WHITE  << "  " << BOLD << "42 School" << RESET
        << WHITE  << " │ HTTP/1.1 Web Server" << RESET
        << GRAY   << " │ " << RESET
        << CYAN   << "C++98" << RESET
        << GRAY   << " │ " << RESET
        << GOLD   << "Phoenix Edition \U0001F525" << RESET << "\n"
        << GRAY   << "  ─────────────────────────────────────────────────────────────\n" << RESET
        << "\n";
}
















