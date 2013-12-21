//================================================================
//Filename:		Logic Objects Implementations
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		This is a collection of various logic objects, not
//					including gates. Pads, PadStates and Wires are
//					predominantly represented, as well as their parent
//					classes.
//Revisions:
//
//================================================================

#include <vector>
#include <string>
#include <iostream>
#include "logicException.h"
#include "logic.h"

using namespace std;

//========================
//Logic Object Constructor
//========================
//Parameters:
//	string	tID	-	The ID with which to label the LogicObj
//Summary:
//	This constructor creates a logic object with the specified ID and sets
//		the object's time to -1 (uninitiated).
//========================
LogicObj::LogicObj(string tID) throw (NullIDException)
{
	if (tID == "")
		throw NullIDException();
	else
		ID = tID;

	time = -1;
}

//========================
//Logic Object Get ID
//========================
//Parameters:
//	none
//Summary:
//	Returns the ID of the Logic Object.
//Returns:
//	string	-	the ID
//========================
string	LogicObj::getID() throw (NullIDException)
{
	if (ID == "")
		throw NullIDException();
	else
		return ID;
}

//========================
//Logic Object Get Time
//========================
//Parameters:
//	none
//Summary:
//	Returns the current time.
//Returns:
//	int	-	current time
//========================
int		LogicObj::getTime()
{
	return time;
}

//========================
//Logic Object Increment Time
//========================
//Parameters:
//	none
//Summary:
//	Increments the current time.
//Returns:
//	n/a
//========================
void	LogicObj::incTime()
{
	time++;
}

//========================
//Logic Object Clear Time
//========================
//Parameters:
//	none
//Summary:
//	Resets the time to uninitiated state (-1).
//Returns:
//	n/a
//========================
void	LogicObj::clrTime()
{
	time = -1;
}

//========================
//Logic Object Tick
//========================
//Parameters:
//	int	ct	-	the circuit's current time
//Summary:
//	Basic top level tick function, manages logic object time in relation
//		to the given circuit time (ct).
//Returns:
//	n/a
//========================
void	LogicObj::tick(int ct) throw (BasicException)
{
	if (ct == time)
		return;
	else if (ct > time)
		time++;
	else
		time = ct;
}

//========================
//Logic Object Inverse Tick
//========================
//Parameters:
//	int	ct	-	the circuit's current time
//Summary:
//	Empty Function.
//Returns:
//	n/a
//========================
void	LogicObj::itick(int ct) throw (BasicException)
{}

//========================
//Logic Object Trace
//========================
//Parameters:
//	int	oB	-	the old biggest length
//Summary:
//	Returns whatever is passed to it -- empty function.
//Returns:
//	int	-	old biggest length
//========================
int		LogicObj::trace(int oB) throw (BasicException)
{
	return oB;
}


//========================
//Connection Constructor
//========================
//Parameters:
//	string	tID	-	the ID of the Logic Object
//	state	def	-	the default state value of the Connection Object
//Summary:
//	Since all Connection Objects hold a single state, this base level
//		constructor holds a current and default state, initialized as
//		passed in the def value.
//========================
Connection::Connection(string tID, state def) throw (NullIDException, UnknownState) :
	LogicObj(tID)
{
	if ((def != low) && (def != high) && (def != indet))
		throw UnknownState();
	else
	{
		defCurrent = def;
		current = def;
	}
}

//========================
//Connection Set Current
//========================
//Parameters:
//	state	t	-	the state to set the current to
//Summary:
//	Sets the current to the state t, provided t is a valid state.
//Returns:
//	n/a
//========================
void	Connection::setCurrent(state t) throw (UnknownState)
{
	if ((t != low) && (t != high) && (t != indet))
		throw UnknownState();
	else
	{
		current = t;
	}
}

//========================
//Connection Get Current
//========================
//Parameters:
//	none
//Summary:
//	Returns the active current state on the Connection.
//Returns:
//	state	-	the current of the Connection.
//========================
state	Connection::getCurrent()
{
	return current;
}

//========================
//Connection Reset Current
//========================
//Parameters:
//	none
//Summary:
//	Sets the active current state to the default current saved back at
//		Connection instantiation.
//Returns:
//	n/a
//========================
void	Connection::resetCurrent()
{
	current = defCurrent;
}

