#include "tinythreads.h"
#include <stdbool.h>

void writeChar(char ch, int pos)
{
	uint8_t *lcdaddr = (uint8_t *) 0xec;	//The Address to the first position on the LCD.
	uint8_t mask;							//Mask to get only the nibble of a value.
	uint8_t nibbleNumber = 0x0;				//Nibble the number that is sent to the LCD.
	
	//SCC Table with the numbers from 0 to 9.
	uint16_t sccTable[10] = {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51}; 
	
	// Check if position is outside or not.
	if (pos < 0 || pos > 5) {
		return;
	}
	
	uint16_t number = 0x0;	// The number to print.
	
	// Check if it is a 0 to 9.
	if (ch >= '0' || ch <= '9')
	{
		number = sccTable[ch - '0'];	//Get the number out of the array.
	}
	
	lcdaddr += pos >> 1;	//Point to the right position. Divide by 2 you can say.
	
	//Check if it is odd or even position.
	if (pos % 2 == 0)
	{
		mask = 0xf0;
	}
	else
	{
		mask = 0x0f;
	}
	
	//Will place out the nibbles on the right LCD address for the number.
	for (int i = 0; i < 4; i++ )
	{
		//Masking the smallest byte.
		nibbleNumber = number & 0xf;
		number = number >> 4;
		
		if(pos % 2 != 0)	//Check position.
		{
			nibbleNumber = nibbleNumber << 4;	//Shift the nibble to the right pos.
		}
		
		*lcdaddr = (*lcdaddr & mask) | nibbleNumber;	//Send the nibble.
		lcdaddr += 5;
	}
}

bool is_prime(long i)
{
	//Loop all the numbers under i and try to divide it with them.
	for(int n = 2; n < i; n++)
	{
		if(i % n == 0)
		{
			return false;
		}
		
	}
	return true;
	
}

void printAt(long num, int pos) {
    int pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    pp++;
    writeChar( num % 10 + '0', pp);
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
            printAt(n, pos);
			yield();
        }
    }
}

int main() {
    spawn(computePrimes, 0);
    computePrimes(3);
}
