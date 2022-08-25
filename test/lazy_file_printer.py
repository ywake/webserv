from time import sleep
import sys

with open(sys.argv[1]) as f:
    buf = f.read()
    print(buf, flush=True)
sleep(0.5)
