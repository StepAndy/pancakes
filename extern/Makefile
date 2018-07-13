CC	   = gcc
my_infile  = calc.c
my_outfile = calc
sources    = minus.c multiply.c plus.c devide.c

#-----------------------------------------------------------------
#-----------------------------------------------------------------

calc: $(my_infile) $(sources) 
	$(CC)  $(my_infile) $(sources) -o $(my_outfile) 

remove:
	rm *.o $(my_outfile) $(libname)

#-----------------------------------------------------------------
#-----------------------------------------------------------------

