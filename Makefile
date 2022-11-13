all:
	g++ -o InfoIP InfoIP.cpp -lpthread
	./InfoIP listado.txt 4
clean:
	rm InfoIP
