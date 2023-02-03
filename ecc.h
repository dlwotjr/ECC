#pragma once
#include <cstdio>
#include<iostream>
typedef unsigned int uint32;

typedef struct bigint {
	uint32 bigInt[8];
	uint32 carry;
}bigint;

typedef struct bigintEcc {
	bigint x;
	bigint y;
}bigintEcc;