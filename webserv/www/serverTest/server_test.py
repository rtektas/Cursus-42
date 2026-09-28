#!/usr/bin/env python3
"""
Serveur de test temporaire pour verifier le fix de www/js/files.js :
sert les fichiers statiques de www/ et implemente POST /upload en
suivant les memes regles que srcs/c_response/build_upload.cpp
(multipart/form-data, Content-Disposition: ... filename="...").

Usage:
    python3 verify_upload_server.py
    -> ouvre http://127.0.0.1:8080/ dans ton navigateur
"""
import os
import re
import string
from http.server import BaseHTTPRequestHandler, HTTPServer

WWW_ROOT = "/home/camy/Bureau/webserv5/www"
UPLOAD_DIR = os.path.join(WWW_ROOT, "upload")
ALLOWED_CHARS = set(string.ascii_letters + string.digits + "._-")


def sanitize_filename(name):
    if not name or name[0] == ".":
        return None
    if "/" in name or "\\" in name or ".." in name:
        return None
    if any(c not in ALLOWED_CHARS for c in name):
        return None
    return name


class Handler(BaseHTTPRequestHandler):
    def _serve_static(self):
        path = self.path.split("?", 1)[0]
        if path == "/":
            path = "/index.html"
        full = os.path.normpath(os.path.join(WWW_ROOT, path.lstrip("/")))
        if not full.startswith(WWW_ROOT) or not os.path.isfile(full):
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b"404 Not Found")
            return
        with open(full, "rb") as f:
            data = f.read()
        ctype = "text/html"
        if full.endswith(".js"):
            ctype = "application/javascript"
        elif full.endswith(".css"):
            ctype = "text/css"
        self.send_response(200)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        self._serve_static()

    def do_POST(self):
        if self.path != "/upload":
            self.send_response(404)
            self.end_headers()
            return

        ctype = self.headers.get("Content-Type", "")
        m = re.search(r'multipart/form-data\s*;\s*boundary=(.+)', ctype, re.I)
        if not m:
            self.send_response(400)
            self.end_headers()
            self.wfile.write(b"400 Bad Request: expected multipart/form-data")
            return
        boundary = m.group(1).strip().strip('"')

        length = int(self.headers.get("Content-Length", "0"))
        body = self.rfile.read(length)
        delim = ("--" + boundary).encode()

        pos = body.find(delim)
        if pos == -1:
            self.send_response(400); self.end_headers(); return
        pos += len(delim) + 2  # skip boundary + \r\n

        head_end = body.find(b"\r\n\r\n", pos)
        if head_end == -1:
            self.send_response(400); self.end_headers(); return
        headers_part = body[pos:head_end].decode(errors="replace")

        fm = re.search(r'filename="([^"]*)"', headers_part)
        if not fm:
            self.send_response(400); self.end_headers(); return
        filename = sanitize_filename(fm.group(1))
        if not filename:
            self.send_response(400); self.end_headers(); return

        content_start = head_end + 4
        end_delim = body.find(b"\r\n--" + boundary.encode(), content_start)
        if end_delim == -1:
            self.send_response(400); self.end_headers(); return
        file_data = body[content_start:end_delim]

        dest = os.path.join(UPLOAD_DIR, filename)
        if os.path.exists(dest):
            self.send_response(409); self.end_headers(); return
        with open(dest, "wb") as f:
            f.write(file_data)

        self.send_response(201)
        self.send_header("Content-Type", "text/html")
        self.send_header("Content-Length", "0")
        self.end_headers()

    def log_message(self, fmt, *args):
        print("[srv]", fmt % args)


if __name__ == "__main__":
    srv = HTTPServer(("127.0.0.1", 8080), Handler)
    print("Serveur de test sur http://127.0.0.1:8080  (Ctrl+C pour arreter)")
    srv.serve_forever()
