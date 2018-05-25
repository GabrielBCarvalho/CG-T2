all:
	g++ -o main main.cpp -lGL -lGLU -lglut
run:
	./main
install:
	sudo apt-get install freeglut3-dev