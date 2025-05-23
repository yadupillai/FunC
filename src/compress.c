#include "../include/compress.h"
#include "../include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 256 // Assuming ASCII characters

// Helper structure for the min-heap
typedef struct MinHeapNode {
    HuffmanNode* h_node;
    unsigned frequency; // Frequency is also in HuffmanNode, but duplicated here for easier heap operations
} MinHeapNode;

typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode** array;
} MinHeap;

// --- Huffman Node Utility Functions ---
HuffmanNode* new_huffman_node(char character, unsigned frequency) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (!node) {
        handle_memory_error();
        return NULL;
    }
    node->left = node->right = NULL;
    node->character = character;
    node->frequency = frequency;
    return node;
}

// --- Min-Heap Utility Functions (for building Huffman Tree) ---

MinHeap* create_min_heap(unsigned capacity) {
    MinHeap* min_heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!min_heap) {
        handle_memory_error();
        return NULL;
    }
    min_heap->size = 0;
    min_heap->capacity = capacity;
    min_heap->array = (MinHeapNode**)malloc(min_heap->capacity * sizeof(MinHeapNode*));
    if (!min_heap->array) {
        handle_memory_error();
        free(min_heap);
        return NULL;
    }
    return min_heap;
}

void swap_min_heap_nodes(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(MinHeap* min_heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < min_heap->size && min_heap->array[left]->frequency < min_heap->array[smallest]->frequency)
        smallest = left;

    if (right < min_heap->size && min_heap->array[right]->frequency < min_heap->array[smallest]->frequency)
        smallest = right;

    if (smallest != idx) {
        swap_min_heap_nodes(&min_heap->array[smallest], &min_heap->array[idx]);
        min_heapify(min_heap, smallest);
    }
}

int is_heap_size_one(MinHeap* min_heap) {
    return (min_heap->size == 1);
}

MinHeapNode* extract_min(MinHeap* min_heap) {
    if (is_heap_size_one(min_heap) && min_heap->size == 0) return NULL; // Check if actually empty

    MinHeapNode* temp = min_heap->array[0];
    min_heap->array[0] = min_heap->array[min_heap->size - 1];
    --min_heap->size;
    if (min_heap->size > 0) { // Only heapify if heap is not empty
       min_heapify(min_heap, 0);
    }
    return temp;
}


