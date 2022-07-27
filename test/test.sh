#!/bin/sh

NGINX=localhost:8081
APACHE=localhost:8082

curl -is http://$NGINX/$1 > nginx.res
curl -is http://$APACHE/$1 > apache.res


echo "\n==== NGINX ====\n"
cat nginx.res
echo "\n==== APACHE ====\n"
cat apache.res
echo