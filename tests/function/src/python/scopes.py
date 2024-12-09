# sleep for argv[1] seconds

import time
import sys

globl = "your grumpy global string"

def sleep(seconds):
    local = "your friendly neighborhood string"
    time.sleep(seconds)

def main():
    pi = 3.14159
    sleep(int(sys.argv[1]))

e = 2.718

if __name__ == "__main__":
    main()