//========================
//Connection Tick
//========================
//Parameters:
//	int	ct	-	circuit time.
//Summary:
//	Wrapper. See LogicObj::tick and derived classes ::tick.
//Returns:
//	n/a
//========================
void	Connection::tick(int ct) throw (BasicException)
{
	LogicObj::tick(ct);
}

//========================
//Connection Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time.
//Summary:
//	Wrapper. See LogicObj::itick and derived classes ::itick.
//Returns:
//	n/a
//========================
void	Connection::itick(int ct) throw (BasicException)
{
	LogicObj::itick(ct);
}

//========================
//Connection Trace
//========================
//Parameters:
//	int	oB	-	old biggest length.
//Summary:
//	Wrapper. See LogicObj::trace and derived classes ::trace.
//Returns:
//	int	-	the new old biggest length
//========================
int		Connection::trace(int oB) throw (BasicException)
{
	return LogicObj::trace(oB);
}

//========================
//Wire Constructor
//========================
//Parameters:
//	string	tID	-	the ID of the Logic Object
//	state	def	-	the default and original current state of the Connection
//Summary:
//	Creates a new Wire object. Wire objects add the additional properties of
//		inputs and outputs to the Connection base object type.
//========================
Wire::Wire(string tID, state def) throw (NullIDException, UnknownState) :
	Connection(tID, def)
{
	input = NULL;
	outputs = new vector<LogicObj*>(0);
}

//========================
//Wire Destructor
//========================
//Summary:
//	Releases the memory used by the vector of outputs.
//========================
Wire::~Wire()
{
	delete outputs;
}

//========================
//Wire Operator =
//========================
//Parameters:
//	const	Wire	&t	-	the Wire to set *this to.
//Summary:
//	Sets all the values of *this equal to those held by t.
//Returns:
//	Wire&	-	Wire *this
//========================
Wire&		Wire::operator= (const Wire &t)
{
	this->outputs = new vector<LogicObj*>(*(t.outputs));
	this->input = t.input;

	return *this;
}

//========================
//Wire Set Input
//========================
//Parameters:
//	LogicObj	*t	-	a pointer to a Logic Object
//Summary:
//	Sets the input to the Wire to a specific Logic Object.
//Returns:
//	n/a
//========================
void		Wire::setInput(LogicObj *t) throw (NullPointerException)
{
	if (t == NULL)
		throw NullPointerException();
	else
		input = t;
}

//========================
//Wire Get Input
//========================
//Parameters:
//	none
//Summary:
//	Returns the current input connection.
//Returns:
//	LogicObj*	-	a pointer to the current input
//========================
LogicObj*	Wire::getInput()
{
	return input;
}

//========================
//Wire Add Output
//========================
//Parameters:
//	LogicObj	*t	-	a pointer to a Logic Object
//Summary:
//	This function adds the pointer to a Logic Object to the Wire.
//Returns:
//	n/a
//========================
void		Wire::addOutput(LogicObj *t) throw (NullPointerException)
{
	if (t == NULL)
		throw NullPointerException();
	else
		outputs->push_back(t);
}

//========================
//Wire Clear Outputs
//========================
//Parameters:
//	none
//Summary:
//	Clears the outputs vector.
//Returns:
//	n/a
//========================
void		Wire::clrOutputs()
{
	outputs->clear();
}

//========================
//Wire Count Outputs
//========================
//Parameters:
//	none
//Summary:
//	Returns the current size of the outputs vector.
//Returns:
//	int	-	the current size of the outputs vector
//========================
int			Wire::cntOutputs()
{
	return outputs->size();
}

//========================
//Wire Remove Output (ID Edition)
//========================
//Parameters:
//	string tID	-	the ID of the Logic Object output connection to remove
//Summary:
//	Provided a Logic Object contained by outputs has a matching ID to tID,
//		removes that Logic Object pointer from the vector.
//Returns:
//	bool	-	true for success, false for failure.
//========================
bool		Wire::remOutput(string tID) throw (NullIDException)
{
	if (tID == "")
		throw NullIDException();
	else
	{
		vector<LogicObj*>::iterator oI = outputs->begin();

		while (oI != outputs->end())
		{
			if ((*oI)->getID() == tID)
			{
				outputs->erase(oI);
				return true;
			}
			oI++;
		}
		return false;
	}
}

