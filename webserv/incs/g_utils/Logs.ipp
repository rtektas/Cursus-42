/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.ipp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <rtektas@student.42belgium.be>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:44:22 by rtektas           #+#    #+#             */
/*   Updated: 2026/05/30 22:15:49 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_IPP
#define LOG_IPP

#include <sstream>
#include <iostream>
#include <stdexcept>

// ==========================================
// Helper to_string
// ==========================================
template <typename T>
std::string Logs::to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// ==========================================
// Formatter generique (fallback)
// ==========================================
namespace Logs
{
    template <typename T>
    struct Formatter
    {
        static std::string format(const T& value)
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    };

    // Spécialisations pour types standards UNIQUEMENT
    template <>
    struct Formatter<std::string>
    {
        static const std::string& format(const std::string& str) 
        {
            return str;
        }
    };

    template <>
    struct Formatter<const char*>
    {
        static std::string format(const char* str) 
        {
            return std::string(str);
        }
    };
}

// ==========================================
// Internal functions
// ==========================================
namespace Logs
{
    namespace internal
    {
		inline void log(Level level, const std::string& message)
		{
			if (level < current_level) return;

			const char* level_str[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
			const char* level_col[] = {
				"\033[38;5;244m",   // DEBUG — gray
				"\033[38;5;51m",    // INFO  — cyan
				"\033[38;5;220m",   // WARN  — yellow
				"\033[38;5;196m",   // ERROR — red
				"\033[38;5;199m"    // FATAL — magenta
			};
			const char* RESET = "\033[0m";
			const char* BOLD  = "\033[1m";

			std::cout << level_col[level] << BOLD
					  << "[" << level_str[level] << "]"
					  << RESET << " "
					  << message << std::endl;
		}

        template <typename T>
        std::string format(const T& value)
        {
            return Formatter<T>::format(value);
        }
    }
}

// ==========================================
// String overloads
// ==========================================
namespace Logs
{
    inline void debug(const std::string& message)
    {
        internal::log(DEBUG, message);
    }
    inline void info(const std::string& message)
    {
        internal::log(INFO, message);
    }
    inline void warn(const std::string& message)
    {
        internal::log(WARN, message);
    }
    inline void error(const std::string& message, const std::string& exception)
    {
        internal::log(ERROR, message + (exception.empty() ? "" : " [Exception: " + exception + "]"));
    }
    inline void fatal(const std::string& message, const std::string& exception)
    {
        internal::log(FATAL, message + (exception.empty() ? "" : " [Exception: " + exception + "]"));
		throw std::runtime_error(message);
    }
}

// ==========================================
// Templates objets
// ==========================================
namespace Logs
{
    template <typename T>
    void debug(const T& obj)
    {
        internal::log(DEBUG, internal::format(obj));
    }

    template <typename T>
    void info(const T& obj)
    {
        internal::log(INFO, internal::format(obj));
    }

    template <typename T>
    void warn(const T& obj)
    {
        internal::log(WARN, internal::format(obj));
    }

    template <typename T>
    void debug(const std::string& prefix, const T& obj)
    {
        internal::log(DEBUG, prefix + " " + internal::format(obj));
    }
}

#endif

