set -x # コマンドを表示

BASEDIR=../../../
BIN="valgrind --leak-check=full $BASEDIR/webserv"

make -C $BASEDIR

# Test 1: 存在しないコンフィグファイル
$BIN "no_exist_config_file"
if [ $? -ne 1 ]; then
	echo "Test 1: Failed"
	exit 1
fi

# Test 2: コンフィグファイルがあるけど権限がない
touch no_permission_config_file
chmod 000 no_permission_config_file
$BIN "no_permission_config_file"
if [ $? -ne 1 ]; then
	echo "Test 2: Failed"
	exit 1
fi

# Test 3: コンフィグファイルにファイル以外
$BIN "dir"
if [ $? -ne 1 ]; then
	echo "Test 3: Failed"
	exit 1
fi

# Test 4: 正しいコンフィグファイル(実行されるのでコメントアウト)
# $BIN "correct_config_file.conf"
# if [ $? -ne 0 ]; then
# 	echo "Test 4: Failed"
# 	exit 1
# fi

# Test 5: コンフィグファイルを入力しない
$BIN
if [ $? -ne 1 ]; then
	echo "Test 5: Failed"
	exit 1
fi

echo "OK!"