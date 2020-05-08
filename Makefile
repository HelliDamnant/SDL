all:
	clear
	gcc -c main.c -o main.o
	gcc main.o -O3 -lSDL2 -lSDL2_image -lm -o out
	strip out
clean:
	rm *.o 
	rm $(TARGET)
