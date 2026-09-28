#!/usr/bin/env python3
import os
import time

cookie_name = "legumes_session"
cookie_value = "carotte_" + str(int(time.time()))

print("Set-Cookie: " + cookie_name + "=" + cookie_value + "; Path=/; Max-Age=3600")
print("Set-Cookie: legumes_pref=brocoli; Path=/; Max-Age=3600")
print("Content-Type: text/html")
print()

existing = os.environ.get("HTTP_COOKIE", "Aucun cookie recu")

print("<html><head><title>Cookies Legumes</title></head><body>")
print("<h1>Cookies du site Legumes</h1>")
print("<p><b>Cookies serveur poses :</b></p>")
print("<ul>")
print("<li>" + cookie_name + " = " + cookie_value + "</li>")
print("<li>legumes_pref = brocoli</li>")
print("</ul>")
print("<p><b>Cookies recus par le serveur :</b> " + existing + "</p>")
print("</body></html>")
