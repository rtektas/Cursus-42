#!/usr/bin/env python3
import os
import sys

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
body = sys.stdin.read(content_length) if content_length > 0 else "(no body)"

print("Content-Type: text/html")
print()
print("<html><body>")
print("<h1>CGI Python POST</h1>")
print(f"<p>REQUEST_METHOD: {os.environ.get('REQUEST_METHOD', '(empty)')}</p>")
print(f"<p>CONTENT_LENGTH: {content_length}</p>")
print(f"<p>BODY: {body}</p>")
print("</body></html>")
