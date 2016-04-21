//
// Created by SungJu Cho on 4/21/16.
//
#include <stdint.h>
#include <stddef.h>
#include <string>

#ifndef ENCODINGS_MEMORY_MANAGER_H
#define ENCODINGS_MEMORY_MANAGER_H

#endif //ENCODINGS_MEMORY_MANAGER_H

#define PATH "/tmp/"

namespace encodings {
namespace utils {

class MemoryManager {
public:
	/**
	 * MemoryManager constructor that creates only files. Actual memory mapping doesn't happens.
	 */
	explicit MemoryManager(std::string file_name, size_t file_size);

	/**
	 * Maps memory region with a given size.
	 */
	void* Map(size_t size);

	/**
	 * Unmaps the memory region. Just unmap memory region.
	 */
	void Unmap();

	/**
	 * Deletes underlying files and file descriptors.
	 */
	~MemoryManager();

private:
	int fd_;
	void* map_;
	size_t file_size_;
	bool is_mapped_;

	void IncreseFile(size_t size);
};

} // end namespace utils
} // end namespace encodings