//========================
//Wire Remove Output (Index Edition)
//========================
//Parameters:
//	int	idx	-	the index of the Logic Object output connection to remove
//Summary:
//	Provided idx is a valid index for outputs, removes the Logic Object
//		pointer at that index from outputs.
//Returns:
//	bool	-	true for success, false for failure.
//========================
bool		Wire::remOutput(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);
	else if (idx > outputs->size() - 1)
		throw IndexOutOfBounds(idx, outputs->size() - 1);
	else
	{
		vector<LogicObj*>::iterator oI = outputs->begin();

		for (int k = 0; k < idx; k++) // Inefficient, but that's okay for this.
		{
			oI++;
		}
		outputs->erase(oI);

		return true;
	}
	return false;
}

//========================
//Wire Get Output (ID Edition)
//========================
//Parameters:
//	string tID	-	the ID of the Logic Object to return
//Summary:
//	Provided there exists a Logic Object in outputs that has a matching ID
//		to tID, returns a pointer to that Logic Object.
//Returns:
//	LogicObj*	-	a pointer to a Logic Object
//========================
LogicObj*	Wire::getOutput(string tID) throw (NullIDException, ElementNotFound)
{
	if (tID == "")
		throw NullIDException();
	else
	{
		vector<LogicObj*>::iterator oI = outputs->begin();

		while (oI != outputs->end())
		{
			if ((*oI)->getID() == tID)
			{
				return (*oI);
			}
			oI++;
		}
		throw ElementNotFound(tID);
	}
}

//========================
//Wire Get Output (Index Edition)
//========================
//Parameters:
//	int	idx	-	the index of the Logic Object to return
//Summary:
//	Provided idx is a valid index in outputs, returns a pointer to the
//		Logic Object referenced at that index.
//Returns:
//	LogicObject*	-	a pointer to a Logic Object
//========================
LogicObj*	Wire::getOutput(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);
	else if (idx > outputs->size() - 1)
		throw IndexOutOfBounds(idx, outputs->size() - 1);
	else
	{
		vector<LogicObj*>::iterator oI = outputs->begin();

		for (int k = 0; k < idx; k++) // Inefficient, but that's okay for this.
		{
			oI++;
		}

		return (*oI);
	}
}

//========================
//Wire Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function is very simple in that it loops through all the wire's outputs,
//		calling each Logic Object's tick function.
//Returns:
//	n/a
//========================
void	Wire::tick(int ct) throw (BasicException)
{
	//Wire is a very simple device. It merely calls the tick() function of its outputs.
	//	Nothing fancy here.

	debugMSG(LogicObj::getID() + " - ");

	vector<LogicObj*>::iterator oI = outputs->begin();

	while (oI != outputs->end())
	{
		(*oI)->tick(ct);
		oI++;
	}

	Connection::tick(ct);
}

//========================
//Wire Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function loops through all its outputs and calls the itick of each.
//Returns:
//	n/a
//========================
void Wire::itick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + " + ");

	vector<LogicObj*>::iterator oI = outputs->begin();

	while (oI != outputs->end())
	{
		(*oI)->itick(ct);
		oI++;
	}

	Connection::itick(ct);
}

//========================
//Wire Trace
//========================
//Parameters:
//	int	oB	-	old biggest length
//Summary:
//	This function facilitates the finding of maximum circuit delay length
//		by calling the trace function of its outputs and returning the largest
//		discovered route among those.
//Returns:
//	int	-	the new biggest length
//========================
int		Wire::trace(int oB) throw (BasicException)
{
	//Calcs gate delays and such.
	int big = oB;

	vector<LogicObj*>::iterator oI = outputs->begin();

	while (oI != outputs->end())
	{
		int t = (*oI)->trace(oB);
		if (t > big)
			 big = t;

		oI++;
	}

	return Connection::trace(big);
}

