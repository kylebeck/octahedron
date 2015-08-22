#include "Octahedron.h"

//const uint8_t edgeLength = 15;

vector::vector(uint8_t i, int8_t l) : start(i), length(l) {}

vector::~vector() {}

uint8_t vector::begin() const {
	return (start-1)*length;
}

uint8_t vector::end() const {
	return (start)*length-1;
}

polygon::polygon( int8_t sides[], uint8_t sideCount, uint8_t edgeLength) : _sides(sides), _sideCount(sideCount), _edgeLength(edgeLength) {}

polygon::~polygon() {}

uint8_t polygon::length() {
	return _sideCount*_edgeLength;
}

uint8_t polygon::parIndex(uint8_t count) {
	int8_t side = _sides[count%_sideCount];	// Select the side this LED is on
	uint8_t offset = count/_sideCount;			// Determine the LED index along each side

	if (side > 0) {
		return vector(side,_edgeLength).begin()+offset;
	}
	else {
		return vector(abs(side),_edgeLength).end()-offset;
	}
}

uint8_t polygon::serIndex(uint8_t count) {
	uint8_t sNum = count/_edgeLength;			// Determine which side number this LED is on
	int8_t side = _sides[sNum];					// Select the side this LED is on
	uint8_t offset = count-sNum*_edgeLength;	// Calculate the LED index along the side
	
	if (side > 0) {
		return vector(_sides[sNum],_edgeLength).begin()+offset;
	}
	else {
		return vector(abs(_sides[sNum]),_edgeLength).end()-offset;
	}
}