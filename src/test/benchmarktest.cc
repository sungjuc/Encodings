#include <iostream>

#include "../varint/varint.h"

using namespace std;
using namespace varint;

int main(int argc, char *argv[]) {
	cout << "Benchmark Test" << endl;
	void *block = malloc(2 << 10);
	VarintCodec<uint64_t> varint_codec;

	uint64_t value = 1024 * 1024 * 1024;
	uint64_t num_bytes = 0;
	uint64_t result = 0;
	num_bytes += varint_codec.Encode(reinterpret_cast<uint8_t *>(block), value);
	varint_codec.Decode(reinterpret_cast<uint8_t *>(block), &result);

	cout << "Bytes: " << num_bytes << endl;
	cout << "Value: " << value << " Result: " << result << endl;
	return 0;
}