//========================
//Pad Constructor
//========================
//Parameters:
//	string	tID	-	the ID of the Logic Object
//	state	def	-	the default state of the Connection Object
//Summary:
//	The Pad constructor adds one layer of functionality to the Connection,
//	which is an ioConnection pointer.
//========================
Pad::Pad(string tID, state def) throw (NullIDException, UnknownState) :
	Connection(tID, def)
{
	ioConnection = NULL;
}

//========================
//Pad Set Input/Output Connection
//========================
//Parameters:
//	Wire	*t	-	the Wire to set ioConnection to
//Summary:
//	Sets ioConnection to the Wire passed to the function.
//Returns:
//	n/a
//========================
void	Pad::setIO(Wire *t) throw (NullPointerException)
{
	if (t == NULL)
		throw NullPointerException();
	else
		ioConnection = t;
}

//========================
//Pad Get Input/Output Connection
//========================
//Parameters:
//	none
//Summary:
//	This function returns the current ioConnection.
//Returns:
//	Wire*	-	a pointer to a Wire
//========================
Wire*	Pad::getIO()
{
	return ioConnection;
}

//========================
//Pad Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	Wrapper function. See Connection::tick and derivative classes :: tick
//Returns:
//	n/a
//========================
void	Pad::tick(int ct) throw (BasicException)
{
	Connection::tick(ct);
}

//========================
//Pad Inverse Tick
//========================
//Parameters:
//	int ct	-	circuit time
//Summary:
//	Wrapper function. See Connection::tick and derivative classes :: tick
//Returns:
//	n/a
//========================
void Pad::itick(int ct) throw (BasicException)
{
	Connection::itick(ct);
}

//========================
//Pad Trace
//========================
//Parameters:
//	int	oB	-	old biggest length
//Summary:
//	Wrapper function. See Connection::tick and derivative classes :: tick
//Returns:
//	int	-	the new biggest length
//========================
int		Pad::trace(int oB) throw (BasicException)
{
	return Connection::trace(oB);
}


//========================
//Input Pad Constructor
//========================
//Parameters:
//	string	tID	-	the ID of the Logic Object
//	state	def	-	the default state of the Connection Object
//Summary:
//	The iPad is an end level specification of Pad, adding final implementations
//		of the various function prototyped in Pad.
//========================
iPad::iPad(string tID, state def) throw (NullIDException, UnknownState) :
	Pad(tID, def)
{}

//========================
//Input Pad Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function sets the Pad's output Wire's state to that held by the Pad.
//Returns:
//	n/a
//========================
void	iPad::tick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + " - ");

	Wire* t = Pad::getIO();

	// Since this is an input pad, after it is set, its tick is called. It forcably sets its
	// output wire to its current charge before calling the tick of its output wire.

	state tS = Connection::getCurrent();

	if ((tS != low) && (tS != high) && (tS != indet))
		throw UnknownState();
	else
	{
		t->setCurrent(tS);
		t->tick(ct);

		Pad::tick(ct);
	}
}

//========================
//Input Pad Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	Calls the iTick of the Connection Wire.
//Returns:
//	n/a
//========================
void iPad::itick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + " + ");

	Pad::getIO()->itick(ct);

	Pad::itick(ct);
}

//========================
//Input Pad Trace
//========================
//Parameters:
//	int	oB	-	old biggest length
//Summary:
//	Calls the trace function of its connection Wire.
//Returns:
//	int	-	the new biggest length
//========================
int	iPad::trace(int oB) throw (BasicException)
{
	int t = Pad::getIO()->trace(oB);

	return Pad::trace(t);
}


//========================
//Output Pad Constructor
//========================
//Parameters:
//	string	tID	-	the ID of the Logic Object
//	state	def	-	the default state of the Connection Object
//Summary:
//	The oPad is an end level specification of Pad, adding final implementations
//		of the various function prototyped in Pad.
//========================
oPad::oPad(string tID, state def) throw (NullIDException, UnknownState) :
	Pad(tID, def)
{}

