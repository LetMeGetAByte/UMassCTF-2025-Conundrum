// DO NOT INCLUDE IN PUBLIC FACING FILES

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void encrypt(unsigned char *key, size_t key_len, unsigned char *data,
             size_t data_len) {
	for (size_t i = 0; i < data_len; i++) {
		data[i] ^= key[i % key_len];
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
		return 1;
	}

	FILE *chall = fopen(argv[1], "rb");
	if (!chall) {
		perror("Failed to open input file");
		return 1;
	}

	printf("Packing: %s\n", argv[1]);
	fseek(chall, 0, SEEK_END);
	size_t size = ftell(chall);
	rewind(chall);
	printf("Size: %zu\n", size);

	FILE *chall_enc = fopen(argv[2], "wb");
	if (!chall_enc) {
		perror("Failed to open output file");
		fclose(chall);
		return 1;
	}

	unsigned char *data = (unsigned char *) malloc(size);
	if (!data) {
		perror("Memory allocation failed");
		fclose(chall);
		fclose(chall_enc);
		return 1;
	}

	fread(data, 1, size, chall);
	fclose(chall);

	const char *hash = "44290e7fab29c878e59fb71be2c0b6d1957e6fe3c70850252f8daec03c83ccce";

	encrypt(hash, 65, data, size);

	fwrite(data, 1, size, chall_enc);
	fclose(chall_enc);
	free(data);
}
