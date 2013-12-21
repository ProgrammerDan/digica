#ifndef LOGICEXCEPTION_H
#define LOGICEXCEPTION_H

//================================================================
//Filename:		Logic Exception Class Definitions File
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		This file contains definitions for the various thrown
//					exceptions referenced in this program, as well as
//					several global functions related to debug and output.
//Revisions:
//
//================================================================

#include <string>

using namespace std;

//==================================
//Class:	BasicException
//Parent:	n/a
//==================================
//Description:
//	This is a very basic exception, holding a string message that
//		hopefully provides some description of the error.
//==================================
class BasicException
{
public:
			BasicException(string msg);

	string	getMessage();

private:

	string	message;
};

//==================================
//Class:	IndexOutOfBounds
//Parent:	BasicException
//==================================
//Description:
//	This holds a message about invalid index use.
//==================================
class IndexOutOfBounds : public BasicException
{
public:
			IndexOutOfBounds(int idx, int bnd);

	int		getIndex();
	int		getBound();

private:

	int		index;
	int		bound;
};

//==================================
//Class:	NegativeRange
//Parent:	BasicException
//==================================
//Description:
//	Warns of an invalid or negative number or range.
//==================================
class NegativeRange : public BasicException
{
public:
			NegativeRange();
};

//==================================
//Class:	NullPointerException
//Parent:	BasicException
//==================================
//Description:
//	Warns of a fatal use of a null pointer.
//==================================
class NullPointerException : public BasicException
{
public:
			NullPointerException();
};

//==================================
//Class:	UnknownState
//Parent:	BasicException
//==================================
//Description:
//	Warns of the use of an unenumerated state.
//==================================
class UnknownState : public BasicException
{
public:
			UnknownState();
};

//==================================
//Class:	InvalidDelay
//Parent:	BasicException
//==================================
//Description:
//	Warns of the use of a negative or invalid delay size.
//==================================
class InvalidDelay : public BasicException
{
public:
			InvalidDelay();
};

//==================================
//Class:	StateTableEmpty
//Parent:	BasicException
//==================================
//Description:
//	Warns of the attempt to acces or use an empty stateTable.
//==================================
class StateTableEmpty : public BasicException
{
public:
			StateTableEmpty();
};

//==================================
//Class:	NullIDException
//Parent:	BasicException
//==================================
//Description:
//	Warns of the attempt to create an Object with a null ID.
//==================================
class NullIDException : public BasicException
{
public:
			NullIDException();
};

//==================================
//Class:	NoInputsException
//Parent:	BasicException
//==================================
//Description:
//	Warns of the lack of any inputs in an object.
//==================================
class NoInputsException : public BasicException
{
public:
			NoInputsException();
};

//==================================
//Class:	NoOutputsException
//Parent:	BasicException
//==================================
//Description:
//	Warns of the lack of any outputs in an object
//==================================
class NoOutputException : public BasicException
{
public:
			NoOutputException();
};

//==================================
//Class:	ElementNotFound
//Parent:	BasicException
//==================================
//Description:
//	Warns of the inability to find a referenced object, either
//		based on ID or index.
//==================================
class ElementNotFound : public BasicException
{
public:
			ElementNotFound(string elem);
			ElementNotFound(int elem);
};

//==================================
//Class:	SourceListEmpty
//Parent:	BasicException
//==================================
//Description:
//	Warns of the lack of any source data during a copy.
//==================================
class SourceListEmpty : public BasicException
{
public:
			SourceListEmpty();
};

// See Implementation
void debugMSG(string temp);

// See Implementation
char* itoa(int num, char *a, int radix);


#endif //LOGICEXCEPTION_H