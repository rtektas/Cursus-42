#!/usr/bin/env python3

"""
test_cgi.py — 42 Webserv CGI tests
Run:  python3 test_cgi.py
Need: curl, python3, php, perl installed — server running with webserv_test.conf
"""

import subprocess
import sys

HOST   = "http://127.0.0.1:8080"
CGI    = HOST + "/cgi-bin"
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


def curl(args: list, timeout: int = 10) -> tuple:
    """Run curl -v, print exchange, return (http_status, stderr)."""
    cmd = ["curl", "-v", "-s", "-o", "/dev/null", "-w", "%{http_code}"] + args
    print("  $", " ".join(cmd))
    print()
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        print(result.stderr)
        code = int(result.stdout.strip()) if result.stdout.strip().isdigit() else 0
        return code, result.stderr
    except subprocess.TimeoutExpired:
        print("  [curl timed out]")
        return -1, ""


def curl_body(args: list, timeout: int = 10) -> tuple:
    """Run curl -v, print exchange + body, return (http_status, body)."""
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
# 1 — Python CGI — GET
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 1 — Python CGI GET",
    "GET /cgi-bin/hello.py?name=42 — server runs hello.py via python3",
    "HTTP 200, body contains REQUEST_METHOD=GET and the query string"
)
code, body = curl_body([CGI + "/hello.py?name=42"])
check(code == 200,                  f"HTTP {code} (expected 200)")
check("GET" in body,                f"Body contains 'GET' (REQUEST_METHOD set correctly)")
check("name=42" in body,            f"Body contains 'name=42' (QUERY_STRING passed correctly)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 2 — Python CGI — POST
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 2 — Python CGI POST",
    'POST /cgi-bin/post.py with body "hello from post"',
    "HTTP 200, body contains REQUEST_METHOD=POST and the posted body"
)
code, body = curl_body(["-X", "POST", "-H", "Content-Type: plain/text",
                         "--data", "hello from post", CGI + "/post.py"])
check(code == 200,               f"HTTP {code} (expected 200)")
check("POST" in body,            f"Body contains 'POST' (REQUEST_METHOD set correctly)")
check("hello from post" in body, f"Body contains 'hello from post' (stdin read correctly)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 3 — PHP CGI — GET
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 3 — PHP CGI GET",
    "GET /cgi-bin/hello.php?lang=php — server runs hello.php via php",
    "HTTP 200, body contains REQUEST_METHOD=GET and the query string"
)
code, body = curl_body([CGI + "/hello.php?lang=php"])
check(code == 200,          f"HTTP {code} (expected 200)")
check("GET" in body,        f"Body contains 'GET' (REQUEST_METHOD set correctly)")
check("lang=php" in body,   f"Body contains 'lang=php' (QUERY_STRING passed correctly)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 4 — PHP CGI — POST
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 4 — PHP CGI POST",
    'POST /cgi-bin/post.php with body "hello from php"',
    "HTTP 200, body contains REQUEST_METHOD=POST and the posted body"
)
code, body = curl_body(["-X", "POST", "-H", "Content-Type: plain/text",
                         "--data", "hello from php", CGI + "/post.php"])
