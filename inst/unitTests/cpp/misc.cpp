// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8 -*-
//
// misc.cpp: Rcpp R/C++ interface class library -- misc unit tests
//
// Copyright (C) 2013    Dirk Eddelbuettel and Romain Francois
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

#include <Rcpp.h>
using namespace Rcpp ;
using namespace std;

class simple {
    Rcpp::Dimension dd;
public:
    simple(SEXP xp) : dd(xp) {}
    int nrow() const { return dd[0]; }
    int ncol() const { return dd[1]; }
};

// [[Rcpp::export]]
SEXP symbol_(){
    return LogicalVector::create( 
        Symbol( Rf_install("foobar") ).asSexp() == Rf_install("foobar"), 
        Symbol( Rf_mkChar("foobar") ).asSexp() == Rf_install("foobar"), 
        Symbol( Rf_mkString("foobar") ).asSexp() == Rf_install("foobar"), 
        Symbol( "foobar" ).asSexp() == Rf_install("foobar") 
    ) ;
}

// [[Rcpp::export]]
Symbol symbol_ctor(SEXP x){ return Symbol(x); }

// [[Rcpp::export]]
List Argument_(){
    Argument x("x"), y("y");
    return List::create( x = 2, y = 3 );
}

// [[Rcpp::export]]
int Dimension_const( SEXP ia ){
    simple ss(ia);
	return ss.nrow();
}

// [[Rcpp::export]] 
SEXP evaluator_error(){
    return Rcpp::Evaluator::run( Rf_lang2( Rf_install("stop"), Rf_mkString( "boom" ) ) ) ;
}

// [[Rcpp::export]]
SEXP evaluator_ok(SEXP x){
    return Rcpp::Evaluator::run( Rf_lang2( Rf_install("sample"), x ) ) ;
}

// [[Rcpp::export]]
void exceptions_(){
    throw std::range_error("boom") ;
}

// [[Rcpp::export]]
LogicalVector has_iterator_( ){
    return LogicalVector::create( 
        (bool)Rcpp::traits::has_iterator< std::vector<int> >::value, 
        (bool)Rcpp::traits::has_iterator< std::list<int> >::value, 
        (bool)Rcpp::traits::has_iterator< std::deque<int> >::value, 
        (bool)Rcpp::traits::has_iterator< std::set<int> >::value, 
        (bool)Rcpp::traits::has_iterator< std::map<std::string,int> >::value, 
        (bool)Rcpp::traits::has_iterator< std::pair<std::string,int> >::value, 
        (bool)Rcpp::traits::has_iterator< Rcpp::Symbol >::value 
        );
}
