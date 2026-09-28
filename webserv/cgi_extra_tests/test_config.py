#!/usr/bin/env python3
"""
test_webserv.py — 42 Webserv evaluation tests
Run:  python3 test_webserv.py
Need: curl installed, server running with webserv_test.conf
"""

import subprocess
import sys

HOST1 = "http://127.0.0.1:8080"
HOST2 = "http://127.0.0.1:8081"
PASS  = "\033[32m  ✓  PASS\033[0m"
FAIL  = "\033[31m  ✗  FAIL\033[0m"
SEP   = "─" * 60

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


def curl(args: list) -> tuple:
    """Run curl -v, print the full exchange, return (http_status, stderr)."""
    cmd = ["curl", "-v", "-s", "-o", "/dev/null", "-w", "%{http_code}"] + args
    print("  $", " ".join(cmd))
    print()
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
    print(result.stderr)
    code = int(result.stdout.strip()) if result.stdout.strip().isdigit() else 0
    return code, result.stderr


def curl_body(args: list) -> tuple:
    """Same as curl() but also prints the response body."""
    cmd = ["curl", "-v", "-s"] + args
    print("  $", " ".join(cmd))
    print()
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
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


def check(condition: bool, message: str):
    global total, passed
    total += 1
    if condition:
        passed += 1
        print(f"{PASS}  {message}")
    else:
        print(f"{FAIL}  {message}")


# ══════════════════════════════════════════════════════════════════════════════
# 1 — Multiple servers on different ports
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 1a — Server 1 responds on port 8080",
    "GET / on port 8080",
    "HTTP 200 and body contains 'Site 1'"
)
code, _ = curl([HOST1 + "/"])
check(code == 200, f"HTTP {code} (expected 200)")
pause()

header(
    "TEST 1b — Server 2 responds on port 8081",
    "GET / on port 8081",
    "HTTP 200 and body contains 'Site 2'"
)
code, _ = curl([HOST2 + "/"])
check(code == 200, f"HTTP {code} (expected 200)")
pause()

header(
    "TEST 1c — Both servers serve their own distinct content",
    "GET / on port 8080 then GET / on port 8081, compare bodies",
    "The two response bodies are NOT identical"
)
_, body1 = curl_body([HOST1 + "/"])
print(SEP)
_, body2 = curl_body([HOST2 + "/"])
check(body1 != body2, "Server 1 and Server 2 returned different content (independent virtual hosts)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 2 — Custom 404 error page
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 2a — Custom 404 page on server 1",
    "GET /nonexistent on port 8080",
    "HTTP 404 with the custom error page body (contains '404 - Not Found')"
)
code, body = curl_body([HOST1 + "/nonexistent"])
check(code == 404,          f"HTTP {code} (expected 404)")
check("404" in body,        f"Body contains '404' (custom error page served)")
pause()

header(
    "TEST 2b — Server 2 also returns 404 for missing paths",
    "GET /nonexistent on port 8081",
    "HTTP 404"
)
code, _ = curl([HOST2 + "/nonexistent"])
check(code == 404, f"HTTP {code} (expected 404)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 3 — Client body size limit  (client_max_body_size 200)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 3a — Body well within limit (100 bytes)",
    'curl -X POST -H "Content-Type: plain/text" --data <100 bytes>',
    "HTTP 200 or 201 — accepted"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "A" * 100, HOST1 + "/uploads"])
check(code in (200, 201), f"HTTP {code} (expected 200 or 201)")
pause()

header(
    "TEST 3b — Body exactly at limit (200 bytes)",
    'curl -X POST -H "Content-Type: plain/text" --data <200 bytes>',
    "HTTP 200 or 201 — boundary value must be accepted"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "B" * 200, HOST1 + "/uploads"])
check(code in (200, 201), f"HTTP {code} (expected 200 or 201)")
pause()

header(
    "TEST 3c — Body one byte over limit (201 bytes)",
    'curl -X POST -H "Content-Type: plain/text" --data <201 bytes>',
    "HTTP 413 Content Too Large"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "C" * 201, HOST1 + "/uploads"])
check(code == 413, f"HTTP {code} (expected 413)")
pause()

header(
    "TEST 3d — Body far over limit (1000 bytes)",
    'curl -X POST -H "Content-Type: plain/text" --data <1000 bytes>',
    "HTTP 413 Content Too Large"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "D" * 1000, HOST1 + "/uploads"])
