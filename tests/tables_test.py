from ctypes import *

libc = CDLL("libc.so.6")

a = libc.malloc(sizeof(c_int))
a = 1
libc.printf(b"%d\n", a)