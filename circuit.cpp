//================================================================
//Filename:		Circuit Objects Implementations
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		Contains implementations for the large scale circuit
//					objects, which include the Vector object and
//					the Circuit object.
//Revisions:
//
//================================================================

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "logicException.h"
#include "logic.h"
#include "gate.h"
#include "circuit.h"

using namespace std;

//========================
//Vector Constructor
//========================
//Parameters:
//	string	tID	-	This is the ID of the vector. Mostly unused.
//Summary:
//	This is a simple constructor that creates a new Vector object.
//		It sets the various attributes of Vector to valid values,
//		and instantiates padStates.
//========================
			Vector::Vector(string tID) throw (NullIDException)
{
	if (tID == "")
		throw NullIDException();

	ID = tID;

	padStates = new list<PadState*>(0);

	nIPadStates = 0;
	nOPadStates = 0;
}

//========================
//Vector Destructor
//========================
//Summary:
//	Deletes the dynamically allocated padStates
//========================
			Vector::~Vector()
{
	delete padStates;
}

//========================
//Vector Operator =
//========================
//Parameters:
//	const	Vector	&v	-	is the rvalue vector that the lvalue vector should be set to.
//Summary:
//	Sets all the attributes of the *this Vector to those held by v.
//Returns:
//	The modified *this, so that you can put multiple ='s in a row.
//========================
Vector&		Vector::operator= (const Vector &v)
{
	this->ID = v.ID;
	this->nIPadStates = v.nIPadStates;
	this->nOPadStates = v.nOPadStates;
	delete this->padStates;
	this->padStates = new list<PadState*>(*(v.padStates));

	return *this;
}

//========================
//Vector Get ID
//========================
//Parameters:
//	None
//Summary:
//	Returns the private attribute ID
//Returns:
//	string	-	The ID as stored in *this Vector
//========================
string		Vector::getID() throw (NullIDException)
{
	if (ID == "")
		throw NullIDException();

	return ID;
}

//========================
//Vector Add PadState (input PadState version)
//========================
//Parameters:
//	iPadstate *a	- takes a pointer to an iPadState
//Summary:
//	If a isn't null, adds it to the front of the padStates vector.
//Returns:
//	n/a
//========================
void		Vector::addPadState(iPadState *a) throw (NullPointerException)
{
	if (a == NULL)
		throw NullPointerException();
	else
	{
		padStates->push_front(a);
		nIPadStates++;
	}
}

//========================
//Vector Add PadState (output PadState version)
//========================
//Parameters:
//	oPadState *b 	- takes a pointer to an oPadState
//Summary:
//	If b isn't null, adds it to the back of the padStates vector.
//Returns:
//	n/a
//========================
void		Vector::addPadState(oPadState *b) throw (NullPointerException)
{
	if (b == NULL)
		throw NullPointerException();
	else
	{
		padStates->push_back(b);
		nOPadStates++;
	}
}

//========================
//Vector Get PadState Based on Index
//========================
//Parameters:
//	int	idx	-	The integer index of the PadState to return
//Summary:
//	Provided idx is within the bounds of the padStates vector, returns the
//		pointer to a PadState held in padStates at that index (idx).
//Returns:
//	PadState*	-	a pointer to a PadState
//========================
PadState*	Vector::getPadState(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);

	if (idx > padStates->size() - 1) // beyond end of table
		throw IndexOutOfBounds(idx, padStates->size() - 1);

	list<PadState*>::iterator PSi = padStates->begin();

	for (int trav = 0; trav < idx; trav++)
	{
		PSi++;
	}

	return (*PSi);
}

//========================
//Vector Get PadState Based on ID
//========================
//Parameters:
//	string	tID	-	The name of the PadState to return
//Summary:
//	Provided an entry has the ID supplied, returns the pointer to
//		a PadState held in padStates with that name.
//Returns:
//	PadState*	-	a pointer to a PadState
//========================
PadState*	Vector::getPadState(string tID) throw (NullIDException, ElementNotFound)
{
	if (tID == "")
		throw NullIDException();

	list<PadState*>::iterator PSi = padStates->begin();

	while (PSi != padStates->end())
	{
		if ((*PSi)->getID() == tID)
			return (*PSi);
		PSi++;
	}

	throw ElementNotFound(tID);
}

