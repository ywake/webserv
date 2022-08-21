import os
import sys

def select_newer_modified_path(path1, path2):
    time1 = os.stat(path1).st_mtime
    time2 = os.stat(path2).st_mtime
    return path1 if time1 > time2 else path2

def print_newer_modified_path(path1, path2):
    exist1 = os.path.exists(path1)
    exist2 = os.path.exists(path2)
    if exist1 and exist2:
        newer = select_newer_modified_path(path1, path2)
        print(newer)
    elif exist1:
        print(path1)
    elif exist2:
        print(path2)
    else:
        print(path1)

if __name__ == '__main__':
    path1 = sys.argv[1]
    path2 = sys.argv[2]
    print_newer_modified_path(path1, path2)
