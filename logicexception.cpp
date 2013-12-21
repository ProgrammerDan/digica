//================================================================
//Filename:		Logic Exceptions Implementations File
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		This file implements the various thrown exceptions
//					scattered through the code. As well, it holds a
//					few global functions accessed throughout the code.
//Revisions:
//
//================================================================

#include <vector>
#include "stdio.h"
#include <string>
#include <iostream>
#include "logicException.h"

using namespace std;

// Enumerated states
enum state {low, high, indet};

// Debug Output control variable
bool debug = false;

//========================
//Basic Exception Constructor
//========================
//Parameters:
//	string	msg	-	the message of the exception
//Summary:
//	As this is the most basic exception possible, it holds a string with
//		a message describing the exception.
//========================
BasicException::BasicException(string msg) :
	message(msg)
{}

//========================
//Basic Exception Get Message
//========================
//Parameters:
//	none
//Summary:
//	This function returns whatever exception message the object was
//		created with.
//Returns:
//	string	-	the message
//========================
string	BasicException::getMessage()
{
	return message;
}

//========================
//Index Out Of Bounds Constructor
//========================
//Parameters:
//	int	idx	-	the index that was being used
//	int	bnd	-	the bound that was violated
//Summary:
//	This creates a generic report of an index violation.
//========================
IndexOutOfBounds::IndexOutOfBounds(int idx, int bnd) :
	BasicException(string("Index Out of Bounds! idx: ").append(itoa(idx, new char[12], 10)).append(" bnd: ").append(itoa(bnd, new char[12], 10))),
	index(idx), bound(bnd)
{}

//========================
//Index Out Of Bounds Get Index
//========================
//Parameters:
//	none
//Summary:
//	This function returns the stored invalid index.
//Returns:
//	int	-	the index
//========================
int	IndexOutOfBounds::getIndex()
{
	return index;
}

//========================
//Index Out Of Bounds Get Bound
//========================
//Parameters:
//	none
//Summary:
//	This function returns the stored violated bound.
//Returns:
//	int	-	the bound
//========================
int	IndexOutOfBounds::getBound()
{
	return bound;
}

//========================
//Negative Range Constructor
//========================
//Parameters:
//	none
//Summary:
//	Creates an exception object that warns of a negative number used in a place
//		that such numbers are not allowed.
//========================
NegativeRange::NegativeRange() :
	BasicException("Attempt to define a range as a negative number not allowed.")
{}

//========================
//Null Pointer Exception Constructor
//========================
//Parameters:
//	none
//Summary:
//	Warns of an attempt to use a null pointer. Typically a failure state.
//========================
NullPointerException::NullPointerException() :
	BasicException("Attempt to use null pointer!")
{}

//========================
//Unknown State Constructor
//========================
//Parameters:
//	none
//Summary:
//	Warns of an attempt to use a state type not defined.
//========================
UnknownState::UnknownState() :
	BasicException("Attempt to use unenumerated state!")
{}

//========================
//Invalid Delay Constructor
//========================
//Parameters:
//	none
//Summary:
//	Warns of an attempt to use a delay that is zero or negative.
//========================
InvalidDelay::InvalidDelay() :
	BasicException("Attempt to use invalid delay. Delay must be > 0.")
{}

//========================
//State Table Empty Constructor
//========================
//Parameters:
//	none
//Summary:
//	Warns of access violation when attempting to access an empty stateTable.
//========================
StateTableEmpty::StateTableEmpty() :
	BasicException("Attempt to access state table failed: state table empty!")
{}

//========================
//Null ID Exception
//========================
//Parameters:
//	none
//Summary:
//	Warns of a fatal attempt to build an object without an ID.
//========================
NullIDException::NullIDException() :
	BasicException("Attempt to set ID to an empty string!")
{}

//========================
//No Inputs Exception
//========================
//Parameters:
//	none
//Summary:
//	Warns of the condition that the Object lacks any inputs.
//========================
NoInputsException::NoInputsException() :
	BasicException("This Object has no inputs defined.")
{}

//========================
//No Output Exception
//========================
//Parameters:
//	none
//Summary:
//	Warns of the condition that the Object lacks an output.
//========================
NoOutputException::NoOutputException() :
	BasicException("This Object has no output defined.")
{}

//========================
//Element Not Found	String Edition Constructor
//========================
//Parameters:
//	string	elem	-	the name of the element not found
//Summary:
//	Implies an ID bound lookup failed, and returns the ID of the supposed
//		element that could not be found.
//========================
ElementNotFound::ElementNotFound(string elem) :
	BasicException(string("Element Not Found: ").append(elem))
{}

//========================
//Element Not Found Index Edition Constructor
//========================
//Parameters:
//	int	elem	-	the index at which an object was not found
//Summary:
//	Implies an index bound lookup failed in some manner, and returns the
//		index that was used.
//========================
ElementNotFound::ElementNotFound(int elem) :
	BasicException(string("Element Not Found: ").append(itoa(elem, new char[12], 10)))
{}

//========================
//Source List Empty Constructor
//========================
//Parameters:
//	none
//Summary:
//	Implies a failure to copy a list or vector.
//========================
SourceListEmpty::SourceListEmpty() :
	BasicException("The source list is empty, unable to generate new list.")
{}

//========================
//Debug Message
//========================
//Parameters:
//	string	temp	-	the string to output
//Summary:
//	Provided global debugging is on (debug == true), then the message passed
//		to this subroutine prints.
//Returns:
//	n/a
//========================
void debugMSG(string temp)
{
	if (debug)
	{
		cout << temp << endl;
	}
}

//========================
//Integer to Ascii, Private Implementation
//========================
//Parameters:
//	int		num		-	the number to translate
//	char	*a		-	an empty and appropriately sized char array
//	int		radix	-	the radix to translate to (ignored)
//Summary:
//	Uses sprintf to cast num as a char array, and returns the results.
//Returns:
//	char*	-	num as a char array.
//========================
char* itoa(int num, char *a, int radix)
{
	if (a == NULL)
		throw NullPointerException();

	//radix ignored for now.
	sprintf(a, "%d", num);

	return a;
}