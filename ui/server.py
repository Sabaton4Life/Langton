#!/usr/bin/env python3
"""
Simple HTTP server for Langton's Ant Web UI
Serves the HTML/JS frontend on localhost:8080
"""

import http.server
import socketserver
import os
import sys
from pathlib import Path

# Get directory of this script
SCRIPT_DIR = Path(__file__).parent.absolute()
PORT = 8080


class MyHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def translate_path(self, path):
        # Serve files from the UI directory
        if path == '/':
            path = '/index.html'
        return str(SCRIPT_DIR / path.lstrip('/'))

    def end_headers(self):
        # Add cache control headers
        self.send_header('Cache-Control', 'no-store, no-cache, must-revalidate')
        super().end_headers()

    def log_message(self, format, *args):
        # Custom logging
        print(f"[{self.client_address[0]}] {format % args}", file=sys.stderr)


if __name__ == '__main__':
    os.chdir(SCRIPT_DIR)

    with socketserver.TCPServer(("", PORT), MyHTTPRequestHandler) as httpd:
        print(f"╔════════════════════════════════════════╗")
        print(f"║  Langton's Ant Web UI Server          ║")
        print(f"╠════════════════════════════════════════╣")
        print(f"║  URL: http://localhost:{PORT}           │")
        print(f"║  Directory: {SCRIPT_DIR}")
        print(f"║  Press Ctrl+C to stop                  │")
        print(f"╚════════════════════════════════════════╝")

        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n\nServer stopped.")
            sys.exit(0)
