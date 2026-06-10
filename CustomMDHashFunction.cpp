// 
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <bitset> // to binary
#include <map>

using namespace std;

int total = 0;
int positive_effect = 0;
int effect_4 = 0;
vector<int> change_each_bit(12);

vector<int> record_2_MD_custom_hash_values_test_sub_avalanche_effect(string hash_value1, string hash_value2){
    bitset<12> mch1(hash_value1);
    bitset<12> mch2(hash_value2);
    bitset<12> mch3(string("000000000000"));

    for (int i = 0; i < 12; i++){
        mch3[i] = mch1[i] ^ mch2[i];
    }
    
    cout << "*****************************" << endl;
    string differ = mch3.to_string();
    cout << "find difference(xor): " << differ << endl;

    for(int k = 0; k < 12; k++){
        if (differ.substr(k, 1) == "1"){
            change_each_bit[k] = change_each_bit[k] + 1;
        
        }
    }

    // int num = 0;
    // for (int i = 0; i < differ.length(); i++){
    //     if (differ.substr(i, 1) == "1"){
    //         num++;
    //     }
    // }

    // float effect = float(num)/float(12);
    // // cout << "effect: " << effect << endl;

    // if (effect > 0.4){
    //     effect_4++;
    // }
    // if (effect > 0.5 | effect == 0.5){
    //     positive_effect++;
    // }

    // cout << "effect > 0.4: " << effect_4 << endl;
    // cout << "effect > 0.5: " << positive_effect << endl;

    return change_each_bit;
}

void avalanche_effect(){
    cout << "total test: " << total << endl;
    vector<float> effect(12);
    for (int i = 0; i < 12; i++) {
        effect[i] = float(change_each_bit[i])/float(total);
        cout << "The effect of bit " << i+1 << ": " << effect[i] << endl;
    }

    // cout << "effect > 0.5: " << positive_effect << endl;
    // float c = float(positive_effect)/float(total);
    // cout << "Tested " << total << " hash values, there are " << c << " of them change more than 50%." << endl;

    // cout << "effect > 0.4: " << effect_4 << endl;
    // float c_4 = float(effect_4)/float(total);
    // cout << "Tested " << total << " hash values, there are " << c_4 << " of them change more than 40%." << endl;

    return;
}

vector<string> split(string message){
    // cout << "start split message" << endl;

    // cout << "message.length(): " << message.length() << endl;

    int m_size = message.length()/12;
    int last_string_num = message.length()%12;
    if (last_string_num != 0){
        // add_0 = padding block?
        string add_0(12 - last_string_num, '0');
        message += add_0;
        m_size ++;
    }
    // cout << "m_size: " << m_size << endl;

    vector<string> splited_m;

    for (int i = 0; i < m_size; i++){
        splited_m.push_back(message.substr(i*12, 12));
    }

    // cout << "Splited elements: " << endl;
    // for (int i = 0; i < m_size; i++){
    //     cout << splited_m[i] << endl;
    // }

    return splited_m;
}

