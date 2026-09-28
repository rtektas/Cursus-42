#!/usr/bin/env python3
import os

print("Content-Type: text/html")
print()
print("<html><body>")
print("<h1>CGI Python GET</h1>")
print(f"<p>QUERY_STRING: {os.environ.get('QUERY_STRING', '(empty)')}</p>")
print(f"<p>REQUEST_METHOD: {os.environ.get('REQUEST_METHOD', '(empty)')}</p>")
print(f"<p>SERVER_NAME: {os.environ.get('SERVER_NAME', '(empty)')}</p>")
print("</body></html>")
