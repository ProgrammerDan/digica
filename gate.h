#ifndef GATE_H
#define GATE_H

//================================================================
//Filename:		Gate Objects Definitions File
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		Contains definitions for Gate objects, including
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

using namespace std;

state XOR(state a, state b) throw (UnknownState);
state XNOR(state a, state b) throw (UnknownState);
state OR(state a, state b) throw (UnknownState);
state AND(state a, state b) throw (UnknownState);
state NOT(state a) throw (UnknownState);

//==================================
//Class:	Gate
//Parent:	LogicObj
//==================================
//Description:
//	The Gate class is the parent class for all the Gate derivatives and
//		provides a full-featured function set for all Gate behaviors, as
//		well as basic global instantiations of the tick, itick, trace, and
//		transform virtual functions.
//==================================
class Gate : public LogicObj
{
public:
				Gate(string tID, state def, int delay) throw (UnknownState, InvalidDelay);

				~Gate();

	Gate&		operator= (const Gate &g);

	int			getDelaySize();
	void		clrDelay();
	void		setState(state a) throw (UnknownState);
	void		modState(state a) throw (UnknownState);
	state		getState() throw (StateTableEmpty, UnknownState); // end state
	string		getStateSnapshot() throw (StateTableEmpty);

	void		setOutput(Wire *t) throw (NullPointerException);
	Wire*		getOutput();

	void		addInput(Wire *t) throw (NullPointerException);
	bool		remInput(string tID) throw (NullIDException);
	bool		remInput(int idx) throw (IndexOutOfBounds);
	Wire*		getInput(string tID) throw (NullIDException, ElementNotFound);
	Wire*		getInput(int idx) throw (IndexOutOfBounds);
	void		clrInputs();
	int			cntInputs();

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);
virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);

private:

	int			delaySize;
list<state>*	delayTable;
	state		defCurrent;
	Wire*		output;
vector<Wire*>*	inputs;

	bool		nTick;
	bool		mTick;
	bool		tTick;
};

//==================================
//Class:	logicOR
//Parent:	Gate
//==================================
//Description:
//	This logic OR gate provides end implementation of the transform function
//		with relation to digital OR logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicOR : public Gate
{
public:
					logicOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};

//==================================
//Class:	logicAND
//Parent:	Gate
//==================================
//Description:
//	This logic AND gate provides end implementation of the transform function
//		with relation to digital AND logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicAND : public Gate
{
public:
					logicAND(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};

//==================================
//Class:	logicNOT
//Parent:	Gate
//==================================
//Description:
//	This logic NOT gate provides end implementation of the transform function
//		with relation to digital NOT logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicNOT : public Gate
{
public:
					logicNOT(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};

//==================================
//Class:	logicNAND
//Parent:	Gate
//==================================
//Description:
//	This logic NAND gate provides end implementation of the transform function
//		with relation to digital NAND logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicNAND : public Gate
{
public:
					logicNAND(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};

//==================================
//Class:	logicNOR
//Parent:	Gate
//==================================
//Description:
//	This logic NOR gate provides end implementation of the transform function
//		with relation to digital NOR logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicNOR : public Gate
{
public:
					logicNOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};

//==================================
//Class:	logicXOR
//Parent:	Gate
//==================================
//Description:
//	This logic XOR gate provides end implementation of the transform function
//		with relation to digital XOR logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicXOR : public Gate
{
public:
					logicXOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};

//==================================
//Class:	logicXNOR
//Parent:	Gate
//==================================
//Description:
//	This logic XNOR gate provides end implementation of the transform function
//		with relation to digital XNOR logic.
//		See Gate for definitions and implementations of Gate's member functions
//		and attributes.
//==================================
class logicXNOR : public Gate
{
public:
					logicXNOR(string tID, state def, int delay) throw (UnknownState, InvalidDelay, NullPointerException);

	virtual void	transform(bool r) throw (NoInputsException, NoOutputException, NullPointerException, UnknownState);
};


#endif // GATE_H