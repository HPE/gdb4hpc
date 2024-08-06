# spin for at least argv[1] seconds

import time
import sys

start = time.time()
tick = 0

# intentionally keeping the hot loop on line numbers that start at 1
while time.time() - start < float(sys.argv[1]):
    print(f"only {time.time() - start} seconds have elapsed, spinning more")
    for i in range(0, 10_000_000):
        tick += 1

print(f"spun for {tick} ticks")
