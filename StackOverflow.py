import sys
import os #os package can be used to run terminal commands (like ulimit -s)
import threading

#Recursive method has no base case. Therefore it will go on to infinity
def recurse(n):
    print(n)
    sys.setrecursionlimit(sys.getrecursionlimit() + 1)
    recurse(n+1)

   #Making a thread with an infinite amount of memory allocated to it
   #This option is great because you can have multiple threads using up memory
   #simultaneously. This will break your computer faster

#Make size of thread
sys.setrecursionlimit(100000)
threading.stack_size(0x9900000)
#Create/Define threads using threading package
t = threading.Thread(target=recurse, args=(0,))
t2 = threading.Thread(target=recurse, args=(0,))
t3 = threading.Thread(target=recurse, args=(0,))
t4 = threading.Thread(target=recurse, args=(0,))
#Start threads
t.start()
t2.start()
t3.start()
t4.start()
t.join()
t2.join()
t3.join()
t4.join()
