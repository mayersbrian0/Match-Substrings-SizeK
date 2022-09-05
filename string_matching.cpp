#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <math.h>


/*
Rolling Hash Class
hash(string) ~ computes the first hash of a string s where s.size() is length of sliding window
add(char c)  ~ adds new char to the hash computation
remove(char c) ~ removes first char in the sliding window from the hash
The above two operations are used to compute the rolling hash 
*/
class Hash {
    //large prime number and salt
    private:
    unsigned int a = 256; 
    unsigned int p = 971;
    unsigned int power = 1;

    //need initial hash to compute rolling hash
    void print_error() {
        std::cout << "Hash value never initalized\n";
    }

    public:
    unsigned int hash_value = 0; //negative value indicates hash() was never called initally
    int k = 0;

    //constructors
    Hash() {}
    Hash(std::string s) {
        hash(s);
    }

    //compute hash of a string
    void hash(std::string& s) {
        if (s.size() == 0) return;
        unsigned int n = s.size();
        k = n; //k is the size of the sliding window 
        unsigned int computed_hash = 0;
        unsigned int power = 1;

        //compute pow(a, k-1) without overflow
        for (int i = 0; i < k-1; i++) {
            power = (power*a) % p;
        }
        this->power = power;

        //compute initial hash
        for (int i = 0; i < n; i++) {
            computed_hash = (computed_hash*a + s[i]) % p;
        }

        hash_value = computed_hash;
    }

    void update(char old, char next) {
        hash_value = ((hash_value+ (p*(a-1)) - old*(power % p)%p)*a + next)%p;
    }

};

//if the hashes match, then check if the two strings are the same
bool is_same(std::string const & s1, std::string const & s2) {
    //std::cout << s2 << "\n";
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) return false;
    }

    return true;
}


void match_strings(std::string const& s1, std::string const& s2, std::vector<std::pair<int, int>>& ret, int j) {
    if (s1.size() == 0 || s2.size() == 0) return;

    int k = s1.size();
    int n = s2.size();

    Hash goal_hash = Hash(s1); //this is our target value

    //Need an initial hash
    Hash hash = Hash(s2.substr(0, k));

    //find indicies that match substrings
    for (int i = 0; i < n-k+1; i++) {
        if (hash.hash_value == goal_hash.hash_value &&  is_same(s1, s2.substr(i,  k))) {
            std::cout << "IN\n";
            ret.push_back({j, i});
        }

        if (i < n -k) {
            hash.update(s2[i], s2[i+k]);
        }
    }

} 

std::vector<std::pair<int, int>> match_strings_size_k(std::string const& s1, std::string const& s2, int k) {
    if (s1.size() == 0 || s2.size() == 0) return {};
    int n = s1.size();
    std::vector<std::pair<int, int>> ans; //store indices

    //iterate over all size k substrings of s1
    for (int i = 0; i < n -k +1; i++) {
        match_strings(s1.substr(i, k), s2, ans, i);
    }

    return ans;
}



void file_to_string(std::string filename, std::string& file) {
    std::ifstream infile;
    std::stringstream buffer;
    std::string temp;

    //open file1
    infile.open(filename);
    if (infile.is_open()) {
        buffer << infile.rdbuf();

        file = buffer.str();
    }

    else {
        std::cout << "Invalid file name\n";
        exit(0);
    }

    
    std::stringstream parse(file);
    file = "";
 
    while (getline(parse, temp, '\n')) {
        file += temp; 
    }

    infile.close();
}

int main(int argc, char** argv) {
    if (argc != 4) return -1;

    //store values in strings
    int k = atoi(argv[1]);
    std::cout << k << "\n";
    std::string file1;
    std::string file2;

    file_to_string(argv[2], file1);
    file_to_string(argv[3], file2);

    std::cout << "Size of File 1: " << file1.size() << "\n";
    std::cout << "Size of File 2: " << file2.size() << "\n";


    std::vector<std::pair<int, int>> ans = match_strings_size_k(file1, file2, k);
    std::cout << "Number of matches: " << ans.size();
}
