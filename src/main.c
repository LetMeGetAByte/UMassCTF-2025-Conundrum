#include <fcntl.h>
#include <openssl/sha.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *expected_hash_hex = "44290e7fab29c878e59fb71be2c0b6d1957e6fe3c70850252f8daec03c83ccce";

int is_being_debugged() {
	FILE *f = fopen("/proc/self/status", "r");
	if (!f)
		return 0;

	char line[256];
	while (fgets(line, sizeof(line), f)) {
		if (strncmp(line, "TracerPid:", 10) == 0) {
			int tracer_pid = atoi(line + 10);
			fclose(f);
			return tracer_pid != 0;
		}
	}
	fclose(f);
	return 0;
}

void decrypt(unsigned char *key, size_t key_len, unsigned char *data,
             size_t data_len) {
	for (size_t i = 0; i < data_len; i++) {
		data[i] ^= key[i % key_len];
	}
}

int main(int argc, char *argv[]) {
    if (is_being_debugged()) {
        return -1;
    }

	FILE *chall_file = fopen("chall_enc", "rb");
	if (!chall_file) {
		perror("Failed to open chall");
		return 1;
	}

	fseek(chall_file, 0, SEEK_END);
	size_t chall_len = ftell(chall_file);
	rewind(chall_file);

	unsigned char *chall_enc = malloc(chall_len);
	if (!chall_enc) {
		perror("malloc");
		fclose(chall_file);
		return 1;
	}

	fread(chall_enc, 1, chall_len, chall_file);
	fclose(chall_file);

	decrypt(expected_hash_hex, 65, chall_enc, chall_len);

	int fd = open("/tmp/payload.elf", O_CREAT | O_WRONLY, 0700);

	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256(chall_enc, chall_len, hash);

	char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		sprintf(&hash_hex[i * 2], "%02x", hash[i]);
	}

	if (strncmp(hash_hex, expected_hash_hex, SHA256_DIGEST_LENGTH * 2) != 0) {
		free(chall_enc);
		return -1;
	}

	write(fd, chall_enc, chall_len);
	close(fd);
	execl("/tmp/payload.elf", "payload.elf", argv[1], NULL);
	system("rm /tmp/payload.elf");
	exit(0);

	return 0;
}
