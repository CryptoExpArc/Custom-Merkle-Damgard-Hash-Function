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

using namespace std;

unsigned long adler32(const char* buf, unsigned int len);

// create a global map to store the diamond tree
map<int, string> history;
map<int, string> tree_history(map<int, string>& diamond_level);

/**
 * Hashes the given array of bytes using a Merkle-Damgard construction.
 *
 * The message should be given as a std::string.  It will be split into 64-bit
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

// Phase1: Find final hash by diamond strcuture

    // Before binary search, the searched array or map should in order,
    // so I am not sure about the binary search.
    // Additionally, there is a problem in message input.
    // If the length of message is not 2^n, the diamond structure is failed.
    // If the length of message is 2^n and after hashed the message, 
    // the result always 10 characters number
    // eg. if input "hell", there are 4 characters, the map will be:
    // The map(size: 4) we got:
    // key: 0, value: 2719290290
    // key: 1, value: 2719290290
    // key: 2, value: 2719290290
    // key: 3, value: 2719290290
    // All the value are same and the value can not be sorted
    // But I got decreasing diamond level
    // uint32: 0 to 2^32-1(4294967295)

// create a map to store hash value
// create a map for first diamond level
// not sure about the size of map (ie. index/key)
map<int, string> map_create(string message, int index){
    map<int, string> hash_value;
    if (index == 0){
        std::cout << "The map is zero." << endl;
        // exit;
    } else {
        for (int i = 0; i < index; i++){
            hash_value.insert(
                map<int, string>::value_type(
                    i, to_string(hashString(message))
                )
            );
        }
    }

    return hash_value;
}

// map<int, string> map_create(string message, int index){
//     map<int, string> hash_value;
//     hash_value.insert(
//         map<int, string>::value_type(
//             index, to_string(hashString(message))
//         )
//     );

//     return hash_value;
// }

// test
void print_map(map<int, string>& hash_value){
    int map_size = hash_value.size();
    std::cout << "The map(size: " << map_size << ") we got: " << endl;
    for (int i = 0; i < map_size; i++){
        std::cout << "key: " << i << ", value: " << hash_value[i] << endl;
    }

    return;
}

// before use binary search, the value of map should in order.
// use quick sort
void sort(string* tempArray, int start, int end){
    // int start = 0;
    // int end = map_size-1;
    if (start < end){
        int pi;
        int index = 0;
        string flag = tempArray[end];
        int flag_index;

        string* next_array = new string[end-start+1];

        for (int i = start; i < end+1; i++){
            if (tempArray[i] < flag){
                next_array[index] = tempArray[i];
                index ++;
            }
        }

        next_array[index] = flag;
        index ++;

        for (int i = start; i < end; i++){
            if (tempArray[i] > flag){
                next_array[index] = tempArray[i];
                index ++;
            }
        }

        index = 0;
        for (int i = start; i < end+1; i++){
            if (tempArray[i] == flag){
                flag_index = i;
            }
            tempArray[i] = next_array[index];
            index ++;
        }

        pi = flag_index;

        // cout << pi << endl;
        sort(tempArray, start, pi-1);
        sort(tempArray, pi+1, end);
    }

    return;
}

map<int, string> quickSort(map<int, string>& hash_value){
	// put the value of map into array
    int map_size = hash_value.size();
    string tempArray[map_size] = {""};
    for (int i = 0; i < map_size; i++){
        string temp = hash_value[i];
        tempArray[i] = temp;
    }
    
    // quick sort the array
    sort(tempArray, 0, map_size-1);

    // return array value to map
    map<int, string> sorted_map;
    for (int i = 0; i < map_size; i++){
        string temp1 = tempArray[i];
        sorted_map[i] = temp1;
    }
    // print out sorted map for test
    cout << "These are sorted map: " << endl;
    for (int i = 0; i < map_size; i++){
        std::cout << "key: " << i << ", value: " << sorted_map[i] << endl;
    }

    return sorted_map;
}

// map<int, string> quickSort(map<int, string>& hash_value){
// 	// put the value of map into array
//     int map_size = hash_value.size();
//     string tempArray[map_size] = "";
//     for (int i = 0; i < map_size; i++){
//         string temp = hash_value[i];
//         tempArray[i] = temp;
//     }

// }

// binary search the value of map and create each level of diamond
map<int, string> binary_search_then_create_new_map_as_diamond_level(map<int, string>& hash_value){
    cout << endl;
    cout << "----------- We are at binary_search_then_create_new_map_as_diamond_level function: --------------" << endl;

    cout << "Start binary_search_then_create_new_map_as_diamond_level" << endl;
    cout << "Print the diamond level that needs to be operated: " << endl;
    print_map(hash_value);

    // create new_map to store 
    map<int, string> new_map;

    // previous_map_size is the size of operated diamond level
    // test: if previous_map_size == 2, don't search; if > 2, search
    int previous_map_size = hash_value.size();
    if (previous_map_size == 2){
        std::cout << "The size of map is 2, just compare these two hash value." << endl;
        if (hash_value[0] == hash_value[1]){
            new_map.insert(
                map<int, string>::value_type(
                    0, hash_value[0]
                )
            );
            std::cout << "Binary search should stop here, they collision." << endl;
            // return new_map;

        } else {
            std::cout << "These two not match." << endl;
            // return hash_value;
        }
    // end (previous_map_size == 2)
    // start (previous_map_size > 2)
    } else {
        // start binary search
        // search round 1, 
        // find h1 collision with hi, search range from h1 to end, start at mid
        // if found, eg. h1 collision h3, 
        // swap h2 value and h3 value, s.t. h1 == h2, h3 is another value
        // search round 2, 
        // find h3 collision with hj, search range from h3 to end, start at mid
        // etc. until finish search
        int new_key = 0;
        for (int key = 0; key < previous_map_size; key +=2){
            string value_to_find = hash_value[key];
            int low = key;
            int high = previous_map_size-1;
            std::cout << "key: " << key << ", low: " << low << ", high: " << high << endl;
            // break;
            if (previous_map_size == 2 && hash_value[0] == hash_value[1]){
                std::cout << "Binary search should stop here.*********************************************" << endl;
                std::cout << "previous_map_size: " << previous_map_size << endl;
                new_map.insert(
                    map<int, string>::value_type(
                        0, hash_value[0]
                    )
                );
                std::cout << "Binary search should stop here." << endl;
                // return new_map;
                break;
            } else {
                while (low <= high) {
                    int mid = low + (high-low)/ 2;
                    std::cout << "mid: " << mid << endl;
                    if (value_to_find == hash_value[mid]){
                        new_map.insert(
                            map<int, string>::value_type(
                                new_key, hash_value[mid]
                            )
                        );
                        new_key ++;
                        std::cout << "new_key: " << new_key << endl;
                        std::cout << "print current new_map: "<< endl;
                        print_map(new_map);
                        // cout << "----------------------------------------------------------------" << endl;
                        // swap hash_value[mid] and hash_value[key], 
                        // the key unchanged,
                        // search from hash_value[key+2] to end.
                        string temp = hash_value[mid];
                        string temp1 = hash_value[key];
                        hash_value[key+1] = temp;
                        hash_value[mid] = temp1;
                        std::cout << "Fund a collision." << endl;
                        std::cout << endl;
                        break;

                    } else if (value_to_find > hash_value[mid]){
                        low = mid + 1;
                    } else if (value_to_find < hash_value[mid]){
                        high = mid - 1;
                    }

                } // while loop end
            }

        }

        // std::cout << "Done binary search in this level." << endl;
        // std::cout << "print current new_map(ie. a level on diamond structure): "<< endl;
        // print_map(new_map);
        // std::cout << "binary_search_then_create_new_map_as_diamond_level function end and return new_map, go to diamond_tree function." << endl;
        // std::cout << endl;
        // std::cout << endl;
        // return new_map;
    }

    std::cout << "Done binary search in this level." << endl;
    std::cout << "print current new_map(ie. next level on diamond structure): "<< endl;
    print_map(new_map);
    // store the level
    cout << "Store it to diamond level history via tree_history function***********************************************" << endl;
    tree_history(new_map);

    cout << endl;
    std::cout << "binary_search_then_create_new_map_as_diamond_level function end in this level" << endl;
    std::cout << "return new_map and store next level to tree_history" << endl;

    cout << endl;
    std::cout << "go to the for loop of diamond_tree function." << endl;
    std::cout << endl;
    // std::cout << endl;

    return new_map;

}

// direct to use find() to find collision - haven't test
// O(n^2) not good
// map<int, string> search_then_create_new_map_as_diamond_level(map<int, string>& hash_value){
//     int new_key = 0;
//     map<int, string> new_map;
//     int map_size = hash_value.size();
//     for (int i = 0; i < map_size; i+=2){
//         for (int j = i; j < map_size; j++){
//             if (hash_value[i] == hash_value[j]){
//                 // found then insert to new_map
//                 new_map.insert(
//                     map<int, string>::value_type(
//                     new_key, hash_value[i]
//                     )
//                 );
//                 new_key ++;
//                 // swap the found hash and second hash
//                 // i start from third hash
//                 string temp = hash_value[i];
//                 string temp1 = hash_value[j];
//                 hash_value[i+1] = temp;
//                 hash_value[j] = temp1;
//                 std::cout << "Fund a collision." << endl;
//                 std::cout << endl;
//                 break;
//             } else {
//                 // not found then continue search
//                 search_then_create_new_map_as_diamond_level(hash_value);
//             }
//         }
        
//     }

//     return new_map;
// }

// create diamond structure
// output n = 32, diamond width k = (n-5)/3 = 9, suffix length (blocks) = 9+lg(9)+1?
string diamond_tree(string message){
    cout << endl;
    cout << "-------------------------------- We are at diamond_tree function: ------------------------------" << endl;

    string final_hash;

    int index;
    index = message.length();

    // create first level of diamond tree
    map<int, string> hash_value;
    hash_value = map_create(message, index);
    cout << "The first level of diamond tree: " << endl;
    print_map(hash_value);

    // save the first level into the diamond tree history
    tree_history(hash_value);
    
    // before binary search, the searched value should in order,
    // but there is a problem in message input,
    // if the length of message is not 2^n, the diamond structure is failed.
    // if the length of message is 2^n and after hashed the message, 
    // the result always 10 characters number
    // eg. if input "hell", there are 4 characters, the map will be:
    // The map(size: 4) we got:
    // key: 0, value: 2719290290
    // key: 1, value: 2719290290
    // key: 2, value: 2719290290
    // key: 3, value: 2719290290
    // the value can not be sorted
    // hash_value = quickSort(hash_value);

    // Use for loop to create other level until there is only one hash value
    // not sure about the diamond width k = 9?
        // test
        // map<int, string> resulting_map;
        // cout << "binary_search_then_create_new_map_as_diamond_level function start: " << endl;
        // resulting_map = binary_search_then_create_new_map_as_diamond_level(hash_value);
        // cout << "exit binary_search_then_create_new_map_as_diamond_level, print resulting_map" << endl;
        // print_map(resulting_map);
        // cout << "end print." << endl;
        // if (resulting_map.size() == 1){
        //     final_hash = resulting_map[0];
        // }

    int map_size = hash_value.size();
    
    std::cout << endl;
    std::cout << "Go to binary_search_then_create_new_map_as_diamond_level function." << endl;

    // cout << "map_size: " << map_size << endl;
    // k = log2(index)+1? 8 = 2^3, 3+1 = 4, 8 4 2 1, so 4 level/width;
    int left_diamond_width = log2(map_size); // the level expect first level
    for (int k = left_diamond_width; k > 0; k--){
        std::cout << endl;
        std::cout << "diamond width k = " << left_diamond_width + 1 << ", we are at level " << k << " ." << endl;

        map<int, string> resulting_map;
        resulting_map = binary_search_then_create_new_map_as_diamond_level(hash_value);

        std::cout << "test resulting_map.size(), ie. the level size of diamond tree where we are located" << endl;
        print_map(resulting_map);
        std::cout << "resulting_map.size(): " << resulting_map.size() 
            << ", for message: " << message << endl;
        cout << endl;

        if (resulting_map.size() == 1){
            cout << "Found!" << endl;
            final_hash = resulting_map[0];
        } else {
            std::cout << "The size of map is more than 1, "
                    << "ie. the element number of this level is more than 1,"
                    << " consider next k = " << k-1 << "level k: " << endl;
            std::cout << "----------------------------------------------------------------" << endl;
            std::cout << endl;
            // clear hash_value, including size or key
            hash_value.clear();
            hash_value = resulting_map;
        }
    }
    std::cout << "----------------------------------------------------------------" << endl;

    return final_hash;
}

// // create a global map to store the diamond tree
// map<int, string> history;
// map<vector<int>, string> history;

map<int, string> tree_history(map<int, string>& diamond_level){
    cout << endl;
    cout << "-------------------------------- We are at tree_history function: ------------------------------" << endl;

    int start = history.size();
    int elements_num = diamond_level.size();
    int end = start + elements_num;

    cout << "Saving this level......................................................" << endl;

    int j = 0;
    for (int i = start; i < end; i++){
        cout <<"tree_history index: start: " << start 
            << ", j: " << j 
            << ", end: " << end 
            << ", saved a level."<< endl;

        history.insert(
            map<int, string>::value_type(
                i, diamond_level[j])
        );
        j++;
        // cout << "j++: " << j << endl;
        // cout << "????????????????????????????????" << endl;

    }

    return history;
}
  
int n_ = 4;
vector<string> suffixes_all;
vector<string> suffixes_after_each_hash;

string get_suffixes_8_bytes(mt19937& engine){
    uniform_int_distribution<int> dist(11111111, 99999999);
    unsigned int rand = dist(engine);
    string Srand = to_string(rand);

    return Srand;
}

vector<string> get_suffixes_8_bytes(){
    random_device rd;
    unsigned int seed = rd();
    mt19937 engine(seed);
    
    for (int i = 0; i < pow(2, n_ / 2); i++){
        suffixes_after_each_hash.push_back(get_suffixes_8_bytes(engine));
    }
    
    return suffixes_after_each_hash;
}

/**
 * Examples of using the hash function.  The hashes for these messages
 * _should_ be 2732396895 and 2868974352.  If you get something else then
 * you should let me know (because the hash is meant to be deterministic and
 * well-defined regardless of what computer you are using).
 */
