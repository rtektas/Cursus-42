#!/usr/bin/env python3

print("Content-Type: text/html")
print()

# Intentional error: divide by zero
result = 1 / 0

print("<html><body><p>You should never see this</p></body></html>")
