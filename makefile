all: functions.o writing.o
	gcc -o functions functions.o
	gcc -o write writing.o

functions.o: functions.c
	gcc -c functions.c

writing.o: writing.c
	gcc -c writing.c

clean:
	rm *.o
	rm *~

gdb: a.out
	gdb a.out
