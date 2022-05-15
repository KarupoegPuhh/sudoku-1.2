all: compile link

compile:
	g++ -c main.cpp -I "SFML-2.5.1\include" -DSFML_STATIC

link: #-sfml-main-d
	g++ main.o -o sudoku -L "SFML-2.5.1/lib"  -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows