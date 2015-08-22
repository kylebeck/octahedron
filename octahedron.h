#ifndef Octahedron_H
#define Octahedron_H

#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
	#include <pins_arduino.h>
#endif

class vector {
	public:
		vector(uint8_t i, int8_t l);
		~vector();
		uint8_t begin(void) const;
		uint8_t end(void) const;
	private:
		const uint8_t start;
		const int8_t length;
};

class polygon {
	public:
		polygon(int8_t sides[], uint8_t sideCount, uint8_t edgeLength = 15);
		~polygon();
		uint8_t length();
		uint8_t parIndex(uint8_t count);
		uint8_t serIndex(uint8_t count);
	private:
		int8_t* _sides;
		const uint8_t _edgeLength;
		const uint8_t _sideCount;
 };
 
 #endif