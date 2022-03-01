// You need to complete this program for the first part of your first project.
/* Fermat's Little Theorem Formula: a^p = a mod p OR a^(p-1) = 1 mod p
      Fermat's little theorem states that if p is a prime number, then for any integer a, the number a^p − a is an integer multiple of p.
      Euclidean Algorithm: an efficient method for computing the greatest common divisor (GCD) of two integers (numbers),
      the largest number that divides them both without a remainder.
      The Euclidean Algorithm for finding GCD(A,B) is as follows:
      - If A = 0 then GCD(A,B)=B, since the GCD(0,B)=B, and we can stop.  
      - If B = 0 then GCD(A,B)=A, since the GCD(A,0)=A, and we can stop.  
      - Write A in quotient remainder form (A = B⋅Q + R)
      - Find GCD(B,R) using the Euclidean Algorithm since GCD(A,B) = GCD(B,R)*/

// Standard libraries
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h> 
#include <assert.h>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

// function prototypes
// calculates Φn
BigUnsigned findOn(BigUnsigned p, BigUnsigned q);
// creates a random large value that is possibly prime
BigUnsigned generateBigValue(int b);
// generates new big values until a new value is prime
BigUnsigned producePrimeValue(int b);
// uses Fermat's Primality Test to assert that the big value is prime
bool primeTest(BigUnsigned e, BigUnsigned prime);
// finds decryption value using encryption value and phi (Φn)
BigUnsigned findD(BigUnsigned &encryption, BigUnsigned On);

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */

	try {
      std::cout << "a couple of test cases for 3460:435/535 Algorithms!!!\n";
      
      // file objects to write p, q, e, d, and n to
      std::ofstream p_q, e_n, d_n;
      p_q.open("p_q.txt");
      e_n.open("e_n.txt");
      d_n.open("d_n.txt");

      // big1
      BigUnsigned big1 = producePrimeValue(512);
      std::cout << "my big1 (p) !!!\n" << big1;
      std::cout << "\nbig1 Bit Length: " << big1.bitLength()-1;

      //write p and q to text file
      p_q << big1 << std::endl;

      // assert that big1 is prime with a test value
      // if function doesn't produce prime value, run it until it does (successful about 2/3 runs on average)
      assert(primeTest(big1-1, big1));

      // big2
      BigUnsigned big2 = producePrimeValue(512);
      // change big2 so it does not equal big1
      //while(primeTest(big2-1, big2) != 1) big2-=rand();
      std::cout << "\nmy big2 (q) !!!\n" << big2;
      std::cout << "\nbig2 Bit Length: " << big2.bitLength()-1;
      p_q << big2;

      assert(primeTest(big2-1, big2));

      std::cout << "\nmy big3 = big1*big2 (n, or p*q) !!!\n";
      BigUnsigned big3 = big1*big2;
      std::cout << big3;

      // Extended Euclidean Algorithm
      std::cout << "\n\n***Implementing Extended Euclidean Algorithm***\n\n";

      // calculate e, d, x, and phi (or Φ(n))
      BigUnsigned On = findOn(big1, big2), e;
      std::cout << "\n\nThis part hangs for a couple of seconds...\n\n";
      BigUnsigned d = findD(e, On);
      std::cout << "\n\nBAM!!!!!\n\n";

      // test to assert that the greatest common divisor of e and Φn is 1, which is equivalent to the following:
      // a*x + b*x = gcd(a, b) = 1, which is also equivalent to e*x + phi(n)*y = gcd(e, phi(n)) = 1
      assert(gcd(e, On) == 1 && modinv(e, On) == d);

      // test to assert that EEA produces proper encryption and decryption values
      BigUnsigned m(23), n = big3;
      BigUnsigned encryption = modexp(m, e, n);
      BigUnsigned decryption = modexp(encryption, d, n);
      std::cout << "\n\ne & d: " << e << ", " << d << "\n\n";
      std::cout << "\n\nMessage: " << m << "\n\nEncryption Value: " << encryption << "\n\nDecryption Value: " << decryption << "\n\n";
      assert(decryption == m);
      n = big3;

      // write e, d, and n to text files
      e_n << e << std::endl << n;
      d_n << d << std::endl << n;

      std::cout << "\nmy big3/big2 !!!\n";
      std::cout << big3/big2;
      
      std::cout <<"\nABC\n";
      BigUnsignedInABase abc = BigUnsignedInABase("ABC", 16);
      std::cout <<"\n***********\n";
      std::cout <<std::string(abc)<<"\n";
      BigUnsigned abc10 = abc;
      std::cout <<abc10<<"\n";

      // close the text files
      p_q.close();
      e_n.close();
      d_n.close();
      
	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}

BigUnsigned findOn(BigUnsigned p, BigUnsigned q) {
   BigUnsigned On = (p-1)*(q-1);
   return On;
}

// creates a random large value that is possibly prime
BigUnsigned generateBigValue(int b) {
   BigUnsigned o;
   o.setBit(0, 1);
   for(int i = 1; i < b; i++) {
      bool rb = rand() % 2;
      o.setBit(i, rb);
   }
   o.setBit(512, 1);
   return o;
}

// generates new big values until a new value is prime
BigUnsigned producePrimeValue(int b) {
   for(;;) {
      BigUnsigned prime(generateBigValue(b));
      if (primeTest(prime-1, prime) == 1) {
         return prime;
      }
   }
}

// uses Fermat's Primality Test to assert that the big value is prime
bool primeTest(BigUnsigned e, BigUnsigned prime) {
   if(modexp(2, e, prime) == 1) {
      return true;
   }
   else {
      return false;
   }
}

// finds decryption value using encryption value and phi (Φn)
BigUnsigned findD(BigUnsigned &encryption, BigUnsigned On) {
   for(;;) {
      BigUnsigned x = producePrimeValue(13), y;
      if(modinv(x, On) != 0) {
         y = modinv(x, On);
         encryption = x;
         return y;
      }
   }
}
