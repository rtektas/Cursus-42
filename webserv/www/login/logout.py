#!/usr/bin/env python3

import os

SCRIPT_DIR   = os.path.dirname(os.path.abspath(__file__))
DATABASE_DIR = os.path.join(SCRIPT_DIR, "database")

set_cookie_header = ""

def getUserIdFromCookie():
    cookies = os.environ.get('HTTP_COOKIE', '')
    for cookie in cookies.split(';'):
        parts = cookie.strip().split('=', 1)
        if len(parts) == 2 and parts[0] == "id":
            return parts[1]
    return None

user_id = getUserIdFromCookie()

if user_id is not None:
    file_path = os.path.join(DATABASE_DIR, f"{user_id}.txt")
    if os.path.exists(file_path):
        os.remove(file_path)
    set_cookie_header = "Set-Cookie: id=; Expires=Thu, 01 Jan 1970 00:00:00 GMT; Path=/\r\n"
    message = "Your account has been successfully deleted."
else:
    message = "An error occurred while logging out."

page = f"""<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8"><title>Logout Page</title></head>
<body>
    <div class="container">
        <h1>{message}</h1>
        <a href="/"><div class="button">Home</div></a>
    </div>
</body>
</html>"""

headers = "Content-Type: text/html; charset=utf-8\r\n" + set_cookie_header + "\r\n"
print(headers, end="")
print(page)

