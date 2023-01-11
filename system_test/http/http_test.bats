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