// the H in question?
// non-trivial
// https://www.movable-type.co.uk/scripts/sha256.html
string customHash(string temp, string last_result){
    // 2^24
    string input = temp + last_result;
    // cout << temp << " + " << last_result << endl;
    // 8388 6070 0000 0101 0101 0101
    // cout << "input: " << input << endl;

    // non-linear map -> S-box - pi from MATH3302 Assignment 2 Q4
    // Π = 1, c, b, 9, 6, a, 3, e, d, 4, 8, 5, f, 7, 2, 0
    // input  = 0,  1,  2, 3, 4, 5,  6,  7,  8, 9, 10, 11, 12, 13, 14, 15
    // output = 1, 12, 11, 9, 6, 10, 3, 14, 13, 4,  8,  5, 15,  7,  2,  0
    int S_box[16] = {1, 12, 11, 9, 6, 10, 3, 14, 13, 4, 8, 5, 15, 7, 2, 0};

    // cout << "-----------------------------" << endl;
    // cout << "input: " << input << endl;
    
    // 24-bits input, operate 4-bits each time, need opearate 6 times in total
    string after_S_box = "";
    // for (int i = 0; i < 6; i++){
    for (int i = 0; i < 24; i+=4){
        // cout << "-----------------------------" << endl;
        // cout << "input: " << input << endl;
        // select 4 bits of input
        string temp_select = "";
        temp_select = input.substr(i, 4);
        // cout << "input.substr(" << i << ", 4): " << temp_select << endl;
        bitset<4> binary_4_bit(temp_select);
        // cout << temp_select << endl;
        // compute the 4 binary bits and trans to decimal number并且把它作为S_box的索引
        int index = binary_4_bit.to_ulong();
        // according to S_box, 
        int tihuandenumber = S_box[index];
        // 将替换后的数转为二进制
        uint32_t binary_temp = static_cast<uint32_t>(tihuandenumber);
        // 将替换后的数转为string
        bitset<4> bt(binary_temp);
        // cout << "change to " << binary_temp << " : " << bt.to_string()<< endl;

        after_S_box = after_S_box + bt.to_string();
    }
    bitset<24> bt_24(after_S_box);
    int dec_bt_24 = bt_24.to_ulong();
    // cout << "after_S_box: " << after_S_box << " : " << dec_bt_24 << ", dec_bt_24%3: " << dec_bt_24%3 << endl;

    bitset<24> after_S_box_bit(after_S_box);
    string hash_value_24 = "";
    for (int j = 0; j < 24; j++){
        bitset<12> obj(string("101010101010"));
        // bitset<2> output(string("00"));
        // cout << bt[1] << " " << bt[2] << endl;
        // cout << obj[0] << endl;

        // for (int j = 0; j < 6; j++){
            // bt[0] = ((bt[3]*5 + bt[1]*11 + obj[i+4] + 3)*19 + (bt[0] << 3 | bt[0] >> 7))%2;
            // bt[1] = ((bt[0]*7 + bt[2]*13 + obj[i+1] + 5)*17 + (bt[0] << 2 | bt[0] >> 11))%2;
            // bt[2] = ((bt[1]*9 + bt[3]*17 + obj[i+2] + 7)*13 + (bt[0] << 5 | bt[0] >> 13))%2;
            // bt[3] = ((bt[2]*3 + bt[0]*19 + obj[i+3] + 9)*11 + (bt[0] << 1 | bt[0] >> 17))%2;

            if (dec_bt_24%3 == 0){
                after_S_box_bit[j] = ((after_S_box_bit[(j+37)%24] + after_S_box_bit[(j+73)%24]*11 + obj[(j+13)%12] + 3)*19)%2;
            }
            
            if (dec_bt_24%3 == 1){
                after_S_box_bit[j] = ((after_S_box_bit[(j+59)%24] + after_S_box_bit[(j+89)%24]*13 + obj[(j+11)%12] + 5)*17)%2;
            }
            
            if (dec_bt_24%3 == 2){
                after_S_box_bit[j] = ((after_S_box_bit[(j+61)%24] + after_S_box_bit[(j+41)%24]*17 + obj[(j+17)%12] + 7)*13)%2;
            }

            // after_S_box_bit[j+3] = ((after_S_box_bit[(j+43)%24] + after_S_box_bit[(j+71)%24]*19 + obj[(j+19)%12] + 9)*12)%2;

            // bt[0] = (bt[3]*5 + bt[1]*11 + obj[i+4] + 3)*19 + (bt[0] << 3 | bt[0] >> 1);
            // bt[1] = (bt[0]*7 + bt[2]*13 + obj[i+1] + 5)*17 + (bt[0] << 3 | bt[0] >> 1);
            // bt[2] = (bt[1]*9 + bt[3]*17 + obj[i+2] + 7)*13 + (bt[0] << 3 | bt[0] >> 1);
            // bt[3] = (bt[2]*3 + bt[0]*19 + obj[i+3] + 9)*11 + (bt[0] << 3 | bt[0] >> 1);

            // cout << bt[0] << endl;
            // cout << bt[1] << endl;
            // cout << bt[2] << endl;
            // cout << bt[3] << endl;

            // output[1] = after_S_box_bit[1] ^ after_S_box_bit[3] ^ obj[1];
            // output[0] = after_S_box_bit[0] ^ after_S_box_bit[2] ^ obj[0];

            // output = output << i;
        // }
        
    }
    
    hash_value_24 = hash_value_24 + after_S_box_bit.to_string();
    int dec_after_S_box_bit = after_S_box_bit.to_ulong();
    // cout << "custom hash_value_24: " << hash_value_24 << " : " << dec_after_S_box_bit << ", dec_after_S_box_bit%2: " << dec_after_S_box_bit%2 << endl;
    
    // add_customHash_round
    // hash_value = customHash(hash_value, last_result);

    string hash_value_12 = "";
    if (dec_after_S_box_bit%2 == 0){
        hash_value_12 = hash_value_24.substr(0, 12);
    }
    else{
        hash_value_12 = hash_value_24.substr(12, 12);
    }
    // cout << "custom hash_value_12: " << hash_value_12 << endl;

    return hash_value_12;
}

