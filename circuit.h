#ifndef CIRCUIT_H
#define CIRCUIT_H

//================================================================
//Filename:		Circuits Objects Definition File
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		Contains definitions and prototypes for the large
//					scale circuit objects, which include the Vector
//					object and the Circuit object.
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

using namespace std;

//==================================
//Class:	Vector
//Parent:	n/a
//==================================
//Description:
//	The Vector class is an access wrapper. Basically, it contains pointers
//		to PadState objects, which in the project layout contain time
//		sequenced state sets for input pads (and store time sequenced state
//		sets for output pads). In its most basic sense, a Vector object
//		merely "gets the ball rolling" and records the output of the circuit
//		for each time increment that is executed. It also includes functions
//		to handle tracing circuit maximum delay length and other useful
//		functions. See the .cpp file for complete descriptions of each.
//==================================
class Vector
{
public:
				Vector(string tID) throw (NullIDException);

				~Vector();

	Vector&		operator= (const Vector &v);

	string		getID() throw (NullIDException);
	void		addPadState(iPadState *a) throw (NullPointerException);
	void		addPadState(oPadState *b) throw (NullPointerException);

	PadState*	getPadState(int idx) throw (IndexOutOfBounds);
	PadState*	getPadState(string tID) throw (NullIDException, ElementNotFound);
	int			cntPadStates();
	int			cntIPadStates();
	int			cntOPadStates();
	void		clrPadStates();

vector<iPadState*>*	getIPadStates() throw (SourceListEmpty);
vector<oPadState*>*	getOPadStates() throw (SourceListEmpty);

	void		tick(int ct) throw (BasicException);
	int			trace() throw (BasicException);

private:

	string				ID;

	list<PadState*>*	padStates;
	int					nIPadStates;
	int					nOPadStates;
};

//==================================
//Class:	Circuit
//Parent:	n/a
//==================================
//Description:
//	The Circuit class is the container class for all the Logic Objects,
//		including a Vector. Basically, a Circuit contains pointers to
//		all the various elements that make up a complete digital logic
//		circuit, and a pointer to a Vector containing input data to
//		allow the circuit to execute. The Circuit object coordinates
//		the various objects involved in setting up a digital circuit by
//		provided a complete set of accessors and mutators for handling
//		logic objects and pads.
//==================================
class Circuit
{
public:
				Circuit(string tID) throw (NullIDException);

				~Circuit();

	Circuit&	operator= (const Circuit &a);

	void		addLogicObj(Gate *a) throw (NullPointerException);
	void		addLogicObj(Wire *a) throw (NullPointerException);
	void		addLogicObj(LogicObj *a) throw (NullPointerException);
	LogicObj*	getLogicObj(string tID) throw (NullIDException, ElementNotFound);
	LogicObj*	getLogicObj(int idx) throw (IndexOutOfBounds);
	int			cntLogicObjs();
	void		clrLogicObjs();

	void		addiPad(iPad *a) throw (NullPointerException);
	iPad*		getiPad(string tID) throw (NullIDException, ElementNotFound);
	iPad*		getiPad(int idx) throw (IndexOutOfBounds);
	int			cntiPads();
	void		clriPads();

	void		addoPad(oPad *a) throw (NullPointerException);
	oPad*		getoPad(string tID) throw (NullIDException, ElementNotFound);
	oPad*		getoPad(int idx) throw (IndexOutOfBounds);
	int			cntoPads();
	void		clroPads();

	void		setVector(Vector *a) throw (NullPointerException);
	Vector*		getVector();

	void		run(int tEnd) throw (BasicException);

	void		clrCircuit();

	string		getID();

private:

	string				ID;
	vector<LogicObj*>*	LogicObjs;
	vector<iPad*>*		inPads;
	vector<oPad*>*		outPads;
	Vector*				flow;
	vector<string>*		wires;
	vector<string>*		gates;
};

#endif //CIRCUIT_H