#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <math.h>
#include <chrono>

//if the hashes match, then check if the two strings are the same
bool is_same(std::string const & s1, std::string const & s2) {
    //std::cout << s2 << "\n";
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) return false;
    }

    return true;
}

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
    long long a = 256; 
    long long p =  2946901;
    long long power = 1;
    long long m = 300000;


    //need initial hash to compute rolling hash
    void print_error() {
        std::cout << "Hash value never initalized\n";
    }

    public:
    long long hash_value = 0; //negative value indicates hash() was never called initally
    int k = 0;
    //std::vector<std::vector<int>> table;
     std::vector<int>* table;

    //constructors
    Hash() {}
    Hash(std::string s, int n) {
        if (n != 0) {
            table = new std::vector<int>[m];
        }
        hash(s);
    }


    //compute hash of a string
    void hash(std::string& s) {
        if (s.size() == 0) return;
        unsigned int n = s.size();
        k = n; //k is the size of the sliding window 
        unsigned int computed_hash = 0;
        long long power =1;

        //compute pow(a, k-1) without overflow
        for (int i = 0; i < k-1; i++) {
            power = (power*a) % p;
        }
        this->power = power;

        //compute initial hash
        for (int i = 0; i < n; i++) {
            computed_hash = (computed_hash*a + s[i]) %p;
        }

        hash_value = computed_hash;
    }

    //update hash value
    void update(char old, char next) {
        hash_value = ((a*(hash_value - old*power)) + next) % p;
    }

    /*
    Insert index into current hash location
    */
    void insert(int i) {
        if (hash_value < 0) table[-1*hash_value % m].push_back(i);
        else table[hash_value %m].push_back(i);
    }


    /*
    Search in the hash table for matches
    */
    void search(long long hash_val, int i, int k, std::string const & substr, std::string const & s2, std::vector<std::pair<int, int>>& ans) {
         if (hash_val < 0) hash_val *= -1;

        //iterate over possible indices
       for (auto idx : table[hash_val % m]) {
            if (is_same(substr, s2.substr(idx, k))) {
                ans.push_back({i, idx});
            }
        }

    }

};


/*
Finds computes the rolling hash of a string and stores a key-value pair of Rolling hash -> list of start indicies of the hash
*/
void compute_rolling_hash(int k, std::string const& str, Hash& hash) {
    if (str.size() == 0) return;

    int n = str.size();

    //find indicies that match substrings
    for (int i = 0; i < n-k+1; i++) {
        //std::cout << "String 2:" << str.substr(i, k) << " " << hash.hash_value % 100000 << "\n";
        hash.insert(i); //insert index to location
        if (i < n -k) {
            hash.update(str[i], str[i+k]);
        }
    }
} 

void get_all_pairs(int k, std::string const &s1, std::string const&s2, std::vector<std::pair<int, int>>& ans, Hash& hash) {
    if (s1.size() == 0) return;

    int n = s1.size();
    Hash curr_hash = Hash(s1.substr(0, k), 0);
    
    for (int i = 0; i < n-k+1; i++) {
        //if the other string hash the same value, check all potential substrings
        //std::cout << "String 1:" << s1.substr(i, k) << " " << curr_hash.hash_value << "\n";
        hash.search(curr_hash.hash_value, i, k, s1.substr(i,k), s2, ans); //search location for potential matches
        if (i < n -k) curr_hash.update(s1[i], s1[i+k]); //update hash
    }
}

/*
Match substrings size k
*/
std::vector<std::pair<int, int>> match_strings_size_k(std::string const& s1, std::string const& s2, int k) {
    if (s1.size() == 0 || s2.size() == 0) return {};

    std::vector<std::pair<int, int>> ans; //store indices

    Hash hash = Hash(s2.substr(0, k), 1);

    //first compute the rolling hash of window size k over the second string
    //store the indices where the rolling hash value occurs in table
    compute_rolling_hash(k, s2, hash);

    //pass in computed hash values on s2 and compare wiht rolling hash on s1
    get_all_pairs(k, s1, s2, ans, hash);
 
    return ans;
}

/*
Move text from file to string
*/
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
    std::string file1;
    std::string file2;

    file_to_string(argv[2], file1);
    file_to_string(argv[3], file2);

    std::cout << "Size of File 1: " << file1.size() << "\n";
    std::cout << "Size of File 2: " << file2.size() << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::pair<int, int>> ans = match_strings_size_k(file1, file2, k);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Time Take: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "Number of matches: " << ans.size() << std::endl;
}
