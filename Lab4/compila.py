import os
import time

os.system("gcc Lab4.c Lab4 -Wall -lpthread")
while True:
  time.sleep(2)
  print("\n")
  os.system("./Lab4")