check(code == 413, f"HTTP {code} (expected 413)")
pause()

header(
    "TEST 3e — Subject curl commands: short body accepted",
    'curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter"',
    "Not 413 — body is short enough"
)
short = "BODY IS HERE write something shorter"
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", short, HOST1 + "/uploads"])
check(code != 413, f"HTTP {code} — {len(short)} bytes accepted (not 413)")
pause()

header(
    "TEST 3f — Subject curl commands: long body rejected",
    'curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something longer than body limit ..."',
    "HTTP 413 — body exceeds the limit"
)
long = "BODY IS HERE " + "write something longer than body limit " * 6
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", long, HOST1 + "/uploads"])
check(code == 413, f"HTTP {code} — {len(long)} bytes rejected (413)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 4 — Routes mapped to different directories
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 4a — Root route serves public directory",
    "GET / on server 1",
    "HTTP 200 with content from ./www0/site1/public/index.html"
)
code, body = curl_body([HOST1 + "/"])
check(code == 200,       f"HTTP {code} (expected 200)")
check("Site 1" in body,  f"Body contains 'Site 1' (correct directory served)")
pause()

header(
    "TEST 4b — /readonly route serves its own directory",
    "GET /readonly/file.txt on server 1",
    "HTTP 200 with content from ./www0/site1/readonly/file.txt"
)
code, body = curl_body([HOST1 + "/readonly/file.txt"])
check(code == 200,                  f"HTTP {code} (expected 200)")
check("hello from readonly" in body, f"Body contains 'hello from readonly' (correct file served)")
pause()

header(
    "TEST 4c — Route isolation: /uploads does not leak into /",
    "GET /uploads/index.html — index.html lives in public/, not uploads/",
    "HTTP 404 — file is not present in the uploads directory"
)
code, _ = curl([HOST1 + "/uploads/index.html"])
check(code in (404, 403), f"HTTP {code} (expected 404 or 403 — file not in uploads/)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 5 — Default index file for directory requests
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 5a — Directory request returns index.html on server 1",
    "GET / (no filename) on port 8080",
    "HTTP 200 with index.html — no directory listing, no 403"
)
code, body = curl_body([HOST1 + "/"])
check(code == 200,              f"HTTP {code} (expected 200)")
check("Index of" not in body,   f"No directory listing in body (index.html served instead)")
pause()

header(
    "TEST 5b — Trailing slash still serves index.html",
    "GET / with explicit trailing slash",
    "HTTP 200 — trailing slash must not break directory resolution"
)
code, _ = curl([HOST1 + "/"])
check(code == 200, f"HTTP {code} (expected 200)")
pause()

header(
    "TEST 5c — Server 2 also returns its own index.html",
    "GET / on port 8081",
    "HTTP 200 with content from ./www0/site2/html/index.html"
)
code, body = curl_body([HOST2 + "/"])
check(code == 200,      f"HTTP {code} (expected 200)")
check("Site 2" in body, f"Body contains 'Site 2' (server 2 serving its own index)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 6 — Accepted methods per route
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 6a — GET is allowed on /readonly",
    "GET /readonly/file.txt",
    "HTTP 200 — GET is in the methods list for /readonly"
)
code, _ = curl([HOST1 + "/readonly/file.txt"])
check(code == 200, f"HTTP {code} (expected 200)")
pause()

header(
    "TEST 6b — POST is rejected on /readonly",
    "POST /readonly/file.txt",
    "HTTP 405 Method Not Allowed — POST is not in the methods list"
)
code, _ = curl(["-X", "POST", "--data", "x", HOST1 + "/readonly/file.txt"])
check(code == 405, f"HTTP {code} (expected 405)")
pause()

header(
    "TEST 6c — DELETE is rejected on /readonly",
    "DELETE /readonly/file.txt",
    "HTTP 405 Method Not Allowed — DELETE is not in the methods list"
)
code, _ = curl(["-X", "DELETE", HOST1 + "/readonly/file.txt"])
check(code == 405, f"HTTP {code} (expected 405)")
pause()

header(
    "TEST 6d — GET is allowed on /",
    "GET /",
    "HTTP 200 — GET is in the methods list for /"
)
code, _ = curl([HOST1 + "/"])
check(code == 200, f"HTTP {code} (expected 200)")
pause()

header(
    "TEST 6e — POST is allowed on /",
    'POST / with a small body',
    "HTTP 200 or 201 — POST is in the methods list for /"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "hello", HOST1 + "/"])
