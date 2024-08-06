# sleep for argv[1] seconds

import time
import sys

def sleep():
    time.sleep(int(sys.argv[1]))

def main():
    sleep()

if __name__ == "__main__":
    main()
