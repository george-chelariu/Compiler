#ifndef TYPEKINDENUM_H
#define TYPEKINDENUM_H

using namespace std;

enum Kind{CONSTANT, VARIABLE, PROCEDURE, UNDEFINED};
//UNDEFINED represents a kind error

enum Type{Integer, Boolean, Universal};
//Universal is to handle type error

#endif
