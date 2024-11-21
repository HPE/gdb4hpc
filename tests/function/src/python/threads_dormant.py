import threading
import time
import sys

# "dormant" as in the GIL is "dormant" (not actively contested) because the
# main thread loop is mostly sleeping.

num = 0
num_lock = threading.Lock()
def inc_num(times):
    time.sleep(1) # so the spawning loop in the main thread can finish before we hit any breakpoints
    global num
    for _ in range(times):
        with num_lock:
            num += 1
            print("thread", threading.get_ident(), num)
            time.sleep(1)

num_threads = int(sys.argv[1])
num_loops = int(sys.argv[2])
threads = []

for _ in range(num_threads):
    threads.append(threading.Thread(
        target=inc_num,
        args=(num_loops,)
    ))

for i in range(num_threads):
    print("starting thread %d" % i);
    threads[i].start()

for i in range(num_threads):
    print("joining thread %d" % i);
    threads[i].join()
