#pragma once

//
// types.h
//
// Miscellaneous types and defines used internally.
//

#define ST_POOL_TAG 'UTPS'

enum class ST_PAGEABLE
{
	YES = 0,
	NO
};

//
// Type-safety when passing around lower case device paths.
// Same definition as UNICODE_STRING so they can be cast between.
//
typedef struct tag_LOWER_UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWCH   Buffer;
}
LOWER_UNICODE_STRING;

enum ST_PROCESS_SPLIT_STATUS
{
	// Traffic should be split.
	ST_PROCESS_SPLIT_STATUS_ON_BY_CONFIG = 0,

	// Traffic should be split.
	ST_PROCESS_SPLIT_STATUS_ON_BY_INHERITANCE,

	// Traffic should not be split.
	ST_PROCESS_SPLIT_STATUS_OFF
};
