#! /bin/sh

echo "status: 400 bad"
echo "Content-type: text/html"
echo "unko:a"
echo "Connection: keep-alive"
echo ""
echo "<html><head><title>CGI Test</title></head>"
echo "<body><h1>CGI Test2</h1>"
echo "<p>CGI is working!</p>"
echo "</body></html>"