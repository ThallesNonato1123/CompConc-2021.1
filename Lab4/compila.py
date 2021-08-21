import os
import time

while True:
  time.sleep(2)
  os.system("gcc Lab4.c Lab4 -Wall -lpthread")
  print("\n")
  os.system("./Lab4")