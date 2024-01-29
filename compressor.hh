//Compressor, author: ZeitHaum.
#pragma once
#ifndef __UTIL_COMPRESSOR__
#define __UTIL_COMPRESSOR__
#include <cstdint>
#include <string.h>
#include <memory>
#include "string.h"
extern "C"{
    #include "LzmaEnc.h"
    #include "LzmaDec.h"
}

class Compressor{
public:
    uint8_t* buffer;
    uint64_t buffer_size;
    Compressor();
    Compressor(uint64_t);
    virtual ~Compressor(); 
    inline void ajustBuffer(uint64_t buffer_len);
    virtual void compress(uint8_t* src, uint64_t src_len, uint64_t& dest_len) = 0;
    virtual void decompress(uint8_t* src, uint64_t src_len, uint64_t& dest_len) = 0;
};

class LzmaCompressor: public Compressor{
public:
    LzmaCompressor();
    LzmaCompressor(uint64_t);
    void compress(uint8_t* src, uint64_t src_len, uint64_t& dest_len);
    void decompress(uint8_t* src, uint64_t src_len, uint64_t& dest_len);
    //Lzma special function
    void hexdump(const uint8_t *buf, int size) ;
private:
    static void *_lzmaAlloc(ISzAllocPtr, size_t size);
    static void _lzmaFree(ISzAllocPtr, void *addr);
    static ISzAlloc _allocFuncs;
    std::unique_ptr<uint8_t[]> lzmaCompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize);
    std::unique_ptr<uint8_t[]> lzmaDecompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize);
};
#endif