int main() {
    std::cout
        << hashString("qwertyuiop") << ' '
        << hashString("Good luck implementing the Nostradamus attack!")
        << std::endl;

    // cout << hashString("abcdefghijklmnopqrstuvwxyz1234567890abcdefghijklmnopqrstuvwxyz1234567890") << endl;
    
    string message;

    // message to binary; string to int: stoi(); int to binary: std::string str = std::bitset<8>(123).to_string();
    // get random number, 0 to 2^32 - 1, 32 bits
    // random_device rd;
    // unsigned int seed = rd();
    // mt19937 engine(seed);
    // unsigned int Srand = engine();
    // cout << "random number: " << Srand << endl;

    // // Srand to_binary_to_string
    // string to_binary_to_string;
    // to_binary_to_string = bitset<32>(Srand).to_string();
    // cout << "convert int random number to binary: "
    //     << to_binary_to_string << endl;
    
    // cin >> message;
    // message = "Hello World! Hello World! Hello!";
    // message = "Hell";
    // message = message + "suffixes";
    vector<string> message_all;

    // for (int i = 0; i < 4; i++){
    //     char c = 'a' + i;
    //     string str(8, c);
    //     message_all.push_back(str);
    // }

    for (int i = 0; i < 26; i++){
        char c = 'a' + i;
        string str(8, c);
        message_all.push_back(str);
    }
    
    for (int i = 0; i < 26; i++){
        char c = 'A' + i;
        string str(8, c);
        message_all.push_back(str);
    }

    for (int i = 0; i < 10; i++){
        char c = '0' + i;
        string str(8, c);
        message_all.push_back(str);
    }

    for (int i = 0; i < 2; i++){
        char c = '+' + i;
        string str(8, c);
        message_all.push_back(str);
    }

    cout << "There are " << message_all.size() << " message in total." << endl;

    get_suffixes_8_bytes();

    // copy suffixes_after_each_hash to suffixes_all

    cout << "Generated suffixes_after_each_hash:" << endl;
    for (int i = 0;  i < pow(2, n_ / 2); i++) {
        cout << suffixes_after_each_hash[i] << endl;
    }

    string published_hash = diamond_tree(message);
    std::cout << "exit diamond_tree function." << endl;

    std::cout << endl;
    std::cout << "The published hash is " << published_hash << endl;

    // int n = to_binary_to_string.size();
    // int n = 32;
    int n = message.size();
    cout << "The message: " << message 
        << " has " << message.size() << " characters." << endl;

    // cout << "Srand: " << Srand << endl;
    
    // diamond_width
    int k = log2(n);
    cout << "diamond_width k = log2(n) = " << k << endl;
    cout << "The diamond tree history: " << endl;
    cout << "The size of message is " <<  n 
        << ", so there are " << pow(2, k+1) - 1 << " value " 
        << "and " << k+1 << " level." << endl;
    // cout << "Each level start at " << endl;
    vector<int> level_start_at(k+1);
    vector<int> num_of_value(k+1);
    // int level_start_at[k+1];
    // int num_of_value[k+1];
    level_start_at[0] = 0;
    level_start_at[1] = n;
    num_of_value[0] = n;
    int index = 2;
    for (int i = 0; i < k; i++){
        num_of_value[index-1] = num_of_value[index-2]/2;
        level_start_at[index] = level_start_at[index-1]+(num_of_value[index-1]);
        index ++;
    }
    for (int i = 0; i < k+1; i++){
        cout << "diamond tree level " << i+1 
            << " start at(in map): " << level_start_at[i] 
            << " and has " << num_of_value[i] << " hash value." 
            << endl;
    }
    // cout << endl;
    print_map(history);
    
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
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#define BASE 65521L /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {s1 += (unsigned char)(buf[i]); s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

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
