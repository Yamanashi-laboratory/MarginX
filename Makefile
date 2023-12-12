MarginX: MarginX.cpp makecir.cpp judge.cpp margin.cpp file.cpp optimize.cpp
	g++ -o MarginX MarginX.cpp makecir.cpp judge.cpp margin.cpp file.cpp optimize.cpp -std=c++17 -lstdc++fs
setup: setup.cpp
	g++ -o setup setup.cpp -std=c++17 -lstdc++fs
mew: mew.cpp
	g++ -o mew mew.cpp -std=c++17 -lstdc++fs