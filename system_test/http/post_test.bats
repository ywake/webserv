# 予めbasic.confでサーバーを起動しておくこと

LOG=post.log
OUTPUT=""

teardown() {
  name=$(printf "======CASE %03d======" "$BATS_SUITE_TEST_NUMBER")
  echo -e "$name\n" >> "$LOG"
  echo -e "$OUTPUT" >> "$LOG"
  echo -e "\n====================\n" >> "$LOG"
}

# POST
@test "post basic" {
  run curl -v -s -o /dev/null -X POST -H "connection: close" -w "%{http_code}\n%header{location}" http://webserv:4242/a -d "body"
  OUTPUT=$output
  code=${lines[-2]}
  location=${lines[-1]}
  [ "$status" -eq 0 ]
  [ "$code" -eq "201" ]
  grep http://webserv:4242 <<<"$location"

  run curl -s "${location}"
  [ "$output" == "body" ]
  curl -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "${location}"
}

@test "post pct-encode" {
  run curl -v -s -o /dev/null -X POST -H "connection: close" -w "%{http_code}\n%header{location}" http://webserv:4242/%E3%81%82
  OUTPUT=$output
  code=${lines[-2]}
  location=${lines[-1]}
  [ "$status" -eq 0 ]
  [ "$code" -eq "201" ]
  grep -v あ <<<"$location"
  curl -v -s -o /dev/null -X DELETE -H "connection: close" -w "%{http_code}" "${location}"
}

@test "post no-perm" {
  run curl -v -s -o /dev/null -X POST -H "connection: close" -w "%{http_code}" http://webserv:4242/dir555/a
  OUTPUT=$output
  code=${lines[-1]}
  [ "$status" -eq 0 ]
  [ "$code" -eq "403" ]
}
