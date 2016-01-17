all:
	em++ drum.cpp -o js/functions.html -s EXPORTED_FUNCTIONS="['_resize', '_setbd', '_addhit', '_step', '_return1', '_getu']"
	rm js/functions.html


