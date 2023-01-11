setup() {
  ROOT_DIR=../..
  WEBSERV=$ROOT_DIR/webserv
}

teardown() {
  echo -e "\t$output" >&3
}

@test "configファイルを引数で指定しない" {
  run $WEBSERV
  [ "$status" -eq 1 ]
}

@test "存在しないconfigファイル" {
  run $WEBSERV error/no_exist_config
  [ "$status" -eq 1 ]
}

# @test "権限がないconfigファイル" {
#   run $WEBSERV error/no_permission_config.conf
#   [ "$status" -eq 1 ]
# }

@test "configの代わりにディレクトリ" {
  run $WEBSERV error/dir.conf
  [ "$status" -eq 1 ]
}

@test "空のコンフィグ" {
  run $WEBSERV error/empty.conf
  [ "$status" -eq 1 ]
}

@test "serverディレクティブがない" {
  run $WEBSERV error/no_server_directive.conf
  [ "$status" -eq 1 ]
}

# serverの構文エラー
@test "serverディレクティブ直後の改行" {
  run $WEBSERV error/server_newline.conf
  [ "$status" -eq 1 ]
}

@test "serverディレクティブ内にserver" {
  run $WEBSERV error/server_in_server.conf
  [ "$status" -eq 1 ]
}

@test "locationディレクティブ内にserver" {
  run $WEBSERV error/server_in_location.conf
  [ "$status" -eq 1 ]
}

# locationの構文エラー
@test "locationの引数がない" {
  run $WEBSERV error/location_0_args.conf
  [ "$status" -eq 1 ]
}

@test "locationの引数が3つ" {
  run $WEBSERV error/location_3_args.conf
  [ "$status" -eq 1 ]
}

@test "serverディレクティブ外にlocation" {
  run $WEBSERV error/location_out_server.conf
  [ "$status" -eq 1 ]
}

@test "locationディレクティブ内にlocation" {
  run $WEBSERV error/location_in_location.conf
  [ "$status" -eq 1 ]
}

# locationの設定値エラー
