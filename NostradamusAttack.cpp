// g++ NostradamusAttack.cpp -o NostradamusAttack
// .\NostradamusAttack

// #include <iostream>
// using namespace std;
 
// int main() 
// {
//     cout << "Hello, World!";
//     return 0;
// }

/**
 * Version 1.0: 14 March 2024, 5:20pm
 *
 * This should be usable with any C++ standard from C++11 onwards.
 * Let me know if you have trouble building or running it.
 *
 * - Ben Burton.
 */

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <chrono>
#include <random>
#include <bitset>
#include <vector>
#include <fstream>
#include <cassert>
#include <stdio.h>
#include <cstdlib>

using namespace std;

unsigned long adler32(const char* buf, unsigned int len);

// create a global map to store the diamond tree
map<int, string> history;
map<int, string> tree_history(map<int, string>& diamond_level);

struct pre_next_level {
    int key;
    string value;
};

/**
 * Hashes the given array of bytes using a Merkle-Damgard construction.
 *
 * The message should be given as a std::string.  It will be split into 64-bit
 * blocks (i.e., 8 character blocks), each of which is compressed into a 32-bit
 * hash using a toy compression function (in this case, the Adler-32 checksum).
 *
 * If you are hashing an entire message, you can just pass the entire message
 * in the first argument (message) - in this case, a default initialisation
 * vector (0xabba1234) will be used.
 *
 * If you have already hashed a prefix of the message and you wish to
 * extend this by hashing the remainder of the message (i.e., a suffix),
 * you should pass the suffix as the first argument (message), and the hash
 * of the prefix as the second argument (prevHash).
 *
 * The final hash will be returned as a 32-bit unsigned integer, of type
 * uint32_t.
 */
uint32_t hashString(std::string message, uint32_t prevHash = 0xabba1234) {
    if (message.length() == 0) {
        return prevHash;
    }
    if (message.length() < 8) {
        // We have fewer than 8 bytes remaining (i.e., 64 bits).
        // Pad on the right with zeroes to give exactly 8 bytes.
        message += std::string(8 - message.length(), char(0));
    }

    // Compress the first 64-bit block of the message (i.e., the first 8 bytes)
    // using our toy compression function (the Adler-32 checksum).
    uint32_t c = adler32(message.c_str(), 8);

    // Pass this hash along with the rest of the message back into the
    // next round of the Merkle-Damgard construction.
    return hashString(message.substr(8), c ^ prevHash);
}

// read na2.txt row by row then get first 8 bytes as P_i in phase 1
// Final hash: 2800816478
// # Blocks: 256
vector<string> read_txt_tree_get_first_8_bytes()
{
    fstream new_file;
    
    vector<string> P_i;

    new_file.open("na2.txt", ios::in); 
    
    // Checking whether the file is open.
    if (new_file.is_open()) { 
        string sa;
        // Read data from the file object and put it into a string.
        while (getline(new_file, sa)) {
            // Print the data of the string.
            // cout << sa << "\n";
            P_i.push_back(sa.substr(0, 8));
            
        }
        
        // Close the file object.
        new_file.close(); 
    }

    // print the first 8 bytes of each line
    // for (int i = 0; i < 256; i++){
    //     cout << P_i[i] << endl;
    // }

    return P_i;
}

// suffixes_not_random
string get_suffixes_8_bytes(int j){
    int num = 10000000 + j;

    return to_string(num);
}

// https://www.geeksforgeeks.org/program-generate-random-alphabets/
const int MAX = 26;
string get_random_8_bytes_char(){
    char alphabet[MAX] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',  
                          'o', 'p', 'q', 'r', 's', 't', 'u', 
                          'v', 'w', 'x', 'y', 'z' };

    // srand(time(NULL));
    string suffixes = "";
    for (int num_char = 0; num_char < 8; num_char++){
        suffixes = suffixes + alphabet[rand() % MAX];
    }

    // cout << suffixes << endl;

    return suffixes;
}

