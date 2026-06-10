# Implementation and Analysis of Custom Merkle-Damgård(MD) Hash Function

## Table of Contents

- [Custom MD Hash Function Overview and Results](#custom-md-hash-function-overview-and-results)
- [Function Discussion and Future Work](#function-discussion-and-future-work)

## Custom MD Hash Function Overview and Results

### i) Processing the input message: `vector<string> split(string message)`<br>
Split message, each 12 bits as a block for Merkle-Damg ard and custom hash.<br>
If the last block is less than 12 bits, then add padding block "0".

### ii) The non-trivial custom hash: `string customHash(string temp, string last_result)`<br>
The input has 24 bits: 12 bits (splitted message) + 12 bits (iv or last result);<br>
Split the input again, make each 4 bits as a block.

#### Nonlinear S-box:<br>
According to S-box, replace the value of each block:<br>
<img width="675" height="91" alt="image" src="https://github.com/user-attachments/assets/21cca918-f894-452b-b5be-948858906032" /><br>
  Eg. Block: 1010 <br>
  In decimal, the block is 2^3 + 2^1 = 8 + 2 = 10 <br>
  Replace it to the output 4, and the binary of 4 is 0100 <br>
After replace all the block, combine these outputs as a new 24 bits message.<br>
In code, name the new 24 bits message as `dec_bt_24` or `after_S_box`.

#### Bitwise operations:<br>
The rule of bitwise operations depends on the reminder of decimal number `dec_bt_24`, ie. `dec_bt_24%3`;<br>
Add `obj` as random seed into the rule of bitwise operations;<br>
Each bit should be operated, then the result is `hash_value_24` or `after_S_box_bit`;<br>
The result has 24 bits, so it still needs to be split;<br>
Split depends on the reminder of decimal number `hash_value_24`, ie: `dec_after_S_box_bit%2`.

### iii) Merkle-Damg ard-based hash function H: `string MD_H(string m)`<br>
`split(m)`: split message function;<br>
`MD_round`: the number of Merkle-Damg and unit;<br>
1^st round/unit: `customHash(splitted message m0, iv)`;<br>
2^nd round/unit: `customHash(splitted message m1, last result)`;<br>
Last round/unit: `customHash(splitted message m || PB, last result)` = final hash value.

### iv) Find collision: `string collision(string message)`<br>
Burte force: try the integer number from 2^23 - 1 to 2^24 - 1. <br>
`MD_H(test_message)` == `MD_H(given message)`

### v) Avalanche effect: 
Not strict avalanche criterion (SAC).<br>
`vector<int> record_2_MD_custom_hash_values_test_sub_avalanche_effect(string hash_value1, string hash_value2)`<br>
`void avalanche_effect()`

The partial output of code:<br>
total test: 10000<br>
The effect of bit 1: 0.6544<br>
The effect of bit 2: 0.6194<br>
The effect of bit 3: 0.5689<br>
The effect of bit 4: 0.5915<br>
The effect of bit 5: 0.5536<br>
The effect of bit 6: 0.5767<br>
The effect of bit 7: 0.5563<br>
The effect of bit 8: 0.6079<br>
The effect of bit 9: 0.603<br>
The effect of bit 10: 0.668<br>
The effect of bit 11: 0.6332<br>
The effect of bit 12: 0.4426<br>

Iterative experiments on round numbers determined 5 to 7 rounds as the optimal range and excessive iterations caused the over-diffusion phenomenon.

## Function Discussion and Future Work

Research Nostradamus attack(Stuck on diamond tree)
