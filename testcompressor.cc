#include "compressor.hh"
#include <iostream>

LzmaCompressor* compressor = new LzmaCompressor();

void test(uint8_t* string, uint32_t size){
    //test compress
    uint8_t input_buffer[4096];
    std::cout << "*******************CompressTest Start!*******************\n";
    std::cout << "Original Size : "<< size << "\n";
    memcpy(input_buffer, string, size);
    compressor->hexdump(input_buffer, size);
    uint64_t comp_len = 0;
    compressor->compress(input_buffer, size, comp_len);
    std::cout<< "compressed length = " << comp_len << "\n";
    compressor->hexdump(compressor->buffer, comp_len);
    memcpy(input_buffer, compressor->buffer, comp_len);
    // test decompress
    uint64_t decomp_len = 0;
    compressor->decompress(input_buffer, comp_len, decomp_len);
    std::cout<< "decompressed length = " << decomp_len << "\n";
    compressor->hexdump(compressor->buffer, decomp_len);
}

void test(const char* string){
    test((uint8_t*) string , strlen(string));
}

int main(){
	test("a");
	test("here is a cool string");
	test("here's something that should compress pretty well: abcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdefabcdef");
}