string combine_P_Q_not_random(int j)
{
    string PQ = "oliveralexander!";
    PQ = PQ + get_suffixes_8_bytes(j);

    return PQ;
}

string combine_P_Q_char()
{
    string PQ = "oliveralexander!";
    PQ = PQ + get_random_8_bytes_char();

    return PQ;
}

// hash value of "oliveralexander!" + "some 8 byte suffixes"
vector<string> h_v_PQ(){
    vector<string> PQ_hash;
    for (int i = 0; i < 64000000; i++){
        string PQ = combine_P_Q_not_random(i);
        string PQ_h = to_string(hashString(PQ));

        PQ_hash.push_back(PQ_h);
    }

    return PQ_hash;
}

// try to use random char as suffixes; has found match; 
// output should be:
/*
Has try all different suffixes in phase 2.
hash_phase_1_P_0 = 2816086146 not equal to hash_Phase_2_PQ_8000000 = 2759856438
hash_phase_1_P_0 = 2816086146 not equal to hash_Phase_2_PQ_16000000 = 2782531860
hash_phase_1_P_i = 2816086146 not match with all PQ, current i = 0 should be added, try other hash_phase_1_P_i
match, the hash value of PQ_12599 = 2775912706 is match with P_1 = 2775912706 in phase 1.
The corresponding P_1 is fwcepYyN
The corresponding PQ_12599 is oliveralexander!gjnmwira in pahse 2.
*/

/*
So, the second row of phase 1 tree is match with 12600th row of phase 2.
ie.
P_1 = fwcepYyNZyziCrq1CWxlfqws8jovwjxMoryLirjug1Q4SV3D7f7eav4ff53cjvlEB99633Cw
PQ_12599 = oliveralexander!gjnmwira

python code:
# start with 6f6c69766572616c6578616e64657221
PQ = b"oliveralexander!gjnmwira"
PQ.hex()

After use python to find the hex value of PQ_12599, the result is '6f6c69766572616c6578616e64657221676a6e6d77697261'
*/

