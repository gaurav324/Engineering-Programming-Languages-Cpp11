// File EPL_traits.h -- special template metafunctions used to test Project3
#pragma once

#ifndef _EPL_traits_h
#define _EPL_traits_h

#include <complex>
#include <vector>
#include <string>

/* this metafunctions in this file will require forward declarations of all the template classes used
 * in your soultion to project 3 (or at least a few of them)
 * So, you can either #include "Valarray.h" or you can provide the forward declarations.
 * I have chosen to do the latter. Please fix this so that it works with your solution
 */

// #include "Valarray.h" // -- I'm not using this method, I'm forward-declaring Valarray<T, R> instead

template <typename T, typename R> class Valarray;

/* the default is empty
 * in that way, if EPL_traits is used on an unsupported type, then we'll have an compile-time error
 */
template <typename T> struct EPL_traits { 
	enum {
		SRank = 1,
		CRank = false,
		VRank = false
	};
};

template <> struct EPL_traits<void> { // some useful constants, exported to the real metafunctions below
	enum {
		INT = 1,
		FLOAT = 2,
		DOUBLE = 3
	};
	static std::string baseTypeName(const int s) {
		switch (s) { // s will be the SRank
		case INT: return "int";
		case FLOAT: return "float";
		case DOUBLE: return "double";
		default: return "unrecognized scalar base type";
		}
	}
};

template <> struct EPL_traits<int> {
	enum {
		SRank = EPL_traits<void>::INT,
		CRank = false,
		VRank = false
	};
};

template <> struct EPL_traits<float> {
	enum {
		SRank = EPL_traits<void>::FLOAT,
		CRank = false,
		VRank = false
	};
};

template <> struct EPL_traits<double> {
	enum {
		SRank = EPL_traits<void>::DOUBLE,
		CRank = false,
		VRank = false
	};
};

template <typename T> struct EPL_traits<std::complex<T>> {
	enum {
		SRank = EPL_traits<T>::SRank,
		CRank = true,
		VRank = false
	};
};



/* YOU MUST EDIT ALL THE FOLLOWING SPECIALIZATION(S) TO ENSURE EPL_traits WORKS WITH YOUR SOLUTION */

/* You must create a specialization for every possible Valarray and Valarray expression
 * So, if operator+(Valarray, Valarray) returns a BinOp<X, Y, Z> type, then you must create
 * a (partial) specialization for EPL_traits<BinOp<X, Y, Z>> 
 * In that specialization, VRank would be true, CRank and SRank would be determined by 
 * the value_type of your BinOp.
 *
 * NOTE: in my solution, I wrap all my BinOps and other proxy objects (e.g., scalar proxies)
 * in a Valarray wrapper. To do that, Valarray becomes a two-argument template: Valarray<T, R>
 * Since I've done that, the result type for any Valarray expression becomes Valarary<T, R>
 * as a result, I only have to write one specialization of EPL_traits (shown below). You may 
 * decide you can't (or don't want) to use this specialization. If so, delete it.
 * 
 * Recall, for any Valarray<T> and for any legal expression of Valarray<T> objects (including 
 * expressions that use sum(), apply(), accumulate and scalars), EPL_traits must work for the result type
 * of that expression and EPL_traits must export SRank, CRank and VRank correctly.
 */

template <typename T, typename R> struct EPL_traits<Valarray<T, R>> {
	enum {
		SRank = EPL_traits<T>::SRank,
		CRank = EPL_traits<T>::CRank,
		VRank = true
	};
};


#endif /* _EPL_traits_h */
