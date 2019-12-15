cc = g++

.PHONY: clean
it: 001
	@echo make OK

001: 001.cpp aes.h base64.h md5sum.h  aes.o base64.o md5sum.o
	$(cc) -pedantic-errors -Wall 001.cpp *.o -o 001

%.o: %.cpp %.h
	$(cc) -pedantic-errors -Wall $< -c

clean:
	@rm -f *.o