void insert_min_heap(MinHeap* min_heap, HuffmanNode* h_node) {
    if (min_heap->size == min_heap->capacity) {
        // This case should ideally not happen if capacity is set correctly (e.g., MAX_TREE_NODES)
        // Or, implement dynamic resizing if needed.
        handle_error("Min-heap is full. Cannot insert.");
        return;
    }
    
    MinHeapNode* min_heap_node = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    if(!min_heap_node){
        handle_memory_error();
        return;
    }
    min_heap_node->h_node = h_node;
    min_heap_node->frequency = h_node->frequency;


    ++min_heap->size;
    int i = min_heap->size - 1;
    min_heap->array[i] = min_heap_node; // Insert new node at the end

    // Fix the min heap property if it is violated
    while (i && min_heap->array[i]->frequency < min_heap->array[(i - 1) / 2]->frequency) {
        swap_min_heap_nodes(&min_heap->array[i], &min_heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

MinHeap* build_and_create_min_heap(const char* data, size_t size, unsigned* frequencies) {
    for (int i = 0; i < 256; ++i) frequencies[i] = 0;
    for (size_t i = 0; i < size; ++i) frequencies[(unsigned char)data[i]]++;

    MinHeap* min_heap = create_min_heap(MAX_TREE_NODES); // Max 256 distinct characters
    if (!min_heap) return NULL;

    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            HuffmanNode* h_node = new_huffman_node((char)i, frequencies[i]);
            if(!h_node){
                // new_huffman_node calls handle_memory_error
                // Need to free min_heap and its array if h_node creation fails mid-way
                for(unsigned j=0; j < min_heap->size; ++j) {
                    free(min_heap->array[j]->h_node); // Free HuffmanNode
                    free(min_heap->array[j]);      // Free MinHeapNode
                }
                free(min_heap->array);
                free(min_heap);
                return NULL;
            }
            insert_min_heap(min_heap, h_node);
        }
    }
    return min_heap;
}

HuffmanNode* build_huffman_tree(MinHeap* min_heap) {
    HuffmanNode *left, *right, *top;

    // Iterate while size of heap doesn't become 1
    while (!is_heap_size_one(min_heap)) {
        // Extract the two minimum freq items from min heap
        MinHeapNode* left_min_node = extract_min(min_heap);
        MinHeapNode* right_min_node = extract_min(min_heap);

        if(!left_min_node || !right_min_node){
             // Should not happen if heap has more than one item
             handle_error("Error extracting from min-heap during tree build.");
             // Free extracted nodes if any
             if(left_min_node) { free(left_min_node->h_node); free(left_min_node); }
             if(right_min_node) { free(right_min_node->h_node); free(right_min_node); }
             // Free remaining nodes in heap (complex cleanup, or rely on program exit for now)
             return NULL;
        }
        
        left = left_min_node->h_node;
        right = right_min_node->h_node;


        // Create a new internal node with frequency equal to the
        // sum of the two nodes' frequencies. Make the two extracted
        // node as left and right children of this new node.
        // '$' is a special value for internal nodes, not used for character data.
        top = new_huffman_node('$', left->frequency + right->frequency);
        if(!top){
            // new_huffman_node calls handle_memory_error
            // Free the extracted nodes as they are no longer in the heap
            free(left_min_node->h_node); free(left_min_node);
            free(right_min_node->h_node); free(right_min_node);
            return NULL;
        }

        top->left = left;
        top->right = right;

        insert_min_heap(min_heap, top);
        
        // Free the MinHeapNode wrappers, HuffmanNodes are now part of the tree
        free(left_min_node);
        free(right_min_node);
    }

    // The remaining node is the root node and the tree is complete.
    MinHeapNode* root_min_node = extract_min(min_heap);
    if (!root_min_node) { // Should ideally not happen
        handle_error("Min-heap became empty unexpectedly.");
        return NULL;
    }
    HuffmanNode* root = root_min_node->h_node;
    free(root_min_node); // Free the MinHeapNode wrapper
    return root;
}


// --- Huffman Code Generation ---
#define MAX_CODE_LENGTH 256 // Max depth of Huffman tree for 256 chars

void generate_codes_recursive(HuffmanNode* root, int arr[], int top, HuffmanCode** codes_head) {
    // Assign 0 to left edge and recur
    if (root->left) {
        arr[top] = 0;
        generate_codes_recursive(root->left, arr, top + 1, codes_head);
    }

    // Assign 1 to right edge and recur
    if (root->right) {
        arr[top] = 1;
        generate_codes_recursive(root->right, arr, top + 1, codes_head);
    }

    // If this is a leaf node, then it contains one of the input characters,
    // print the character and its code from arr[]
    if (!(root->left) && !(root->right)) { // is_leaf(root)
        HuffmanCode* new_code_entry = (HuffmanCode*)malloc(sizeof(HuffmanCode));
        if(!new_code_entry){
            handle_memory_error();
            return; // Cannot add this code, potential memory leak of previously added codes if not handled
        }
        new_code_entry->character = root->character;
        new_code_entry->code = (char*)malloc(top + 1);
        if(!new_code_entry->code){
            handle_memory_error();
            free(new_code_entry);
            return;
        }
        for (int i = 0; i < top; ++i)
            new_code_entry->code[i] = arr[i] + '0'; // Convert int 0/1 to char '0'/'1'
        new_code_entry->code[top] = '\0';
        
        // Add to linked list of codes
        new_code_entry->next = *codes_head;
        *codes_head = new_code_entry;
    }
}

HuffmanCode* get_huffman_codes(HuffmanNode* root) {
    int arr[MAX_CODE_LENGTH];
    HuffmanCode* codes_head = NULL;
    generate_codes_recursive(root, arr, 0, &codes_head);
    return codes_head;
}

void free_huffman_tree(HuffmanNode* root) {
    if (root == NULL) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

void free_huffman_codes(HuffmanCode* codes_head) {
    HuffmanCode* current = codes_head;
    while (current != NULL) {
        HuffmanCode* next = current->next;
        free(current->code);
        free(current);
        current = next;
    }
}


// --- Serialization/Deserialization of Huffman Tree/Frequencies ---
// For simplicity, we will serialize the frequencies.
// Format: [char_1][freq_1_byte1]...[freq_1_byte4][char_2]...
// Or even simpler: list of (char, freq) pairs until a sentinel.
// Or fixed size 256 frequencies.

// --- Main Compression/Decompression Functions ---

char* huffman_compress(const char* input, size_t input_len, size_t* output_len) {
    if (!input || input_len == 0) {
        handle_error("Invalid input for Huffman compression");
        *output_len = 0;
        return NULL;
    }

    unsigned frequencies[256];
    MinHeap* min_heap = build_and_create_min_heap(input, input_len, frequencies);
    if (!min_heap) {
        // Error handled in build_and_create_min_heap
        *output_len = 0;
        return NULL;
    }
    
    // Handle case of single unique character or empty effective input after frequency calculation
    if (min_heap->size == 0) { // No characters with frequency > 0
        handle_error("Input has no characters with positive frequency for Huffman compression.");
        // Free MinHeap (it's empty or contains only already freed nodes)
        free(min_heap->array);
        free(min_heap);
        *output_len = 0;
        return NULL;
    }
     if (min_heap->size == 1) {
        // Special case: input contains only one distinct character
        // Or only one character type.
        // The standard Huffman tree building might not work as expected or be optimal.
        // A simple encoding: [char][original_length_as_bytes]...
        // Or just store the char and its count.
        // For now, let's just indicate it's not a typical case for standard Huffman.
        // Or, proceed and let it form a simple tree.
        // The current build_huffman_tree extracts the single node as root.
    }


    HuffmanNode* root = build_huffman_tree(min_heap); // min_heap is consumed (emptied and its array possibly modified)
    // Free the min_heap structure itself and its array (nodes are now in the Huffman tree)
    free(min_heap->array); // The array itself
    free(min_heap);       // The MinHeap struct

    if (!root) {
        handle_error("Failed to build Huffman tree.");
        *output_len = 0;
        return NULL;
    }

    HuffmanCode* codes = get_huffman_codes(root);
    if(!codes){
        handle_error("Failed to generate Huffman codes.");
        free_huffman_tree(root);
        *output_len = 0;
        return NULL;
    }

    // --- Actual bitstream encoding ---
    // 1. Serialize frequencies (or tree structure) for decompression
    //    For now, let's assume frequencies are written directly.
    //    Size of frequency table: 256 * sizeof(unsigned) for character counts.
    size_t freq_table_size = 256 * sizeof(unsigned);
    
    // 2. Calculate size of encoded data.
    //    Iterate through input, look up code for each char, sum lengths of codes.
    size_t encoded_data_bits = 0;
    for (size_t i = 0; i < input_len; ++i) {
        HuffmanCode* current_code_entry = codes;
        while(current_code_entry) {
            if (current_code_entry->character == input[i]) {
                encoded_data_bits += strlen(current_code_entry->code);
                break;
            }
            current_code_entry = current_code_entry->next;
        }
        if(!current_code_entry){
             // Should not happen if all input characters are in the codes list
            handle_error("Character in input not found in Huffman codes during size calculation.");
            free_huffman_tree(root);
            free_huffman_codes(codes);
            *output_len = 0;
            return NULL;
        }
    }
    size_t encoded_data_bytes = (encoded_data_bits + 7) / 8; // Round up to nearest byte

    // 3. Allocate output buffer: freq_table + encoded_data_bytes + potentially other headers
    *output_len = freq_table_size + encoded_data_bytes + sizeof(size_t); // Add space for original_len
    char* output = (char*)malloc(*output_len);
    if (!output) {
        handle_memory_error();
        free_huffman_tree(root);
        free_huffman_codes(codes);
        *output_len = 0;
        return NULL;
    }

    // 4. Write frequency table to output
    char* ptr = output;
    memcpy(ptr, frequencies, freq_table_size);
    ptr += freq_table_size;

    // Store original length (needed for decompression if padding bits exist)
    memcpy(ptr, &input_len, sizeof(size_t));
    ptr += sizeof(size_t);

    // 5. Write encoded bitstream
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    for (size_t i = 0; i < input_len; ++i) {
        HuffmanCode* current_code_entry = codes;
        while(current_code_entry) {
            if (current_code_entry->character == input[i]) {
                char* code_str = current_code_entry->code;
                for (int j = 0; code_str[j] != '\0'; ++j) {
                    bit_buffer <<= 1;
                    if (code_str[j] == '1') {
                        bit_buffer |= 1;
                    }
                    bit_count++;
                    if (bit_count == 8) {
                        *ptr++ = bit_buffer;
                        bit_buffer = 0;
                        bit_count = 0;
                    }
                }
                break;
            }
            current_code_entry = current_code_entry->next;
        }
         if(!current_code_entry){
             // Should not happen if all input characters are in the codes list
            handle_error("Character in input not found in Huffman codes during encoding.");
            // free output, tree, codes
            free(output);
            free_huffman_tree(root);
            free_huffman_codes(codes);
            *output_len = 0;
            return NULL;
        }
    }

    // Write any remaining bits in the buffer (padding the last byte)
    if (bit_count > 0) {
        bit_buffer <<= (8 - bit_count); // Left shift to pad with zeros at the end
        *ptr++ = bit_buffer;
    }
    
    // Update output_len to the actual size written.
    // The allocated *output_len was an estimate. The actual ptr - output might be slightly different.
    // For now, the initial *output_len calculation should be correct.
    // If we want exact, *output_len = ptr - output;

    // Cleanup
    free_huffman_tree(root);
    free_huffman_codes(codes);

    return output;
}

char* huffman_decompress(const char* input, size_t input_len, size_t* output_len) {
    if (!input || input_len == 0) {
        handle_error("Invalid input for Huffman decompression");
        *output_len = 0;
        return NULL;
    }

    // 1. Read frequency table
    if (input_len < (256 * sizeof(unsigned) + sizeof(size_t))) {
        handle_error("Input data too short for Huffman header.");
        *output_len = 0;
        return NULL;
    }
    unsigned frequencies[256];
    const char* ptr = input;
    memcpy(frequencies, ptr, 256 * sizeof(unsigned));
    ptr += (256 * sizeof(unsigned));

    size_t original_data_len;
    memcpy(&original_data_len, ptr, sizeof(size_t));
    ptr += sizeof(size_t);
    
    size_t compressed_data_offset = (256 * sizeof(unsigned)) + sizeof(size_t);


    // 2. Rebuild Huffman tree from frequencies
    // Create a min-heap with capacity equal to the number of unique characters.
    unsigned unique_chars_count = 0;
    for(int i=0; i<256; ++i) if(frequencies[i] > 0) unique_chars_count++;
    
    if (unique_chars_count == 0) {
        // This means the original file was empty or contained no encodable characters.
        // The compression might have produced minimal output based on this.
        // If original_data_len is 0, we can return an empty string.
        if (original_data_len == 0) {
            *output_len = 0;
            char* decompressed_output = (char*)malloc(1);
            if (!decompressed_output) { handle_memory_error(); return NULL; }
            decompressed_output[0] = '\0';
            return decompressed_output;
        } else {
            // Frequencies are all zero but original_data_len > 0. This is an inconsistent state.
            handle_error("Inconsistent Huffman header: no frequencies but original data length > 0.");
            *output_len = 0;
            return NULL;
        }
    }


    MinHeap* min_heap = create_min_heap(unique_chars_count > 0 ? unique_chars_count : 1); // ensure capacity >=1
    if(!min_heap){
        *output_len = 0;
        return NULL;
    }

    for (int i = 0; i < 256; ++i) {
        if (frequencies[i] > 0) {
            HuffmanNode* h_node = new_huffman_node((char)i, frequencies[i]);
            if(!h_node){
                 for(unsigned j=0; j < min_heap->size; ++j) {
                    free(min_heap->array[j]->h_node);
                    free(min_heap->array[j]);
                }
                free(min_heap->array);
                free(min_heap);
                *output_len = 0;
                return NULL;
            }
            insert_min_heap(min_heap, h_node);
        }
    }
    
    // Handle single unique character case for decompression
    // If unique_chars_count is 1, build_huffman_tree will return that single node.
    // Decompression loop needs to handle this (always taking the same path, or just repeating the char).

    HuffmanNode* root = build_huffman_tree(min_heap); // min_heap is consumed
    free(min_heap->array);
    free(min_heap);

    if (!root) {
        handle_error("Failed to rebuild Huffman tree during decompression.");
        *output_len = 0;
        return NULL;
    }

    // 3. Allocate output buffer for decompressed data
    //    The original_data_len is the exact size.
    char* decompressed_output = (char*)malloc(original_data_len + 1);
    if (!decompressed_output) {
        handle_memory_error();
        free_huffman_tree(root);
        *output_len = 0;
        return NULL;
    }

    // 4. Decode bitstream
    HuffmanNode* current_node = root;
    size_t decompressed_count = 0;
    size_t compressed_data_len = input_len - compressed_data_offset;

    for (size_t i = 0; i < compressed_data_len && decompressed_count < original_data_len; ++i) {
        unsigned char byte = ptr[i];
        for (int bit = 7; bit >= 0 && decompressed_count < original_data_len; --bit) {
            if ((byte >> bit) & 1) { // If current bit is 1
                current_node = current_node->right;
            } else { // If current bit is 0
                current_node = current_node->left;
            }

            if (!current_node) {
                // This indicates a corrupted stream or error in tree traversal
                handle_error("Error during Huffman decompression: Invalid path in tree.");
                free_huffman_tree(root);
                free(decompressed_output);
                *output_len = 0;
                return NULL;
            }

            if (!current_node->left && !current_node->right) { // Leaf node
                decompressed_output[decompressed_count++] = current_node->character;
                current_node = root; // Reset to root for next character
            }
        }
    }
    
    if (decompressed_count != original_data_len) {
        // This could happen if the compressed data was truncated or the original_data_len was wrong.
        handle_error("Mismatch between expected and actual decompressed data length.");
        // Depending on strictness, might still return partially decompressed data or fail.
        // For now, let's consider it an error.
        free_huffman_tree(root);
        free(decompressed_output);
        *output_len = 0;
        return NULL;
    }


    decompressed_output[decompressed_count] = '\0';
    *output_len = decompressed_count;

    // Cleanup
    free_huffman_tree(root);

    return decompressed_output;
}
/* RLE functions removed as Huffman coding is now implemented. */ 