maintab: maintab.o tab.o
	gcc -Wall -o maintab $^

maintab.o: maintab.c tab.h
	gcc -Wall -c $<

tab.o: tab.c tab.h
	gcc -Wall -c $<

clean:
	rm -f *.o maintab