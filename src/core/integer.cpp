/*********************************************************************
 * Factorial Number System
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 *
 ********************************************************************/

#include "integer.hpp"

/// PUBLIC

integer::integer() {
	initialized = false;
	init_ui(static_cast<int32_t>(0));
}

integer::integer(int32_t i) {
	initialized = false;
	init_si(static_cast<int32_t>(i));
}

integer::integer(const char *s, int32_t base) {
	initialized = false;
	init(s, base);
}

integer::integer(const string& s, int32_t base) {
	initialized = false;
	init(s, base);
}

integer::integer(const integer& i) {
	initialized = false;
	*this = i;
}

integer::~integer() {
	clear();
}

/* ALLOC AND DEALLOC */

void integer::init() {
	if (!is_initialized()) {
		mpz_init(val);
		initialized = true;
	}
}

void integer::init_si(int32_t i) {
	init();
	set_si(i);
}

void integer::init_ui(uint32_t i) {
	init();
	set_ui(i);
}

void integer::init(const char *s, int32_t base) {
	init();
	set(s, base);
}

void integer::init(const string& s, int32_t base) {
	init();
	set(s, base);
}

void integer::init(const integer& i) {
	if (i.is_initialized()) {
		init();
		set(i);
	}
}

void integer::clear() {
	if (is_initialized()) {
		mpz_clear(val);
		initialized = false;
	}
}

/* SET VALUE */

void integer::set_si(int32_t i)					{ mpz_set_si(val, i); }
void integer::set_ui(uint32_t i)				{ mpz_set_ui(val, i); }
void integer::set(const char *s, int32_t b)		{ mpz_set_str(val, s, b); }
void integer::set(const string& s, int32_t b)	{ mpz_set_str(val, s.c_str(), b); }
void integer::set(const integer& i)				{ mpz_set(val, i.val); }

/* OPERATORS */

integer& integer::operator= (int32_t i) {
	if (is_initialized()) {
		set_si(static_cast<int64_t>(i));
	}
	else {
		init(static_cast<int64_t>(i));
	}
	return *this;
}

integer& integer::operator= (const char *s) {
	if (is_initialized()) {
		set(s);
	}
	else {
		init(s);
	}
	return *this;
}

integer& integer::operator= (const string& s) {
	if (is_initialized()) {
		set(s);
	}
	else {
		init(s);
	}
	return *this;
}

integer& integer::operator= (const integer& i) {
	if (i.is_initialized()) {
		if (is_initialized()) {
			set(i);
		}
		else {
			init(i);
		}
	}
	return *this;
}

bool integer::operator== (int32_t i) const 			{ 				return mpz_cmp_si(val, i) == 0; }
bool integer::operator== (const char *s) const		{ integer k(s);	return mpz_cmp(val, k.val) == 0; }
bool integer::operator== (const string& s) const	{ integer k(s);	return mpz_cmp(val, k.val) == 0; }
bool integer::operator== (const integer& i) const 	{ 				return mpz_cmp(val, i.val) == 0; }

bool integer::operator!= (int32_t i) const			{ return not (*this == i); }
bool integer::operator!= (const char *s) const		{ return not (*this == s); }
bool integer::operator!= (const string& s) const	{ return not (*this == s); }
bool integer::operator!= (const integer& i) const	{ return not (*this == i); }

bool integer::operator< (int32_t i) const 			{ 				return mpz_cmp_si(val, i)  < 0; }
bool integer::operator< (const char *s) const		{ integer k(s);	return mpz_cmp(val, k.val) < 0; }
bool integer::operator< (const string& s) const		{ integer k(s); return mpz_cmp(val, k.val) < 0; }
bool integer::operator< (const integer& i) const 	{ 				return mpz_cmp(val, i.val) < 0; }

bool integer::operator<= (int32_t i) const 			{ 				return mpz_cmp_si(val, i)  <= 0; }
bool integer::operator<= (const char *s) const		{ integer k(s); return mpz_cmp(val, k.val) <= 0; }
bool integer::operator<= (const string& s) const	{ integer k(s); return mpz_cmp(val, k.val) <= 0; }
bool integer::operator<= (const integer& i) const 	{ 				return mpz_cmp(val, i.val) <= 0; }

bool integer::operator> (int32_t i) const 			{ 				return mpz_cmp_si(val, i)  > 0; }
bool integer::operator> (const char *s) const		{ integer k(s); return mpz_cmp(val, k.val) > 0; }
bool integer::operator> (const string& s) const		{ integer k(s); return mpz_cmp(val, k.val) > 0; }
bool integer::operator> (const integer& i) const 	{ 				return mpz_cmp(val, i.val) > 0; }

bool integer::operator>= (int32_t i) const 			{ 				return mpz_cmp_si(val, i)  >= 0; }
bool integer::operator>= (const char *s) const		{ integer k(s); return mpz_cmp(val, k.val) >= 0; }
bool integer::operator>= (const string& s) const	{ integer k(s); return mpz_cmp(val, k.val) >= 0; }
bool integer::operator>= (const integer& i) const 	{ 				return mpz_cmp(val, i.val) >= 0; }

integer integer::operator+ (int32_t i) const		{ integer a(*this); a += i;					return a; }
integer integer::operator+ (const char *s) const	{ integer a(*this); a += s;					return a; }
integer integer::operator+ (const string& s) const	{ integer a(*this); a += s;					return a; }
integer integer::operator+ (const integer& i) const	{ integer a(*this); a += i;					return a; }