//========================
//Output Pad Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function sets the Pad's Connection state to that held on its Input
//		Wire.
//Returns:
//	n/a
//========================
void	oPad::tick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + " - ");

	Wire* t = Pad::getIO();

	// Since this is an output pad and wires are passive, it forcibly sets its current charge
	// to that stored on the wire.

	// This is a terminal unit. No external tick functions are called, only its own inherited tick.

	state tS = t->getCurrent();

	if ((tS != low) && (tS != high) && (tS != indet))
		throw UnknownState();
	else
	{
		Connection::setCurrent(tS);

		Pad::tick(ct);
	}
}

//========================
//Output Pad Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	Calls the iTick of the Connection Wire.
//Returns:
//	n/a
//========================
void oPad::itick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() + " + ");
	Pad::itick(ct);
}

//========================
//Output Pad Trace
//========================
//Parameters:
//	int	oB	-	old biggest length
//Summary:
//	Final end to the trace() journey, returns the oB to date.
//Returns:
//	int	-	the new biggest length
//========================
int	oPad::trace(int oB) throw (BasicException)
{
	return Pad::trace(oB);
}

//========================
//PadState Constructor
//========================
//Parameters:
//	string	tID		-	ID of the Logic Object
//	state	def		-	Default state
//	Pad*	tLink	-	pointer to a Pad
//Summary:
//	Creates an extension on LogicObj that includes holding a default state,
//		a pointer to a Pad (vital), and the creation of a state history table.
//========================
PadState::PadState(string tID, state def, Pad* tLink) throw (NullIDException, NullPointerException, UnknownState) :
	LogicObj(tID)
{
	if (tLink == NULL)
		throw NullPointerException();
	else
		link = tLink;

	if ((def != low) && (def != high) && (def != indet))
		throw UnknownState();
	else
	{
		stateTable = new vector<state>(0, def);
		defState = def;
	}
}

//========================
//PadState Destructor
//========================
//Summary:
//	Deletes the state history table.
//========================
PadState::~PadState()
{
	delete stateTable;
}

//========================
//PadState Operator =
//========================
//Parameters:
//	const	PadState	&p	-	the PadState to set *this equal to
//Summary:
//	Sets all the elements of *this to p.
//Returns:
//	PadState&	-	PadState *this
//========================
PadState&	PadState::operator= (const PadState &p)
{
	this->stateTable = new vector<state>(*(p.stateTable));
	this->link = p.link;
	this->defState = p.defState;

	return *this;
}

//========================
//PadState Get Link
//========================
//Parameters:
//	none
//Summary:
//	Returns the link pointer held by the PadState
//Returns:
//	Pad*	-	a pointer to the linked Pad
//========================
Pad*		PadState::getLink()
{
	return link;
}

//========================
//PadState Add State (Simple Edition)
//========================
//Parameters:
//	state	t	-	the state to add to the PadState's history
//Summary:
//	This function merely appends the state to the end of the state table.
//Returns:
//	n/a
//========================
void		PadState::addState(state t) throw (UnknownState)
{
	if ((t != low) && (t != high) && (t != indet))
		throw UnknownState();
	else
		stateTable->push_back(t);
}

//========================
//PadState Add State (Index Edition)
//========================
//Parameters:
//	state	t	-	the state to add to the PadState's history
//	int		idx	-	the index at which to put the state
//Summary:
//	This is a wrapper function for the more complicated
//		addState(state, int, int). See its comments for specifics.
//Returns:
//	n/a
//========================
void		PadState::addState(state t, int idx) throw (UnknownState, IndexOutOfBounds)
{	//wrapper for more specific function.
	addState(t, idx, 1);

}

