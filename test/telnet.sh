#!/bin/sh

RESDIR=response
NGINX_RES=$RESDIR/nginx_$(basename "$1").res
APACHE_RES=$RESDIR/apache_$(basename "$1").res

python3 lazy_file_printer.py "$1" | telnet localhost 8081 > "$NGINX_RES"
python3 lazy_file_printer.py "$1" | telnet localhost 8082 > "$APACHE_RES"

echo "\n==== NGINX ====\n"
cat "$NGINX_RES"
echo "\n==== APACHE ====\n"
cat "$APACHE_RES"
echo