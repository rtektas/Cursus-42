#!/usr/bin/env python3
"""
test_cookies.py — 42 Webserv cookies and session tests
Run:  python3 test_cookies.py
Need: curl installed, server running with webserv_test.conf pointing to your cookie/login CGI
"""

import subprocess
import sys
import os
import re

HOST  = "http://127.0.0.1:8080"
PASS  = "\033[32m  ✓  PASS\033[0m"
FAIL  = "\033[31m  ✗  FAIL\033[0m"
SEP   = "─" * 60

total  = 0
passed = 0

COOKIE_JAR = "/tmp/webserv_test_cookies.txt"


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
    cmd = ["curl", "-v", "-s"] + args
    print("  $", " ".join(cmd))
    print()
    try:
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
        print(result.stderr)
        if result.stdout.strip():
            print("  --- response body ---")
            # truncate long HTML for readability
            body = result.stdout
            if len(body) > 800:
                print(body[:800])
                print("  ... (truncated)")
            else:
                print(body)
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


def extract_cookie(stderr: str, name: str) -> str:
    """Extract a Set-Cookie value from curl -v stderr."""
    for line in stderr.splitlines():
        if "set-cookie" in line.lower() and name in line:
            m = re.search(rf"{name}=([^;]+)", line, re.IGNORECASE)
            if m:
                return m.group(1)
    return ""


# Clean up any leftover cookie jar
if os.path.exists(COOKIE_JAR):
    os.remove(COOKIE_JAR)


# ══════════════════════════════════════════════════════════════════════════════
# 1 — First visit sets a session cookie (cookie.py)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 1 — First visit to cookie page sets a session_id cookie",
    "GET /cookie/cookie.py with no cookies — new client",
    "HTTP 200 and response contains Set-Cookie: session_id=<uuid>"
)
code, stderr = curl([
    "-c", COOKIE_JAR,   # save cookies to jar
    HOST + "/cookie/cookie.py"
])
session_id = extract_cookie(stderr, "session_id")
check(code == 200,          f"HTTP {code} (expected 200)")
check(len(session_id) > 0,  f"Set-Cookie: session_id={session_id[:16]}... received")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 2 — Second visit sends cookie back, server recognises session
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 2 — Second visit sends cookie back, server recognises the session",
    "GET /cookie/cookie.py sending the session_id cookie from test 1",
    "HTTP 200 — server reads HTTP_COOKIE, no new Set-Cookie issued (session already exists)"
)
code, stderr = curl([
    "-b", COOKIE_JAR,   # send saved cookies
    "-c", COOKIE_JAR,
    HOST + "/cookie/cookie.py"
])
new_session = extract_cookie(stderr, "session_id")
check(code == 200,          f"HTTP {code} (expected 200)")
check(new_session == "" or new_session == session_id,
      f"No new session_id issued — server recognised existing session ✓")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 3 — Cookie persists: theme change is saved server-side
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 3 — Server-side session stores state (theme preference)",
    "POST /cookie/cookie.py with theme=dark, then GET — theme should be remembered",
    "HTTP 200 on POST, then GET returns page with dark theme background color"
)
# POST to set dark theme
code, _ = curl([
    "-b", COOKIE_JAR,
    "-c", COOKIE_JAR,
    "-X", "POST",
    "--data", "theme=dark",
    HOST + "/cookie/cookie.py"
])
check(code == 200, f"POST theme=dark → HTTP {code} (expected 200)")

# GET to verify theme was saved
code, body = curl_body([
    "-b", COOKIE_JAR,
    "-c", COOKIE_JAR,
    HOST + "/cookie/cookie.py"
])
check(code == 200,          f"GET after POST → HTTP {code} (expected 200)")
check("#333" in body,       f"Body contains dark theme color #333 (theme saved server-side)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 4 — Login: new account creation sets session cookie
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 4 — Login: POST credentials creates a session and sets id cookie",
    "POST /login/login.py with username=testuser&password=testpass",
    "HTTP 200 and Set-Cookie: id=<uuid> in response"
)
# Clean jar for login tests
login_jar = "/tmp/webserv_login_cookies.txt"
if os.path.exists(login_jar):
    os.remove(login_jar)

code, stderr = curl([
    "-c", login_jar,
    "-X", "POST",
    "--data", "username=testuser&password=testpass",
    HOST + "/login/login.py"
])
login_id = extract_cookie(stderr, "id")
check(code == 200,         f"HTTP {code} (expected 200)")
check(len(login_id) > 0,   f"Set-Cookie: id={login_id[:16]}... received")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 5 — Session: returning user is recognised and welcomed
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 5 — Session: returning user is recognised by their cookie",
    "GET /login/login.py sending the id cookie from test 4",
    "HTTP 200 and body contains 'Welcome testuser' — session restored from server-side file"
)
code, body = curl_body([
    "-b", login_jar,
    "-c", login_jar,
    HOST + "/login/login.py"
])
check(code == 200,              f"HTTP {code} (expected 200)")
check("testuser" in body,       f"Body contains 'testuser' (session recognised, welcome page shown)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 6 — Logout: cookie is invalidated and session file deleted
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 6 — Logout: session cookie is invalidated",
    "GET /login/logout.py sending the id cookie — deletes session and expires cookie",
    "HTTP 200, Set-Cookie: id= with past Expires date, session file removed"
)
code, stderr = curl([
    "-b", login_jar,
    "-c", login_jar,
    HOST + "/login/logout.py"
])
expired_cookie = extract_cookie(stderr, "id")
check(code == 200,              f"HTTP {code} (expected 200)")
check(expired_cookie == "" or expired_cookie == "",
      f"Cookie id expired/cleared (session deleted)")

# Confirm session file is gone
session_file = f"./www0/login/database/{login_id}.txt"
check(not os.path.exists(session_file),
      f"Session file {session_file} deleted from server ✓")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 7 — After logout, old cookie no longer grants access
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 7 — After logout, old session cookie no longer grants access",
    "GET /login/login.py sending the now-expired id cookie",
    "Server does not find the session file — shows login page, not welcome page"
)
code, body = curl_body([
    "-b", login_jar,
    "-c", login_jar,
    HOST + "/login/login.py"
])
check(code == 200,              f"HTTP {code} (expected 200)")
check("Login" in body,          f"Body shows Login page (session gone, not welcome page)")
check("testuser" not in body,   f"Body does NOT contain 'testuser' (old session rejected)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# Summary
# ══════════════════════════════════════════════════════════════════════════════

# Clean up
for jar in [COOKIE_JAR, login_jar]:
    if os.path.exists(jar):
        os.remove(jar)

print(f"{'═' * 60}")
print(f"  RESULTS : {passed}/{total} passed")
if passed == total:
    print("  \033[32mAll tests passed ✓\033[0m")
else:
    print(f"  \033[31m{total - passed} test(s) failed ✗\033[0m")
print(f"{'═' * 60}\n")

sys.exit(0 if passed == total else 1)

