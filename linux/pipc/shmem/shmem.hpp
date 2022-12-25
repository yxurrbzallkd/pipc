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

namespace pipc {
	class shmem {
		private:
			char* shm_name;
			int sfd;
			int shm_mode, map_flag;
			char* map;
			size_t file_size;
			bool shm_create;
			bool issetup = false;
		public:
			shmem(char* name, size_t size, int sflag, int mflag)
			: shm_name(name), shm_mode(sflag), file_size(size), map_flag(mflag)
			{ shm_create = true; }

			shmem(char* name, int sflag, int mflag)
			: shm_name(name), shm_mode(sflag), map_flag(mflag)
			{ shm_create = false; }


			int setup() {
				int flag = 0;
				if (shm_mode == O_RDWR)
					flag = S_IRUSR | S_IWUSR;
				else if (shm_mode == O_RDONLY)
					flag = S_IRUSR;
				else if (shm_mode == O_WRONLY)
					flag = S_IWUSR;
				else
					return SHM_ERROR | INVALID_FLAG;
				unlink(shm_name);
				shm_mode = flag;
				if (shm_create)
					sfd = sfd = shm_open(shm_name, O_CREAT | O_RDWR, shm_mode);
				else
					sfd = shm_open(shm_name, O_RDWR, shm_mode);
				if (sfd < 0)
					return SHM_ERROR | FAILED_TO_OPEN;
				
				if (shm_create) {
					if (ftruncate(sfd, file_size) < 0)
						return SHM_ERROR | FAILED_TO_FTRUNCATE;
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