# File Processor - Data Compression and Encryption Tool

A command-line tool for compressing, encrypting, and processing text files.

## Features

- Run-Length Encoding (RLE) compression
- XOR and Caesar cipher encryption
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

```bash
# Compress a file
./file_processor --compress -i input.txt -o output.rle

# Decompress a file
./file_processor --decompress -i input.rle -o output.txt

# Encrypt a file
./file_processor --encrypt -i input.txt -o output.enc -k "secretkey"

# Decrypt a file
./file_processor --decrypt -i input.enc -o output.txt -k "secretkey"

# Search in a file
./file_processor --search -i input.txt -s "keyword"

# Sort lines in a file
./file_processor --sort -i input.txt -o output.txt

# Show help
./file_processor --help
```

## Project Structure

- `main.c` - Main program entry point
- `cli.c/h` - Command-line interface handling
- `compress.c/h` - RLE compression implementation
- `encrypt.c/h` - Encryption algorithms
- `io.c/h` - File I/O operations
- `search.c/h` - Keyword search functionality
- `sort.c/h` - Line sorting implementation

## License

This project is licensed under the MIT License. 