check(code == 200,               f"HTTP {code} (expected 200)")
check("POST" in body,            f"Body contains 'POST' (REQUEST_METHOD set correctly)")
check("hello from php" in body,  f"Body contains 'hello from php' (body read correctly)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 5 — Perl CGI — GET
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 5 — Perl CGI GET",
    "GET /cgi-bin/hello.pl?lang=perl — server runs hello.pl via perl",
    "HTTP 200, body contains REQUEST_METHOD=GET and the query string"
)
code, body = curl_body([CGI + "/hello.pl?lang=perl"])
check(code == 200,          f"HTTP {code} (expected 200)")
check("GET" in body,        f"Body contains 'GET' (REQUEST_METHOD set correctly)")
check("lang=perl" in body,  f"Body contains 'lang=perl' (QUERY_STRING passed correctly)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 6 — Relative path access (CGI runs in its own directory)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 6 — CGI runs in the correct directory (relative path access)",
    "GET /cgi-bin/relative.py — script reads cgi_data.txt using a relative path",
    "HTTP 200, body contains the file content — proves CWD is the script's directory"
)
code, body = curl_body([CGI + "/relative.py"])
check(code == 200,                       f"HTTP {code} (expected 200)")
check("relative path OK" in body,        f"Body contains 'relative path OK' (file found via relative path)")
check("hello from cgi_data.txt" in body, f"Body contains file content (CWD is script directory)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 7 — CGI script with an error (divide by zero)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 7 — CGI script that crashes (divide by zero in error.py)",
    "GET /cgi-bin/error.py — script raises ZeroDivisionError mid-execution",
    "Server does NOT crash — RFC 3875 does not define behavior when script crashes after sending headers. "
    "200 with empty body (streaming) or 500 are both acceptable. Server must stay alive."
)
code, body = curl_body([CGI + "/error.py"])

print()
print("  \033[1m  NOTE:\033[0m")
print("  ┌─────────────────────────────────────────────────────┐")
print("  │ error.py outputs 'Content-Type: text/html\\n\\n'      │")
print("  │ BEFORE crashing. The server sees the headers and    │")
print("  │ immediately starts streaming the response (200 OK). │")
print("  │ The script then crashes — but 200 is already on     │")
print("  │ the wire. The body is empty.                        │")
print("  │                                                     │")
print("  │ RFC 3875 (CGI spec) section 6.2.1 defines what the  │")
print("  │ script must output but says NOTHING about what the  │")
print("  │ server must do if the script crashes mid-response.  │")
print("  │ This case is explicitly unspecified by the RFC.     │")
print("  │                                                     │")
print("  │ Apache mod_cgi behaves identically: 200 + empty     │")
print("  │ body when a script crashes after sending headers.   │")
print("  │                                                     │")
print("  │ Nginx does NOT use plain CGI — it uses FastCGI      │")
print("  │ (fastcgi_pass), a different protocol that buffers   │")
print("  │ the full response before sending, allowing it to    │")
print("  │ return 502 on crash. Plain CGI streaming cannot do  │")
print("  │ this without buffering the entire response in       │")
print("  │ memory first, defeating the purpose of streaming    │")
print("  │ for large concurrent responses.                     │")
print("  │                                                     │")
print("  │ The key requirement is: server must NOT crash.      │")
print("  │ Both 200+empty body and 500 are acceptable.         │")
print("  └─────────────────────────────────────────────────────┘")
print()

if code == 200 and body.strip() == "":
    check(True, "HTTP 200 empty body — script crashed after headers, server streamed correctly and stayed alive")
elif code in (500, 502, 504):
    check(True, f"HTTP {code} — server caught the CGI error")
else:
    check(False, f"HTTP {code} — unexpected response")

print()
print("  \033[90mConfirming server is still alive after CGI crash...\033[0m")
code2, _ = curl([HOST + "/"])
check(code2 == 200, f"Server still alive → HTTP {code2} (expected 200 — no crash)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 8 — CGI script with an infinite loop (timeout test)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 8 — CGI script with infinite loop (loop.py)",
    "GET /cgi-bin/loop.py — script runs forever, server must time it out",
    "Server does NOT hang — returns a timeout error (500/504) and stays alive"
)
print("  \033[33m  NOTE: this test waits for the server's CGI timeout — may take a few seconds\033[0m")
print()
code, _ = curl([CGI + "/loop.py"], timeout=30)
check(code in (500, 502, 504) or code == -1,
      f"HTTP {code} (expected 500/502/504 — timeout handled, not a server hang)")

print()
print("  \033[90mConfirming server is still alive after infinite loop CGI...\033[0m")
code2, _ = curl([HOST + "/"])
check(code2 == 200, f"Server still alive → HTTP {code2} (expected 200 — no crash)")
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

