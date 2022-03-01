# messageDigest435
## How to Run
1. Compile the messageDigest435.cc program using `make all`.
2. Program has three different commands:
<pre>
<b>./messageDigest435</b> // exits program immediately with an error message
// as user needs to enter a file name to be signed and verified
<b>./messageDigest435 s [filename].[filetype]</b> // prints hash value, e, d, n, and signature to the screen,
// and saves the signature to a text file
<b>./messageDigest v [filename].[filetype] [filename].[filetype].signature</b> // takes signature from text file
// and compares it to newly generated hash value from original file, checking if it is authentic or modified
</pre>
**Code will work with varying data, but for the simplicity of this project, the same rand() seed is passed in for each run.**\
**You may test the program with different data if you wish.**