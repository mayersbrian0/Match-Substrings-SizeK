#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <math.h>
#include <chrono>


/*
Function stores all substrings size k in a map
*/
void get_map(std::string const& str, int k, std::unordered_map<std::string, std::vector<int>>& mp) {
    int n = str.size();

    for (int i = 0; i < n-k-1; i++) {
        mp[str.substr(i,k)].push_back(i); //store start index of occuranc 
    }
}

/*
Match substrings size k
*/
std::vector<std::pair<int, int>> match_strings_size_k(std::string const& s1, std::string const& s2, int k) {
    if (s1.size() == 0 || s2.size() == 0) return {};
    int n = s1.size();
    std::vector<std::pair<int, int>> ans; //store indices

    std::unordered_map<std::string, std::vector<int>> mp; //hash table to store all occuracnes of a string
    get_map(s2, k, mp);


    //iterate over all size k substrings of s1
    for (int i = 0; i < n -k +1; i++) {
        //add any matching substrings
        if (mp.find(s1.substr(i, k)) != mp.end()) {
            for (auto idx : mp[s1.substr(i, k)]) {
                ans.push_back({i, idx});
            }
        }
    }

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
