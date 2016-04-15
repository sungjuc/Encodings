#include <iostream>
#include <array>

#include "../varint/varint.h"
#include "../groupvarint/groupvarint.h"

using namespace std;
using namespace varint;
using namespace groupvarint;

int main(int argc, char *argv[]) {
	uint64_t SEED = 12343335;
	srand(SEED);
	cout << "Benchmark Test" << endl;
	void *block_varint = malloc(2 << 10);
	void *block_group_varint = malloc(2 << 10);
	VarintCodec<uint64_t> varint_codec;
	GroupVarintCodec<array<uint64_t, 4> > groupvarint_codec;

	array<uint64_t, 4> values;

	for (uint8_t i = 0; i < 4; i++) {
		values[i] = (uint64_t) 1 << (i * 8);
	}

	uint64_t num_bytes_varint = 0;
	uint64_t num_bytes_group_varint = 0;
	uint64_t result = 0;
	array<uint64_t, 4> results = {0, 0, 0, 0};

	for (uint8_t i = 0; i < 4; i++) {
		num_bytes_varint += varint_codec.Encode(reinterpret_cast<uint8_t *>(block_varint) + num_bytes_varint, values[i]);
		cout << "Varint Bytes: " << num_bytes_varint << endl;
	}

	num_bytes_group_varint += groupvarint_codec.Encode(reinterpret_cast<uint8_t *>(block_group_varint), values);
	cout << "GroupVarint Bytes: " << num_bytes_group_varint << endl;

	uint64_t num_bytes_read = 0;
	for (uint8_t i = 0; i < 4; i++) {
		num_bytes_read += varint_codec.Decode(reinterpret_cast<uint8_t *>(block_varint) + num_bytes_read, &result);
		cout << "Value: " << values[i] << " Result: " << result << endl;
	}

	groupvarint_codec.Decode(reinterpret_cast<uint8_t *>(block_group_varint), &results);

	for (uint8_t i = 0; i < 4; i++) {
		cout << "Value: " << values[i] << " Result: " << results[i] << endl;
	}

	return 0;
};
