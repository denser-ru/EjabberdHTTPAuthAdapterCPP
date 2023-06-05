from http.server import BaseHTTPRequestHandler, HTTPServer

class EchoHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        request_data = self.rfile.read(content_length)

        print("Received data:", request_data.decode('utf-8'))

        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()

        self.wfile.write(request_data)

def run_server(port=8080):
    server_address = ('', port)
    httpd = HTTPServer(server_address, EchoHandler)
    print(f'Starting echo server on port {port}...')
    httpd.serve_forever()

run_server()
