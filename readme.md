# How to build
Make sure all submodules are already initialized.

Make testcompressor

```bash
g++ -I7z/C/ -o testcompressor -std=c++14 -D_7ZIP_ST testcompressor.cc compressor.cc 7z/C/LzFind.c  7z/C/LzmaDec.c  7z/C/LzmaEnc.c 7z/C/CpuArch.c
```
Make testcompressor:

```bash
./testcompressor
```

OutPut:
```txt
*******************CompressTest Start!*******************
Original Size : 1
00000000 | 61                                              | a
compressed length = 19
00000000 | 5d 00 10 00 00 01 00 00 00 01 00 00 00 00 30 7f | ].............0
00000010 | fc 00 00                                        | ...
decompressed length = 1
00000000 | 61                                              | a
*******************CompressTest Start!*******************
Original Size : 21
00000000 | 68 65 72 65 20 69 73 20 61 20 63 6f 6f 6c 20 73 | here is a cool s
00000010 | 74 72 69 6e 67                                  | tring
compressed length = 39
00000000 | 5d 00 10 00 00 15 00 00 00 15 00 00 00 00 34 19 | ].............4.
00000010 | 4a aa e9 4e 9d 5c 0c b3 af 24 99 6f e2 c7 a1 b2 | J..N.\...$.o....
00000020 | bc 4a 62 71 d3 36 b9                            | .Jbq.6.
decompressed length = 21
00000000 | 68 65 72 65 20 69 73 20 61 20 63 6f 6f 6c 20 73 | here is a cool s
00000010 | 74 72 69 6e 67                                  | tring
*******************CompressTest Start!*******************
Original Size : 267
00000000 | 68 65 72 65 27 73 20 73 6f 6d 65 74 68 69 6e 67 | here's something
00000010 | 20 74 68 61 74 20 73 68 6f 75 6c 64 20 63 6f 6d |  that should com
00000020 | 70 72 65 73 73 20 70 72 65 74 74 79 20 77 65 6c | press pretty wel
00000030 | 6c 3a 20 61 62 63 64 65 66 61 62 63 64 65 66 61 | l: abcdefabcdefa
00000040 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
00000050 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
00000060 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
00000070 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
00000080 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
00000090 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
000000a0 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
000000b0 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
000000c0 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
000000d0 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
000000e0 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
000000f0 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
00000100 | 62 63 64 65 66 61 62 63 64 65 66                | bcdefabcdef
compressed length = 73
00000000 | 5d 00 10 00 00 0b 01 00 00 0b 01 00 00 00 34 19 | ].............4.
00000010 | 4a aa e9 a1 11 c6 6d bc 26 9a 72 77 e3 22 ef 92 | J.....m.&.rw."..
00000020 | ab 3e 7e d4 ab 13 a7 c1 f2 65 72 c7 92 39 2f 40 | .>~......er..9/@
00000030 | 48 2f a3 1b 87 cb a8 84 e3 98 31 2b e5 59 63 54 | H/........1+.YcT
00000040 | cb be dd 96 a6 ed 93 00 00                      | .........
decompressed length = 267
00000000 | 68 65 72 65 27 73 20 73 6f 6d 65 74 68 69 6e 67 | here's something
00000010 | 20 74 68 61 74 20 73 68 6f 75 6c 64 20 63 6f 6d |  that should com
00000020 | 70 72 65 73 73 20 70 72 65 74 74 79 20 77 65 6c | press pretty wel
00000030 | 6c 3a 20 61 62 63 64 65 66 61 62 63 64 65 66 61 | l: abcdefabcdefa
00000040 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
00000050 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
00000060 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
00000070 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
00000080 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
00000090 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
000000a0 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
000000b0 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
000000c0 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
000000d0 | 62 63 64 65 66 61 62 63 64 65 66 61 62 63 64 65 | bcdefabcdefabcde
000000e0 | 66 61 62 63 64 65 66 61 62 63 64 65 66 61 62 63 | fabcdefabcdefabc
000000f0 | 64 65 66 61 62 63 64 65 66 61 62 63 64 65 66 61 | defabcdefabcdefa
00000100 | 62 63 64 65 66 61 62 63 64 65 66                | bcdefabcdef
```

# References:
1. https://gist.github.com/phoe/c33e1f8ec651e7892f82596be6d0d3af
2. https://www.7-zip.org/
3. https://github.com/kornelski/7z.git