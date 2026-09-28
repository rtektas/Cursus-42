/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <rtektas@student.42belgium.be>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:44:22 by rtektas           #+#    #+#             */
/*   Updated: 2026/05/30 22:15:49 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
#define LOG_HPP

#include <string>

namespace Logs 
{
    enum Level { DEBUG, INFO, WARN, ERROR, FATAL };

    // Variable globale (déclaration seulement - définition dans .cpp)
    extern Level current_level;

    inline void set_level(Level level)
    {
        current_level = level; 
    }
    inline Level get_level()
    {
        return current_level;
    }

    // Helper to_string
    template <typename T>
    std::string to_string(const T& value);

    // Classe template Formatter (déclaration)
    template <typename T>
    struct Formatter;

    namespace internal
    {
        // Logs simple (définition dans .ipp car inline)
        void log(Level level, const std::string& message);

        // Dispatcher template
        template <typename T>
        std::string format(const T& value);
    }

    // Overloads pour std::string (pas de template = pas d'ambiguïté)
    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message, const std::string& exception = "");
    void fatal(const std::string& message, const std::string& exception = "");

    // Templates pour objets complexes
    template <typename T>
    void debug(const T& obj);

    template <typename T>
    void info(const T& obj);

    template <typename T>
    void warn(const T& obj);

    template <typename T>
    void debug(const std::string& prefix, const T& obj);
}

// Inclusion des implémentations templates (obligatoire C++98)
#include <g_utils/Logs.ipp>

#endif

