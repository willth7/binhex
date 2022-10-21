//   Copyright 2022 Will Thomas
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int8_t binhex(uint8_t b) {
	b &= 15;
	if (b == 0) {
		return '0';
	}
	else if (b == 1) {
		return '1';
	}
	else if (b == 2) {
		return '2';
	}
	else if (b == 3) {
		return '3';
	}
	else if (b == 4) {
		return '4';
	}
	else if (b == 5) {
		return '5';
	}
	else if (b == 6) {
		return '6';
	}
	else if (b == 7) {
		return '7';
	}
	else if (b == 8) {
		return '8';
	}
	else if (b == 9) {
		return '9';
	}
	else if (b == 10) {
		return 'a';
	}
	else if (b == 11) {
		return 'b';
	}
	else if (b == 12) {
		return 'c';
	}
	else if (b == 13) {
		return 'd';
	}
	else if (b == 14) {
		return 'e';
	}
	else if (b == 15) {
		return 'f';
	}
}

int16_t main(int argc, char** argv) {
	if (argc != 3) {
		printf("usage: binhex [binary.bin] [hexadecimal.hex]\n");
		return -1;
	}
	int32_t fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		printf("failed to open file '%s'\n", argv[1]);
		return -1;
	}
	
	struct stat fs;
	fstat(fd, &fs);
	
	uint64_t binsz = fs.st_size;
	uint8_t* bin = mmap(0, binsz, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	
	fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
	if (fd == -1) {
		printf("failed to create file '%s'\n", argv[2]);
		munmap(bin, binsz);
		return -1;
	}
	uint64_t hexsz = (binsz * 4) + (binsz / 8);
	ftruncate(fd, hexsz);
	uint8_t* hex = mmap(0, hexsz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	for (uint64_t i = 0; i < binsz; i++) {
		hex[(i * 4) + (i / 8) + 0] = binhex(bin[i] >> 4);
		hex[(i * 4) + (i / 8) + 1] = binhex(bin[i]);
		hex[(i * 4) + (i / 8) + 2] = ',';
		hex[(i * 4) + (i / 8) + 3] = ' ';
		if (i % 8 == 7) {
			hex[(i * 4) + (i / 8) + 4] = '\n';
		}
	}
	
	munmap(bin, binsz);
	munmap(hex, hexsz);
	close(fd);
	return 0;
}
