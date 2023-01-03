#! /bin/sh

echo "Content-type: text/html"

echo ""
echo "<html><head><title>CGI Test</title></head>"
echo "<body><h1>CGI Test</h1>"
sec=0
for i in `seq 0 5`
do
	echo "<p>$sec CGI is working!</p>"
	sleep 5
	sec=$(($sec + 1))
done
echo "</body></html>"