//========================
//PadState Add State (Range Edition)
//========================
//Parameters:
//	state	t	-	the state to add to the PadState's history
//	int		idx	-	the index at which to begin inserting the state
//	int		rng	-	the length of the insertion
//Summary:
//	This is a complicated history management routine for PadStates.
//		Basically, provided idx is non-negative, this routine will handle
//		the insertion of idx for a length of rng, regardless of where
//		idx points. There is no invalid upward index, as any index beyond
//		the current index will just cause addState to fill in the stateHistory
//		vector with the last available value (or the default if stateHistory
//		is currently empty) up to idx, then filling the vector with t until
//		rng is satisfied.Insertion inside the history is simple as well, as
//		the function merely overwrites whatever is currently at index and
//		travels until rng, creating new elements on the end of the history
//		if rng extends beyond the previous end.
//Returns:
//	n/a
//========================
void		PadState::addState(state t, int idx, int rng) throw (UnknownState, IndexOutOfBounds, NegativeRange)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);

	if (rng < 1)
		throw NegativeRange();

	if ((t != low) && (t != high) && (t != indet))
		throw UnknownState();

	if (stateTable->size() == 0) // table empty.
	{
		for (int kaa = 0; kaa < idx; kaa++)
		{
			stateTable->push_back(defState);
		} // fill to below idx with defState;

		for (int ka = idx; ka < idx + rng; ka++)
		{
			stateTable->push_back(t);
		} // fill to idx + rng with t.
	}
	else						// table NOT empty
	{
		if (idx > stateTable->size() - 1) // beyond end of table
		{
			state st = stateTable->at(stateTable->size() - 1); // last element.

			for (int kb = stateTable->size(); kb < idx; kb ++)
			{
				stateTable->push_back(st);
			}// fill with last value up until index, then place new value.

			for (int kd = 0; kd < rng; kd++)
			{
				stateTable->push_back(t);
			}
		}
		else
		{ // idx within stateTable.
			//rng may extend beyond stateTable or may be inside stateTable

			if (idx + rng < stateTable->size())	// inside.
			{
				for (int kf = idx; kf < idx + rng; kf++)
				{
					stateTable->at(kf) = t;
				}
			}
			else	//if (idx + rng >= stateTable->size()) - extends beyond
			{
				for (int kg = idx; kg < stateTable->size(); kg++)
				{
					stateTable->at(kg) = t;
				}

				for (int kh = stateTable->size(); kh < idx + rng; kh++)
				{
					stateTable->push_back(t);
				}
			}
		}
	}
}

//========================
//PadState Get State
//========================
//Parameters:
//	int	idx	-	the index from which to retrieve a state
//Summary:
//	This function, provided idx is non-negative, returns
//		the state recorded in stateTable (history) at that
//		index. If idx is beyond the end of the history, it
//		merely returns the last recorded history.
//Returns:
//	state	-	the state recorded at idx
//========================
state		PadState::getState(int idx) throw (IndexOutOfBounds, StateTableEmpty)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);

	if (stateTable->size() == 0)
		throw StateTableEmpty();

	if (idx > stateTable->size() - 1) // beyond end of table
		idx = stateTable->size() - 1; // send back last state

	return stateTable->at(idx);
}

//========================
//PadState Get State
//========================
//Parameters:
//	none
//Summary:
//	This function returns the state contained at the end of the
//		stateTable history.
//Returns:
//	state	-	the last recorded state
//========================
state		PadState::getState() throw (StateTableEmpty)
{
	if (stateTable->size() == 0)
		throw StateTableEmpty();

	return stateTable->at(stateTable->size() - 1); // send back last state
}

//========================
//PadState Count States
//========================
//Parameters:
//	none
//Summary:
//	This function returns the size of the stateTable history.
//Returns:
//	int	-	the size of stateTable
//========================
int			PadState::cntStates()
{
	return stateTable->size();
}

//========================
//PadState Clear States
//========================
//Parameters:
//	none
//Summary:
//	This function clears the contents of the stateTable history.
//Returns:
//	n/a
//========================
void		PadState::clrStates()
{
	stateTable->clear();
}

//========================
//PadState	Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function is a wrapper function. It adds no functionality, and
//		merely calls its parent's tick function.
//Returns:
//	n/a
//========================
void PadState::tick(int ct) throw (BasicException)
{
	LogicObj::tick(ct);
}

//========================
//PadState Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function is a wrapper function. It adds no functionality, and
//		merely calls its parent's itick function.
//Returns:
//	n/a
//========================
void PadState::itick(int ct) throw (BasicException)
{
	LogicObj::itick(ct);
}

//========================
//PadState Trace
//========================
//Parameters:
//	int	oB	-	old Biggest length
//Summary:
//	This function is a wrapper function. It adds no functionality, and
//		merely calls its parent's trace function.
//Returns:
//	int	-	the new biggest length
//========================
int	PadState::trace(int oB) throw (BasicException)
{
	return LogicObj::trace(oB);
}

