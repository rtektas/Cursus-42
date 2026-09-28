#!/usr/bin/env python3

import os
import uuid
import time
import urllib.parse
import sys

# All paths relative to this script's directory, not CWD
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DATABASE_DIR = os.path.join(SCRIPT_DIR, "database")

if not os.path.exists(DATABASE_DIR):
    os.makedirs(DATABASE_DIR)

set_cookie_header = ""

def get_new_user_theme(user_id):
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    raw = sys.stdin.read(content_length) if content_length > 0 else ""
    form_data = urllib.parse.parse_qs(raw)
    newTheme = form_data.get("theme", [None])[0]
    if newTheme is None:
        newTheme = get_user_theme(user_id)
    try:
        with open(os.path.join(DATABASE_DIR, f"{user_id}.txt"), "w") as file:
            file.write(newTheme)
    except Exception as e:
        pass
    return newTheme

def get_user_theme(user_id):
    try:
        with open(os.path.join(DATABASE_DIR, f"{user_id}.txt"), "r") as file:
            theme = file.read().strip()
            return theme if theme in ["light", "dark"] else "light"
    except FileNotFoundError:
        return "light"

def generateId():
    return str(uuid.uuid4())

def generateExpirationDate():
    expiration_time = time.time() + 60 * 60 * 24 * 30
    return time.strftime("%a, %d-%b-%Y %H:%M:%S GMT", time.gmtime(expiration_time))

def createNewCookie():
    global set_cookie_header
    user_id = generateId()
    expiration_date = generateExpirationDate()
    set_cookie_header = "Set-Cookie: session_id=" + user_id + "; Expires=" + expiration_date + "; Path=/\r\n"
    with open(os.path.join(DATABASE_DIR, f"{user_id}.txt"), "w") as file:
        file.write("light")
    return user_id

cookies = os.environ.get('HTTP_COOKIE', '')
user_id = None

if cookies == "":
    user_id = createNewCookie()
else:
    for cookie in cookies.split(';'):
        parts = cookie.strip().split('=', 1)
        if len(parts) == 2 and parts[0] == "session_id":
            user_id = parts[1]
            break

if user_id is None:
    user_id = createNewCookie()

user_theme = get_new_user_theme(user_id)

if user_theme == "dark":
    background_color = "#333"
    text_color = "#f0f0f0"
    buttonColor = "white"
else:
    background_color = "#f0f0f0"
    text_color = "#333"
    buttonColor = "black"

light_selected = "selected" if user_theme == "light" else ""
dark_selected = "selected" if user_theme == "dark" else ""

html_content = f"""<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Cookie Page</title>
    <style>
        body {{ font-family: sans-serif; background-color: {background_color}; color: {text_color}; display: flex; justify-content: center; align-items: center; height: 100vh; }}
        .container {{ text-align: center; }}
        .button {{ margin-top: 2rem; border: none; padding: 1rem 3rem; background-color: {buttonColor}; border-radius: 15px; color: {background_color}; cursor: pointer; }}
        form {{ display: flex; flex-direction: column; align-items: center; }}
        #theme {{ margin-top: 1rem; padding: 5px 20px; background-color: {text_color}; border-radius: 3px; color: {background_color}; }}
    </style>
</head>
<body>
    <div class="container">
        <h2>Cookie Page</h2>
        <form action="/cookie/cookie.py" method="post">
            <label for="theme">Choisissez un thème :</label>
            <select name="theme" id="theme">
                <option {light_selected} value="light">Light</option>
                <option {dark_selected} value="dark">Dark</option>
            </select>
            <button class="button" type="submit">Save</button>
        </form>
    </div>
</body>
</html>
"""

headers = "Content-Type: text/html; charset=utf-8\r\n" + set_cookie_header + "\r\n"
print(headers, end="")
print(html_content)

