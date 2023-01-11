# 予めbasic.confでサーバーを起動しておくこと

teardown() {
	echo -e "\t$output" >&3
}

# GET
@test "「http://webserv:4242/」のStatusCodeは200" {
  run curl -s -o /dev/null -w "%{http_code}" http://webserv:4242/
  [ "$status" -eq 0 ]
  [ "$output" -eq "200" ]
}

@test "「http://localhost:4242/」のHTTP Versionは1.1" {
  run curl -s -o /dev/null -w "%{http_version}" http://localhost:4242/
  [ "$status" -eq 0 ]
  [ "$output" = "1.1" ]
}

@test "「http://localhost:4242/」のContent-Typeはtext/html" {
  run curl -s -o /dev/null -w "%{content_type}" http://localhost:4242/
  [ "$status" -eq 0 ]
  [ "$output" = "text/html" ]
}

@test "「http://localhost:4242/」のHeader比較" {
  run curl -s -o /dev/null -w "%header{content-length}" http://localhost:4242/
  [ "$status" -eq 0 ]
  [ "$output" -eq "13" ]
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

