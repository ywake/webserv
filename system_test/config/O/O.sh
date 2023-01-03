set -x # コマンドを表示

BASEDIR=../../../
BIN="valgrind --leak-check=full $BASEDIR/webserv"

make -C $BASEDIR

# Test 1: serverディレクティブの代わりに存在しないディレクティブ
$BIN "o1.conf"
if [ $? -ne 1 ]; then
	echo "Test 1: Failed"
	exit 1
fi

# Test 2: serverブロック内に存在しないディレクティブ
$BIN "o2.conf"
if [ $? -ne 1 ]; then
	echo "Test 2: Failed"
	exit 1
fi

# Test 3: locationディレクティブの代わりに存在しないディレクティブ
$BIN "o3.conf"
if [ $? -ne 1 ]; then
	echo "Test 3: Failed"
	exit 1
fi

# Test 4: locationブロック内に存在しないディレクティブ
$BIN "o4.conf"
if [ $? -ne 1 ]; then
	echo "Test 4: Failed"
	exit 1
fi

# Test 5: locationブロック内に新規ブロックを作る
$BIN "o5.conf"
if [ $? -ne 1 ]; then
	echo "Test 5: Failed"
	exit 1
fi

echo "OK!"