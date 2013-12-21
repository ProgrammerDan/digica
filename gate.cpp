//================================================================
//Filename:		Gate Objects Implementations
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		Contains implementations for Gate objects, including
//					parent Gate class and its derivatives AND, OR, NOT
//					NOR, NAND, XOR, and XNOR.
//Revisions:
//
//================================================================

#include <list>
#include <string>
#include <iostream>
#include "logicException.h"
#include "logic.h"
#include "gate.h"

using namespace std;

// This file contains the logic for Gates
// Note gates are defined as follows:
//
// X | Y | XY | X+Y | X' | (XY)' | (X+Y)' | X(+)Y | (X(+)Y)'
//---+---+----+-----+----+-------+--------+-------+----------
// 0 | 0 | 0  |  0  | 1  |   1   |   1    |   0   |     1
// 0 | 1 | 0  |  1  | 1  |   1   |   0    |   1   |     0
// 0 | X | 0  |  X  | 1  |   1   |   X    |   X   |     X
// 1 | 0 | 0  |  1  | 0  |   1   |   0    |   1   |     0
// 1 | 1 | 1  |  1  | 0  |   0   |   0    |   0   |     1
// 1 | X | X  |  1  | 0  |   X   |   0    |   X   |     X
// X | 0 | 0  |  X  | X  |   1   |   X    |   X   |     X
// X | 1 | X  |  1  | X  |   0   |   0    |   X   |     X
// X | X | X  |  X  | X  |   X   |   X    |   X   |     X
//		  asso asso  sing                   asso      asso
//
// asso = associative property ... XYZ = (XY)Z = X(YZ) etc.
// sing = single input

//========================
//XOR Logic
//========================
//Parameters:
//	state	a	-	first logic value
//	state	b	-	second logic value
//Summary:
//	This function performs (a XOR b) and returns the result. Note that
//		XOR is really a composite function defined as ((a(~b))+((~a)b))
//		and is built as such in this routine.
//Returns:
//	state	-	a XOR b
//========================
state XOR(state a, state b) throw (UnknownState)
{ // XOR
	state c;

	if ((a != low) && (a != high) && (a != indet))
		throw UnknownState();
	if ((b != low) && (b != high) && (b != indet))
		throw UnknownState();

	c = OR(AND(a, NOT(b)), AND(b, NOT(a)));

	return c;
}

//========================
//XNOR Logic
//========================
//Parameters:
//	state	a	-	first logic value
//	state	b	-	second logic value
//Summary:
//	This function performs (a XNOR b) and returns the result. Note that
//		XNOR is really a composite function defined as ((ab)+((~a)(~b)))
//		and is built as such in this routine.
//Returns:
//	state	-	a XNOR b
//========================
state XNOR(state a, state b) throw (UnknownState)
{ // XNOR
	state c;

	if ((a != low) && (a != high) && (a != indet))
		throw UnknownState();
	if ((b != low) && (b != high) && (b != indet))
		throw UnknownState();

	c = OR(AND(a, b), AND(NOT(a), NOT(b)));

	return c;
}

//========================
//OR Logic
//========================
//Parameters:
//	state	a	-	first logic value
//	state	b	-	second logic value
//Summary:
//	This function performs (a OR b) and returns the result.
//Returns:
//	state	-	a OR b
//========================
state OR(state a, state b) throw (UnknownState)
{ // OR
	state c;

	if ((a != low) && (a != high) && (a != indet))
		throw UnknownState();
	if ((b != low) && (b != high) && (b != indet))
		throw UnknownState();

	if ((a == high) || (b == high))
		c = high;
	else if ((a == low) && (b == low))
		c = low;
	else
		c = indet;

	return c;
}

//========================
//AND logic
//========================
//Parameters:
//	state	a	-	first logic value
//	state	b	-	second logic value
//Summary:
//	This function performs (a AND b) and returns the result.
//Returns:
//	state	-	a AND b
//========================
state AND(state a, state b) throw (UnknownState)
{ // AND
	state c;

	if ((a != low) && (a != high) && (a != indet))
		throw UnknownState();
	if ((b != low) && (b != high) && (b != indet))
		throw UnknownState();

	if ((a == low) || (b == low))
		c = low;
	else if ((a == high) && (b == high))
		c = high;
	else
		c = indet;

	return c;
}

