all:
	em++ drum.cpp -o functions.html -s EXPORTED_FUNCTIONS="['_resize', '_setbd', '_addhit', '_step', '_return1', '_getu']" -s TOTAL_MEMORY=268435456


