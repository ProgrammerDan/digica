#ifndef LOGIC_H
#define LOGIC_H

//================================================================
//Filename:		Logic Objects Definitions
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		This is a collection of defintions for various logic
//					objects, not including gates. Pads, PadStates and
//					Wires are predominantly represented, as well as
//					their parent classes.
//Revisions:
//
//================================================================

#include <vector>
#include <string>
#include <iostream>
#include "logicException.h"

using namespace std;

enum state {low, high, indet};

//==================================
//Class:	LogicObj
//Parent:	n/a
//==================================
//Description:
//	This is the most base of all Logic Objects -- it defines the global
//		templates and basic information access that is necessary later.
//==================================
class LogicObj
{
public:
				LogicObj(string tID) throw (NullIDException);

	string		getID() throw (NullIDException);

	int			getTime();
	void		incTime();
	void		clrTime();

virtual	void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);

private:

	string		ID;
	int			time;
};


//==================================
//Class:	Connection
//Parent:	LogicObj
//==================================
//Description:
//	The Connection object definition extends on the Logic Object
//		by adding a new layer of functionality to the tick, itick,
//		and trace functions, as well as adding the concept of
//		single state and default state to the Logic Object.
//==================================
class Connection : public LogicObj
{
public:
				Connection(string tID, state def) throw (NullIDException, UnknownState);

	void		setCurrent(state t) throw (UnknownState);
	state		getCurrent();
	void		resetCurrent();

virtual	void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);

private:

	state	current;
	state	defCurrent;
};

//==================================
//Class:	Wire
//Parent:	Connection
//==================================
//Description:
//	This class provides a full suite of accessor and mutator
//		functions relating to the added functionality of a list of
//		output Logic Objects and an input Logic Object.
//==================================
class Wire : public Connection
{
public:
				Wire(string tID, state def) throw (NullIDException, UnknownState);

				~Wire();

	Wire&		operator= (const Wire &t);

	void		setInput(LogicObj *t) throw (NullPointerException);
	LogicObj*	getInput();

	void		addOutput(LogicObj *t) throw (NullPointerException);
	void		clrOutputs();
	int			cntOutputs();
	bool		remOutput(string tID) throw (NullIDException);
	bool		remOutput(int idx) throw (IndexOutOfBounds);
	LogicObj*	getOutput(string tID) throw (NullIDException, ElementNotFound);
	LogicObj*	getOutput(int idx) throw (IndexOutOfBounds);

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);

private:

	LogicObj*			input;
	vector<LogicObj*>*	outputs;
};


//==================================
//Class:	Pad
//Parent:	Connection
//==================================
//Description:
//	Wrapper stepping stone class, adds the ioConnection pointer to a Wire
//		on top of Connection's set of features.
//==================================
class Pad : public Connection
{
public:
			Pad(string tID, state def) throw (NullIDException, UnknownState);

	void	setIO(Wire *t) throw (NullPointerException);
	Wire*	getIO();

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);

private:

	Wire*	ioConnection;
};

//==================================
//Class:	iPad
//Parent:	Pad
//==================================
//Description:
//	End level definition of the virtual functions prototyped in Pad,
//		Connection, and LogicObj, with Input slant.
//==================================
class iPad : public Pad
{
public:
				iPad(string tID, state def) throw (NullIDException, UnknownState);

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);
};

//==================================
//Class:	oPad
//Parent:	Pad
//==================================
//Description:
//	End level definition of the virtual functions prototyped in Pad,
//		Connection, and LogicObj, with Output slant.
//==================================
class oPad : public Pad
{
public:
				oPad(string tID, state def) throw (NullIDException, UnknownState);

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);
};


//==================================
//Class:	PadState
//Parent:	LogicObj
//==================================
//Description:
//	Root level extension from Logic Object which adds the features
//		of extended state history, advanced state access (even beyond
//		current availabel, advanced state recording (mutators), and a
//		link to a Pad.
//==================================
class PadState : public LogicObj
{
public:
				PadState(string tID, state def, Pad* Link) throw (NullIDException, NullPointerException, UnknownState);
				~PadState();

	PadState&	operator= (const PadState &p);

	Pad*		getLink();

	void		addState(state t) throw (UnknownState);
	void		addState(state t, int idx) throw (UnknownState, IndexOutOfBounds);
	void		addState(state t, int idx, int rng) throw (UnknownState, IndexOutOfBounds, NegativeRange);
	state		getState(int idx) throw (IndexOutOfBounds, StateTableEmpty);
	state		getState() throw (StateTableEmpty);
	int			cntStates();
	void		clrStates();

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);

private:

	Pad*		link;
vector<state>*	stateTable;
	state		defState;
};


//==================================
//Class:	iPadState
//Parent:	PadState
//==================================
//Description:
//	End level implementation of the functions prototyped in PadState
//		and LogicObj, with an Input slant.
//==================================
class iPadState : public PadState
{
public:
				iPadState(string tID, state def, iPad* Link) throw (NullIDException, NullPointerException, UnknownState);

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);
};


//==================================
//Class:	oPadState
//Parent:	PadState
//==================================
//Description:
//	End level implementation of the functions prototyped in PadState
//		and LogicObj, with an Output slant.
//==================================
class oPadState : public PadState
{
public:
				oPadState(string tID, state def, oPad* Link) throw (NullIDException, NullPointerException, UnknownState);

virtual void	tick(int ct) throw (BasicException);
virtual	void	itick(int ct) throw (BasicException);
virtual int		trace(int oB) throw (BasicException);
};

#endif //LOGIC_H