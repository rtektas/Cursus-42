#!/usr/bin/env python3
"""
test_basic.py — 42 Webserv basic checks
Run:  python3 test_basic.py
Need: curl installed, server running with webserv_test.conf
"""

import subprocess
import sys
import os
import glob

HOST    = "http://127.0.0.1:8080"
UPLOAD  = HOST + "/uploads"
PASS    = "\033[32m  ✓  PASS\033[0m"
FAIL    = "\033[31m  ✗  FAIL\033[0m"
SEP     = "─" * 60

total   = 0
passed  = 0


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
    """Run curl -v, print exchange + response body, return (http_status, body)."""
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


def latest_upload() -> str:
    """Return the path of the most recently uploaded file."""
    files = sorted(glob.glob("../www0/site1/uploads/uploaded_raw_file_*"),
                   key=os.path.getmtime)
    return files[-1] if files else ""


# ══════════════════════════════════════════════════════════════════════════════
# 1 — GET
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 1a — GET existing resource",
    "GET / on port 8080",
    "HTTP 200 OK — page is returned"
)
code, body = curl_body([HOST + "/"])
check(code == 200, f"HTTP {code} (expected 200 OK)")
pause()

header(
    "TEST 1b — GET non-existing resource",
    "GET /this-does-not-exist on port 8080",
    "HTTP 404 Not Found — server handles it without crashing"
)
code, _ = curl_body([HOST + "/this-does-not-exist"])
check(code == 404, f"HTTP {code} (expected 404 Not Found)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 2 — POST
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 2a — POST plain text body",
    'curl -X POST -H "Content-Type: plain/text" --data "Hello from POST"',
    "HTTP 201 Created — server accepts the body and stores it"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "Hello from POST", UPLOAD])
check(code in (200, 201), f"HTTP {code} (expected 201 Created)")
pause()

header(
    "TEST 2b — POST a file from disk",
    "curl -X POST -F with a local file (./www0/site1/public/index.html)",
    "HTTP 201 Created — file is uploaded to the server"
)
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data-binary", "@./www0/site1/public/index.html", UPLOAD])
check(code in (200, 201), f"HTTP {code} (expected 201 Created)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 3 — Upload a file then retrieve it (GET after POST)
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 3 — Upload a file then retrieve it with GET",
    "Step 1: POST content 'RETRIEVE ME' to /uploads. Step 2: GET the saved file back.",
    "Step 1: HTTP 201 — Step 2: HTTP 200 with the original content in the body"
)

# Step 1 — upload
print("  \033[90mStep 1: uploading...\033[0m")
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "RETRIEVE ME", UPLOAD])
check(code in (200, 201), f"Step 1 upload → HTTP {code} (expected 201)")

# Step 2 — retrieve
print()
print("  \033[90mStep 2: retrieving the uploaded file...\033[0m")
fname = latest_upload()
if fname:
    filename  = os.path.basename(fname)
    get_url   = UPLOAD + "/" + filename
    code, body = curl_body([get_url])
    check(code == 200,           f"Step 2 GET → HTTP {code} (expected 200)")
    check("RETRIEVE ME" in body, f"Body contains 'RETRIEVE ME' (correct file returned)")
else:
    check(False, "No uploaded file found in ./www0/site1/uploads/ — step 1 may have failed")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 4 — DELETE
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 4a — DELETE an existing file",
    "Step 1: POST a file. Step 2: DELETE it. Step 3: GET it — should be gone.",
    "Step 1: 201 — Step 2: 200 or 204 — Step 3: 404 (file no longer exists)"
)

# Step 1 — upload something to delete
print("  \033[90mStep 1: uploading a file to delete...\033[0m")
code, _ = curl(["-X", "POST", "-H", "Content-Type: plain/text",
                "--data", "DELETE ME", UPLOAD])
check(code in (200, 201), f"Step 1 upload → HTTP {code} (expected 201)")

# Step 2 — delete it
print()
print("  \033[90mStep 2: deleting the file...\033[0m")
fname = latest_upload()
if fname:
    filename   = os.path.basename(fname)
    delete_url = UPLOAD + "/" + filename
    code, _    = curl(["-X", "DELETE", delete_url])
    check(code in (200, 204), f"Step 2 DELETE → HTTP {code} (expected 200 or 204)")

    # Step 3 — confirm it's gone
    print()
    print("  \033[90mStep 3: confirming the file is gone...\033[0m")
    code, _ = curl([delete_url])
    check(code == 404, f"Step 3 GET deleted file → HTTP {code} (expected 404 — file is gone)")
else:
    check(False, "No uploaded file found — step 1 may have failed")
pause()

header(
    "TEST 4b — DELETE on a route that forbids it returns 405",
    "DELETE /readonly/file.txt — DELETE is not in the methods list for /readonly",
    "HTTP 405 Method Not Allowed"
)
code, _ = curl(["-X", "DELETE", HOST + "/readonly/file.txt"])
check(code == 405, f"HTTP {code} (expected 405 Method Not Allowed)")
pause()

# ══════════════════════════════════════════════════════════════════════════════
# 5 — Unknown / unsupported method
# ══════════════════════════════════════════════════════════════════════════════

header(
    "TEST 5a — Unknown method UNKNOWN",
    'curl -X UNKNOWN http://127.0.0.1:8080/',
    "Server does NOT crash — returns 400 or 405, and is still reachable after"
)
code, _ = curl(["-X", "UNKNOWN", HOST + "/"])
check(code in (400, 405, 501), f"HTTP {code} (expected 400, 405, or 501 — not a crash)")
pause()

header(
    "TEST 5b — Unknown method FAKEMETHOD",
    'curl -X FAKEMETHOD http://127.0.0.1:8080/',
    "Server does NOT crash — returns an error code and stays alive"
)
code, _ = curl(["-X", "FAKEMETHOD", HOST + "/"])
check(code in (400, 405, 501), f"HTTP {code} (expected 400, 405, or 501 — not a crash)")

# Confirm server is still alive after the bad requests
print()
print("  \033[90mConfirming server is still alive after unknown methods...\033[0m")
code, _ = curl([HOST + "/"])
check(code == 200, f"Server still responds → HTTP {code} (expected 200 — no crash)")
pause()

header(
    "TEST 5c — Garbage request line",
    'curl -X "THIS IS NOT HTTP" http://127.0.0.1:8080/',
    "Server does NOT crash — returns an error and stays alive"
)
code, _ = curl(["-X", "THIS_IS_NOT_HTTP", HOST + "/"])
check(code in (400, 405, 501), f"HTTP {code} (expected 400, 405, or 501)")

print()
print("  \033[90mFinal server health check after all garbage requests...\033[0m")
code, _ = curl([HOST + "/"])
check(code == 200, f"Server still alive → HTTP {code} (expected 200 — no crash)")
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

