from mpi4py import MPI
import math



comm = MPI.COMM_WORLD
process_count = comm.Get_size()
procrank = comm.Get_rank()
range_val = None
if procrank == 0:
    range_val = int(input("Enter num: "))
range_val = comm.bcast(range_val, root = 0)

def is_prime(n):
    top = int(math.sqrt(n) + 0.0001)
    for d in range(2, top + 1):
        if n % d == 0:
            return False
    return True


start_time = MPI.Wtime()

dt = range_val / process_count
min_val = 2 if procrank == 0 else int(procrank * dt) + 1
max_val = range_val if procrank == process_count - 1 else int((procrank + 1) * dt)

count = 0

if procrank != 0:
    for i in range(min_val, max_val + 1):
        if is_prime(i):
            count += 1
    data_to_send = [count, min_val, max_val]
    comm.send(data_to_send, dest=0)
else:
    for i in range(min_val, max_val + 1):
        if is_prime(i):
            count += 1

    elapsed = MPI.Wtime() - start_time

    for ct in range(1, process_count):
        data_received = comm.recv(source=MPI.ANY_SOURCE)
        count += data_received[0]

    print(f"\nNumber of primes in 1 to {range_val} are \n{count}")
    print(f"Time elapsed: {elapsed} seconds.")

MPI.Finalize()