// find hashString("oliveralexander!" + "some 8 byte suffixes") == hashString(P_i in phase 1)
void find_match_char(){
    // get P_i in phase 1
    vector<string> phase_1_tree_P_i_first_8_bytes;
    phase_1_tree_P_i_first_8_bytes = read_txt_tree_get_first_8_bytes();

    // hash P_i in phase 1 and store them in P_i.txt
    vector<string> hash_phase_1_P_i;
    // get hash value of P_i in phash 1
    fstream new_file_P_i;
    new_file_P_i.open("P_i.txt", ios::out);
    if (new_file_P_i.is_open()) {
        for (int i = 0; i < 256; i++){
            unsigned int tmp = hashString(phase_1_tree_P_i_first_8_bytes[i]);
            // cout << "unsigned int tmp: " << tmp << endl;
            // cout << "string tmp: " << to_string(tmp) << endl;
            hash_phase_1_P_i.push_back(to_string(tmp));
            // cout << hash_phase_1_P_i[i] << endl;
            new_file_P_i << hash_phase_1_P_i[i] << "\n"; // Inserting text.
        }
        new_file_P_i.close(); // Close the file object
    }

    // get hash value of PQ with different suffixes in phash 2 and store them in PQ_j.txt
    fstream new_file_PQ_char;
    // Open a file to perform a write operation using a file object.
    new_file_PQ_char.open("PQ_j_char.txt", ios::out); 
    vector<string> hash_Phase_2_PQ_char;
    vector<string> Phase_2_PQ_char;
    // Checking whether the file is open.
    if (new_file_PQ_char.is_open()) {
        for (int i = 0; i < 17000000; i++){
            string PQ = combine_P_Q_char();
            string PQ_h = to_string(hashString(PQ));

            Phase_2_PQ_char.push_back(PQ);
            hash_Phase_2_PQ_char.push_back(PQ_h);
            new_file_PQ_char << hash_Phase_2_PQ_char[i] << "\n"; // Inserting text.

            // if (i % 10000 == 0){
            //     cout << "Has tried " << i << " suffixes in phase 2." << endl;
            // }
        }
        cout << "Has try all different suffixes in phase 2." << endl;

        new_file_PQ_char.close(); // Close the file object.
    }

    // while true {
    //     generate random suffix
    //     test suffix against phase 1 tree
    //     if it works, output and finish
    //     j is just for console output "trying #3000 #4000 ...
        
    //     average #tries 2^24 roughly16000000"
    // }

    // find match
    int i = 0;
    int j = 0;
    while(hash_phase_1_P_i[i] != hash_Phase_2_PQ_char[j]){
        string temp_1 = hash_phase_1_P_i[i];
        // cout << "start while loop" << endl;
        if (j < 16999999){
            // cout << "not match, add j = " << j 
            // << ", try other hash_Phase_2_PQ = " << hash_Phase_2_PQ[j] << endl;
            j++;

            if (j%8000000 == 0){
                cout << "hash_phase_1_P_" << i << " = " << hash_phase_1_P_i[i] 
                << " not equal to hash_Phase_2_PQ_" << j << " = " << hash_Phase_2_PQ_char[j]
                << endl;
            }
        }
        if (j == 16999999 && i < 256){
            cout << "hash_phase_1_P_i = " << hash_phase_1_P_i[i] 
            << " not match with all PQ, current i = " << i 
            << " should be added, try other hash_phase_1_P_i" << endl;
            j = 0;
            i++;
        }
        if (i == 256){
            cout << "Can not find match?" << endl;
            break;
        }
        if (hash_phase_1_P_i[i] == hash_Phase_2_PQ_char[j]){
            cout << "match, the hash value of PQ_"<< j << " = " << hash_Phase_2_PQ_char[j]
            << " is match with P_" << i << " = " << hash_phase_1_P_i[i] 
            << " in phase 1." << endl;
            cout << "The corresponding P_" << i << " is " << phase_1_tree_P_i_first_8_bytes[i] << endl;
            cout << "The corresponding PQ_" << j << " is " << Phase_2_PQ_char[j] << " in pahse 2." << endl;
        }
    }

    return;
}

