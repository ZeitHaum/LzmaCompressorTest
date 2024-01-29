//Compressor, author: ZeitHaum.
#include "compressor.hh"
#include <stdio.h>
#include <cassert>

Compressor::Compressor()
{
    //default size 512KB
    buffer_size = 512 * 1024;
    buffer = new uint8_t[buffer_size];
}

Compressor::Compressor(uint64_t buffer_sz)
:buffer_size(buffer_sz)
{
    buffer = new uint8_t[buffer_size];
}

Compressor::~Compressor(){
    delete[] buffer;
}

inline void Compressor::ajustBuffer(uint64_t buffer_len){
    if(buffer_size >= buffer_len) return;
    else{
        delete[] buffer;
        buffer_size = buffer_len;
        buffer = new uint8_t[buffer_size];
    }
}

LzmaCompressor::LzmaCompressor()
:Compressor()
{
	init();
}

LzmaCompressor::LzmaCompressor(uint64_t buffer_size)
:Compressor(buffer_size * 4) // lzma compress_size may be larger than input
{
	init();
}

void LzmaCompressor::init(){
	// prepare space for the encoded properties
	propsSize = 5;
	propsEncoded = new uint8_t[propsSize];
	LzmaEncProps_Init(&props);
	props.fb = 40;
}

void LzmaCompressor::compress(uint8_t* src, uint64_t src_len, uint64_t& dest_len){
	const SizeT off = propsSize + 8; // propsSize + decompressSize(8 bytes)
	// set up properties
	if (src_len >= (1 << 20))
		props.dictSize = 1 << 20; // 1mb dictionary
	else
		props.dictSize = src_len; // smaller dictionary = faster!

	uint64_t outputSize64 = buffer_size;

	int lzmaStatus = LzmaEncode(
		buffer + off, &outputSize64, src, src_len,
		&props, propsEncoded, &propsSize, 0,
		NULL,
		&_allocFuncs, &_allocFuncs);

	dest_len = outputSize64 + off;
	if (lzmaStatus == SZ_OK) {
		// tricky: we have to generate the LZMA header
		// propsSize bytes properties + 8 byte uncompressed size
		memcpy(buffer, propsEncoded, propsSize);
		for (int i = 0; i < 8; i++)
			buffer[propsSize + i] = (src_len >> (i * 8)) & 0xFF;
	}
	else{
		assert(0);
	}
}

void LzmaCompressor::decompress(uint8_t* src, uint64_t src_len, uint64_t& dest_len){
	const SizeT off = propsSize + 8;
	assert(src_len >= off &&"invalid lzma header!");
	UInt64 size = 0;
	for (int i = 0; i < 8; i++)
		size |= (src[propsSize + i] << (i * 8));
	if (size <= (256 * 1024 * 1024)) {
		ELzmaStatus lzmaStatus;
		SizeT procOutSize = size, procInSize = src_len - off;
		int status = LzmaDecode(buffer, &procOutSize, src + off, &procInSize, src, propsSize, LZMA_FINISH_END, &lzmaStatus, &_allocFuncs);

		if (status == SZ_OK && procOutSize == size) {
			dest_len = size;
		}
		else{
			assert(0);
		}
	}
	else{
		assert(0);
	}
}

void* LzmaCompressor::_lzmaAlloc(ISzAllocPtr, size_t size){
    return new uint8_t[size];
}

void LzmaCompressor::_lzmaFree(ISzAllocPtr, void *addr){
    if (!addr)
		return;
	delete[] reinterpret_cast<uint8_t *>(addr);
}

ISzAlloc LzmaCompressor::_allocFuncs = {
	LzmaCompressor::_lzmaAlloc, LzmaCompressor::_lzmaFree
};


void LzmaCompressor::hexdump(const uint8_t *buf, int size) {
	int lines = (size + 15) / 16;
	for (int i = 0; i < lines; i++) {
		printf("%08x | ", i * 16);

		int lineMin = i * 16;
		int lineMax = lineMin + 16;
		int lineCappedMax = (lineMax > size) ? size : lineMax;

		for (int j = lineMin; j < lineCappedMax; j++)
			printf("%02x ", buf[j]);
		for (int j = lineCappedMax; j < lineMax; j++)
			printf("   ");

		printf("| ");

		for (int j = lineMin; j < lineCappedMax; j++) {
			if (buf[j] >= 32 && buf[j] <= 127)
				printf("%c", buf[j]);
			else
				printf(".");
		}
		printf("\n");
	}
}