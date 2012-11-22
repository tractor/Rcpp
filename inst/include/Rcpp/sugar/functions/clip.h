// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// clip.h: Rcpp R/C++ interface class library -- clip
//
// Copyright (C) 2012 Dirk Eddelbuettel and Romain Francois
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp__sugar__clip_h
#define Rcpp__sugar__clip_h

namespace Rcpp{
namespace sugar{

template <int RTYPE, bool NA>
struct clip_operator{
    typedef typename Rcpp::traits::storage_type<RTYPE>::type STORAGE ;
	
    clip_operator(STORAGE lhs_, STORAGE rhs_ ) : lhs(lhs_), rhs(rhs_){}
    
    inline STORAGE operator()(STORAGE x) const {
        return lhs < x ? lhs : (x < rhs ? x : rhs ) ;
    }
    STORAGE lhs, rhs ;    
} ;
// need to write this qpecial version
template <>
struct clip_operator<REALSXP,true> {
    clip_operator(double lhs_, double rhs_ ) : lhs(lhs_), rhs(rhs_){}
    
    inline double operator()(double x) const {
        if( Rcpp::traits::is_na<REALSXP>(x) ) ;
        return lhs < x ? lhs : (x < rhs ? x : rhs ) ;
    }
    double lhs, rhs ;    
} ;



template <
	int RTYPE, 
	bool NA, typename T
	>
class Clip_Primitive_Vector_Primitive : public VectorBase< 
	RTYPE , 
	NA ,
	Clip_Primitive_Vector_Primitive<RTYPE,NA,T>
> {
public:
	typedef typename Rcpp::traits::storage_type<RTYPE>::type STORAGE ;
	typedef clip_operator<RTYPE,NA> OPERATOR ;
	
	Clip_Primitive_Vector_Primitive( STORAGE lhs_, const T& vec_, STORAGE rhs_) : vec(vec_), op(lhs_,rhs_) {}
	
	inline STORAGE operator[]( int i ) const {
		return op( vec[i] ) ;
	}
	inline int size() const { return vec.size() ; }
	         
private:
	const T& vec ;
	OPERATOR op ; 
} ;



} // sugar

template <int RTYPE, bool NA, typename T>
inline sugar::Clip_Primitive_Vector_Primitive<RTYPE,NA,T> 
clip( 
	typename Rcpp::traits::storage_type<RTYPE>::type lhs,
	const Rcpp::VectorBase<RTYPE,NA,T>& vec,  
	typename Rcpp::traits::storage_type<RTYPE>::type rhs
	){
	return sugar::Clip_Primitive_Vector_Primitive<RTYPE,NA,T>( lhs, vec.get_ref(), rhs ) ;
}


} // Rcpp

#endif