// use number as suffixes; has tried 64 million but still can not find match; not sure why
// find hashString("oliveralexander!" + "some 8 byte suffixes") == hashString(P_i in phase 1)
void find_match_not_random(){
    // get P_i in phase 1
    vector<string> phase_1_tree_P_i_first_8_bytes;
    phase_1_tree_P_i_first_8_bytes = read_txt_tree_get_first_8_bytes();

    // hash P_i in phase 1 and store them in P_i.txt
    vector<string> hash_phase_1_P_i;
    // get hash value of P_i in phash 1
    fstream new_file_P_i;
    new_file_P_i.open("P_i.txt", ios::out);
    if (new_file_P_i.is_open()) {
        for (int i = 0; i < 256; i++){
            unsigned int tmp = hashString(phase_1_tree_P_i_first_8_bytes[i]);
            // cout << "unsigned int tmp: " << tmp << endl;
            // cout << "string tmp: " << to_string(tmp) << endl;
            hash_phase_1_P_i.push_back(to_string(tmp));
            // cout << hash_phase_1_P_i[i] << endl;
            new_file_P_i << hash_phase_1_P_i[i] << "\n"; // Inserting text.
        }
        new_file_P_i.close(); // Close the file object
    }

    // get hash value of PQ with different suffixes in phash 2 and store them in PQ_j.txt
    fstream new_file;
    // Open a file to perform a write operation using a file object.
    new_file.open("PQ_j.txt", ios::out); 
    vector<string> hash_Phase_2_PQ;
    // Checking whether the file is open.
    if (new_file.is_open()) {
        for (int i = 0; i < 64000000; i++){
            string PQ = combine_P_Q_not_random(i);
            string PQ_h = to_string(hashString(PQ));

            hash_Phase_2_PQ.push_back(PQ_h);
            new_file << hash_Phase_2_PQ[i] << "\n"; // Inserting text.

            // if (i % 10000 == 0){
            //     cout << "Has tried " << i << " suffixes in phase 2." << endl;
            // }
        }
        cout << "Has try all different suffixes in phase 2." << endl;

        new_file.close(); // Close the file object.
    }

    // while true {
    //     generate random suffix
    //     test suffix against phase 1 tree
    //     if it works, output and finish
    //     j is just for console output "trying #3000 #4000 ...
        
    //     average #tries 2^24 roughly16000000"
    // }

    // find match
    int i = 0;
    int j = 0;
    while(hash_phase_1_P_i[i] != hash_Phase_2_PQ[j]){
        string temp_1 = hash_phase_1_P_i[i];
        // cout << "start while loop" << endl;
        if (j < 63999999){
            // cout << "not match, add j = " << j 
            // << ", try other hash_Phase_2_PQ = " << hash_Phase_2_PQ[j] << endl;
            j++;

            if (j%8000000 == 0){
                cout << "hash_phase_1_P_" << i << " = " << hash_phase_1_P_i[i] 
                << " not equal to hash_Phase_2_PQ_" << j << " = " << hash_Phase_2_PQ[j]
                << endl;
            }
        }
        if (j == 63999999 && i < 256){
            cout << "hash_phase_1_P_i = " << hash_phase_1_P_i[i] 
            << " not match with all PQ, current i = " << i 
            << " should be added, try other hash_phase_1_P_i" << endl;
            j = 0;
            i++;
        }
        if (i == 256){
            cout << "Can not find match?" << endl;
            break;
        }
        if (hash_phase_1_P_i[i] == hash_Phase_2_PQ[j]){
            cout << "match, the hash value of PQ_"<< j << " = " << hash_Phase_2_PQ[j]
            << "is match with P_" << i << " = " << hash_phase_1_P_i[i] 
            << "in phase 1." << endl;
        }
    }

    return;
}

// -------------------------------------------------------------------------

/**
 * Examples of using the hash function.  The hashes for these messages
 * _should_ be 2732396895 and 2868974352.  If you get something else then
 * you should let me know (because the hash is meant to be deterministic and
 * well-defined regardless of what computer you are using).
 */
int main() {
    // test
    std::cout
        << hashString("oliveralexander!") << " "
        << hashString("xJrkCWEFVn5ExmEyFtl0HAVjOC2cF8rzErluDcsyQUr2EPulauO37nGMHrjW0u6pDWRSq6gx")
        << std::endl;

    // try suffixes until h(PQ in phase 2) = h(P_i in phase 1)
    // find_match();
    // if match, this function will print out the index of P_i, ie. the value of i
    // the ith row of txt tree will be the answer

    find_match_char();

    return 0;
}

// -------------------------------------------------------------------------

/**
 * What follows is an implementation of the Adler-32 checksum, modified from
 * adler32.c as taken from the XMILL website at
 * https://homes.cs.washington.edu/~suciu/XMLTK/xmill/www/index.html .
 *
 * The original adler32.c is Copyright (C) 1995-1998 Mark Adler, and is
 * available under the following license:
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#define BASE 65521L /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {s1 += (unsigned char)(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

unsigned long adler32(const char* buf, unsigned int len) {
    unsigned long adler = 1;
    unsigned long s1 = adler & 0xffff;
    unsigned long s2 = (adler >> 16) & 0xffff;
    int k;

    if (buf == nullptr) return 1L;

    while (len > 0) {
        k = len < NMAX ? len : NMAX;
        len -= k;
        while (k >= 16) {
            DO16(buf);
            buf += 16;
            k -= 16;
        }
        if (k != 0) do {
            s1 += (unsigned char)(*buf++);
            s2 += s1;
        } while (--k);
        s1 %= BASE;
        s2 %= BASE;
    }
    return (s2 << 16) | s1;
}