//========================
//NOT Logic
//========================
//Parameters:
//	state	a	-	logic value
//Summary:
//	This function performs (NOT a) and returns the result.
//Returns:
//	state	-	NOT a (logical inverse)
//========================
state NOT(state a) throw (UnknownState)
{ // Logical Inversion.
	state b;

	if (a == low)
		b = high;
	else if (a == high)
		b = low;
	else if (a == indet) // I'm not driving this gate. Indet in = Indet out.
		b = indet;			// True logic would demand indet in = high out.
	else
		throw UnknownState();

	return b;
}


//========================
//Gate Constructor
//========================
//Parameters:
//	string	tID		-	the ID to assign to the gate
//	state	def		-	the default state condition of the delay table
//	int		delay	-	the size of the delay table
//Summary:
//	Creates a new basic gate... this is never actually called directly, but
//		is called through a wrapper class like logicAND. However, this code
//		instantiates the Gate, sets its basic properties and builds a default
//		delay state table.
//========================
Gate::Gate(string tID, state def, int delay) throw (UnknownState, InvalidDelay) :
	LogicObj(tID)
{
	if ((def != low) && (def != high) && (def != indet))
		throw UnknownState();
	else
		defCurrent = def;

	if (delay < 1)
		throw InvalidDelay();
	else
		delaySize = delay;

	delayTable = new list<state>(delaySize, defCurrent);

	inputs = new vector<Wire*>(0);

	output = NULL;

	nTick = false;
	mTick = false;
	tTick = false;
}

//========================
//Gate Destructor
//========================
//Summary:
//	Deletes the delay state table and the inputs vectors.
//========================
Gate::~Gate()
{
	delete delayTable;
	delete inputs;
}

//========================
//Gate Operator =
//========================
//Parameters:
//	const	Gate	&g	-	The gate to set *this equal to.
//Summary:
//	Sets all the attributes of *this to those held by g.
//Returns:
//	Gate&	-	*this modified to contain g's values.
//========================
Gate&		Gate::operator= (const Gate &g)
{
	this->delaySize = g.delaySize;
	delete this->delayTable;
	this->delayTable = new list<state>(*(g.delayTable));
	this->defCurrent = g.defCurrent;
	this->output = g.output;
	delete this->inputs;
	this->inputs = new vector<Wire*>(*(g.inputs));
	this->nTick = g.nTick;
	this->mTick = g.mTick;
	this->tTick = g.tTick;

	return *this;
}

//========================
//Gate Get Delay Table Size
//========================
//Parameters:
//	none
//Summary:
//	Retrieves the size of the delay table.
//Returns:
//	int	-	the size of the delay state table.
//========================
int			Gate::getDelaySize()
{
		return delayTable->size();
}

//========================
//Gate Clear Delay Table
//========================
//Parameters:
//	none
//Summary:
//	This function clears the delay table and recreates it using the stored
//		default state.
//Returns:
//	n/a
//========================
void		Gate::clrDelay()
{
	if (delayTable != NULL)
		delete delayTable;

	delayTable = new list<state>(delaySize, defCurrent);
}

//========================
//Gate Set State
//========================
//Parameters:
//	state	a	-	The state to push onto the list.
//Summary:
//	The delay table for a Gate looks somewhat like this:
//		[X,X,X,1,0,X,X,1,1,1,1] provided the delay table has size 11.
//		All operations occur at the end of the table.
//		Set State pushes the incoming state onto the beginning of the list,
//		and removes the old end of the list. This works in all cases since
//		the delay table cannot legally be less than one element in size.
//Returns:
//	n/a
//========================
void		Gate::setState(state a) throw (UnknownState)
{
	if ((a != low) && (a != high) && (a != indet))
		throw UnknownState();
	else
	{
		delayTable->push_front(a); // add to front and remove from back.
		delayTable->pop_back();
	}
}

//========================
//Gate Modify State
//========================
//Parameters:
//	state	a	-	the state to replace the first state in the delay table
//						with
//Summary:
//	This function takes a and replaces the first element of the delay state
//		table with it. This function allows Gate objects to be rerun without
//		"overpushing" their delay tables.
//Returns:
//	n/a
//========================
void		Gate::modState(state a) throw (UnknownState)
{
	if ((a != low) && (a != high) && (a != indet))
		throw UnknownState();
	else
	{
		delayTable->pop_front();
		delayTable->push_front(a); // remove from front and add to front
	}
}

