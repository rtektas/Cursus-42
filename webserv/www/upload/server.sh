#!/bin/bash

echo "=== Création du serveur Python HTTP reverse-proxy ==="

cat > server.py << 'EOF'
from http.server import BaseHTTPRequestHandler, HTTPServer
import requests

TARGET = "http://localhost:8444"  # ou https si ton backend est en HTTPS

class ProxyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            r = requests.get(TARGET + self.path)

            self.send_response(r.status_code)
            for header, value in r.headers.items():
                if header.lower() not in ["content-encoding", "transfer-encoding", "content-length"]:
                    self.send_header(header, value)
            self.end_headers()
            self.wfile.write(r.content)

        except Exception as e:
            self.send_response(500)
            self.end_headers()
            self.wfile.write(f"Erreur proxy: {e}".encode())

server = HTTPServer(("0.0.0.0", 9443), ProxyHandler)
print("Serveur HTTP en écoute sur le port 9443…")
server.serve_forever()
EOF

echo "=== Script terminé ==="
echo "Lance le serveur avec : python3 server.py"

