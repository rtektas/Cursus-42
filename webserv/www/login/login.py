#!/usr/bin/env python3

import os
import uuid
import time
import urllib.parse
import sys

SCRIPT_DIR   = os.path.dirname(os.path.abspath(__file__))
DATABASE_DIR = os.path.join(SCRIPT_DIR, "database")

if not os.path.exists(DATABASE_DIR):
    os.makedirs(DATABASE_DIR)

isNewClient      = False
set_cookie_header = ""

def generateId():
    return str(uuid.uuid4())

def generateExpirationDate():
    expiration_time = time.time() + 60 * 60 * 24 * 30
    return time.strftime("%a, %d-%b-%Y %H:%M:%S GMT", time.gmtime(expiration_time))

def createNewCookie():
    global isNewClient, set_cookie_header
    isNewClient = True
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    raw = sys.stdin.read(content_length) if content_length > 0 else ""
    form_data = urllib.parse.parse_qs(raw)
    name     = form_data.get("username", [None])[0]
    password = form_data.get("password", [None])[0]
    if name is None or password is None:
        return None
    isNewClient = False
    user_id        = generateId()
    expiration_date = generateExpirationDate()
    set_cookie_header = f"Set-Cookie: id={user_id}; Expires={expiration_date}; Path=/\r\n"
    with open(os.path.join(DATABASE_DIR, f"{user_id}.txt"), "w") as file:
        file.write(f"{name}\n{password}")
    return user_id

def getUserIdFromCookie():
    cookies = os.environ.get('HTTP_COOKIE', '')
    for cookie in cookies.split(';'):
        parts = cookie.strip().split('=', 1)
        if len(parts) == 2 and parts[0] == "id":
            return parts[1]
    return None

def saveNote(user_id):
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    raw = sys.stdin.read(content_length) if content_length > 0 else ""
    form_data = urllib.parse.parse_qs(raw)
    note = form_data.get("note", [None])[0]
    if note is None:
        return
    file_path = os.path.join(DATABASE_DIR, f"{user_id}.txt")
    if not os.path.exists(file_path):
        return
    with open(file_path, "r") as f:
        lines = f.readlines()
    if len(lines) < 2:
        return
    with open(file_path, "w") as f:
        f.write(f"{lines[0].strip()}\n{lines[1].strip()}\n{note}")

def getUserInfo(user_id):
    saveNote(user_id)
    file_path = os.path.join(DATABASE_DIR, f"{user_id}.txt")
    if not os.path.exists(file_path):
        return None
    with open(file_path, "r") as f:
        lines = f.readlines()
    if len(lines) < 2:
        return None
    return {
        "username": lines[0].strip(),
        "password": lines[1].strip(),
        "note":     lines[2].strip() if len(lines) > 2 else ""
    }

user_id  = getUserIdFromCookie()
userInfo = {"username": "", "password": "", "note": ""}

if user_id is None:
    user_id = createNewCookie()
else:
    info = getUserInfo(user_id)
    if info is None:
        isNewClient = True
    else:
        userInfo = info

loginPage = """<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8"><title>Login Page</title></head>
<body>
    <div class="container">
        <h1>Login</h1>
        <form action="./" method="post">
            <label for="username">Username</label>
            <input type="text" id="username" name="username" required>
            <label for="password">Password</label>
            <input type="password" id="password" name="password" required>
            <button type="submit">Login</button>
        </form>
    </div>
</body>
</html>"""

resultPage = f"""<!DOCTYPE html>
<html lang="en">
<head><meta charset="UTF-8"><title>Welcome</title></head>
<body>
    <div class="container">
        <h1>Welcome {userInfo['username']}</h1>
        <p>Your password is: {userInfo['password']}</p>
        <textarea id="note" name="note" rows="4" cols="50">{userInfo['note']}</textarea>
        <div class="buttons">
            <button onclick="saveNote()">Save notes</button>
            <a href="/">Home</a>
            <button onclick="window.location.href='./logout.py'">Delete Account</button>
        </div>
    </div>
</body>
</html>"""

headers = "Content-Type: text/html; charset=utf-8\r\n" + set_cookie_header + "\r\n"
print(headers, end="")
print(loginPage if isNewClient else resultPage)

