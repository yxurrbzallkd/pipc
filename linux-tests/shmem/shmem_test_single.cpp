#include <stdio.h>
#include <iostream>
#include <pipc/shmem/shmem.hpp>
#include <bitset>
#include <stdio.h>

using namespace std;

int main(int argc, char* argv[]) {
	pipc::shmem s((char*)"/shm_object", 1024, SF_RDWR, MF_RDWR);
	if (s.setup() != SUCCESS) {
		perror("failed setup");
		return -1;
	}
	char msg[] = "hello world!";
	if (s.write_shm(msg, 13, 0) != SUCCESS)
		return -2;
	char content[1025];
	if (s.read_shm(content, 1024, 0) != SUCCESS)
		return -2;
	content[1024] = '\0';
	std::cout << content << std::endl;
	return 0;
}
