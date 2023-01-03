#ifndef SHMEM_HEADER
#define SHMEM_HEADER

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pipc/errors_and_warnings.hpp>
#include <bitset>

#define SF_RDWR S_IRUSR | S_IWUSR
#define SF_RD S_IRUSR
#define SF_WR S_IWUSR

#define MF_RD PROT_READ
#define MF_WR PROT_WRITE
#define MF_RDWR PROT_READ | PROT_WRITE

namespace pipc {
	class shmem {
		private:
			char* shm_name;
			int sfd;
			int shm_flag, map_flag;
			char* map;
			size_t file_size;
			bool shm_create;
			bool issetup = false;
		public:
			shmem(char* name, size_t size, int sflag, int mflag)
			: shm_name(name), shm_flag(sflag), file_size(size), map_flag(mflag)
			{ shm_create = true; }

			shmem(char* name, int sflag, int mflag)
			: shm_name(name), shm_flag(sflag), map_flag(mflag)
			{ shm_create = false; }


			int setup() {
				if (!(shm_flag == SF_RDWR) &&
					!(shm_flag == SF_RD) &&
					!(shm_flag == SF_WR))
					return SHM_ERROR | INVALID_FLAG;

				if (!(map_flag == MF_RD) &&
					!(map_flag == MF_WR) &&
					!(map_flag == MF_RDWR))
					return SHM_ERROR | INVALID_FLAG;

				unlink(shm_name);
				if (shm_create)
					sfd = shm_open(shm_name, O_CREAT | O_RDWR, shm_flag);
				else
					sfd = shm_open(shm_name, O_RDWR, shm_flag);
				
				if (sfd < 0) {
					std::cout << "failed to open" << std::endl;
					return SHM_ERROR | FAILED_TO_OPEN;
				}
				
				if (shm_create) {
					if (ftruncate(sfd, file_size) < 0) {
						std::cout << "failed to ftruncate" << std::endl;
						return SHM_ERROR | FAILED_TO_FTRUNCATE;
					}
				} else
					file_size = lseek(sfd, 0, SEEK_END);
				
				map = (char *)mmap(NULL,
		                        file_size, // how many bytes to read
		                        map_flag, MAP_FILE|MAP_SHARED, /*flags*/ sfd, /*file*/ 
		                        0 // offset
								);
				if (map == NULL)
					return SHM_ERROR | FAILED_TO_MMAP;
			
				issetup = true;
				return SUCCESS;
			}

			int write_shm(char* buf, size_t n, size_t offset) {
				if (!issetup)
					return SHM_ERROR | NOT_SETUP;
				if (offset+strlen(buf) > file_size) // no space
					return OUT_OF_BOUNDS;
				strcpy(map + offset, buf);
				if (strcmp(buf, map+offset) != 0) // check correctness
					return BAD_WRITE;
				return SUCCESS;
			}

			int read_shm(char* buf, size_t n, size_t offset) {
				if (!issetup)
					return SHM_ERROR | NOT_SETUP;
				if (offset + n > file_size)
					n = file_size - offset;
				strcpy(buf, map+offset);
				return SUCCESS;
			}

			int resize_shm(size_t new_size) {
				if (!issetup)
					return SHM_ERROR | NOT_SETUP;
				if (ftruncate(sfd, new_size) < 0)
					return SHM_ERROR | FAILED_TO_FTRUNCATE;
				if (munmap(map, file_size) < 0)
					return SHM_ERROR | FAILED_TO_MUNMAP;
				char* new_map = (char*)mmap(NULL,
			                        new_size, // how many bytes to read
			                        map_flag, MAP_FILE|MAP_SHARED, /*flags*/ sfd, /*file*/ 
			                        0 // offset
									);
				if (new_map == NULL)
					return SHM_ERROR | FAILED_TO_MMAP;
				map = new_map;
				return SUCCESS;
			}

			~shmem() {
				//if ( < 0)
				//	shm_unlink(shm_name);
				if (map != NULL)
					munmap(map, file_size);
				if (!(sfd < 0))
					close(sfd);
			}
	};
}

#endif