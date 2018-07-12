CC = gcc

#-------------------
#	lib
#-------------------

libname = libcalc.a
libkey = -lcalc
libpath = -L.

#-------------------
# file with main()
# ------------------

my_infile = calc.c
my_outfile = calc

#-------------------------
# sources and object files
# ------------------------

sources = minus.c multiply.c plus.c devide.c
object_files = devide.o minus.o plus.o multiply.o

calc: libcalc_a
	$(CC)  $(my_infile) -o $(my_outfile) $(libpath) $(libkey)

libcalc_a: .o
	ar cr $(libname) $(object_files)
	
.o:  minus.c multiply.c plus.c devide.c
	$(CC) -c $(sources) 
remove:
	rm *.o $(my_outfile) $(libname)

