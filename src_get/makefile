execute: compile
	./Get_Face

compile:
	g++ Get_Face.cpp -o Get_Face `pkg-config --cflags --libs opencv`

filter: compile_filter
	./filter

compile_filter:
	g++ filter_personnes.cpp -o filter `pkg-config --cflags --libs opencv`