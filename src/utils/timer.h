//
// Created by SungJu Cho on 4/21/16.
//

#include <stdint.h>
#include <chrono>

#ifndef ENCODINGS_TIMER_H
#define ENCODINGS_TIMER_H

#endif //ENCODINGS_TIMER_H
namespace encodings {
namespace utils {

struct ScopedTimer {
	explicit ScopedTimer(uint64_t *const elapsed_time) : start(std::chrono::steady_clock::now()),
	                                                     elapsed_time(elapsed_time) { }

	~ScopedTimer() {
		uint64_t delta = std::chrono::duration_cast<std::chrono::nanoseconds>(
				std::chrono::steady_clock::now() - start).count();
		if (elapsed_time != nullptr) {
			*elapsed_time += delta;
		}
	}
	const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> start;
	uint64_t* const elapsed_time;
};

} // end namespace utils
} // end namespace encodigs