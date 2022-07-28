all: clean assembler

assembler : assembler.o linked_list_utils.o preprocessor.o first_scan.o second_scan.o queue.o symbol_table.o output.o
	gcc -g -Wall -ansi -pedantic assembler.o linked_list_utils.o preprocessor.o first_scan.o second_scan.o queue.o symbol_table.o output.o -o assembler

assembler.o : assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

linked_list_utils.o: linked_list_utils.h linked_list_utils.c
	gcc -g -Wall -ansi -pedantic -c linked_list_utils.c -o linked_list_utils.o

preprocessor.o: preprocessor.c preprocessor.h
	gcc -g -Wall -ansi -pedantic -c preprocessor.c -o preprocessor.o

first_scan.o : first_scan.c compilation.h
	gcc -c -Wall -ansi -pedantic first_scan.c -o first_scan.o

second_scan.o : second_scan.c compilation.h
	gcc -c -Wall -ansi -pedantic second_scan.c -o second_scan.o

queue.o : queue.c compilation.h
	gcc -c -Wall -ansi -pedantic queue.c -o queue.o

symbol_table.o : symbol_table.c compilation.h
	gcc -c -Wall -ansi -pedantic symbol_table.c -o symbol_table.o

output.o : output.c compilation.h
	gcc -c -Wall -ansi -pedantic output.c -o output.o


clean:
	rm -rf assembler *.o