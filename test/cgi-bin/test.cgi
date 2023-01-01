#! /bin/sh

echo "Content-type: text/html"
### Client Redirect ###
# echo "Location: http://www.google.com" # client redirect

### Local redirect ###
# echo "Location: /index.html" # local redirect
# echo "Location: /cgi-bin/test2.cgi" # local redirect
# echo "Location: " # local redirect # →302 Found
# echo "Location: /cgi-bin/test.cgi" # local redirect # 500 `Request exceeded the limit of 10 internal redirects due to probable configuration error. Use 'LimitInternalRecursion' to increase the limit if necessary. Use 'LogLevel debug' to get a backtrace.`
# echo "Location: ../" #local redirect (directory) # 404 script not found or unable to stat: /usr/local/apache2/cgi-bin/cgi.py

echo ""
echo $0 $1 $2 $A
export
# echo "<html><head><title>CGI Test</title></head>"
# echo "<body><h1>CGI Test</h1>"
# echo "<p>CGI is working!</p>"
# echo "</body></html>"