//========================
//Gate Get State
//========================
//Parameters:
//	none
//Summary:
//	Provided the delay table exists, this function returns the next-up state.
//		In other words, it returns the state at the end of the state delay
//		table.
//Returns:
//	state	-	The state at the end of the delay table.
//========================
state		Gate::getState() throw (StateTableEmpty, UnknownState) // end state
{
	if (delayTable->size() == 0)
		throw StateTableEmpty();
	else
	{
		state tState = delayTable->back(); // return end.
		if ((tState != low) && (tState != high) && (tState != indet))
			throw UnknownState();
		else
			return tState;
	}
}

//========================
//Gate Get Delay State Table Snapshot
//========================
//Parameters:
//	none
//Summary:
//	Returns a string representation of the entire Delay table
//Returns:
//	string	-	Delay State Table in a string
//========================
string		Gate::getStateSnapshot() throw (StateTableEmpty)
{
	if (delayTable->size() == 0)
		throw StateTableEmpty();
	else
	{
		list<state>::iterator dTi = delayTable->begin();

		state tState;

		string tRet = "[";

		while (dTi != delayTable->end())
		{
			if (dTi != delayTable->begin())
				tRet.append(",");

			tState = *dTi;

			if ((tState != low) && (tState != high) && (tState != indet))
				throw UnknownState();
			else
				tRet.append(itoa(tState, new char[2], 10));

			dTi++;
		}

		tRet.append("]");

		return tRet;
	}
}

//========================
//Gate Set Output Wire
//========================
//Parameters:
//	Wire	*t	-	The wire to set the Gate's output to.
//Summary:
//	Sets the Gate's output to t.
//Returns:
//	n/a
//========================
void		Gate::setOutput(Wire *t) throw (NullPointerException)
{
	if (t == NULL)
		throw NullPointerException();
	else
		output = t;
}

//========================
//Gate Get Output Wire
//========================
//Parameters:
//	none
//Summary:
//	Returns the Gate's output wire.
//Returns:
//	Wire*	-	the Gate's output wire.
//========================
Wire*		Gate::getOutput()
{
	return output;
}

//========================
//Gate Add Input Wire
//========================
//Parameters:
//	Wire	*t	-	a pointer to the wire to add.
//Summary:
//	Adds a wire to the inputs vector.
//Returns:
//	n/a
//========================
void		Gate::addInput(Wire *t) throw (NullPointerException)
{
	if (t == NULL)
		throw NullPointerException();
	else
		inputs->push_back(t);
}

//========================
//Gate Remove Input Wire ID Edition
//========================
//Parameters:
//	string	tID	-	the ID of the wire to remove
//Summary:
//	This function searches the inputs vector, trying to match a wire
//		contained by the vector to an ID passed to the function.
//		If a wire is found that has a matching ID, that wire is
//		removed from the vector.
//Returns:
//	bool	-	true if successful, false if remove failed
//========================
bool		Gate::remInput(string tID) throw (NullIDException)
{
	if (tID == "")
		throw NullIDException();
	else
	{
		vector<Wire*>::iterator iI = inputs->begin();

		while (iI != inputs->end())
		{
			if ((*iI)->getID() == tID)
			{
				inputs->erase(iI);
				return true;
			}
			iI++;
		}
		return false;
	}
}

//========================
//Gate Remove Input Wire Index Edition
//========================
//Parameters:
//	int	idx	-	the index of the wire in inputs to remove
//Summary:
//	Provided idx is a valid index for inputs, iterates to the index
//		requested and removes the wire at that index.
//Returns:
//	bool	-	true if successful, false is failure
//========================
bool		Gate::remInput(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);
	else if (idx > inputs->size() - 1)
		throw IndexOutOfBounds(idx, inputs->size() - 1);
	else
	{
		vector<Wire*>::iterator iI = inputs->begin();

		for (int k = 0; k < idx; k++) // Inefficient, but that's okay for this.
		{
			iI++;
		}
		inputs->erase(iI);

		return true;
	}
	return false;
}

