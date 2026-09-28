#!/usr/bin/env python3
import os

print("Content-Type: text/plain\r")
print("\r")
print("SCRIPT_NAME  =", os.environ.get("SCRIPT_NAME", ""))
print("PATH_INFO    =", os.environ.get("PATH_INFO", ""))
print("QUERY_STRING =", os.environ.get("QUERY_STRING", ""))
EOF
