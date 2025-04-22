#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void decrypt(unsigned char *key, size_t key_len, unsigned char *data,
             size_t data_len) {
	for (size_t i = 0; i < data_len; i++) {
		data[i] ^= key[i % key_len];
	}
}

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

char *get_flag() {
	FILE *flag_file = fopen("flag.txt", "rb");
	if (!flag_file)
		return NULL;

	fseek(flag_file, 0, SEEK_END);
	size_t flag_length = ftell(flag_file);
	rewind(flag_file);

	char *flag = malloc(flag_length + 1);
	if (!flag) {
		fclose(flag_file);
		return NULL;
	}

	if (fread(flag, 1, flag_length, flag_file) != flag_length) {
		free(flag);
		fclose(flag_file);
		return NULL;
	}

	fclose(flag_file);
	return flag;
}

unsigned char *gen_key(unsigned char j) {
    unsigned char *key = malloc(1024);
    if (!key)
        return NULL;
    
    for (size_t i = 0; i < 1024; i++) {
        key[i] = j;
        j = ((j << 1) + j) & 0xFF;
    }

    return key;
}

int main(int argc, char *argv[]) {
	int store[2];
	pipe(store);
	dup2(store[1], STDOUT_FILENO);
	close(store[1]);
	if (!is_being_debugged()) {
		char *flag = get_flag();
		decrypt(gen_key(*argv[1]), 1024, (unsigned char *) flag, 47);
		printf("%s\n", flag);
	} else {
		char *flag = get_flag();
		printf("%s\n", flag);
	}
	close(store[0]);
	return 0;
}
