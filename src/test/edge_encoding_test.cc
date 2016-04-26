//
// Created by SungJu Cho on 4/21/16.
//
#include <iostream>
#include "../varint/varint.h"
#include "../groupvarint/groupvarint.h"
#include "../utils/memory_manager.h"
#include "../utils/timer.h"

using namespace std;
using namespace encodings::common;
using namespace encodings::varint;
using namespace encodings::groupvarint;
using namespace encodings::utils;

int main(int argc, char *argv[]) {
	cout << "Edge Encoding Test" << endl;

	cout << "Number of linkages: " << (uint64_t) kNLinkages << endl;

	MemoryManager varint_mem("varint_block", 1);
	MemoryManager groupvarint_mem("groupvarint_block", 1);
	uint8_t *block_varint = reinterpret_cast<uint8_t *>(varint_mem.Map(1 << 30));
	uint8_t *block_group_varint = reinterpret_cast<uint8_t *>(groupvarint_mem.Map(1 << 30));

	const uint16_t SEED = 12345;
	srand(SEED);

	uint64_t varint_encoding_time = 0;
	uint64_t groupvarint_encoding_time = 0;
	uint64_t varint_decoding_time = 0;
	uint64_t groupvarint_decoding_time = 0;

	uint64_t varint_encoding_offset = 0;
	uint64_t varint_decoding_offset = 0;

	uint64_t groupvarint_encoding_offset = 0;
	uint64_t groupvarint_decoding_offset = 0;

	for (uint64_t k = 0; k < 10000; ++k) {
		array<Id, kNLinkages + 1> values;

		for (uint8_t i = 0; i < kNLinkages + 1; i++) {
			uint16_t off = rand() % 4 + 1;
			values[i] = (uint64_t) 1 << (off * 7);
		}

		Id id = (Id) 1 << ((rand() % 3 + 2) * 7);

		EdgeWithId edge(values[0], values[1], values[2], id);

		EdgeVarintCodec edgevarint_codec;
		EdgeGroupVarintCodec edgegroupvarint_codec;

		{
			ScopedTimer varint_timer(&varint_encoding_time);
			varint_encoding_offset += edgevarint_codec.Encode(block_varint + varint_encoding_offset, edge);
		}
		{
			ScopedTimer varint_timer(&groupvarint_encoding_time);
			groupvarint_encoding_offset += edgegroupvarint_codec.Encode(block_group_varint + groupvarint_encoding_offset,
			                                                            edge);
		}

		EdgeWithId result;
		EdgeWithId result_groupvarint;

		{
			ScopedTimer varint_timer(&varint_decoding_time);
			varint_decoding_offset += edgevarint_codec.Decode(block_varint + varint_decoding_offset, &result);
		}
		{
			ScopedTimer varint_timer(&groupvarint_decoding_time);
			groupvarint_decoding_offset += edgegroupvarint_codec.Decode(block_group_varint + groupvarint_decoding_offset,
			                                                            &result_groupvarint);
		}


		cout << "Original edge: " << edge.link.linkages[kSubject] << " : "
		<< edge.link.linkages[kPredicate] << " : "
		<< edge.link.linkages[kObject] << " : "
		<< edge.id << endl;

		cout << "Result edge: " << result.link.linkages[kSubject] << " : "
		<< result.link.linkages[kPredicate] << " : "
		<< result.link.linkages[kObject] << " : "
		<< result.id << endl;

		cout << "Result edge: " << result_groupvarint.link.linkages[kSubject] << " : "
		<< result_groupvarint.link.linkages[kPredicate] << " : "
		<< result_groupvarint.link.linkages[kObject] << " : "
		<< result_groupvarint.id << endl;


		cout << "Varint encoding offset: " << varint_encoding_offset << endl;
		cout << "Varint decoding offset: " << varint_decoding_offset << endl;
		cout << "GroupVarint encoding offset: " << groupvarint_encoding_offset << endl;
		cout << "GroupVarint decoding offset: " << groupvarint_decoding_offset << endl;
	}
	cout << "Varint encoding time: \t" << varint_encoding_time << endl;
	cout << "GroupVarint encoding time: \t" << groupvarint_encoding_time << endl;

	cout << "Varint decoding time: \t" << varint_decoding_time << endl;
	cout << "GroupVarint decoding time: \t" << groupvarint_decoding_time << endl;

	return 0;
}
