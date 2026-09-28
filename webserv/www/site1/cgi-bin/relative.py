#!/usr/bin/env python3
import os

print("Content-Type: text/html")
print()
print("<html><body>")
print("<h1>CGI Relative Path Test</h1>")
print(f"<p>CWD: {os.getcwd()}</p>")
try:
    with open("cgi_data.txt", "r") as f:
        content = f.read()
    print(f"<p>File content: {content}</p>")
    print("<p>STATUS: relative path OK</p>")
except Exception as e:
    print(f"<p>ERROR: {e}</p>")
    print("<p>STATUS: relative path FAILED</p>")
print("</body></html>")
