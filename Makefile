all:
	em++ drum.cpp -o js/functions.html -s EXPORTED_FUNCTIONS="['_resize', '_setbd', '_addhit', '_step', '_getMin', '_getMax']"  -s ALLOW_MEMORY_GROWTH=1 -O2
	rm js/functions.html


