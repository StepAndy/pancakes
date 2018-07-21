CC = gcc

#-------------------
#	lib
#-------------------

libname = libcalc.so
libkey  = -lcalc -ldl
libpath = -L.

#-------------------
# file with main()
# ------------------

my_infile  = calc.c
my_outfile = calc

#-------------------------
# sources and object files
# ------------------------

sources      = minus.c multiply.c plus.c devide.c
object_files = devide.o minus.o plus.o multiply.o

#-----------------------------------------------------------------
#-----------------------------------------------------------------
calc: libcalc_so
	$(CC)  $(my_infile) -o $(my_outfile) $(libpath) $(libkey)

libcalc_so: .o
	gcc -shared $(object_files) -o $(libname)

.o:  minus.c multiply.c plus.c devide.c
	$(CC) -c -fPIC $(sources) 

remove:
	rm *.o $(my_outfile) $(libname)
#-----------------------------------------------------------------
#-----------------------------------------------------------------

