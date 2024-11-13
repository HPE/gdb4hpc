# spin for at least argv[1] seconds

import time
import sys

start = time.time()
tick = 0

# intentionally keeping the hot loop on line numbers that start at 1
while time.time() - start < float(sys.argv[1]):
    print("only %f seconds have elapsed, spinning more" % (time.time() - start))
    for i in range(0, 10000000):
        tick += 1

print("spun for %d ticks" % tick)
