main.out: main.cpp
	g++ -o main.out main.cpp -lFindFile -I include -L lib -no-pie