//========================
//Gate Get Input Wire, ID Edition
//========================
//Parameters:
//	string	tID	-	the ID of the wire to return
//Summary:
//	This function loops through the inputs vector, looking for a wire
//		whose ID matches tID. If that Wire is found, a pointer to it
//		is returned.
//Returns:
//	Wire*	-	the pointer to the matching wire.
//========================
Wire*		Gate::getInput(string tID) throw (NullIDException, ElementNotFound)
{
	if (tID == "")
		throw NullIDException();
	else
	{
		vector<Wire*>::iterator iI = inputs->begin();

		while (iI != inputs->end())
		{
			if ((*iI)->getID() == tID)
			{
				return (*iI);
			}
			iI++;
		}
		throw ElementNotFound(tID);
	}
}

//========================
//Gate Get Input Wire, Index Edition
//========================
//Parameters:
//	int	idx	-	the index of the wire to return
//Summary:
//	This function, provided idx is a valid index of inputs, iterates
//		to the Wire* contained at idx in inputs and returns that Wire*.
//Returns:
//	Wire*	-	a pointer to the Wire contained at idx.
//========================
Wire*		Gate::getInput(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);
	else if (idx > inputs->size() - 1)
		throw IndexOutOfBounds(idx, inputs->size() - 1);
	else
	{
		vector<Wire*>::iterator iI = inputs->begin();

		for (int k = 0; k < idx; k++) // Inefficient, but that's okay for this.
		{
			iI++;
		}

		return (*iI);
	}
}

//========================
//Gate Clear Inputs
//========================
//Parameters:
//	none
//Summary:
//	Clears the inputs vector.
//Returns:
//	n/a
//========================
void		Gate::clrInputs()
{
	inputs->clear();
}

//========================
//Gate Count Inputs
//========================
//Parameters:
//	none
//Summary:
//	This function returns the size of the input vector.
//Returns:
//	int	-	the size of inputs
//========================
int			Gate::cntInputs()
{
	return inputs->size();
}

//========================
//Gate Tick
//========================
//Parameters:
//	int	ct	-	The current time of the latest tick
//Summary:
//	This function calls the appropriate methods to handle an actual
//		gate event. Basically, the Gate tick is called by its parent
//		up the branching link list chain. After running its own logic
//		and saving the results to its delay state table (and outputting
//		the endmost state table result to its output wire), it calls
//		its output wire's state.
//Returns:
//	n/a
//========================
void	Gate::tick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + getStateSnapshot() + " - ");

	if (output == NULL)
		throw NoOutputException();

	state tState = getState();

	if ((tState != low) && (tState != high) && (tState != indet))
		throw UnknownState();

	if (nTick)
	{
		// Don't rerun it if it's already been run. Don't call its dependencies either. They
		// have already been called.
		return;
	}

	// Call transform(true) if rerunning, e.g. ct <= getTime();
	// Call transform(false) if not rerunning, e.g. ct > getTime();

	if (ct > LogicObj::getTime())
	{
		output->setCurrent(tState); // need to take end of stack first. Then
									// perform transform to shift stack.
		this->transform(false);
	}
	else
		this->transform(true);

	nTick = true;
	mTick = false;

	output->tick(ct); // Tick output object.

	LogicObj::tick(ct);
}

//========================
//Gate Inverse Tick
//========================
//Parameters:
//	int	ct	-	the current time of the latest tick.
//Summary:
//	This function resets the various tick guards, so that each gate will
//		be ready to run again next tick. Note that this function calls the
//		itick of its output wire.
//Returns:
//	n/a
//========================
void Gate::itick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + getStateSnapshot() + " + ");

	if (mTick)
		return;

	mTick = true;
	nTick = false;
	tTick = false;

	output->itick(ct);

	LogicObj::itick(ct);
}

//========================
//Gate Transform (virtual, base level template)
//========================
//Parameters:
//	bool	r	-	whether or not this function is being rerun.
//Summary:
//	This is the most basic shared functionality between all the various
//		Gate derivatives. Note that this is solely exception handling.
//Returns:
//	n/a
//========================
void	Gate::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	if (output == NULL)
		throw NoOutputException();

	if (inputs->size() == 0)
		throw NoInputsException();
}