integer& integer::operator+= (int32_t i) {
	if (i < 0) {
		mpz_sub_ui(val, val, -i);
	}
	else {
		mpz_add_ui(val, val, i);
	}
	return *this;
}
integer& integer::operator+= (const char *s)		{ integer k(s); mpz_add(val, val, k.val);	return *this; }
integer& integer::operator+= (const string& s)		{ integer k(s); mpz_add(val, val, k.val);	return *this; }
integer& integer::operator+= (const integer& i)		{ 				mpz_add(val, val, i.val);	return *this; }

integer integer::operator- () const					{ integer a(*this);	mpz_neg(a.val, a.val);	return a; }
integer integer::operator- (int32_t i) const		{ integer a(*this); a -= i;					return a; }
integer integer::operator- (const char *s) const	{ integer a(*this); a -= s;					return a; }
integer integer::operator- (const string& s) const	{ integer a(*this); a -= s;					return a; }
integer integer::operator- (const integer& i) const	{ integer a(*this); a -= i;					return a; }

integer& integer::operator- ()						{ 				mpz_neg(val, val);			return *this; }
integer& integer::operator-= (int32_t i) {
	if (i < 0) {
		mpz_add_ui(val, val, -i);
	}
	else {
		mpz_sub_ui(val, val, i);
	}
	return *this;
}
integer& integer::operator-= (const char *s)		{ integer k(s);	mpz_sub(val, val, k.val);	return *this; }
integer& integer::operator-= (const string& s)		{ integer k(s); mpz_sub(val, val, k.val);	return *this; }
integer& integer::operator-= (const integer& i)		{ 				mpz_sub(val, val, i.val);	return *this; }

integer integer::operator* (int32_t i) const		{ integer a(*this); a *= i;					return a; }
integer integer::operator* (const char *s) const	{ integer a(*this); a *= s;					return a; }
integer integer::operator* (const string& s) const	{ integer a(*this); a *= s;					return a; }
integer integer::operator* (const integer& i) const	{ integer a(*this); a *= i;					return a; }

integer& integer::operator*= (int32_t i)			{ 				mpz_mul_si(val, val, i);	return *this; }
integer& integer::operator*= (const char *s)		{ integer k(s); mpz_mul(val, val, k.val);	return *this; }
integer& integer::operator*= (const string& s)		{ integer k(s);	mpz_mul(val, val, k.val);	return *this; }
integer& integer::operator*= (const integer& i)		{ 				mpz_mul(val, val, i.val);	return *this; }

integer integer::operator/ (int32_t i) const		{ integer a(*this); a /= i;					return a; }
integer integer::operator/ (const char *s) const	{ integer a(*this); a /= s;					return a; }
integer integer::operator/ (const string& s) const	{ integer a(*this); a /= s;					return a; }
integer integer::operator/ (const integer& i) const	{ integer a(*this); a /= i;					return a; }

integer& integer::operator/= (int32_t i) {
	if (i < 0) {
		mpz_div_ui(val, val, -i);
		mpz_neg(val, val);
	}
	else {
		mpz_div_ui(val, val, i);
	}
	return *this;
}
integer& integer::operator/= (const char *s)		{ integer k(s); mpz_div(val, val, k.val);	return *this; }
integer& integer::operator/= (const string& s)		{ integer k(s); mpz_div(val, val, k.val);	return *this; }
integer& integer::operator/= (const integer& i)		{ 				mpz_div(val, val, i.val);	return *this; }

integer integer::operator^ (uint32_t i)	 const		{ integer r(*this); r ^= i;					return r; }
integer integer::operator^ (const integer& i) const { integer r(*this); r ^= i;					return r; }

integer& integer::operator^= (uint32_t i) {
	mpz_pow_ui(val, val, i);
	return *this;
}

integer& integer::operator^= (const integer& i) {
	gmp_utils::mpz_pow_mpz(val, val, i.val);
	return *this;
}

integer integer::operator% (const integer& i) const {
	integer r;
	r.init();
	mpz_mod(r.val, val, i.val);
	return r;
}

uint64_t integer::operator% (uint64_t i) const {
	return mpz_fdiv_ui(val, i);
}

integer& integer::operator++ () {
   *this += 1;
   return *this;
}

integer integer::operator++ (int32_t) {
   integer r(*this);
   ++(*this);
   return r;
}

integer& integer::operator-- () {
   *this -= 1;
   return *this;
}

integer integer::operator-- (int32_t) {
   integer r(*this);
   --(*this);
   return r;
}

/* GETTERS */

bool integer::is_initialized() const {
	return initialized;
}

int32_t integer::get_sign() const {
	return mpz_sgn(val);
}

bool integer::is_negative() const {
	return get_sign() == -1;
}

size_t integer::bytes() const {
	if (not is_initialized()) {
		return 0;
	}
	return gmp_utils::bytes(val);
}

const mpz_t& integer::get_raw_value() const	{
	return val;
}

int32_t integer::to_int() const {
	return int32_t(mpz_get_si(val));
}

uint32_t integer::to_uint() const {
	return uint32_t(mpz_get_si(val));
}

int64_t integer::to_long_int() const {
	return mpz_get_si(val);
}

uint64_t integer::to_long_uint() const {
	return mpz_get_ui(val);
}

/* CONVERTERS */

string integer::to_string() const {
	string k;
	to_string(k);
	return k;
}

void integer::to_string(string& s) const {
	if (!is_initialized()) {
		s = "integer uninitialized";
		return;
	}
	
	char *buf = NULL;
	buf = mpz_get_str(buf, 10, val);
	s = string(buf);
	free(buf);
}

