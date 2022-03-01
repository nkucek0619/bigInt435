/***
   prepared for CS435 Project 1 part 2

   - Sign a file with a name of your choice: ./messageDigest435 s [filename].[filetype]
   - Verify the file: ./messageDigest435 v [filename].[filetype] [filename].[filetype].signature
**/

#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "sha256.h"
#include "BigIntegerLibrary.hh"
 
int main(int argc, char *argv[])
{

    // demonstrating how sha256 works
    // program also generates an m file so that the second hash value can be compared to the encrypted signature
    try {

    if((argc != 3 || argv[1][0]!='s') && (argc !=4 || argv[1][0]!='v')) {
       std::cout << "wrong format! should be \"./messageDigest435 s [filename].[filetype]\" or " << 
       "\"./messageDigest435 v [filename].[filetype] [filename].[filetype].signature\"" << "\n";
       return 1;
    }

    if(argc == 3 && argv[1][0] == 's') {

        // create and read a new file to put random content into to test arbitrary data each run
        std::string filename = argv[2];
        std::ofstream file (filename.c_str(), std::ios::binary), writeSignatureFile;
        std::ifstream d_n("d_n.txt"), e_n("e_n.txt");
        file.open(filename);
        std::string input = std::to_string(rand());
        file << input << std::endl;
        // generate SHA-256 hash
        std::string firsthash = sha256(input);
        std::cout << "sha256('"<< input << "'): " << firsthash << "\n";

        // get d and n
        BigUnsignedInABase firstHashBaseTen(firsthash, 16);
        BigInteger m = BigInteger(firstHashBaseTen);
        std::cout << "\nm: " << m << "\n";
        std::string dstring, nstring, estring;
        d_n >> dstring;
        d_n >> nstring;
        e_n >> estring;
        BigUnsigned d = stringToBigUnsigned(dstring), n = stringToBigUnsigned(nstring), e = stringToBigUnsigned(estring);
        // verify d and n are being read in from external files
        std::cout << "\nd: " << d << "\n";
        std::cout << "\nn: " << n << "\n";
        std::cout << "\ne: " << e << "\n";

        // create file signature using private key d
        BigUnsigned signature = modexp(m, d, n);
        std::cout << "\nFile Signature: " << signature << "\n";
        std::cout << "\nSignature Bit Length: " << signature.bitLength() << "\n";

        // store the signature in a file
        writeSignatureFile.open("file.txt.signature");
        writeSignatureFile << signature;
        writeSignatureFile.close();
        std::cout << "\nGenerated signature file " << filename << ".signature\n";
        file.close();
        e_n.close();
        d_n.close();
    }

    if(argc == 4 && argv[1][0] == 'v') {

        // verify the signature using public key e and generated signature
        std::ifstream d_n("d_n.txt"), e_n("e_n.txt");
        std::string dstring, nstring, estring, input = std::to_string(rand()), secondhash = sha256(input), stringFileSignature;
        std::cout << "New Hash Value: " << secondhash << "\n";
        d_n >> dstring;
        d_n >> nstring;
        e_n >> estring;
        BigUnsigned d = stringToBigUnsigned(dstring), n = stringToBigUnsigned(nstring), e = stringToBigUnsigned(estring);
        std::string filename = argv[2];
        std::ifstream readSignatureFile(filename+".signature");
        readSignatureFile >> stringFileSignature;
        BigInteger fileSignature = stringToBigInteger(stringFileSignature), bigIntegerSignature = BigInteger(fileSignature);
        BigUnsignedInABase secondHashBaseTen(secondhash, 16);
        BigUnsigned m = modexp(bigIntegerSignature, e, n), mPrime = secondHashBaseTen;
        std::cout << "\nm: " << m << "\n";
        std::cout << "\nmPrime: " << mPrime << "\n";
        if (m == mPrime) std::cout << "Authentic file\n";
        else std::cout << "Modified file\n";
        readSignatureFile.close();
        e_n.close();
        d_n.close();
    }
    } catch (char const* err) {
        std::cout << "The library threw an exception:\n"
            << err << std::endl;
    }

    return 0;
}