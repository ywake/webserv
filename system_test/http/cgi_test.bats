# 予めbasic.confでサーバーを起動しておくこと
LOG=cgi_test.log

teardown() {
  name=$(printf "======CASE %03d======" "$BATS_SUITE_TEST_NUMBER")
  echo -e "$name\n" >> "$LOG"
  echo -e "$output" >> "$LOG" 
  echo -e "\n====================" >> "$LOG"
}

CURL_FORMAT="\n%{http_version}\n"\
"%{http_code}\n"\
"%{content_type}\n"\
"%header{server}\n"\
"%header{location}\n"

## Document Response
# Exist
@test "cgi basic" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/test.cgi -H "Host:cgi"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "200" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}
@test "cgi random" {
  rand=`cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 20 | head -n 1`
  run curl -s -o ./cgi_rand_out -w $CURL_FORMAT http://webserv:4242/cat.cgi -H "Host:cgi" -d "$rand" -X GET
  [ "$status" -eq 0 ]
  diff cgi_rand_out <(echo -n "$rand")
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "200" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}
# No exist
@test "cgi noexist" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/noexist.cgi -H "Host:cgi"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "404" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}
# No perm
@test "cgi file333" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/file333.cgi -H "Host:cgi"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "403" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}
# No Regular
@test "cgi dir" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/dir.cgi -H "Host:cgi"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "404" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}
@test "cgi fifo" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/fifo.cgi -H "Host:cgi"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "404" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}

## local redirect
@test "cgi local redirect" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/localredirect.cgi -H "Host:cgi"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "200" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[3]}" = "webserv/1.0" ]
}

## client redirect
@test "cgi client redirect" {
  run curl -s -w $CURL_FORMAT http://webserv:4242/clientredirect.cgi -H "Host:cgi" -H "connection: close"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "302" ]
  [ "${lines[2]}" = "webserv/1.0" ]
  [ "${lines[3]}" = "http://www.google.com" ]
}

## client redirect with doc
@test "cgi client redirect with doc" {
  run curl -s -w $CURL_FORMAT http://webserv:4242/crwd.cgi -H "Host:cgi" -H "connection: close"
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "hello world!" ]
  [ "${lines[1]}" = "1.1" ]
  [ "${lines[2]}" = "302" ]
  [ "${lines[3]}" = "text/html" ]
  [ "${lines[4]}" = "webserv/1.0" ]
  [ "${lines[5]}" = "http://www.google.com" ]
}
