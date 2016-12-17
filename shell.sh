clear
g++ -c src/imageprocessing.cpp -o src/imageprocessing.o
g++ -c main.cpp -o main.o
g++ -o main main.o src/imageprocessing.o
./main sg.pgm edge.pgm 32