string MD_H(string m){
    // 2^12
    string iv = "010101010101";
    // cout << "MD_H message: " << m << endl;
    vector<string> splited_m = split(m);
    int MD_round = splited_m.size();
    // cout << "The MD round: " << MD_round << endl;

    string temp_result;
    string final_result;

    for (int i = 0; i < MD_round; i++){
        string temp_m = splited_m[i];
        // cout << "i " << i << " : " << temp_m << endl;
        if (i == 0){
            // cout << "iv" << endl;
            temp_result = customHash(temp_m, iv);
            
        }
        else if (i != 0 && i < MD_round - 1){
            temp_result = customHash(temp_m, temp_result);
            
        }
        else {
            // cout << "PB" << endl;
            // string PB = "100000000000";
            // temp_m = temp_m + PB; // splited_m || PB
            temp_result = customHash(temp_m, temp_result);
            
            final_result = temp_result;  
        }
        
    }

    cout << "MD custom hash value: " << final_result << endl;

    return final_result;
}

string MD_H_add_test(string m){
    // 2^12
    string iv = "010101010101";
    // cout << "MD_H message: " << m << endl;
    vector<string> splited_m = split(m);
    int MD_round = splited_m.size();
    // cout << "The MD round: " << MD_round << endl;

    string temp_result;
    string final_result;

    for (int i = 0; i < MD_round; i++){
        string temp_m = splited_m[i];
        // cout << "i " << i << " : " << temp_m << endl;
        if (i == 0){
            // cout << "iv" << endl;
            temp_result = customHash(temp_m, iv);
            // add round?
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
            temp_result = customHash(temp_m, iv);
        }
        else if (i != 0 && i < MD_round - 1){
            temp_result = customHash(temp_m, temp_result);
            // add round?
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
        }
        else {
            // cout << "PB" << endl;
            // string PB = "100000000000";
            // temp_m = temp_m + PB; // splited_m || PB
            temp_result = customHash(temp_m, temp_result);
            // add round?
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            temp_result = customHash(temp_m, temp_result);
            final_result = temp_result;  
        }
        
    }

    cout << "MD custom hash value: " << final_result << endl;

    return final_result;
}

void collision_avalanche_effect_test(){
    cout << endl;

    // input 2^24
    int start = pow(2, 23) + 1;
    int end = pow(2, 23) + 10000;
    string message_previous = "";
    string message_second = "";
    int j = 0;
    for (int i = start; i < end; i++){
        bitset<24> i_bit(i);
        cout << "Try message to find collision: " << i_bit.to_string() << endl;
        string message2_hashed = MD_H_add_test(i_bit.to_string());

        // cout << "*****************j********************: " << j << endl;
        // test avalanche effect
        if (j == 0){
            message_previous = message2_hashed;
            message_second = message_second;
            cout << j << ": get the first hash value  " << message_previous << endl;
            cout << j << ": get the second hash value " << message_second << endl;
            if (message_second.length() != 0){
                record_2_MD_custom_hash_values_test_sub_avalanche_effect(message_previous, message_second);
            }
        }
        if (j == 1){
            message_previous = message_previous;
            message_second = message2_hashed;
            cout << j << ": get the first hash value  " << message_previous << endl;
            cout << j << ": get the second hash value " << message_second << endl;
            record_2_MD_custom_hash_values_test_sub_avalanche_effect(message_previous, message_second);
        }

        if (j == 2){
            message_previous = message2_hashed;
            message_second = message_second;
            cout << j << ": get the first hash value  " << message_previous << endl;
            cout << j << ": get the second hash value " << message_second << endl;
            record_2_MD_custom_hash_values_test_sub_avalanche_effect(message_previous, message_second);
            j = 0;
        }

        j++;

        // cout << "Its hash value:" << message_ << endl;
        // message2_hashed = "010010001000";
        // message_hashed = "010010001000";
            cout << "i: " << i << ", start: " << start << endl;
            total = i - start + 2;
            // cout << "total tested: " << total << endl;

        cout << endl;
        
    }

    return;
    
}

