# File Processor - Data Compression and Encryption Tool

A command-line tool for compressing, encrypting, and processing text files.

## Features

- Huffman coding for compression
- XOR cipher encryption
- Keyword search in decrypted text
- Line sorting
- Command-line interface with various modes
- Debug mode support

## Build Instructions

### Prerequisites
- GCC compiler
- Make

### Building
```bash
# Build release version
make

# Build debug version
make debug

# Clean build files
make clean
```

## Usage

All commands should be run from the root directory of the project.

```bash
# Compress a file (output will be a .huff file)
./bin/file_processor --compress -i input.txt -o output.huff

# Decompress a file
./bin/file_processor --decompress -i output.huff -o output.txt

# Encrypt a file
./bin/file_processor --encrypt -i input.txt -o output.enc -k "secretkey"

# Decrypt a file
./bin/file_processor --decrypt -i output.enc -o output.txt -k "secretkey"

# Search in a file
./bin/file_processor --search -i input.txt -s "keyword"

# Sort lines in a file
./bin/file_processor --sort -i input.txt -o output_sorted.txt

# Show help
./bin/file_processor --help
```

## Project Structure

- `Makefile` - Manages the build process.
- `bin/` - Directory for the compiled executable (`file_processor`).
- `include/` - Directory for header files (`.h`).
  - `cli.h`
  - `compress.h`
  - `encrypt.h`
  - `io.h`
  - `search.h`
  - `sort.h`
- `src/` - Directory for C source files (`.c`).
  - `main.c` - Main program entry point.
  - `cli.c` - Command-line interface handling.
  - `compress.c` - Huffman coding implementation.
  - `encrypt.c` - XOR encryption algorithm.
  - `io.c` - File I/O operations.
  - `search.c` - Keyword search functionality.
  - `sort.c` - Line sorting implementation.
- `obj/` - Directory for compiled object files (`.o`), created during build.

## License

This project is licensed under the MIT License. 