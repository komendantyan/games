all:
	g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system

debug:
	g++ main.cpp -lsfml-graphics -lsfml-window -lsfml-system -g
