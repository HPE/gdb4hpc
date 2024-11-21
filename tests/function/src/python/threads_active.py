import threading
import time
import sys

# "active" as in the GIL is actively contested

num = 0
num_lock = threading.Lock()

def inc_num(times):
    global num
    for _ in range(times):
        with num_lock:
            for _ in range(100000):
                num += 1
        print("thread", threading.get_ident(), num)

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
