CC = gcc

#-------------------
#	lib
#-------------------

keys  = -ldl

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

calc: plus_so devide_so multiply_so minus_so
	$(CC)  $(my_infile) -o $(my_outfile) $(keys)

plus_so: devide_so
	gcc -shared plus.o -o plus_plugin.so

devide_so: multiply_so
	gcc -shared devide.o -o devide_plugin.so
	
multiply_so: minus_so
	gcc -shared multiply.o -o multiply_plugin.so

minus_so: _o 
	gcc -shared minus.o -o minus_plugin.so

_o:  minus.c multiply.c plus.c devide.c
	$(CC) -c -fPIC $(sources) 

remove:
	rm *.o *.so $(my_outfile) $(libname) 
#-----------------------------------------------------------------
#-----------------------------------------------------------------

