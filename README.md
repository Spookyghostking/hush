### Hush
Trying to learn c so here is a hashtable implementation.

Yesterday I experienced my first segfault when I tried to set a value to a 
pointer without allocating memory first. It was really intriguing.
I am compelled to make more segfaults in the future. (Of course, only the ones 
made unintentionally are interesting :)

Implementation takes inspiration from [this video](https://www.youtube.com/watch?v=n-S9DBwPGTo)

### What?
This project is an implementation of a hashtable/hashmap in c.
It is applied to the problem of counting words in a text file.
When ran it will read the Shakespeare.txt file and count the numer of times each word appears in the text and output the result into a file named out.txt.

### How to use
The implemantation currently (and probably in the future too) is only made to work on windows.

To run the code, simply use a C compiler of your choosing to compile the hush.c file. For example:

    gcc -o hush hush.c

Then run the resulting binary.


