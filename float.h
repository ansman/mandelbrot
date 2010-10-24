/*
 *  float.h
 *  MandelbrotOSX
 *
 *  Created by Nicklas Ansman on 23/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FLOAT_H_
#define FLOAT_H_

#include "gmp.h"

struct Float {
	mpf_t value;
	
	Float();
	~Float();
	Float(double);
	Float(float);
	Float(long);
	Float(int);
	Float(short);
	Float(const Float &);
	
	double to_double() const;
	long to_long() const;
	
	Float operator+(const Float &) const;
	Float operator-(const Float &) const;
	Float operator*(const Float &) const;
	Float operator/(const Float &) const;
	Float & operator=(const Float &);
	bool operator<(const Float &) const;
	bool operator<=(const Float &) const;
	bool operator==(const Float &) const;
	bool operator!=(const Float &) const;
	bool operator>(const Float &) const;
	bool operator>=(const Float &) const;
};

inline Float operator+(double a, const Float &b) {
	return b+a;
}

inline Float operator+(long a, const Float &b) {
	return b+a;
}

inline Float operator-(double a, const Float &b) {
	return b-a;
}

inline Float operator-(long a, const Float &b) {
	return b-a;
}

inline Float operator*(double a, const Float &b) {
	return b*a;
}

inline Float operator*(long a, const Float &b) {
	return b*a;
}

inline Float operator/(double a, const Float &b) {
	return b/a;
}

inline Float operator/(long a, const Float &b) {
	return b/a;
}

inline bool operator<(double a, const Float &b) {
	return b >= a;
}

inline bool operator<(long a, const Float &b) {
	return b >= a;
}

inline bool operator<=(double a, const Float &b) {
	return b > a;
}

inline bool operator<=(long a, const Float &b) {
	return b > a;
}

inline bool operator==(double a, const Float &b) {
	return b == a;
}

inline bool operator==(long a, const Float &b) {
	return b == a;
}

inline bool operator!=(double a, const Float &b) {
	return b != a;
}

inline bool operator!=(long a, const Float &b) {
	return b != a;
}

inline bool operator>(double a, const Float &b) {
	return b <= a;
}

inline bool operator>(long a, const Float &b) {
	return b <= a;
}

inline bool operator>=(double a, const Float &b) {
	return b < a;
}

inline bool operator>=(long a, const Float &b) {
	return b < a;
}


#endif /* FLOAT_H_ */