//========================
//Vector Count PadStates
//========================
//Parameters:
//	none
//Summary:
//	Returns padStates's size.
//Returns:
//	int	-	the size of padStates
//========================
int			Vector::cntPadStates()
{
	return padStates->size();
}

//========================
//Vector Count IPadStates
//========================
//Parameters:
//	none
//Summary:
//	Returns the number of IPadStates held by the vector
//Returns:
//	int	-	the number of IPadStates in padStates
//========================
int			Vector::cntIPadStates()
{
	return nIPadStates;
}

//========================
//Vector Count OPadStates
//========================
//Parameters:
//	none
//Summary:
//	Returns the number of OPadStates held by the vector
//Returns:
//	int	-	the number of OPadStates in padStates
//========================
int			Vector::cntOPadStates()
{
	return nOPadStates;
}

//========================
//Vector Clear PadStates
//========================
//Parameters:
//	none
//Summary:
//	Clears the padStates vector and sets nIPadStates and nOPadStates to zero.
//Returns:
//	n/a
//========================
void		Vector::clrPadStates()
{
	padStates->clear();
	nIPadStates = 0;
	nOPadStates = 0;
}

//========================
//Vector Get IPadStates
//========================
//Parameters:
//	none
//Summary:
//	Returns only the IPadStates held by the padStates vector.
//Returns:
//	vector<iPadState*>*	-	A vector containing all the iPadState pointers held
//								by padStates.
//========================
vector<iPadState*>*	Vector::getIPadStates() throw (SourceListEmpty)
{
	if (padStates->size() == 0)
		throw SourceListEmpty();

	if (nIPadStates == 0)
		throw SourceListEmpty();

	vector<iPadState*>* tIPadStates = new vector<iPadState*>(0);

	list<PadState*>::iterator PSi = padStates->begin();

	for (int trav = 0; trav < nIPadStates; trav++)
	{
		tIPadStates->push_back((iPadState*) *PSi);
		PSi++;
	}

	return tIPadStates;
}

//========================
//Vector Get OPadStates
//========================
//Parameters:
//	none
//Summary:
//	Returns only the OPadStates held by the padStates vector.
//Returns:
//	vector<oPadState*>*	-	a vector containing all the oPadState pointers held
//								by padStates.
//========================
vector<oPadState*>*	Vector::getOPadStates() throw (SourceListEmpty)
{
	if (padStates->size() == 0)
		throw SourceListEmpty();

	if (nOPadStates == 0)
		throw SourceListEmpty();

	vector<oPadState*>* tOPadStates = new vector<oPadState*>(0);

	list<PadState*>::iterator PSi = padStates->end(); // start at the end.
	PSi--; // end() returns one beyond the end. -- to start at end.

	for (int trav = 0; trav < nOPadStates; trav++)
	{
		tOPadStates->push_back((oPadState*) *PSi);
		PSi--;
	}

	return tOPadStates;
}

//========================
//Vector Tick
//========================
//Parameters:
//	int	ct	-	the current time.
//Summary:
//	Since my code executes via a branching linked list execution fashion,
//		this tick function calls the tick function of all the PadStates held
//		by padStates. They in turn call the tick functions of their linked
//		pads, which calls the tick of their wires, which call their gates, etc.
//		Note: the call to itick resets all the gate's execution records, so
//			that they will execute next tick.
//Returns:
//	n/a
//========================
void		Vector::tick(int ct) throw (BasicException)
{
	list<PadState*>::iterator PSi = padStates->begin();

	while (PSi != padStates->end())
	{
		(*PSi)->tick(ct); // branches

		(*PSi)->itick(ct); // branches, undoing the incrementers that prevent gates from being run
							// more than once.

		PSi++;
	}

}

//========================
//Vector Trace
//========================
//Parameters:
//	none
//Summary:
//	Again, the branching linked list style of execution allows this kind
//		of depth first searching. Basically, it propogates through the circuit,
//		finding the longest single path from iPad to oPad.
//Returns:
//	int	-	The longest delay path from some iPad to some oPad, or equivalent.
//========================
int			Vector::trace() throw (BasicException)
{
	int oB = 0;
	int big = 0;
	int bPS = 0;
	int cPS = 0;

	list<PadState*>::iterator PSi = padStates->begin();

	while (PSi != padStates->end()) // branching search for longest pathway.
	{
		if (cPS < cntIPadStates())
			if ((*PSi)->cntStates() > bPS)
				bPS = (*PSi)->cntStates(); // finds biggest padstate input vector.

		int t = (*PSi)->trace(oB); // trace branches through objects.

		if (t > big)
			big = t; // keep longest path to date.

		PSi++;
		cPS++;
	}

	return (bPS + big);
}

