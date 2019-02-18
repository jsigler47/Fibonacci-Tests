#include <iostream>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <istream>
using namespace std;
unsigned long long int FibLoop(int);
unsigned long long int FibRecur(int);
unsigned long long int FibRecurDP(int);
unsigned long long int FibMatrix(int x);

void test(int, int, ofstream&);
//Helper functions
void clear(unsigned long long int*); //Clear the cache for FibRecurDP
void mult(unsigned long long int F[2][2], unsigned long long int M[2][2]); //Multiply two 2x2 matricies
void pow(unsigned long long int F[2][2], int x); //raid a 2x2 matrix to a power.
int bitsNeeded(uint64_t x); //Determine the number of bits needed to represent x.

//The for loop in main will run from 0 to X
//and run the test function for that value.
#define LOOPSIZE 90
#define RECURSIZE 40
#define RECURDPSIZE 90
#define MATRIXSIZE 90

//The test function will calculate the fib(n)
// for the current loop counter, n and plot the
// value of n and the time taken to calculate
// the fib(n) value to goo.dat which will then
// be plotted using gnuplot.

	//Methods: 
	//	1: Loop
	//	2: Recursive
	//  3: Dynamic Recursive
	//	4: Matrix Multiplication
	#define LOOP 1
	#define RECURSIVE 2
	#define RECURSIVEDP 3
	#define MATRIX 4

//Global array used for the dynamic recursive methoid.
unsigned long long int cache[RECURDPSIZE];
const bool DEBUG = false;
//BITS determines if the x-axis of the data
//is represented by the min number of bits
//required to represent the data or the data itself.
const bool BITS = false;

int main(){
	ofstream outfile;
	char const *loopTest = "loopTest.dat";
	char const *recurTest = "recurTest.dat";
	char const *recurDPTest = "recurDPTest.dat";
	char const *matrixTest = "matrixTest.dat";
//------------------------------------------------------------------------------------------
//	LOOP TESTS
//------------------------------------------------------------------------------------------
	//Open loop.dat in write mode to clear any contents.
	outfile.open(loopTest);
	//file error checking.
	if(!outfile.is_open()){
		fprintf(stderr, "cannot open target file.\n");
		exit(1);
	}
	if(DEBUG)
		cout << "Testing loop function to fib(" << LOOPSIZE << ")" << endl;
	//Test the loop function and write the results to the loop data file.
	for(int i = 0; i <= LOOPSIZE; i++){
		test(LOOP, i, outfile);
	}
	//Close the loop file.
	outfile.close();
//------------------------------------------------------------------------------------------
//	RECURSIVE TESTS
//------------------------------------------------------------------------------------------
	//Repeat test for recursive function.
	outfile.open(recurTest);
	//file error checking.
	if(!outfile.is_open()){
		fprintf(stderr, "cannot open target file.\n");
		exit(1);
	}
	if(DEBUG)
		cout << "Testing recursive function to fib(" << RECURSIZE << ")" << endl;
	//Test the recursive function and write the results to the recursive data file.
	for(int i = 0; i <= RECURSIZE; i++){
		test(RECURSIVE, i, outfile);
	}
	//Close the recursive file.
	outfile.close();
//------------------------------------------------------------------------------------------
//	DYNAMIC RECURSIVE TESTS
//------------------------------------------------------------------------------------------
	//Repeat test for dynamic recursive function.
	outfile.open(recurDPTest);
	//file error checking.
	if(!outfile.is_open()){
		fprintf(stderr, "cannot open target file.\n");
		exit(1);
	}
	if(DEBUG)
		cout << "Testing dynamic recursive function to fib(" << RECURDPSIZE << ")" << endl;
	//Test the dynamic recursive function and write the results to the dynamic recursive data file.
	for(int i = 0; i <= RECURDPSIZE; i++){
		test(RECURSIVEDP, i, outfile);
	}
	//Close the dynamic recursive file.
	outfile.close();
//------------------------------------------------------------------------------------------
//	MATRIX MULTIPLICATION TESTS
//------------------------------------------------------------------------------------------
	//Repeat test for matrix function.
	outfile.open(matrixTest);
	//file error checking.
	if(!outfile.is_open()){
		fprintf(stderr, "cannot open target file.\n");
		exit(1);
	}
	if(DEBUG){
		cout << "Testing matrix function to fib(" << MATRIXSIZE << ")" << endl;
	}
	//Test the matrix function and write the results to the matrix data file.
	for(int i = 0; i <= MATRIXSIZE; i++){
		test(MATRIX, i, outfile);
	}
	//Close the matrix file.
	outfile.close();
//------------------------------------------------------------------------------------------
//	END OF TESTS
//------------------------------------------------------------------------------------------
	return 0;
}

