#include <iostream>

#include "../varint/varint.h"
#include "../groupvarint/groupvarint.h"
#include "../utils/memory_manager.h"
#include "../utils/timer.h"

using namespace std;
using namespace encodings::varint;
using namespace encodings::groupvarint;
using namespace encodings::utils;

int main(int argc, char *argv[]) {
	cout << "Performance Test" << endl;
	MemoryManager varint_mem("varint_block", 1);
	MemoryManager groupvarint_mem("groupvarint_block", 1);
	void *block_varint = varint_mem.Map(1 << 30);
	void *block_group_varint = groupvarint_mem.Map(1 << 30);

	const uint64_t test_count = 10000000;
	const uint16_t SEED = 12345;
	srand(SEED);

	array<uint64_t, 4> values;

	for (uint8_t i = 0; i < 4; i++) {
		uint16_t off = rand() % 3 + i;
		values[i] = (uint64_t) 1 << (i * 8);
	}

	VarintCodec<uint64_t> varint_codec;
	GroupVarintCodec<array<uint64_t, 4> > groupvarint_codec;

	uint64_t num_bytes_varint = 0;
	uint64_t num_bytes_group_varint = 0;
	uint64_t result = 0;
	array<uint64_t, 4> results = {0, 0, 0, 0};

	uint64_t varint_encoding_time = 0;
	uint64_t groupvarint_encoding_time = 0;
	uint64_t varint_decoding_time = 0;
	uint64_t groupvarint_decoding_time = 0;

	for (uint64_t i = 0; i < test_count; i++) {
		for (int64_t j = 0; j < 4; j++) {
			ScopedTimer varint_timer(&varint_encoding_time);
			num_bytes_varint += varint_codec.Encode(reinterpret_cast<uint8_t *>(block_varint) + num_bytes_varint, values[j]);
		}
		{
			ScopedTimer groupvarint_timer(&groupvarint_encoding_time);
			num_bytes_group_varint += groupvarint_codec.Encode(
					reinterpret_cast<uint8_t *>(block_group_varint) + num_bytes_group_varint, values);
		}
	}

	cout << "Varint encoding time: \t" << varint_encoding_time << endl;
	cout << "GroupVarint encoding time: \t" << groupvarint_encoding_time << endl;

	num_bytes_group_varint = 0;
	num_bytes_varint = 0;

	for (uint64_t i = 0; i < test_count; i++) {
		for (int64_t j = 0; j < 4; j++) {
			ScopedTimer varint_timer(&varint_decoding_time);
			num_bytes_varint += varint_codec.Decode(reinterpret_cast<uint8_t *>(block_varint) + num_bytes_varint, &result);
		}
		{
			ScopedTimer groupvarint_timer(&groupvarint_decoding_time);
			num_bytes_group_varint += groupvarint_codec.Decode(
					reinterpret_cast<uint8_t *>(block_group_varint) + num_bytes_group_varint, &results);
		}
	}

	cout << "Varint decoding time: \t" << varint_decoding_time << endl;
	cout << "GroupVarint decoding time: \t" << groupvarint_decoding_time << endl;

	return 0;
}