//========================
//Circuit Constructor
//========================
//Parameters:
//	string	tID	-	The ID of the Circuit
//Summary:
//	Instantiates the various vectors stored in the circuit, of which there
//		are currently five.
//========================
Circuit::Circuit(string tID) throw (NullIDException)
{
	if (tID == "")
		throw NullIDException();

	ID = tID;

	LogicObjs = new vector<LogicObj*>(0);
	inPads = new vector<iPad*>(0);
	outPads = new vector<oPad*>(0);
	flow = NULL;
	wires = new vector<string>(0);
	gates = new vector<string>(0);
}

//========================
//Circuit Destructor
//========================
//Summary:
//	Deletes the various pieces of information stored dynamically by circuit,
//		such as all the logic objects, gates, wires, etc.
//========================
Circuit::~Circuit()
{
	delete LogicObjs;
	delete inPads;
	delete outPads;
	delete wires;
	delete gates;
	if (flow != NULL)
		delete flow;
}

//========================
//Circuit Operator =
//========================
//Parameters:
//	const	Circuit	&a	-	The circuit to set *this to.
//Summary:
//	Sets each data element of *this to those held in a.
//Returns:
//	Circuit&	-	*this, the replaced circuit.
//========================
Circuit&	Circuit::operator= (const Circuit &a)
{
	this->ID = a.ID;
	delete this->LogicObjs;
	this->LogicObjs = new vector<LogicObj*>(*(a.LogicObjs));
	delete this->inPads;
	this->inPads = new vector<iPad*>(*(a.inPads));
	delete this->outPads;
	this->outPads = new vector<oPad*>(*(a.outPads));
	delete this->wires;
	this->wires = new vector<string>(*(a.wires));
	delete this->gates;
	this->gates = new vector<string>(*(a.gates));

	if (!(this->flow == a.flow)) // If they aren't equal, clear before
	{								// replacing the information.
		delete flow;
		this->flow = a.flow;
	}

	return *this;
}

//========================
//Circuit Add Logic Object (Gate Edition)
//========================
//Parameters:
//	Gate *a	-	pointer to a Gate object.
//Summary:
//	This function was added sometime after original coding to allow me
//		to keep track of the names of the gates that were being added.
//		Although this functionality is not especially useful for much
//		besides debug output, it is retained here just for that eventuality.
//		It adds the Gate to LogicObjs and adds the Gate's name to gates.
//Returns:
//	n/a
//========================
void		Circuit::addLogicObj(Gate *a) throw (NullPointerException)
{
	if (a == NULL)
	{
		debugMSG("Circuit::Gate a is NULL");
		throw NullPointerException();
	}
	else
	{
		debugMSG("Circuit::Gate Not Null, adding");

		LogicObjs->push_back(a);
		gates->push_back(a->getID());

		debugMSG("Circuit::Added object");
	}
}

//========================
//Circuit Add Logic Object (Wire Edition)
//========================
//Parameters:
//	Wire	*a	-	pointer to a Wire object.
//Summary:
//	See addLogicObj(Gate *a), except referencing Wires, not Gates.
//Returns:
//	n/a
//========================
void		Circuit::addLogicObj(Wire *a) throw (NullPointerException)
{
	if (a == NULL)
	{
		debugMSG("Circuit::Wire a is NULL");
		throw NullPointerException();
	}
	else
	{
		debugMSG("Circuit::Wire Not Null, adding");

		LogicObjs->push_back(a);
		wires->push_back(a->getID());

		debugMSG("Circuit::Added object");
	}
}

//========================
//Circuit Add Logic Object (default)
//========================
//Parameters:
//	LogicObj	*a	-	pointer to some LogicObj derivitive
//Summary:
//	Adds the LogicObject pointed to by a to the LogicObjs vector.
//Returns:
//	n/a
//========================
void		Circuit::addLogicObj(LogicObj *a) throw (NullPointerException)
{
	if (a == NULL)
	{
		debugMSG("Circuit::a is NULL");
		throw NullPointerException();
	}
	else
	{
		debugMSG("Circuit::Not Null, adding");

		LogicObjs->push_back(a);

		debugMSG("Circuit::Added object");
	}
}

