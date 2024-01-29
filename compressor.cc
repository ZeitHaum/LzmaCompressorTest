//Compressor, author: ZeitHaum.
#include "compressor.hh"
#include <stdio.h>

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
{}

LzmaCompressor::LzmaCompressor(uint64_t buffer_size)
:Compressor(buffer_size * 4) // lzma compress_size may be larger than input
{}

void LzmaCompressor::compress(uint8_t* src, uint64_t src_len, uint64_t& dest_len){
    uint32_t tmp = 0;
    auto result = lzmaCompress(src, src_len, &tmp);
    dest_len = tmp;
    memcpy(buffer, result.get(), dest_len);
}

void LzmaCompressor::decompress(uint8_t* src, uint64_t src_len, uint64_t& dest_len){
    uint32_t tmp = 0;
    auto result = lzmaDecompress(src, src_len, &tmp);
    dest_len = tmp;
    memcpy(buffer, result.get(), dest_len);
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

std::unique_ptr<uint8_t[]> LzmaCompressor::lzmaCompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize) {
	std::unique_ptr<uint8_t[]> result;

	// set up properties
	CLzmaEncProps props;
	LzmaEncProps_Init(&props);
	if (inputSize >= (1 << 20))
		props.dictSize = 1 << 20; // 1mb dictionary
	else
		props.dictSize = inputSize; // smaller dictionary = faster!
	props.fb = 40;

	// prepare space for the encoded properties
	SizeT propsSize = 5;
	uint8_t propsEncoded[5];

	// allocate some space for the compression output
	// this is way more than necessary in most cases...
	// but better safe than sorry
	//   (a smarter implementation would use a growing buffer,
	//    but this requires a bunch of fuckery that is out of
	///   scope for this simple example)
	SizeT outputSize64 = inputSize * 1.5;
	if (outputSize64 < 1024)
		outputSize64 = 1024;
	auto output = std::make_unique<uint8_t[]>(outputSize64);

	int lzmaStatus = LzmaEncode(
		output.get(), &outputSize64, input, inputSize,
		&props, propsEncoded, &propsSize, 0,
		NULL,
		&_allocFuncs, &_allocFuncs);

	*outputSize = outputSize64 + 13;
	if (lzmaStatus == SZ_OK) {
		// tricky: we have to generate the LZMA header
		// 5 bytes properties + 8 byte uncompressed size
		result = std::make_unique<uint8_t[]>(outputSize64 + 13);
		uint8_t *resultData = result.get();

		memcpy(resultData, propsEncoded, 5);
		for (int i = 0; i < 8; i++)
			resultData[5 + i] = (inputSize >> (i * 8)) & 0xFF;
		memcpy(resultData + 13, output.get(), outputSize64);
	}

	return result;
}

std::unique_ptr<uint8_t[]> LzmaCompressor::lzmaDecompress(const uint8_t *input, uint32_t inputSize, uint32_t *outputSize){
    if (inputSize < 13)
		return NULL; // invalid header!

	// extract the size from the header
	UInt64 size = 0;
	for (int i = 0; i < 8; i++)
		size |= (input[5 + i] << (i * 8));

	if (size <= (256 * 1024 * 1024)) {
		auto blob = std::make_unique<uint8_t[]>(size);

		ELzmaStatus lzmaStatus;
		SizeT procOutSize = size, procInSize = inputSize - 13;
		int status = LzmaDecode(blob.get(), &procOutSize, &input[13], &procInSize, input, 5, LZMA_FINISH_END, &lzmaStatus, &_allocFuncs);

		if (status == SZ_OK && procOutSize == size) {
			*outputSize = size;
			return blob;
		}
	}

	return NULL;
}

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