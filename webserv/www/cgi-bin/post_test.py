#!/usr/bin/env python3
import sys
import os

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
body = sys.stdin.read(content_length) if content_length > 0 else ""

sys.stdout.write("Content-Type: text/html\r\n")
sys.stdout.write("\r\n")
sys.stdout.write("<html><body>")
sys.stdout.write("<h1>POST CGI Works!</h1>")
sys.stdout.write("<p>Received: " + body + "</p>")
sys.stdout.write("</body></html>")
sys.stdout.flush()

