cc = g++

.PHONY: clean
exe: 001
	@echo make OK

001: 001.cpp des.h base64.h md5sum.h  des.o base64.o md5sum.o
	$(cc) -pedantic-errors -Wall 001.cpp *.o -o 001

%.o: %.cpp %.h
	$(cc) -pedantic-errors -Wall $< -c

clean:
	@rm -f *.o
