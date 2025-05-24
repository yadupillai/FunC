Application Developed by
1. Yadu Pillai (24966285)
2. Mohammad Karimi (24733370)
3. Nek Sultan (25430341)
4. Mario Apostolov

# File Processor - Data Compression and Encryption Tool

This application is a text file compression, encryption, and processing tool that is used through the command-line interface.

## Features

- Huffman code for compression
- The XOR cipher encryption
- Keyword searching in decrypted texts
- Line sorting

Instructions for building

### Requirements
- GCC compiler
- Build

### Building
Make


# Clean Build Files
clean up


## Usage
All commands need to be run from the project root directory.

# Show help (Lists all the possuble actions)
./bin/file_processor --help

Compress a file (output will be a .huff file)
./bin/file_processor --compress -i input.txt -o output.huff

# Decompressive a file
./bin/file_processor --decompress -i output.huff -o output.txt

# Encrypt a File
./bin/file_processor --encrypt -i input.txt -o output.enc -k "EnterYourKey"

# Decrypt a file
./bin/file_processor --decrypt -i output.enc -o output.txt -k "EnterYourKey"

# Search in a file
./bin/file_processor --search -i input.txt -s "keyword"

# Sort lines in a file
./bin/file_processor --sort -i input.txt -o output_sorted.txt
