#include "mpi.h"
#include <iostream>
#include <string>
#include <math.h>
#include <iomanip>

int my_Function(int x);

using namespace std;

 typedef struct{
     string name;
     int age;
 }person;


int main(int argc, char *argv[]){

       int rank, size, ierr;
       float  u; // 1 byte
       int x,y,z; // 4 bytes
       int my_int = 88;
       char a,b;   //1 byte
       double d,v,h;   // 8 bytes
       string word = "Errors"; //8 bytes
       bool tf; // 1 byte
       signed int si;  //4 bytes
       unsigned int usi; // 4 bytes

       signed short int sshi = -32766;  // 2 bytes  ok
       unsigned short int ushi = 32766;  // 2 bytes  ok

	   double PI = 3.14159265;	
       int *test = 0; //  8 bytes
       int ret = 0;  //4 bytes  
       
       long double 		ld = sin(90.001 * PI/180);  //8 bytes
       double 			dbl = sin(270*PI/180);  //8 bytes

       unsigned long int uli = 4294967296;  //4 bytes
       signed long int sli = -2147483647; //4 bytes

       unsigned char uc = 0xED; //1 byte
       signed char sc = uc; //1 byte

      
       
       
       
       person myself;
       myself.name = "Test";
       myself.age = 60;
       
       x = 5;
       y = 8;
       z = x*y;
       
       a = 'n';
       b = 'c';
       
       d = 57.2301;
       v = 60.55;
       h = d+v;

       tf = false;

       u = 6.558;
       si =  -1 *  my_int; //-22;
       usi = my_int;



       if(x < y){
		  tf = false;
       }
       else{
		  tf = true;
       }


       test = &x;
       cout << endl << "Datattype" << setw(18) << "Value" << setw(18) << char(9) << "Sizeof" << endl;
       cout << "___________________________________________________________________" << endl;

       cout << endl << "Char = " << setw(18) <<  b << setw(27) <<  " Size = " << sizeof(b) << " Bytes" << endl;
       cout << "Unsigned char = " << char(9)  << (int)uc <<  setw(25) << " Size = " << sizeof(uc) << " Bytes" << endl;
       cout << "Signed char = " << setw(13) << (int)sc << setw(25)  << " Size = " << sizeof(sc) << " Bytes" << endl;
       cout << "Boolean = " << setw(15) << tf << setw(27) << " Size = " << sizeof(tf) << " Bytes" << endl << endl;
      
       cout << "Unsigned short int = " << char (9) << ushi << setw(23) << " Size = " << sizeof(ushi) << " Bytes" << endl;
       cout << "Signed short int =  " << char (9) << sshi << setw(22) << " Size = " << sizeof(sshi) << " Bytes" << endl << endl;

       cout << "Integer = " << setw(15) << x << setw(27) <<" Size = " << sizeof(x) << " Bytes" << endl;
       cout << "Signed int = " << setw(14) << si << setw(25) << " Size = " << sizeof(si) << " Bytes" << endl;
       cout << "Unsigned int = " << setw(11) << usi <<  setw(26) << " Size = " << sizeof(usi) << " Bytes" << endl;
       cout << "Int Multiplication = " << char (9) << z << setw(27) << " Size = " << sizeof(z) << " Bytes" << endl;
       cout << "Float = " << setw(22) << u << setw(22)  << " Size = " << sizeof(u) << " Bytes" << endl;
       cout << "Struct member = " << char (9) << myself.age << setw(25) << " Size = " << sizeof(myself.age) << " Bytes" << endl << endl;

       cout << "Struct member = " << char (9) << myself.name << setw(24) << " Size = " << sizeof(myself.name) << " Bytes" << endl;
       cout << "String  = " << setw(21) << word <<  setw(21) << " Size = " << sizeof(word) << " Bytes" << endl;
       cout << "Pointer to int = " << char (9) << *test << setw(27) << " Size = " << sizeof(test) << " Bytes" << endl;
       cout << "Long double = " << setw(17) << ld <<  setw(21) << " Size = " << sizeof(ld) << " Bytes" << endl;
       cout << "Double = " << setw(22) << dbl << setw(21) << " Size = " << sizeof(dbl) << " Bytes" << endl;
       cout << "unsigned long int = " << char (9) << uli << setw(18) << " Size = " << sizeof(uli) << " Bytes" << endl;
       cout << "signed long int = " << char (9) << sli << setw(17) << " Size = " << sizeof(sli) << " Bytes" << endl << endl;


       int (*fun)(int);
       fun  = &my_Function;
       ret = fun(4);

       cout << "Function pointer returned: " << ret << " It should return: 6" << endl << endl;
       

       ierr = MPI_Init(&argc, &argv);
       if (ierr != MPI_SUCCESS) {
			   cout << stderr << "Error: MPI_init() failed: " << ierr;
               return(2);
       }

       ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
       if (ierr != MPI_SUCCESS) {
			   cout << stderr << "Error: MPI_init() failed: " << ierr;
               return(2);
       }

       ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);
       if (ierr != MPI_SUCCESS) {
			   cout << stderr << "Error: MPI_init() failed: " << ierr;
               return(2);
       }

       fprintf(stderr, "\nHello world!  I am rank %d of %d\n", rank, size);

       ierr = MPI_Finalize();
       if (ierr != MPI_SUCCESS) {
			   cout << stderr << "Error: MPI_init() failed: " << ierr;
               return(2);
       }
} 

int my_Function(int x){

  int y = 2;
  int z = x + y;
  cout << endl << "Call to function pointer received: " << x << " It should receive: 4" << endl;

  return z;
}
