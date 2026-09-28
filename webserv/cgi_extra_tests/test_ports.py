#!/usr/bin/env python3
"""
test_ports.py — 42 Webserv port and interface tests
Run:  python3 test_ports.py
Need: curl installed, server running with webserv_test.conf
"""

import subprocess
import sys
import time

HOST1  = "http://127.0.0.1:8080"
HOST2  = "http://127.0.0.1:8081"
PASS   = "\033[32m  ✓  PASS\033[0m"
FAIL   = "\033[31m  ✗  FAIL\033[0m"
SEP    = "─" * 60

total  = 0
passed = 0


def pause():
    input("\n\033[90m  Press ENTER to continue...\033[0m\n")


def header(title, what, expect):
    print(f"\n{'═' * 60}")
    print(f"  \033[1m{title}\033[0m")
    print(f"{'═' * 60}")
    print(f"  WHAT   : {what}")
    print(f"  EXPECT : {expect}")
    print(SEP)


def curl(args: list, timeout: int = 5) -> tuple:
    cmd = ["curl", "-v", "-s", "-o", "/dev/null", "-w", "%{http_code}"] + args
    print("  $", " ".join(cmd))
    print()
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        print(result.stderr)
        code = int(result.stdout.strip()) if result.stdout.strip().isdigit() else 0
        return code, result.stderr
    except subprocess.TimeoutExpired:
        print("  [curl timed out — port not responding]")
        return -1, ""


def curl_body(args: list, timeout: int = 5) -> tuple:
    cmd = ["curl", "-v", "-s"] + args
    print("  $", " ".join(cmd))
    print()
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        print(result.stderr)
        if result.stdout.strip():
            print("  --- response body ---")
            print(result.stdout)
            print("  ---------------------")
        code = 0
        for line in result.stderr.splitlines():
            if line.startswith("< HTTP"):
                try:
                    code = int(line.split()[2])
                except Exception:
                    pass
        return code, result.stdout
    except subprocess.TimeoutExpired:
        print("  [curl timed out]")
        return -1, ""


def check(condition: bool, message: str):
    global total, passed
    total += 1
    if condition:
        passed += 1
        print(f"{PASS}  {message}")
    else:
        print(f"{FAIL}  {message}")


# ══════════════════════════════════════════════════════════════════════════════
# 1 — Multiple interfaces and ports serving different websites
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 1a — Server 1 responds on port 8080 with its own content",
    "GET / on 127.0.0.1:8080",
    "HTTP 200 with 'Site 1' in body"
)
code, body = curl_body([HOST1 + "/"])
check(code == 200,      f"HTTP {code} (expected 200)")
check("Site 1" in body, f"Body contains 'Site 1' (server 1 serving its own website)")
pause()

header(
    "TEST 1b — Server 2 responds on port 8081 with its own content",
    "GET / on 127.0.0.1:8081",
    "HTTP 200 with 'Site 2' in body"
)
code, body = curl_body([HOST2 + "/"])
check(code == 200,      f"HTTP {code} (expected 200)")
check("Site 2" in body, f"Body contains 'Site 2' (server 2 serving its own website)")
pause()

header(
    "TEST 1c — Each port serves a completely different website",
    "GET / on port 8080 and port 8081 — compare bodies",
    "The two responses are NOT identical — different websites on different ports"
)
_, body1 = curl_body([HOST1 + "/"])
print(SEP)
_, body2 = curl_body([HOST2 + "/"])
check(body1 != body2, "Port 8080 and port 8081 serve different content ✓")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 2 — Same interface:port with multiple server_name (virtual hosts)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 2 — Multiple server_name on same interface:port (virtual hosts)",
    "GET / with Host: localhost on port 8080, then Host: localhost2 on port 8080",
    "Each Host header routes to its own server block (virtual host) OR both go to default server"
)
print("  \033[33m  NOTE: virtual host support is optional in 42 webserv.\033[0m")
print("  \033[33m  If implemented: different Host headers return different content.\033[0m")
print("  \033[33m  If not implemented: all requests go to the first/default server block.\033[0m")
print()

code1, body1 = curl_body(["-H", "Host: localhost", HOST1 + "/"])
print(SEP)
code2, body2 = curl_body(["-H", "Host: localhost2", HOST1 + "/"])

check(code1 == 200, f"Host: localhost → HTTP {code1} (expected 200)")
check(code2 == 200, f"Host: localhost2 → HTTP {code2} (expected 200)")

if body1 != body2:
    print(f"  \033[32m  ℹ  Virtual hosts implemented — different Host headers return different content\033[0m")
else:
    print(f"  \033[33m  ℹ  No virtual hosts — both Host headers return the same default server content\033[0m")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 3 — Two webserv instances with overlapping ports
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 3 — Launch a second webserv instance with a conflicting port",
    "Start a second ./webserv process using a config that binds to port 8080 (already in use)",
    "Second instance should fail to bind and exit with an error — NOT silently succeed or crash"
)
print("  \033[33m  NOTE: the first webserv must already be running on port 8080\033[0m")
print()

# Write a minimal conflicting config to /tmp
conflict_conf = """\
server {
    listen 8080;
    host   127.0.0.1;
    root   /tmp;
    location / {
        methods GET;
        root    /tmp;
    }
}
"""
with open("/tmp/conflict.conf", "w") as f:
    f.write(conflict_conf)

print("  \033[90m  Starting second webserv instance with conflicting port 8080...\033[0m")
print("  $ ../webserv /tmp/conflict.conf &")
print()

result = subprocess.run(
    ["../webserv", "/tmp/conflict.conf"],
    capture_output=True, text=True, timeout=5
)

print("  --- second instance output ---")
print(result.stdout)
print(result.stderr)
print("  ------------------------------")
print()

# The second instance should have exited with an error
exited_with_error = result.returncode != 0
bind_error_in_output = "bind" in result.stderr.lower() or "bind" in result.stdout.lower() \
                    or "error" in result.stderr.lower() or "fatal" in result.stderr.lower()

check(exited_with_error or bind_error_in_output,
      f"Second instance failed to bind (exit code {result.returncode}) — correct behavior")

# Confirm first instance is still alive
print()
print("  \033[90m  Confirming first webserv instance is still alive...\033[0m")
code, _ = curl([HOST1 + "/"])
check(code == 200, f"First instance still responding → HTTP {code} (expected 200)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# Summary
# ══════════════════════════════════════════════════════════════════════════════

print(f"{'═' * 60}")
print(f"  RESULTS : {passed}/{total} passed")
if passed == total:
    print("  \033[32mAll tests passed ✓\033[0m")
else:
    print(f"  \033[31m{total - passed} test(s) failed ✗\033[0m")
print(f"{'═' * 60}\n")

sys.exit(0 if passed == total else 1)

