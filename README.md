## Match Substrings Size k

This algorithm takes in two strings s1 and s2 and returns the indicies of all matching substrings with size k. Both algorithms have a function called match\_strings\_size\_k() which takes in two strings s1 and s2 and returns a vector of pairs of indicies representing the start index of two matching strings from s1 and s2.

Usage: ./a.out [k] [file of s1] [file of s2]

## inbuilt.cpp
This stores all of s2's substrings of size k and their start indicies in a hash table. After iterating over s2, the algorithm iterates over s1 and checks if the string is found in the hash table storing s2's substrings and start indicies.


## rolling\_match.cpp
This algorithm follows the same general approch to inbuilt.cpp but uses a custom hash table with a rolling hash function. The starting index of s2's substrings are stored in a array of vectors. Next, it iterates over s1 and computes the rolling hash of each substring s1 to index into the hash table. The substring of s1 is compared with each substring of s2 that has a starting index in the location of the hash table.
