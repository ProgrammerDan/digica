#ifndef GUI_H
#define GUI_H

//================================================================
//Filename:		Graphic User Interface Class Definitions File
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		Contains definitions for the GUI classes,
//					including PadStateDisplay and UserInterface.
//Revisions:
//
//================================================================

#include <vector>
#include <string>
#include "logicException.h"
#include "logic.h"
#include "circuit.h"
#include "label.h"
#include "ray.h"

using namespace std;

//==================================
//Class:	PadStateDisplay
//Parent:	n/a
//==================================
//Description:
//	The PadStateDisplay object contains a set of E-Z Windows objects
//		instantiated in such a way as to communicate graphically the
//		condition of the data stored by a PadState.
//==================================
class PadStateDisplay
{
public:
						PadStateDisplay(SimpleWindow *wt, PadState *tPS, int tdl, Position tc, float tW) throw (NullPointerException, NegativeRange);
						~PadStateDisplay();

	PadStateDisplay&	operator = (const PadStateDisplay &p);

	int					draw(); // called by UI draw()
	int					draw(int toN);
	int					getDisplayLen();
	void				setWidth(float tW) throw (NegativeRange);
	float				getWidth();
	void				setTop(float tT) throw (NegativeRange);
	float				getTop();
	Position			getLeft();

private:
	SimpleWindow*			win;
	PadState*				padState;
	Label*					padLabel;
	vector<RaySegment*>*	raySegments;
	int						displayLen;
	Position				left;
	float					width;
	float					top;
};

//==================================
//Class:	UserInterface
//Parent:	n/a
//==================================
//Description:
//	The UserInterface Class is a container class to hold
//		various PadStateDisplays; as well, it provides functionality
//		for animated display of PadStateDisplays as well as generic
//		display of PadStateDisplays superimposed on a graph of time.
//		It also provides complete resizing support for SimpleWindow.
//==================================
class UserInterface
{
public:
						UserInterface(Circuit *ct, SimpleWindow *wt) throw (NullPointerException);
						~UserInterface();

	UserInterface&		operator = (const UserInterface &UI);

	Circuit*			getCircuit() throw (NullPointerException);
	SimpleWindow*		getWindow() throw (NullPointerException);
	PadStateDisplay*	getPadDisplay(int idx) throw (IndexOutOfBounds);

	int					draw(); // refreshcallback function.
	int					timedDraw(int time);
	void				incTime();
	void				resetTime();
	int					getTime();
	int					getMaxTime();

private:
	SimpleWindow *				win;
	Circuit *					circuit;
	vector<PadStateDisplay*>*	stateDisplays;
	int							maxTime;
	int							curTime;
	vector<RaySegment*>*		timeLines;
	vector<Label*>*				timeLabels;
};

#endif //GUI_H