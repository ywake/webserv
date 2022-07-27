#!/bin/sh

NGINX=localhost:8081
APACHE=localhost:8082

curl -Is http://$NGINX/$1 > nginx.res
curl -Is http://$APACHE/$1 > apache.res


echo "==== NGINX ====\n"
cat nginx.res
echo "\n==== APACHE ====\n"
cat apache.res