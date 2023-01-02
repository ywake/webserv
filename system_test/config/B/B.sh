set -x # コマンドを表示

BASEDIR=../../../
BIN="valgrind --leak-check=full $BASEDIR/webserv"

make -C $BASEDIR

# Test 1: serverディレクティブがない
# Test 1-1: 空のコンフィグファイル
$BIN "b1-empty.conf"
if [ $? -ne 1 ]; then
	echo "Test 1-1: Failed"
	exit 1
fi

# Test 1-2: 空ではないがserverディレクティブがない
$BIN "b1-not-empty.conf"
if [ $? -ne 1 ]; then
	echo "Test 1-2: Failed"
	exit 1
fi

# Test 2: 構文の正しいserverディレクティブ
# 成功するので省略

# Test 3-1: 構文の誤っているserverディレクティブ
$BIN "b3.conf"
if [ $? -ne 1 ]; then
	echo "Test 3: Failed"
	exit 1
fi

# Test 3-2: serverディレクティブの中にserverディレクティブがある
$BIN "b3-server-in-server.conf"
if [ $? -ne 1 ]; then
	echo "Test 3: Failed"
	exit 1
fi

echo "OK!"