//========================
//Input PadState Constructor
//========================
//Parameters:
//	string	tID		-	the ID of the Logic Object
//	state	def		-	the default stateHistory state
//	iPad*	tLink	-	a pointer to an Input Pad
//Summary:
//	This constructor makes PadState more specific. By accepting a specific
//		type of Pad, it allows its member function to deal with the Link pinter
//		as a pointer to an Input Pad.
//========================
iPadState::iPadState(string tID, state def, iPad* tLink) throw (NullIDException, NullPointerException, UnknownState) :
	PadState(tID, def, tLink)
{}

//========================
//Input PadState Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This is an Input PadState, therefore it is assumed that
//		it contains an anticipatory history to "give" to its
//		linked iPad. Therefore, this function retrieves the
//		history assumed to be stored for time ct and pushes
//		it onto the input Pad it is linked to. Then it calls
//		that Pad's tick function.
//Returns:
//	n/a
//========================
void iPadState::tick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() +  " - ");

	state tempS = indet;

	try
	{
		tempS = PadState::getState(ct);
	}
	catch (BasicException e)
	{
		cout << "Unable to getState in PadState" << endl;
		throw e;
	}

	PadState::getLink()->setCurrent(tempS);

	PadState::getLink()->tick(ct);

	PadState::tick(ct);
}

//========================
//Input PadState Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function doesn't do anything besides call the itick function
//		of its linked iPad, therefore starting the process of the itick
//		on its way.
//Returns:
//	n/a
//========================
void iPadState::itick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() +  " + ");

	PadState::getLink()->itick(ct);

	PadState::itick(ct);
}

//========================
//Input PadState Trace
//========================
//Parameters:
//	int	oB	-	old biggest length
//Summary:
//	The second leg in the chain (besides the Vector object) for the trace
//		function. This calls the trace of its linked iPad and sends back the
//		value that the iPad discovers as the longest pathway.
//Returns:
//	int	-	the new biggest length
//========================
int	iPadState::trace(int oB) throw (BasicException)
{
	int t = PadState::getLink()->trace(oB);
	return PadState::trace(t);
}

//========================
//Output PadState Constructor
//========================
//Parameters:
//	string	tID		-	the ID of the Logic Object
//	state	def		-	the default state of the stateHistory
//	oPad*	tLink	-	the oPad to link to
//Summary:
//	This further specifies the PadState object by restricting its
//		link pointer to a pointer to an Output Pad, allowing the methods
//		and functions of oPadState to treat link as such.
//========================
oPadState::oPadState(string tID, state def, oPad* tLink) throw (NullIDException, NullPointerException, UnknownState) :
	PadState(tID, def, tLink)
{}

//========================
//Output PadState Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	This function reflects the passive nature of the Output PadState.
//		Assuming that its tick is being called last, it inserts
//		whatever current is sitting in its linked Pad into its
//		history at index ct.
//Returns:
//	n/a
//========================
void oPadState::tick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() +  " - ");

	state tempS = indet;

	try
	{
		tempS = PadState::getLink()->getCurrent();
	}
	catch (BasicException e)
	{
		cout << "Unable to getState in PadState" << endl;
		throw e;
	}

	PadState::addState(tempS, ct);

	PadState::tick(ct);
}

//========================
//Output PadState Inverse Tick
//========================
//Parameters:
//	int	ct	-	circuit time
//Summary:
//	As this is assumed to be called last, it does nothing, thereby
//		allowing all the potential earlier branching functions to
//		attempt a new branch.
//Returns:
//	n/a
//========================
void oPadState::itick(int ct) throw (BasicException)
{
	debugMSG(LogicObj::getID() +  " + ");

	PadState::itick(ct);
}

//========================
//Output PadState Trace
//========================
//Parameters:
//	int	oB	-	old biggest length
//Summary:
//	As this is assumed to be the last object called, it does nothing
//		but return whatever value is passed it, thereby allowing all
//		the earlier branching routines to attempt other branches.
//Returns:
//	int	-	the new old biggest length
//========================
int	oPadState::trace(int oB) throw (BasicException)
{
	return PadState::trace(oB);
}