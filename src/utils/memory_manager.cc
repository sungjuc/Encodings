//
// Created by SungJu Cho on 4/21/16.
//

#include <sys/fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "memory_manager.h"

namespace encodings {
namespace utils {

using namespace std;

MemoryManager::MemoryManager(string file_name, size_t file_size) : file_size_(0), is_mapped_(false) {
	string file_path = PATH + file_name;
	fd_ = open(file_path.data(), O_RDWR | O_CREAT | O_TRUNC, (mode_t) 0600);
	if (fd_ == -1) {
		perror("Error opening file for writing");
		exit(EXIT_FAILURE);
	}

	IncreseFile(file_size);
}
MemoryManager::~MemoryManager() {
	if (!is_mapped_) {
		Unmap();
	}

	close(fd_);
}

void* MemoryManager::Map(size_t size) {
	IncreseFile(size);
	map_ = mmap(0, file_size_, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
	is_mapped_ = true;

	return map_;
}

void MemoryManager::Unmap() {
	if (munmap(map_, file_size_) == -1) {
		perror("Error un-mmapping the file");
	}
	is_mapped_ = false;
}

void MemoryManager::IncreseFile(size_t size) {
	if (size > file_size_) {
		off_t result = lseek(fd_, size - 1, SEEK_SET);
		if (result == -1) {
			close(fd_);
			perror("Error calling lseek() to 'stretch' the file");
			exit(EXIT_FAILURE);
		}

		result = write(fd_, "", 1);
		if (result != 1) {
			close(fd_);
			perror("Error writing last byte of the file");
			exit(EXIT_FAILURE);
		}
		file_size_ = size;
	}
}

} // end namespace utils
} // end namespace encodings