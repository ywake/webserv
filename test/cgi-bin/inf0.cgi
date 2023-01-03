#! /bin/sh

echo "Content-type: text/html"

echo ""
echo "<html><head><title>CGI Test</title></head>"
echo "<body><h1>CGI Test</h1>"
sec=0
while [ 1 ]
do
	echo "<p>$sec CGI is working!</p>"
	# echo "$sec"
	if [ $? != 0 ]; then
		echo "=====================ERROR=========================" 1>&2
		exit 1;
	fi
	sec=$(($sec + 1))
	# sleep 0.003
done
echo "</body></html>"