//========================
//Gate Trace
//========================
//Parameters:
//	int	oB	-	old biggest pathway length
//Summary:
//	As a link in the chain of a depth-first search, this function adds
//		the delay state table size to the old biggest pathway length as
//		reported to the function. It then calls the trace of its output.
//		If what is returned is larger than the current biggest, the new
//		biggest is returned. Note that for functional consistency (and
//		since this is a virtual function) the parent LogicObj function
//		is also called. Note also that trace is only called once per
//		function per trace execution. Trace's retrace preventer is reset
//		by itick(int ct).
//Returns:
//	int	-	the new biggest pathway length
//========================
int		Gate::trace(int oB) throw (BasicException)
{
	if (tTick)
		return oB;

	int big = oB + getDelaySize();

	tTick = true;

	int t = getOutput()->trace(big);

	if (t > big)
		big = t;

	return LogicObj::trace(big);
}

//========================
//Logic NOT Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicNOT::logicNOT(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic NOT Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual NOT logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicNOT::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);	//	Parent function called first, as it contains global
						//		features.

	state ttState = Gate::getInput(0)->getCurrent(); // only cares about one input.

	if ((ttState != low) && (ttState != high) && (ttState != indet))
		throw UnknownState();

	ttState = NOT(ttState);

	if (r)
		Gate::modState(ttState);
	else
		Gate::setState(ttState);

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}

//========================
//Logic AND Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicAND::logicAND(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic AND Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual AND logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicAND::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);

	if (Gate::cntInputs() < 2)		// need at least two inputs.
		throw NoInputsException();

	state tState1 = Gate::getInput(0)->getCurrent();
	if ((tState1 != low) && (tState1 != high) && (tState1 != indet))
		throw UnknownState();

	state tState2 = Gate::getInput(1)->getCurrent();
	if ((tState2 != low) && (tState2 != high) && (tState2 != indet))
		throw UnknownState();

	state tStateF = AND(tState1, tState2);

	state tState3;

	for (int ktS = 2; ktS < Gate::cntInputs(); ktS++) // Associative. Take past Final, and AND with
	{													// new input.
		tState3 = Gate::getInput(ktS)->getCurrent();

		if ((tState3 != low) && (tState3 != high) && (tState3 != indet))
			throw UnknownState();

		tStateF = AND(tStateF, tState3);
	}

	if (r)
		Gate::modState(tStateF);
	else
		Gate::setState(tStateF);

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}

//========================
//Logic OR Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicOR::logicOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic OR Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual OR logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicOR::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);

	if (Gate::cntInputs() < 2)		// need at least two inputs.
		throw NoInputsException();

	state tState1 = Gate::getInput(0)->getCurrent();
	if ((tState1 != low) && (tState1 != high) && (tState1 != indet))
		throw UnknownState();

	state tState2 = Gate::getInput(1)->getCurrent();
	if ((tState2 != low) && (tState2 != high) && (tState2 != indet))
		throw UnknownState();

	state tStateF = OR(tState1, tState2);

	state tState3;

	for (int ktS = 2; ktS < Gate::cntInputs(); ktS++)
	{
		tState3 = Gate::getInput(ktS)->getCurrent();

		if ((tState3 != low) && (tState3 != high) && (tState3 != indet))
			throw UnknownState();

		tStateF = OR(tStateF, tState3);
	}

	if (r)
		Gate::modState(tStateF);
	else
		Gate::setState(tStateF);

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}

//========================
//Logic NAND Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicNAND::logicNAND(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic NAND Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual NAND logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicNAND::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);

	if (Gate::cntInputs() < 2)		// need at least two inputs.
		throw NoInputsException();

	state tState1 = Gate::getInput(0)->getCurrent();
	if ((tState1 != low) && (tState1 != high) && (tState1 != indet))
		throw UnknownState();

	state tState2 = Gate::getInput(1)->getCurrent();
	if ((tState2 != low) && (tState2 != high) && (tState2 != indet))
		throw UnknownState();

	state tStateF = AND(tState1, tState2);

	state tState3;

	for (int ktS = 2; ktS < Gate::cntInputs(); ktS++)
	{
		tState3 = Gate::getInput(ktS)->getCurrent();

		if ((tState3 != low) && (tState3 != high) && (tState3 != indet))
			throw UnknownState();

		tStateF = AND(tStateF, tState3);
	}

	if (r)
		Gate::modState(NOT(tStateF)); // Logical Inverse of AND gate.
	else
		Gate::setState(NOT(tStateF));

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}

