//
// Created by SungJu Cho on 4/21/16.
//

#include <stdint.h>
#include <array>

#ifndef ENCODINGS_TYPES_H
#define ENCODINGS_TYPES_H

#endif //ENCODINGS_TYPES_H
namespace encodings {
namespace common {
const uint8_t kNLinkages = 3;
typedef uint64_t Id;
const Id kInvalidId = 0;

enum Linkage {
	kSubject, kPredicate, kObject
};

struct Link {
	Link(const Id subject, const Id predicate, const Id object) : linkages() {
		linkages[kSubject] = subject;
		linkages[kPredicate] = predicate;
		linkages[kObject] = object;
	};

	Id GetLinkage(Linkage linkage) const {
		return linkages[linkage];
	}

	std::array<Id, 3> linkages;
};

struct EdgeWithId {
	explicit EdgeWithId() : link(kInvalidId, kInvalidId, kInvalidId), id(kInvalidId) { }

	EdgeWithId(const Link link, const Id id) : link(link), id(id) { }

	EdgeWithId(const Id subject, const Id predicate, const Id object, const Id id) : link(subject, predicate, object),
	                                                                                 id(id) { }

	Link link;
	Id id;
};
}  // end namespace common
}  // end namespace encodings