//========================
//Circuit Get Logic Object based on ID
//========================
//Parameters:
//	string	tID	-	the ID of the LogicObject to look for
//Summary:
//	If ID not null and matches a LogicObj in LogicObjs, sends that element back
//		out.
//Returns:
//	LogicObj*	-	The LogicObj with the ID that matches tID.
//========================
LogicObj*	Circuit::getLogicObj(string tID) throw (NullIDException, ElementNotFound)
{
	if (tID == "")
		throw NullIDException();

	vector<LogicObj*>::iterator LOi = LogicObjs->begin();

	while (LOi != LogicObjs->end())
	{
		if ((*LOi)->getID() == tID)
			return (*LOi);
		LOi++;
	}

	throw ElementNotFound(tID);
}

//========================
//Circuit Get Logic Object based on Index
//========================
//Parameters:
//	int	idx	-	the index of the LogicObject to return
//Summary:
//	Provided idx is within the LogicObjs bounds, returns that LogicObj.
//Returns:
//	LogicObj*	-	The LogicObj at index idx.
//========================
LogicObj*	Circuit::getLogicObj(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);

	if (idx > LogicObjs->size() - 1) // beyond end of table
		throw IndexOutOfBounds(idx, LogicObjs->size() - 1);

	return LogicObjs->at(idx);
}

//========================
//Circuit Count Logic Objects
//========================
//Parameters:
//	none
//Summary:
//	Returns the size of the LogicObjs vector.
//Returns:
//	int	-	the size of the LogicObjs vector.
//========================
int			Circuit::cntLogicObjs()
{
	return LogicObjs->size();
}

//========================
//Circuit Clear Logic Objects
//========================
//Parameters:
//	none
//Summary:
//	Clears the LogicObjs vector.
//Returns:
//	n/a
//========================
void		Circuit::clrLogicObjs()
{
	if (LogicObjs != NULL)
		LogicObjs->clear();
}

//========================
//Circuit Add Input Pad
//========================
//Parameters:
//	iPad	*a	-	a pointer to an iPad.
//Summary:
//	Provided a is not NULL, adds the Input Pad to the inPads vector.
//Returns:
//	n/a
//========================
void		Circuit::addiPad(iPad *a) throw (NullPointerException)
{
	if (a == NULL)
		throw NullPointerException();
	else
		inPads->push_back(a);
}

//========================
//Circuit Get Input Pad by ID
//========================
//Parameters:
//	string	tID	-	the ID of the iPad to find.
//Summary:
//	Provided ID isn't empty and there exists an iPad with that ID in the
//		inPads vector, returns that iPad.
//Returns:
//	iPad*	-	pointer to the iPad with the ID that matches tID.
//========================
iPad*		Circuit::getiPad(string tID) throw (NullIDException, ElementNotFound)
{
	if (tID == "")
		throw NullIDException();

	vector<iPad*>::iterator iPi = inPads->begin();

	while (iPi != inPads->end())
	{
		if ((*iPi)->getID() == tID)
			return (*iPi);
		iPi++;
	}

	throw ElementNotFound(tID);
}

//========================
//Circuit Get Input Pad by Index
//========================
//Parameters:
//	int	idx	-	index of iPad to retrieve
//Summary:
//	Provided idx is within the bounds of inPads, returns the iPad at that
//		index.
//Returns:
//	iPad*	-	pointer to the iPad at index idx.
//========================
iPad*		Circuit::getiPad(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);

	if (idx > inPads->size() - 1) // beyond end of table
		throw IndexOutOfBounds(idx, inPads->size() - 1);

	return inPads->at(idx);
}

//========================
//Circuit Count Input Pads
//========================
//Parameters:
//	none
//Summary:
//	Returns the number of Input Pads held by this circuit.
//Returns:
//	int	-	the size of inPads vector.
//========================
int			Circuit::cntiPads()
{
	return inPads->size();
}

//========================
//Circuit Clear Input Pads
//========================
//Parameters:
//	none
//Summary:
//	Clears the inPads vector.
//Returns:
//	n/a
//========================
void		Circuit::clriPads()
{
	if (inPads != NULL)
		inPads->clear();
}