//========================
//Logic NOR Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicNOR::logicNOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic NOR Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual NOR logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicNOR::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);

	if (Gate::cntInputs() < 2)		// need at least two inputs.
		throw NoInputsException();

	state tState1 = Gate::getInput(0)->getCurrent();
	if ((tState1 != low) && (tState1 != high) && (tState1 != indet))
		throw UnknownState();

	state tState2 = Gate::getInput(1)->getCurrent();
	if ((tState2 != low) && (tState2 != high) && (tState2 != indet))
		throw UnknownState();

	state tStateF = OR(tState1, tState2);

	state tState3;

	for (int ktS = 2; ktS < Gate::cntInputs(); ktS++)
	{
		tState3 = Gate::getInput(ktS)->getCurrent();

		if ((tState3 != low) && (tState3 != high) && (tState3 != indet))
			throw UnknownState();

		tStateF = OR(tStateF, tState3);
	}

	if (r)
		Gate::modState(NOT(tStateF)); // Logical Inverse of AND gate.
	else
		Gate::setState(NOT(tStateF));

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}

//========================
//Logic XOR Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicXOR::logicXOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic XOR Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual XOR logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicXOR::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);

	if (Gate::cntInputs() < 2)		// need at least two inputs.
		throw NoInputsException();

	state tState1 = Gate::getInput(0)->getCurrent();
	if ((tState1 != low) && (tState1 != high) && (tState1 != indet))
		throw UnknownState();

	state tState2 = Gate::getInput(1)->getCurrent();
	if ((tState2 != low) && (tState2 != high) && (tState2 != indet))
		throw UnknownState();

	state tStateF = XOR(tState1, tState2);

	state tState3;

	for (int ktS = 2; ktS < Gate::cntInputs(); ktS++)
	{
		tState3 = Gate::getInput(ktS)->getCurrent();

		if ((tState3 != low) && (tState3 != high) && (tState3 != indet))
			throw UnknownState();

		tStateF = XOR(tStateF, tState3);
	}

	if (r)
		Gate::modState(tStateF);
	else
		Gate::setState(tStateF);

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}

//========================
//Logic XNOR Constructor
//========================
//Parameters:
//	string	tID		-	See Gate constructor
//	state	def		-	See Gate constructor
//	int		delay	-	See Gate constructor
//Summary:
//	See Gate constructor -- this is a wrapper constructor.
//========================
logicXNOR::logicXNOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException) :
	Gate(tID, def, delay)
{}

//========================
//Logic XNOR Transform
//========================
//Parameters:
//	bool	r	-	whether or not transform is being rerun (true = rerun,
//															 false = run)
//Summary:
//	This function performs the actual XNOR logic and updates the Gate's
//		delay state table.
//Returns:
//	n/a
//========================
void	logicXNOR::transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState)
{
	Gate::transform(r);

	if (Gate::cntInputs() < 2)		// need at least two inputs.
		throw NoInputsException();

	state tState1 = Gate::getInput(0)->getCurrent();
	if ((tState1 != low) && (tState1 != high) && (tState1 != indet))
		throw UnknownState();

	state tState2 = Gate::getInput(1)->getCurrent();
	if ((tState2 != low) && (tState2 != high) && (tState2 != indet))
		throw UnknownState();

	state tStateF = XOR(tState1, tState2);

	state tState3;

	for (int ktS = 2; ktS < Gate::cntInputs(); ktS++)
	{
		tState3 = Gate::getInput(ktS)->getCurrent();

		if ((tState3 != low) && (tState3 != high) && (tState3 != indet))
			throw UnknownState();

		tStateF = XOR(tStateF, tState3);
	}

	if (r)
		Gate::modState(NOT(tStateF)); // Logical Inverse of XOR gate.
	else
		Gate::setState(NOT(tStateF));

	debugMSG(Gate::getID() + ": " + Gate::getStateSnapshot());
}