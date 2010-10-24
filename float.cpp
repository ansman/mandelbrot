/*
 *  float.cpp
 *  MandelbrotOSX
 *
 *  Created by Nicklas Ansman on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "float.h"

Float::Float() {
	mpf_init(value);
}

Float::~Float() {
	mpf_clear(value);
}

Float::Float(float v) {
	mpf_init_set_d(value, (double)v);
}

Float::Float(long v) {
	mpf_init_set_si(value, v);
}

Float::Float(int v) {
	mpf_init_set_si(value, (long)v);
}

Float::Float(short v) {
	mpf_init_set_si(value, (long)v);
}

Float::Float(double v) {
	mpf_init_set_d(value, v);
}

Float::Float(const Float & reference) {
	mpf_init_set(value, reference.value);
}

double Float::to_double() const {
	return mpf_get_d(value);
}

long Float::to_long() const {
	return mpf_get_ui(value);
}

Float Float::operator+(const Float & reference) const {
	Float ret;
	mpf_add(ret.value, value, reference.value);
	return ret;
}

Float Float::operator-(const Float & reference) const {
	Float ret;
	mpf_sub(ret.value, value, reference.value);
	return ret;
}

Float Float::operator*(const Float & reference) const {
	Float ret;
	mpf_mul(ret.value, value, reference.value);
	return ret;
}


Float Float::operator/(const Float & reference) const {
	Float ret;
	mpf_div(ret.value, value, reference.value);
	return ret;
}

Float & Float::operator=(const Float & reference) {
	mpf_set(value, reference.value);
	return *this;
}

bool Float::operator<(const Float & reference) const {
	return mpf_cmp(value, reference.value) < 0;
}

bool Float::operator<=(const Float & reference) const {
	return mpf_cmp(value, reference.value) <= 0;
}

bool Float::operator==(const Float & reference) const {
	return mpf_cmp(value, reference.value) == 0;
}

bool Float::operator!=(const Float & reference) const {
	return mpf_cmp(value, reference.value) != 0;
}

bool Float::operator>(const Float & reference) const {
	return mpf_cmp(value, reference.value) > 0;
}

bool Float::operator>=(const Float & reference) const {
	return mpf_cmp(value, reference.value) >= 0;
}