//========================
//Circuit Add Output Pad
//========================
//Parameters:
//	oPad	*a	-	a pointer to an oPad.
//Summary:
//	Provided a is not NULL, adds it to the outPads vector
//Returns:
//	n/a
//========================
void		Circuit::addoPad(oPad *a) throw (NullPointerException)
{
	if (a == NULL)
		throw NullPointerException();
	else
		outPads->push_back(a);
}

//========================
//Circuit Get Output Pad by ID
//========================
//Parameters:
//	string	tID	-	the ID of the oPad to find.
//Summary:
//	Provided ID isn't empty and there exists an oPad with that ID in the
//		outPads vector, returns that oPad.
//Returns:
//	oPad*	-	pointer to the oPad with the ID that matches tID.
//========================
oPad*		Circuit::getoPad(string tID) throw (NullIDException, ElementNotFound)
{
	if (tID == "")
		throw NullIDException();

	vector<oPad*>::iterator oPi = outPads->begin();

	while (oPi != outPads->end())
	{
		if ((*oPi)->getID() == tID)
			return (*oPi);
		oPi++;
	}

	throw ElementNotFound(tID);
}

//========================
//Circuit Get Output Pad by Index
//========================
//Parameters:
//	int	idx	-	index of oPad to retrieve
//Summary:
//	Provided idx is within the bounds of outPads, returns the oPad at that
//		index.
//Returns:
//	oPad*	-	pointer to the oPad at index idx.
//========================
oPad*		Circuit::getoPad(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);

	if (idx > outPads->size() - 1) // beyond end of table
		throw IndexOutOfBounds(idx, outPads->size() - 1);

	return outPads->at(idx);
}

//========================
//Circuit Count Output Pads
//========================
//Parameters:
//	none
//Summary:
//	Returns the number of Output Pads held by this circuit.
//Returns:
//	int	-	the size of outPads vector.
//========================
int			Circuit::cntoPads()
{
	return outPads->size();
}

//========================
//Circuit Clear Output Pads
//========================
//Parameters:
//	none
//Summary:
//	Clears the outPads vector.
//Returns:
//	n/a
//========================
void		Circuit::clroPads()
{
	if (outPads != NULL)
		outPads->clear();
}

//========================
//Circuit Set Vector
//========================
//Parameters:
//	Vector	*a	-	a pointer to the Vector object to add to the circuit.
//Summary:
//	Provided a is not NULL, sets the Circuit's Vector to a.
//Returns:
//	n/a
//========================
void		Circuit::setVector(Vector *a) throw (NullPointerException)
{
	if (a == NULL)
		throw NullPointerException();
	else
		flow = a;
}

//========================
//Circuit Get Vector
//========================
//Parameters:
//	none
//Summary:
//	Returns whatever the Circuit's Vector is set to.
//Returns:
//	Vector*	-	a pointer to the Circuit's Vector.
//========================
Vector*		Circuit::getVector()
{
	return flow;
}

//========================
//Circuit Run
//========================
//Parameters:
//	int	tEnd	-	the number of nanoseconds to run the circuit.
//Summary:
//	Provided tEnd is not zero or negative, executes Circuit's Vector's tick
//		function once for each time interval from 0 to tEnd.
//Returns:
//	n/a
//========================
void		Circuit::run(int tEnd) throw (BasicException)
{
	if (tEnd < 1)
		throw BasicException("Time to run is zero or negative. INVALID!");

	for (int bca = 0; bca < tEnd + 1; bca++)
	{
		flow->tick(bca);

		int y = 0;
	}
}

//========================
//Circuit Clear Circuit
//========================
//Parameters:
//	none
//Summary:
//	Deletes all the vector data held by Circuit
//Returns:
//	n/a
//========================
void		Circuit::clrCircuit()
{
	if (flow != NULL)
	{
		delete flow;
		flow = NULL;
	}

	LogicObjs->clear();
	inPads->clear();
	outPads->clear();
	wires->clear();
	gates->clear();
}

//========================
//Circuit Get ID
//========================
//Parameters:
//	none
//Summary:
//	Returns the ID of the Circuit.
//Returns:
//	string	-	ID of Circuit.
//========================
string		Circuit::getID()
{
	return ID;
}