/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PageError.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtektas <rtektas@student.42belgium.be>    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 20:45:25 by rtektas           #+#    #+#             */
/*   Updated: 2026/05/27 20:45:25 by rtektas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <c_response/PageError.hpp>
#include <g_utils/Logs.hpp>
#include <g_utils/Convertion.hpp>
#include <fstream>
#include <sstream>

// ─── table de tous les codes HTTP ──────────────────────────────────────────
const ErrorEntry PageError::ERROR_TABLE[] =
{
    // 4xx Client Errors
    { 400, "Bad Request" },
    { 401, "Unauthorized" },
    { 402, "Payment Required" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
    { 405, "Method Not Allowed" },
    { 406, "Not Acceptable" },
    { 407, "Proxy Authentication Required" },
    { 408, "Request Timeout" },
    { 409, "Conflict" },
    { 410, "Gone" },
    { 411, "Length Required" },
    { 412, "Precondition Failed" },
    { 413, "Payload Too Large" },
    { 414, "URI Too Long" },
    { 415, "Unsupported Media Type" },
    { 416, "Range Not Satisfiable" },
    { 417, "Expectation Failed" },
    { 418, "I'm a Teapot" },
    { 422, "Unprocessable Entity" },
    { 423, "Locked" },
    { 424, "Failed Dependency" },
    { 426, "Upgrade Required" },
    { 428, "Precondition Required" },
    { 429, "Too Many Requests" },
    { 431, "Request Header Fields Too Large" },
    { 451, "Unavailable For Legal Reasons" },
    // 5xx Server Errors
    { 500, "Internal Server Error" },
    { 501, "Not Implemented" },
    { 502, "Bad Gateway" },
    { 503, "Service Unavailable" },
    { 504, "Gateway Timeout" },
    { 505, "HTTP Version Not Supported" },
    { 507, "Insufficient Storage" },
    { 508, "Loop Detected" },
    { 511, "Network Authentication Required" },
    // sentinel
    {   0, NULL }
};

// ─── isKnown ────────────────────────────────────────────────────────────────
bool PageError::isKnown(int code)
{
    for (int i = 0; ERROR_TABLE[i].code != 0; i++)
        if (ERROR_TABLE[i].code == code)
            return (true);
    return (false);
}

// ─── getMessage ─────────────────────────────────────────────────────────────
const STR PageError::getMessage(int code)
{
    for (int i = 0; ERROR_TABLE[i].code != 0; i++)
        if (ERROR_TABLE[i].code == code)
            return (STR(ERROR_TABLE[i].message));
    return ("Unknown");
}

// ─── getHtml : retourne le HTML en memoire sans ecrire sur le disque ────────
STR PageError::getHtml(int code)
{
    const STR codeStr = Convertion::toString((SIZET)code);
    const STR message = getMessage(code);
    std::ostringstream f;

    f << "<!DOCTYPE html>\n"
      << "<html lang=\"fr\">\n"
      << "<head>\n"
      << "    <meta charset=\"UTF-8\">\n"
      << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
      << "    <title>Error " << codeStr << " - " << message << "</title>\n"
      << "    <style>\n"
      << "        * { margin: 0; padding: 0; box-sizing: border-box; }\n"
      << "        body {\n"
      << "            background-color: #0a0a0f;\n"
      << "            color: #e8d5b0;\n"
      << "            font-family: 'Courier New', Courier, monospace;\n"
      << "            min-height: 100vh;\n"
      << "            display: flex;\n"
      << "            flex-direction: column;\n"
      << "            align-items: center;\n"
      << "            justify-content: center;\n"
      << "            overflow: hidden;\n"
      << "        }\n"
      << "        .container {\n"
      << "            text-align: center;\n"
      << "            padding: 2rem;\n"
      << "            animation: fadeIn 1.5s ease-in;\n"
      << "            position: relative;\n"
      << "            z-index: 1;\n"
      << "        }\n"
      << "        .error-code {\n"
      << "            font-size: 8rem;\n"
      << "            font-weight: bold;\n"
      << "            color: #ff6a00;\n"
      << "            text-shadow: 0 0 20px #ff6a00, 0 0 50px #ff4500, 0 0 100px #ff2200;\n"
      << "            letter-spacing: 0.15em;\n"
      << "            animation: pulse 2.5s ease-in-out infinite;\n"
      << "            line-height: 1;\n"
      << "        }\n"
      << "        .error-message {\n"
      << "            margin-top: 1.2rem;\n"
      << "            font-size: 1.8rem;\n"
      << "            font-weight: bold;\n"
      << "            color: #ffd700;\n"
      << "            text-shadow: 0 0 10px #ffd700, 0 0 30px #ff8c00;\n"
      << "            letter-spacing: 0.1em;\n"
      << "            animation: flicker 3s infinite alternate;\n"
      << "        }\n"
      << "        .divider {\n"
      << "            margin: 1.8rem auto;\n"
      << "            width: 60%;\n"
      << "            height: 1px;\n"
      << "            background: linear-gradient(to right, transparent, #ff6a00, #ffd700, #ff6a00, transparent);\n"
      << "            box-shadow: 0 0 8px #ff6a00;\n"
      << "        }\n"
      << "        .tagline {\n"
      << "            margin-top: 0.5rem;\n"
      << "            font-size: 0.85rem;\n"
      << "            color: #b87333;\n"
      << "            letter-spacing: 0.2em;\n"
      << "            text-transform: uppercase;\n"
      << "        }\n"
      << "        .back-link {\n"
      << "            display: inline-block;\n"
      << "            margin-top: 2rem;\n"
      << "            padding: 0.6rem 1.8rem;\n"
      << "            border: 1px solid #ff6a00;\n"
      << "            color: #ff8c42;\n"
      << "            text-decoration: none;\n"
      << "            font-size: 0.9rem;\n"
      << "            letter-spacing: 0.12em;\n"
      << "            text-transform: uppercase;\n"
      << "            box-shadow: 0 0 8px #ff4500;\n"
      << "            transition: all 0.3s;\n"
      << "        }\n"
      << "        .back-link:hover {\n"
      << "            background: #ff6a00;\n"
      << "            color: #0a0a0f;\n"
      << "            box-shadow: 0 0 20px #ff6a00;\n"
      << "        }\n"
      << "        .embers {\n"
      << "            position: fixed;\n"
      << "            top: 0; left: 0;\n"
      << "            width: 100%; height: 100%;\n"
      << "            pointer-events: none;\n"
      << "            overflow: hidden;\n"
      << "            z-index: 0;\n"
      << "        }\n"
      << "        .ember {\n"
      << "            position: absolute;\n"
      << "            bottom: -10px;\n"
      << "            width: 3px; height: 3px;\n"
      << "            border-radius: 50%;\n"
      << "            background: #ff6a00;\n"
      << "            box-shadow: 0 0 6px #ff4500;\n"
      << "            animation: rise linear infinite;\n"
      << "        }\n"
      << "        @keyframes rise {\n"
      << "            0%   { transform: translateY(0) translateX(0) scale(1); opacity: 1; }\n"
      << "            100% { transform: translateY(-100vh) translateX(var(--drift)) scale(0); opacity: 0; }\n"
      << "        }\n"
      << "        @keyframes flicker {\n"
      << "            0%   { opacity: 0.85; text-shadow: 0 0 8px #ff6a00, 0 0 20px #ff4500; }\n"
      << "            50%  { opacity: 1;    text-shadow: 0 0 12px #ff8c00, 0 0 30px #ff6500; }\n"
      << "            100% { opacity: 0.9;  text-shadow: 0 0 6px #ff5500, 0 0 16px #ff3500; }\n"
      << "        }\n"
      << "        @keyframes pulse {\n"
      << "            0%   { transform: scale(1);    text-shadow: 0 0 20px #ff6a00, 0 0 50px #ff4500, 0 0 100px #ff2200; }\n"
      << "            50%  { transform: scale(1.04); text-shadow: 0 0 30px #ff8c00, 0 0 70px #ff6500, 0 0 130px #ff3300; }\n"
      << "            100% { transform: scale(1);    text-shadow: 0 0 20px #ff6a00, 0 0 50px #ff4500, 0 0 100px #ff2200; }\n"
      << "        }\n"
      << "        @keyframes fadeIn {\n"
      << "            from { opacity: 0; transform: translateY(20px); }\n"
      << "            to   { opacity: 1; transform: translateY(0); }\n"
      << "        }\n"
      << "    </style>\n"
      << "</head>\n"
      << "<body>\n"
      << "\n"
      << "    <div class=\"embers\" id=\"embers\"></div>\n"
      << "\n"
      << "    <div class=\"container\">\n"
      << "        <div class=\"error-code\">" << codeStr << "</div>\n"
      << "        <div class=\"error-message\">" << message << "</div>\n"
      << "        <div class=\"divider\"></div>\n"
      << "        <div class=\"tagline\">Webserv &mdash; 42 School &mdash; Phoenix Edition</div>\n"
      << "        <a class=\"back-link\" href=\"/\">&#8592; Retour a l'accueil</a>\n"
      << "    </div>\n"
      << "\n"
      << "    <script>\n"
      << "        const container = document.getElementById('embers');\n"
      << "        const count = 30;\n"
      << "        for (let i = 0; i < count; i++) {\n"
      << "            const ember = document.createElement('div');\n"
      << "            ember.className = 'ember';\n"
      << "            const left   = Math.random() * 100;\n"
      << "            const delay  = Math.random() * 8;\n"
      << "            const dur    = 4 + Math.random() * 6;\n"
      << "            const drift  = (Math.random() - 0.5) * 120 + 'px';\n"
      << "            const size   = 2 + Math.random() * 3;\n"
      << "            const colors = ['#ff6a00','#ff8c00','#ffd700','#ff4500','#ffaa00'];\n"
      << "            const color  = colors[Math.floor(Math.random() * colors.length)];\n"
      << "            ember.style.cssText = `\n"
      << "                left: ${left}%;\n"
      << "                animation-delay: ${delay}s;\n"
      << "                animation-duration: ${dur}s;\n"
      << "                width: ${size}px;\n"
      << "                height: ${size}px;\n"
      << "                background: ${color};\n"
      << "                box-shadow: 0 0 ${size * 2}px ${color};\n"
      << "                --drift: ${drift};\n"
      << "            `;\n"
      << "            container.appendChild(ember);\n"
      << "        }\n"
      << "    </script>\n"
      << "\n"
      << "</body>\n"
      << "</html>\n";

    return (f.str());
}

// ─── getHtml(code, path) : lit le fichier custom, sinon genere en memoire ────
STR PageError::getHtml(int code, const STR &custom_path)
{
    std::ifstream f(custom_path.c_str());
    if (f.is_open())
    {
        STR content((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());
        return (content);
    }
    return (getHtml(code));
}

