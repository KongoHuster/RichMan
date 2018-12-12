rich: rich.o function.o assist.o
	gcc rich.o function.o assist.o -o rich
rich.o: rich.c
	gcc -c rich.c
function.o: function.c
	gcc -c function.c
assist.o: assist.c
	gcc -c assist.c
clean:
	rm *.o ./rich
