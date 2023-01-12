# 予めbasic.confでサーバーを起動しておくこと

LOG=http.log

teardown() {
  name=$(printf "======CASE %03d======" "$BATS_SUITE_TEST_NUMBER")
  echo -e "$name\n" >> "$LOG"
	echo -e "$output" >> "$LOG" 
  echo -e "\n====================" >> "$LOG"

  
}

# GET
@test "「http://webserv:4242/」のStatusCodeは200" {
  run curl -v -s -o /dev/null -w "%{http_code}" http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "「http://webserv:4242/」のHTTP Versionは1.1" {
  run curl -v -s -o /dev/null -w "%{http_version}" http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" == "1.1" ]
}

@test "「http://webserv:4242/」のContent-Typeはtext/html" {
  run curl -v -s -o /dev/null -w "%{content_type}" http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" == "text/html" ]
}

@test "「http://webserv:4242/」のHeader比較" {
  run curl -v -s -o /dev/null -w "%header{content-length}" http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "13" ]
}

@test "get content-length-0" {
  conlen=$(echo -n $'content-length:0')
  run curl -v -s -o /dev/null -X GET -H "$conlen" -H "connection: close" -w "\n%{http_code}" "http://webserv:4242/"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "get chunked-0" {
  trae=$(echo -n $'transfer-encoding:chunked')
  run curl -v -s -o /dev/null -X GET -H "$trae" -H "connection: close" -w "\n%{http_code}" "http://webserv:4242/" -d ""
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "get obs-fold" {
  host=$(echo -n $'host:\x0d\x0a a')
  run curl -v -s -o /dev/null -X GET -H "$host" -H "connection: close" -w "\n%{http_code}" "http://webserv:4242/"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "get obs-fold content-length-0" {
  conlen=$(echo -n $'content-length:\x0d\x0a 0')
  run curl -v -s -o /dev/null -X GET -H "$conlen" -H "connection: close" -w "\n%{http_code}" "http://webserv:4242/"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "get obs-fold chunked-0" {
  trae=$(echo -n $'transfer-encoding:\x0d\x0a chunked')
  run curl -v -s -o /dev/null -X GET -H "$trae" -H "connection: close" -w "\n%{http_code}" "http://webserv:4242/" -d ""
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "http/1.0" {
  run curl -v -s -o /dev/null -w "%{http_code}" --http1.0 http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "400" ]
}

@test "uri too long" {
  long=`cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 10000 | head -n 1`
  run curl -v -s -o /dev/null -w "%{http_code}" --http1.0 "http://webserv:4242/$long"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "414" ]
}

@test "method too long" {
  long=`cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 10000 | head -n 1`
  run curl -v -s -o /dev/null -X "LONGMETHOD" -w "%{http_code}" --http1.0 "http://webserv:4242/$long"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "501" ]
}

@test "method not allowed" {
  run curl -v -s -o /dev/null -X "POST" -w "%{http_code}\n%header{allow}" "http://webserv:4243/"
  [ "$status" -eq 0 ]
  [ "${lines[-2]}" -eq "405" ]
}

## Exist
# GET
# AutoIndex
# DELETE
# POST
# CGI

## No Exist
# GET
# AutoIndex
# DELETE
# POST
# CGI

## No Perm
# GET
# AutoIndex
# DELETE
# POST
# CGI

## Not Regular
# GET
# DELETE
# CGI

## Special case
# 2byte string
# obs-fold
# RequestのHTTPVersionが違うとき
# Requestがchunkedで送信

## Error case
# Method not implemented
# Method not allowed
# URI TOO LONG
# Path traversal
# Content Lengthが0
# Chunked 0