check(code in (200, 201), f"HTTP {code} (expected 200 or 201, not 405)")
pause()

header(
    "TEST 6f — DELETE is rejected on /",
    "DELETE /",
    "HTTP 405 Method Not Allowed — DELETE is not in the methods list for /"
)
code, _ = curl(["-X", "DELETE", HOST1 + "/"])
check(code == 405, f"HTTP {code} (expected 405)")
pause()

header(
    "TEST 6g — DELETE is allowed on /uploads",
    "Step 1: POST a file to /uploads to create it. Step 2: DELETE that same file.",
    "Step 1: HTTP 201 (file created) — Step 2: HTTP 200 (file deleted)"
)
# Step 1 — upload a file so we have something to delete
print("  \033[90mStep 1: uploading a file to /uploads...\033[0m")
upload_cmd = ["curl", "-v", "-s", "-X", "POST",
              "-H", "Content-Type: plain/text",
              "--data", "delete me",
              "-D", "-",
              HOST1 + "/uploads"]
print("  $", " ".join(upload_cmd))
print()
upload_result = subprocess.run(upload_cmd, capture_output=True, text=True, timeout=10)
print(upload_result.stderr)

# Parse the filename from the Location header or server logs
# The server saves files as uploaded_raw_file_XXXXXX — grab the name from the response
uploaded_filename = None
for line in upload_result.stderr.splitlines():
    if "Location" in line or "location" in line:
        uploaded_filename = line.split("/")[-1].strip()
        break

upload_code = 0
for line in upload_result.stderr.splitlines():
    if line.startswith("< HTTP"):
        try:
            upload_code = int(line.split()[2])
        except Exception:
            pass
check(upload_code in (200, 201), f"Step 1 upload → HTTP {upload_code} (expected 201)")

print()
print(f"  \033[90mStep 2: deleting the uploaded file...\033[0m")
# If we got a Location header use it, otherwise try a known uploaded file
if uploaded_filename:
    delete_url = HOST1 + "/uploads/" + uploaded_filename
else:
    # fallback: list what was uploaded and pick the most recent
    import os, glob
    files = sorted(glob.glob("../www0/site1/uploads/uploaded_raw_file_*"))
    if files:
        delete_url = HOST1 + "/uploads/" + os.path.basename(files[-1])
    else:
        delete_url = HOST1 + "/uploads/uploaded_raw_file_fallback"

code, _ = curl(["-X", "DELETE", delete_url])
check(code in (200, 204), f"Step 2 delete → HTTP {code} (expected 200 or 204)")
pause()

header(
    "TEST 6h — POST is allowed on /uploads",
    "POST /uploads with a small body",
    "HTTP 200 or 201 — POST is in the methods list for /uploads"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "file content", HOST1 + "/uploads"])
check(code in (200, 201), f"HTTP {code} (expected 200 or 201, not 405)")
pause()

header(
    "TEST 6i — Server 2 rejects POST (GET only)",
    "POST / on port 8081",
    "HTTP 405 — only GET is allowed on server 2"
)
code, _ = curl(["-X", "POST", "--data", "x", HOST2 + "/"])
check(code == 405, f"HTTP {code} (expected 405)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 7 — HTTP status codes
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 7a — 200 OK",
    "GET an existing resource",
    "HTTP 200 OK"
)
code, _ = curl([HOST1 + "/"])
check(code == 200, f"HTTP {code} (expected 200 OK)")
pause()

header(
    "TEST 7b — 404 Not Found",
    "GET a resource that does not exist",
    "HTTP 404 Not Found"
)
code, _ = curl([HOST1 + "/does-not-exist-at-all"])
check(code == 404, f"HTTP {code} (expected 404 Not Found)")
pause()

header(
    "TEST 7c — 405 Method Not Allowed",
    "DELETE / (DELETE not in methods list for /)",
    "HTTP 405 Method Not Allowed"
)
code, _ = curl(["-X", "DELETE", HOST1 + "/"])
check(code == 405, f"HTTP {code} (expected 405 Method Not Allowed)")
pause()

header(
    "TEST 7d — 413 Content Too Large",
    "POST a 500-byte body (limit is 200 bytes)",
    "HTTP 413 Content Too Large"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "X" * 500, HOST1 + "/uploads"])
check(code == 413, f"HTTP {code} (expected 413 Content Too Large)")
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