string collision(string message){
    // cout << "error?" << endl;
    string message_hashed = MD_H(message);
    // cout << "After hash it: " << message_hashed << endl;
    cout << endl;

    // input 2^24
    int start = pow(2, 23) + 1;
    int end = pow(2, 24) - 1;
    string message_previous = "";
    string message_second = "";
    int j = 0;
    for (int i = start; i < end; i++){
        bitset<24> i_bit(i);
        cout << "Try message to find collision: " << i_bit.to_string() << endl;
        string message2_hashed = MD_H(i_bit.to_string());

        // cout << "*****************j********************: " << j << endl;
        // test avalanche effect
        if (j == 0){
            message_previous = message2_hashed;
            message_second = message_second;
            cout << j << ": get the first hash value  " << message_previous << endl;
            cout << j << ": get the second hash value " << message_second << endl;
            if (message_second.length() != 0){
                record_2_MD_custom_hash_values_test_sub_avalanche_effect(message_previous, message_second);
            }
        }
        if (j == 1){
            message_previous = message_previous;
            message_second = message2_hashed;
            cout << j << ": get the first hash value  " << message_previous << endl;
            cout << j << ": get the second hash value " << message_second << endl;
            record_2_MD_custom_hash_values_test_sub_avalanche_effect(message_previous, message_second);
        }

        if (j == 2){
            message_previous = message2_hashed;
            message_second = message_second;
            cout << j << ": get the first hash value  " << message_previous << endl;
            cout << j << ": get the second hash value " << message_second << endl;
            record_2_MD_custom_hash_values_test_sub_avalanche_effect(message_previous, message_second);
            j = 0;
        }

        j++;

        // cout << "Its hash value:" << message_ << endl;
        // message2_hashed = "010010001000";
        // message_hashed = "010010001000";
        if (message2_hashed == message_hashed){
            cout << "found" << endl;
            cout << "i: " << i << ", start: " << start << endl;
            total = i - start + 2;
            cout << "total tested: " << total << endl;
            return i_bit.to_string();
        }
        cout << endl;
        
    }
    
    // cout << "cccccc" << endl;
    // return 0;
}

int main(){
    // has 40 bits
    string input_message = "0101101011000101010101010000101001010100";
    cout << "The input_message is " << input_message << endl;

    string result = collision(input_message);

    // cout << "collision message: " << result << endl;
    // // found collision message: 100000000000101000101000

    collision_avalanche_effect_test();
    avalanche_effect();

    cout << "collision message: " << result << endl;
    // found collision message: 100000000000101000101000

    // only 1 round hash
    /*
    total test: 10000
    The effect of bit 1: 0.6544
    The effect of bit 2: 0.6194
    The effect of bit 3: 0.5689
    The effect of bit 4: 0.5915
    The effect of bit 5: 0.5536
    The effect of bit 6: 0.5767
    The effect of bit 7: 0.5563
    The effect of bit 8: 0.6079
    The effect of bit 9: 0.603
    The effect of bit 10: 0.668
    The effect of bit 11: 0.6332
    The effect of bit 12: 0.4426
    collision message: 100000000000101000101000
    */

    // add 5 round hash
    /*
    total test: 10000
    The effect of bit 1: 0.6008
    The effect of bit 2: 0.6124
    The effect of bit 3: 0.5914
    The effect of bit 4: 0.5927
    The effect of bit 5: 0.5517
    The effect of bit 6: 0.5597
    The effect of bit 7: 0.5525
    The effect of bit 8: 0.6172
    The effect of bit 9: 0.6086
    The effect of bit 10: 0.6349
    The effect of bit 11: 0.6345
    The effect of bit 12: 0.4598
    collision message: 100000000000101000101000
    */

    // add 10 round hash
    /*
    total test: 10000
    The effect of bit 1: 0.5836
    The effect of bit 2: 0.6057
    The effect of bit 3: 0.5965
    The effect of bit 4: 0.5977
    The effect of bit 5: 0.5464
    The effect of bit 6: 0.5583
    The effect of bit 7: 0.5499
    The effect of bit 8: 0.6243
    The effect of bit 9: 0.6126
    The effect of bit 10: 0.6465
    The effect of bit 11: 0.6494
    The effect of bit 12: 0.4554
    collision message: 100000000000101000101000
    */

    return 0;
}
