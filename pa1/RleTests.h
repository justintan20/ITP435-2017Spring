#pragma once
#include "MiniCppUnit.hpp"
#include "RleData.h"
#include <iostream>
#include <iomanip>

// Helper functions
bool buffersAreSame(const char* expected, const char* actual, size_t size);
void runCompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);
void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);

// IMPORTANT NOTE --
// The tests all use "sizeof" to find the size of the input
// buffer. However, remember that this only works because the
// character arrays are statically allocated.
// If you just have a pointer to a C-style string, "sizeof"
// will return the size of the pointer type (either 4 or 8).
class CompressionTests : public TestFixture<CompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(CompressionTests, "Testing Compression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive runs test");
		// TODO: Add more Compression test cases
        TEST_CASE_DESCRIBE(testLongPositiveRuns, "Long positive runs test");
        TEST_CASE_DESCRIBE(testBasicNegativeRuns, "Basic negative runs test");
        TEST_CASE_DESCRIBE(testAlternatingRuns, "Alternating runs test");
        TEST_CASE_DESCRIBE(testLongerThanMaxRuns, "Longer than max runs test");
        TEST_CASE_DESCRIBE(testCrazyLongRuns, "Crazy long runs test");
        TEST_CASE_DESCRIBE(testLongAlternatingRuns, "Long Alternating runs test");
        TEST_CASE_DESCRIBE(testLongerThanMaxNegativeRuns, "Longer than max negative runs test");
        TEST_CASE_DESCRIBE(testEndZeroRuns, "Ending in zero runs test");
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
			"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
    
    void testLongPositiveRuns()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x64" "a";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testBasicNegativeRuns()
    {
        char test[] = "qwertyuiopasdfghjklzxcvbnm";
        char expected[] = "\xe6" "qwertyuiopasdfghjklzxcvbnm";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testAlternatingRuns()
    {
        char test[] = "aaaabbcbcabbbbbbbbbccaaaabcabaccaabcabcabbbcaa";
        char expected[] = "\x04" "a" "\x02" "b" "\xfc" "cbca" "\x09" "b" "\x02" "c" "\x04" "a" "\xfb" "bcaba" "\x02" "c" "\x02" "a" "\xfa" "bcabca" "\x03" "b" "\x01" "c" "\x02" "a";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongerThanMaxRuns()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7f" "a" "\x17" "a";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testCrazyLongRuns()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        char expected[] = "\x7f" "a" "\x7f" "a" "\x7f" "a" "\x77" "a";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongAlternatingRuns()
    {
        char test[] = "aaabbbbccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccddd";
        char expected[] = "\x03" "a" "\x04" "b" "\x7f" "c" "\x17" "c" "\x03" "d";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongerThanMaxNegativeRuns()
    {
        char test[] = "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf";
        char expected[] = "\x81" "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasd" "\xdf" "fasdfasdfasdfasdfasdfasdfasdfasdf";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testEndZeroRuns()
    {
        char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" "\x00\x00";
        char expected[] = "\x7f" "a" "\x7b" "a" "\x00\x00";
        runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
};

class DecompressionTests : public TestFixture<DecompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(DecompressionTests, "Testing Decompression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive run test");
		// TODO: Add more Decompression test  cases
        TEST_CASE_DESCRIBE(testBasicNegativeRuns, "Basic negative run test");
        TEST_CASE_DESCRIBE(testAlternatingRuns, "Alternating run test");
        TEST_CASE_DESCRIBE(testLongPositiveRuns, "Long positive run test");
        TEST_CASE_DESCRIBE(testLongNegativeRuns, "Long negative run test");
        
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
    
    void testBasicNegativeRuns()
    {
        char test[] = "\xe6" "qwertyuiopasdfghjklzxcvbnm";
        char expected[] = "qwertyuiopasdfghjklzxcvbnm";
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testAlternatingRuns()
    {
        char test[] = "\x04" "a" "\x02" "b" "\xfc" "cbca" "\x09" "b" "\x02" "c" "\x04" "a" "\xfb" "bcaba" "\x02" "c" "\x02" "a" "\xfa" "bcabca" "\x03" "b" "\x01" "c" "\x02" "a";
        char expected[] = "aaaabbcbcabbbbbbbbbccaaaabcabaccaabcabcabbbcaa";
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongPositiveRuns()
    {
        char test[] = "\x7f" "x" "\x7f" "x" "\x7f" "x" "\x13" "x";
        char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
    
    void testLongNegativeRuns()
    {
        char test[] = "\x81" "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasd" "\xdf" "fasdfasdfasdfasdfasdfasdfasdfasdf";
        char expected[] = "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf";
        runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
    }
};

REGISTER_FIXTURE(CompressionTests);
REGISTER_FIXTURE(DecompressionTests);

bool buffersAreSame(const char* expected, const char* actual, size_t size)
{
	if (actual == nullptr)
	{
		std::cerr << std::endl << "Actual data is null." << std::endl;
		return false;
	}

	bool retVal = true;

	for (size_t i = 0; i < size; i++)
	{
		if (expected[i] != actual[i])
		{
			retVal = false;
			std::cerr << std::endl
				<< "Expected 0x" << std::hex << static_cast<unsigned>(expected[i])
				<< " at index " << std::dec << i << " but saw 0x"
				<< std::hex << static_cast<unsigned>(actual[i]) << std::endl;
			break;
		}
	}

	return retVal;
}

void runCompressionTest(char* test, size_t testSize, 
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Compress(test, testSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}

void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Decompress(test, testSize, expectedSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}
