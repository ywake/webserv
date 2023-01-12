# 予めbasic.confでサーバーを起動しておくこと
LOG=get_test.log

teardown() {
  name=$(printf "======CASE %03d======" "$BATS_SUITE_TEST_NUMBER")
  echo -e "$name\n" >> "$LOG"
  echo -e "$output" >> "$LOG" 
  echo -e "\n====================" >> "$LOG"
}

CURL_FORMAT="\n%{http_version}\n"\
"%{http_code}\n"\
"%{content_type}\n"\
"%header{content-length}\n"\
"%header{server}\n"

REDIR_FMT="$CURL_FORMAT"\
"%header{location}\n"

## Exist
@test "get basic" {
  run curl -s -w $CURL_FORMAT http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1hello world!" ]
  [ "${lines[1]}" = "1.1" ]
  [ "${lines[2]}" = "200" ]
  [ "${lines[3]}" = "text/html" ]
  [ "${lines[4]}" = "13" ]
  [ "${lines[5]}" = "webserv/1.0" ]
}

## No Exist
@test "get not found" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/noexist
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "404" ]
  [ "${lines[2]}" = "text/html" ]
  # [ "${lines[3]}" = "190" ]
  [ "${lines[4]}" = "webserv/1.0" ]
}

## No Perm
@test "get file333" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/file333
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "403" ]
  [ "${lines[2]}" = "text/html" ]
  # [ "${lines[3]}" = "190" ]
  [ "${lines[4]}" = "webserv/1.0" ]
}

@test "get dir333/file" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/dir333/file
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "403" ]
  [ "${lines[2]}" = "text/html" ]
  # [ "${lines[3]}" = "190" ]
  [ "${lines[4]}" = "webserv/1.0" ]
}

## No Regular
@test "get dir" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/b
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "301" ]
  [ "${lines[2]}" = "text/html" ]
  # [ "${lines[3]}" = "190" ]
  [ "${lines[4]}" = "webserv/1.0" ]
}

@test "get fifo" {
  run curl -s -o /dev/null -w $CURL_FORMAT http://webserv:4242/fifo
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "404" ]
  [ "${lines[2]}" = "text/html" ]
  # [ "${lines[3]}" = "190" ]
  [ "${lines[4]}" = "webserv/1.0" ]
}

@test "get redirect" {
  run curl -s -o /dev/null -w "$REDIR_FMT" http://webserv:4242/redirect
  [ "$status" -eq 0 ]
  [ "${lines[0]}" = "1.1" ]
  [ "${lines[1]}" = "301" ]
  [ "${lines[2]}" = "text/html" ]
  [ "${lines[4]}" = "webserv/1.0" ]
  grep :// <<<"${lines[5]}"
  curl -v -s "${lines[5]}"
}