void clear(unsigned long long int* cache){
	//Clear the global cache array used for the recursive dynamic function.
	for(int i = 0; i <= RECURDPSIZE; i++){
		//-1 indicates the value has not been found.
		cache[i] = -1;
	}
}

void mult(unsigned long long int F[2][2], unsigned long long int M[2][2]){

	unsigned long long int a = F[0][0]*M[0][0] + F[0][1]*M[1][0];
	unsigned long long int b = F[0][0]*M[0][1] + F[0][1]*M[1][1];
	unsigned long long int c = F[1][0]*M[0][0] + F[1][1]*M[1][0];
	unsigned long long int d = F[1][0]*M[0][1] + F[1][1]*M[1][1];

	F[0][0] = a;
	F[0][1] = b;
	F[1][0] = c;
	F[1][1] = d;
}

void pow(unsigned long long int F[2][2], int x){
	if(x == 0 || x == 1)
		return;
	unsigned long long int M[2][2] = {{1,1},
				   					  {1,0}};
	pow(F, x/2);
	mult(F, F);

	if(x % 2 != 0)
		mult(F, M);
}

int bitsNeeded(uint64_t x){
	int bits = 0;
	for(int test = 16; test > 0; test >>= 1){
		if(x >> test != 0){
			bits += test;
			x >>= test;
		}
	}
	return bits + x;
}

unsigned long long int FibLoop(int x){
	//Use a simple loop that continuously adds
	//numbers and adjusts the values accordingly
	//to find the fib(x) value.
	unsigned long long int num1 = 0, num2 = 1, sum = 0;
	if(x <= 1)
		return (unsigned long long int)x;

	for(int i = 1; i < x; ++i){
		sum = num1 + num2;
		num1 = num2;
		num2 = sum;

	}
	return sum;
}

unsigned long long int FibRecur(int x){
	//Recursive fibonacci function.
	if(x <= 1){	
		return x;
	}
	else{
		return FibRecur(x-1) + FibRecur(x-2);
	}
}

unsigned long long int FibRecurDP(int x){
	//Very similar to the FibRecur function but if
	//a result has already been calculated and stored
	//in the global cache array, it does not need
	//to be calculated again, which can save time.
	if(x <= 1){	
		return x;
	}
	else if(cache[x] != -1){
		return cache[x];
	}
	else{
		unsigned long long int result = FibRecurDP(x-1) + FibRecurDP(x-2);
		cache[x] = result;
		return result;
	}
}

unsigned long long int FibMatrix(int x){
	unsigned long long int F[2][2] = {{1,1},
				   					  {1,0}};
	if(x == 0 || x == 1)
		return (unsigned long long int)x;
	pow(F, x-1);

	return F[0][0];
}


void test(int method, int x, ofstream& f){

	//Initialize the timers and temp variable.
	auto start = chrono::high_resolution_clock::now();
	auto end = chrono::high_resolution_clock::now();
	unsigned long long int tmp = 0;

	switch(method)
	{
		case 1:
			start = chrono::high_resolution_clock::now();
			tmp = FibLoop(x);
			end = chrono::high_resolution_clock::now();
			//Only output the result if it is the final iteration of the test.
			if(x == LOOPSIZE)
				cout << "Loop: f(" << x << ") = " << tmp << endl;
			break;
		case 2:
			start = chrono::high_resolution_clock::now();
			tmp = FibRecur(x);
			end = chrono::high_resolution_clock::now();
			if(x == RECURSIZE)
				cout << "Recursion: f(" << x << ") = " << tmp << endl;
			break;
		case 3:
 			clear(cache);
 			start = chrono::high_resolution_clock::now();
			tmp = FibRecurDP(x);
			end = chrono::high_resolution_clock::now();
			if(x == RECURDPSIZE)
				cout << "Dynamic Recursion: f(" << x << ") = " << tmp << endl;
			break;
		case 4:
			start = chrono::high_resolution_clock::now();
			tmp = FibMatrix(x);
			end = chrono::high_resolution_clock::now();
			if(x == MATRIXSIZE)
				cout << "Matrix: f(" << x << ") = " << tmp << endl;
			break;
		default:
			cout << "Invalid selection" << endl;
		break;

	}

	//Write the data to the file to be used for plotting.
	// Use BITS to write the minimum number of bits needed to represent the input as the
	// x axis for the data in the file, otherwise just write the inputer directly.
	if(BITS){
		f << bitsNeeded(x) << " " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << endl;
	}
	else{
		f << x << " " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << endl;
	}
	
	if(DEBUG){
		cout << x << " " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() << endl;
		cout << "Bits needed for " << x << " = " << bitsNeeded(x) << endl;
	}


}
