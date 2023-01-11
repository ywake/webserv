# 予めbasic.confでサーバーを起動しておくこと

LOG=delete.log

teardown() {
  name=$(printf "======CASE %03d======" "$BATS_TEST_NUMBER")
  echo -e "$name\n" >> "$LOG"
	echo -e "$output" >> "$LOG" 
  echo -e "\n====================" >> "$LOG"
}

# DELETE
@test "delete basic" {
  run curl -s -o /dev/null -X POST -H "connection: close" -w "%{http_code}\n%header{location}" http://webserv:4242/a
  location=${lines[1]}
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "${location}"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "delete pct-encode" {
  run curl -s -o /dev/null -X POST -H "connection: close" -w "%{http_code}\n%header{location}" http://webserv:4242/%E3%81%82
  location=${lines[1]}
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "${location}"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "200" ]
}

@test "delete not found" {
  randpath=`cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 20 | head -n 1`
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "http://webserv:4242/$randpath"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "404" ]
}

@test "delete dir" {
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "http://webserv:4242/"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "403" ]
}

@test "delete fifo" {
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "http://webserv:4242/fifo"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "404" ]
}

@test "delete dir555/fifo" {
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "http://webserv:4242/dir555/fifo"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "403" ]
}

@test "delete dir555/file" {
  run curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "http://webserv:4242/dir555/file"
  [ "$status" -eq 0 ]
  [ "${lines[-1]}" -eq "403" ]
}

## Exist
# DELETE

## No Exist
# DELETE

## No Perm
# DELETE

## Not Regular
# DELETE

## Special case
# 2byte string
# obs-fold
# RequestのHTTPVersionが違うとき
# Requestがchunkedで送信
