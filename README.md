
## Description

This utility implements a simple word counter for text files. The words to be 
counted are selected from a list contained in the file "dictfile" which must be 
present on the current directory. Only the first word of each line in the file 
is parsed by the program, all additional words are neglected.
In order to have an almost constant search time, the dictionary words are stored 
on a hash table which is implemented using the standard chaining scheme, 
i.e. as an array of linked lists.  

## Compilation

For compiling simply type:

```
make
```

This command uses by default the GNU gcc compiler and compiles with full 
optimization options (-O3). The program has also been tested with the Intel icc 
compiler. It is likely not working on Windows. In order to test the correctness 
of the program with the input files test/*.txt use:

```
make check
```

The results of the test are written on the file test/CHECK.output
In order to print out additional information on the program execution and 
turn on of debugging symbols, the program can be compiled in debug mode with:

```
make debug
```

## Usage

Assuming that a text file "./dictfile" containing the dictionary is present 
in the working directory, use the word counter as follows:

```
./wordcount file1.txt file2.txt ...
```

An unlimited concatenation of input files can be given to the program. 
Otherwise standard input can also be used: 

```
./wordcount < file.txt
```

or simply past the text after running the program and use `CTRL+D` when 
the text is finished.

The output on stdout has the following format:

N1  \t word1
N2  \t word2
...
NN  \t wordN
NN1 \t total dictionary words
NN2 \t total words in the text


## Limitations

- for speeding up calculations a builtin compiler function has been used. This is 
likely not working with the Windows compiler (not tested). The actual performance 
improvement derived by the use of this builtin has not been tested carefully and should 
be relevant only if the number of collisions in the hash table is very low.

- the function for tokenizing the text uses a set of common word delimiters. The use of 
these delimiters hampers from counting words containing one or more characters present 
in the delimiters' set such as "o'clock" or "clock-maker".

- the output of the program is not ordered aphabetically, but depending on the hash 
value. It would be better to have an ordered output for readability.

- a function for computing the current size of the hash table is missing.

- an appropriate rescaling of the hash table should be performed if the buckets are 
getting full. This rescaling involves also a rehshing of the keys. 
