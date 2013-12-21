//================================================================
//Filename:		Graphic User Interface Class Implementations
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		Contains implementations for the GUI class definitions
//					including PadStateDisplay and UserInterface.
//Revisions:
//
//================================================================

#include <vector>
#include <string>
#include "logicException.h"
#include "logic.h"
#include "circuit.h"
#include "gui.h"

using namespace std;

//========================
//PadStateDisplay Constructor
//========================
//Parameters:
//	SimpleWindow	*wt		-	The SimpleWindow object everything is drawn onto
//	PadState		*tPS	-	The PadState object whose state table this
//									PadStateDisplay object represents
//	int				tdl		-	The length of PadState's display table to represent
//	Position		tc		-	The left edge center at which to draw the
//									PadStateDisplay object
//	float			tW		-	The width of the PadStateDisplay object
//Summary:
//	This constructor instantiates the PadStateDisplay object with all the various
//		pieces of data passed to it. It constructs a vector full of RaySegments
//		that represent the logic values stored in the PadState tPS. As well, it
//		stores a label that identifies which PadState is being displayed. All objects
//		are color-coded.
//========================
PadStateDisplay::PadStateDisplay(SimpleWindow *wt, PadState *tPS, int tdl, Position tc, float tW) throw (NullPointerException, NegativeRange)
{
	if (wt == NULL)
		throw NullPointerException();
	else
		win = wt;

	if (tPS == NULL)
		throw NullPointerException();
	else
		padState = tPS;

	if (tdl <= 0)
		throw NegativeRange();
	else
		displayLen = tdl;

	raySegments = new vector<RaySegment*>(0); // make the vector available

	if (tW <= 0.0)
		throw NegativeRange();
	else
		width = tW;

	left = tc;

	Position tL = Position(left.GetXDistance() + (width * 0.033), left.GetYDistance());
	// Label is assured first 10% of width

	top = tL.GetYDistance();

	padLabel = new Label(*wt, tL, padState->getID(), Black, White);

	// States get next 90%, of which there are displayLen states to make room for.
	float indStateLen = (width * .933) / (displayLen + 1);

	Position morph = Position(indStateLen, 0.0);	// change in x between elems

	//temp logic high - red
	Position tBh = Position(left.GetXDistance() + (width * 0.066), left.GetYDistance() - 0.1);
	Position tEh = Position(tBh.GetXDistance() + indStateLen, left.GetYDistance() - 0.1);

	//temp logic low - black
	Position tBl = Position(left.GetXDistance() + (width * 0.066), left.GetYDistance() + 0.1);
	Position tEl = Position(tBl.GetXDistance() + indStateLen, left.GetYDistance() + 0.1);

	//temp logic indeterminate - green
	Position tBi = Position(left.GetXDistance() + (width * 0.066), left.GetYDistance());
	Position tEi = Position(tBi.GetXDistance() + indStateLen, left.GetYDistance());

	state t = indet;
	for (int k = 0; k < displayLen; k++)
	{
		t = padState->getState(k);

		if (t == low)
		{
			RaySegment* j = new RaySegment(*win, tBl, tEl, Black, 0.1f, false);
			raySegments->push_back(j);
		}
		else if (t == high)
		{
			RaySegment* j = new RaySegment(*win, tBh, tEh, Red, 0.1f, false);
			raySegments->push_back(j);
		}
		else if (t == indet)
		{
			RaySegment* j = new RaySegment(*win, tBi, tEi, Green, 0.1f, false);
			raySegments->push_back(j);
		}
		else
			throw UnknownState();

		tBh = tBh + morph;
		tEh = tEh + morph;

		tBl	= tBl + morph;
		tEl = tEl + morph;

		tBi = tBi + morph;
		tEi = tEi + morph;
	}
}

//========================
//PadStateDisplay Destructor
//========================
//Summary:
//	Deletes everything that PadStateDisplay creates dynamically.
//========================
			PadStateDisplay::~PadStateDisplay()
{
	delete padLabel;

	if (raySegments != NULL) // deletes all the dynamically allocated
	{						 //  elements stored in raySegments
		for (int delSeg = 0; delSeg < raySegments->size(); delSeg++)
		{
			delete raySegments->at(delSeg);
		}
		delete raySegments;
	}
}

//========================
//PadStateDisplay Operator =
//========================
//Parameters:
//	const	PadStateDisplay	&p	-	the PadStateDisplay to set *this
//										equal to.
//Summary:
//	This function sets all of *this's attributes to those held by &p.
//Returns:
//	PadStateDisplay&	-	*this
//========================
PadStateDisplay&	PadStateDisplay::operator = (const PadStateDisplay &p)
{
	this->win = p.win;
	this->padState = p.padState;

	delete this->padLabel;
	this->padLabel = new Label(*(p.padLabel));

	delete this->raySegments;
	this->raySegments = new vector<RaySegment*>(*(p.raySegments));

	this->displayLen = p.displayLen;
	this->left = p.left;
	this->width = p.width;
	this->top = p.top;

	return *this;
}

//========================
//PadStateDisplay Draw
//========================
//Parameters:
//	none
//Summary:
//	This is a wrapper for draw(int). It calls draw in such a way that all
//		the RaySegments that PadStateDisplay contains are drawn.
//Returns:
//	int	-	the return of the more advanced draw(int);
//========================
int			PadStateDisplay::draw()	// Wrapper function to draw all segments.
{
	return draw(raySegments->size() - 1);
}

//========================
//PadStateDisplay Draw (segments up to an integer)
//========================
//Parameters:
//	int	toN	-	the last ending RaySegment to draw
//Summary:
//	This function draws certain elements of PadStateDisplay, which include
//		a label and a specific number of RaySegments, up to the index specified
//		by toN, provided toN is not negative. If toN is larger than the number
//		of RaySegments held, this function just draws all the RaySegments.
//Returns:
//	int	-	returns 1 for success.
//========================
int			PadStateDisplay::draw(int toN)
{
	if (toN < 0)
		throw NegativeRange();

	if (win == NULL)
		throw NullPointerException();

	if (padState == NULL)
		throw NullPointerException();

	if (padLabel == NULL)
		throw NullPointerException();

	padLabel->Erase();
	padLabel->Draw();

	for (int kk = 0; kk < raySegments->size(); kk++)
	{
		raySegments->at(kk)->Erase();
		if (kk <= toN)
			raySegments->at(kk)->Draw();
	}
	return 1;
}

//========================
//PadStateDisplay Get Display Length
//========================
//Parameters:
//	none
//Summary:
//	Returns the number of raySegments that PadStateDisplay holds, as set
//		by *this's constructor.
//Returns:
//	int	-	the number of raySegments PadStateDisplay holds
//========================
int			PadStateDisplay::getDisplayLen()
{
	return displayLen;
}

//========================
//PadStateDisplay Set Width
//========================
//Parameters:
//	float	tW	-	the new width
//Summary:
//	This function resizes all the elements contained by it so they fit the
//		original constructor percentages, but with the new overall width.
//Returns:
//	n/a
//========================
void		PadStateDisplay::setWidth(float tW) throw (NegativeRange)
{
	if (tW <= 0)
		throw NegativeRange();

	if (tW != width)
	{
		float ratio = tW / width;

		Position tP = Position(padLabel->GetPosition().GetXDistance() * ratio, padLabel->GetPosition().GetYDistance());
		padLabel->SetPosition(tP);

		for (int kk = 0; kk < raySegments->size(); kk++)
		{
			Position tP2 = Position(raySegments->at(kk)->GetStartPoint().GetXDistance() * ratio, raySegments->at(kk)->GetStartPoint().GetYDistance());
			Position tP3 = Position(raySegments->at(kk)->GetEndPoint().GetXDistance() * ratio, raySegments->at(kk)->GetEndPoint().GetYDistance());
			raySegments->at(kk)->SetPoints(tP2, tP3);
		}

		width = tW;
	}
}

//========================
//PadStateDisplay Get Width
//========================
//Parameters:
//	none
//Summary:
//	Returns the width of the display element.
//Returns:
//	float	-	PadStateDisplay's width
//========================
float		PadStateDisplay::getWidth()
{
	return width;
}

//========================
//PadStateDisplay Set Top
//========================
//Parameters:
//	float	tT	-	the new center location
//Summary:
//	This function moves all the elements so that they line up with the new
//		center, at tT
//Returns:
//	n/a
//========================
void		PadStateDisplay::setTop(float tT) throw (NegativeRange)
{
	if (tT <= 0)
		throw NegativeRange();

	if (top != tT)
	{
		float difT = tT - top;

		Position tP = Position(padLabel->GetPosition().GetXDistance(), padLabel->GetPosition().GetYDistance() + difT);
		padLabel->SetPosition(tP);

		for (int kk = 0; kk < raySegments->size(); kk++)
		{
			Position tP2 = Position(raySegments->at(kk)->GetStartPoint().GetXDistance(), raySegments->at(kk)->GetStartPoint().GetYDistance() + difT);
			Position tP3 = Position(raySegments->at(kk)->GetEndPoint().GetXDistance(), raySegments->at(kk)->GetEndPoint().GetYDistance() + difT);
			raySegments->at(kk)->SetPoints(tP2, tP3);
		}

		top = tT;
		left = Position(left.GetXDistance(), top);
	}
}

//========================
//PadStateDisplay Get Top
//========================
//Parameters:
//	none
//Summary:
//	Sends back the center top location of the PadStateDispay.
//Returns:
//	float	-	the center top location of the PadStateDisplay
//========================
float		PadStateDisplay::getTop()
{
	return top;
}

//========================
////PadStateDisplay Get Left
//========================
//Parameters:
//	none
//Summary:
//	Sends back the left centered location of the PadStateDisplay
//Returns:
//	Position	-	the left centered location of the PadStateDisplay
//========================
Position	PadStateDisplay::getLeft()
{
	return left;
}


//========================
//User Interface Constructor
//========================
//Parameters:
//	Circuit			*ct	-	The Circuit to represent
//	SimpleWindow	*wt	-	The Window to draw everything to
//Summary:
//	Creates a proto graphical representation of a Circuit object,
//		it draws a representation of the input and output PadState
//		objects against a time diagram.
//========================
UserInterface::UserInterface(Circuit *ct, SimpleWindow *wt) throw (NullPointerException)
{
	if (wt == NULL)
		throw NullPointerException();
	else
		win = wt;

	if (ct == NULL)
		throw NullPointerException();
	else
		circuit = ct;

	timeLines = new vector<RaySegment*>(0);
	timeLabels = new vector<Label*>(0);

	if (circuit->getVector() == NULL)
		throw NullPointerException();
	else
	{
		float winwidth = win->GetWidth();
		float winheight = win->GetHeight();
		float elemLeft = 0; // flush with side of window
		float elemWidth = winwidth;

		// note that global labels get top 10%
		// everybody else divides it up evenly, based on number of padstates.

		int tSize = circuit->getVector()->cntPadStates();

		stateDisplays = new vector<PadStateDisplay*>(0);

		float elemheight = (winheight * 0.86);

		if (tSize > -1)
			elemheight = elemheight / (tSize);

		Position tPos = Position(elemLeft, (winheight * 0.14) + (elemheight / 2)); // yields a centered position

		int Runtime = circuit->getVector()->trace();

		maxTime = Runtime;

		// Create vertical time lines every few nanoseconds.
		int lineSpace = 0; // Code picks an appropriate number of lines to display.

		if (Runtime > 300)		lineSpace = (int) (Runtime / 20);
		else if (Runtime > 200) lineSpace = 15;
		else if (Runtime > 50)	lineSpace = 10;
		else if (Runtime > 10)	lineSpace = 5;
		else					lineSpace = 3;

		float timingLine = (elemWidth * .933) / (Runtime + 1);
		timingLine = timingLine * lineSpace;

		Position sLine = Position((elemWidth * 0.066), (winheight * 0.14)); // start at 0 time.
		Position eLine = Position((elemWidth * 0.066), winheight);
		Position lLoc =  Position((elemWidth * 0.066), (winheight * 0.12));

		Position lMorph = Position(timingLine, 0);

		for (int doLine = 0; doLine < Runtime; doLine += lineSpace)
		{
			Label *tLLab = new Label(*win, lLoc, itoa(doLine, new char[50], 10), Blue, White);
			timeLabels->push_back(tLLab);

			RaySegment *tLRay = new RaySegment(*win, sLine, eLine, Blue, 0.01f, false);
			timeLines->push_back(tLRay);

			sLine = sLine + lMorph;
			eLine = eLine + lMorph;
			lLoc  = lLoc  + lMorph;
		}

		try
		{
			vector<iPadState*>* tIPadStates = circuit->getVector()->getIPadStates();

			vector<oPadState*>* tOPadStates = circuit->getVector()->getOPadStates();

			for (int ab = 0; ab < tIPadStates->size(); ab++)
			{
				PadStateDisplay* temp = new PadStateDisplay(win, tIPadStates->at(ab), Runtime, tPos, elemWidth);
				stateDisplays->push_back(temp);
				tPos = tPos + Position(0, elemheight);
			}

			for (int ba = 0; ba < tOPadStates->size(); ba++)
			{
				PadStateDisplay* temp = new PadStateDisplay(win, tOPadStates->at(ba), Runtime, tPos, elemWidth);
				stateDisplays->push_back(temp);
				tPos = tPos + Position(0, elemheight);
			}
		}
		catch (SourceListEmpty e) // not inputs
		{
		}
	}
	curTime = 0;
}

//========================
//User Interface Destructor
//========================
//Summary:
//	Deletes all the dynamically created elements stored by User Interface.
//========================
					UserInterface::~UserInterface()
{
	if (stateDisplays != NULL)
	{
		for (int delSD = 0; delSD < stateDisplays->size(); delSD++)
		{
			delete stateDisplays->at(delSD);
		}
		delete stateDisplays;
	}
	if (timeLines != NULL)
	{
		for (int delTL = 0; delTL < timeLines->size(); delTL++)
		{
			delete timeLines->at(delTL);
		}
		delete timeLines;
	}
	if (timeLabels != NULL)
	{
		for (int delTiL = 0; delTiL < timeLabels->size(); delTiL++)
		{
			delete timeLabels->at(delTiL);
		}
		delete timeLabels;
	}
}

//========================
//User Interface Operator =
//========================
//Parameters:
//	const	UserInterface	&UI	-	The UserInterface to set *this to
//Summary:
//	Sets all the elements of *this to those contained by UI.
//Returns:
//	UserInterface&	-	The modified *this
//========================
UserInterface&		UserInterface::operator = (const UserInterface &UI)
{
	this->win = UI.win;
	this->circuit = UI.circuit;
	delete stateDisplays;
	this->stateDisplays = new vector<PadStateDisplay*> (*(UI.stateDisplays));
	this->maxTime = UI.maxTime;
	this->curTime = UI.curTime;
	delete timeLines;
	this->timeLines = new vector<RaySegment*> (*(UI.timeLines));
	delete timeLabels;
	this->timeLabels = new vector<Label*> (*(UI.timeLabels));
	return *this;
}

//========================
//User Interface Get Circuit
//========================
//Parameters:
//	none
//Summary:
//	Returns the Circuit object associated with this User Interface.
//Returns:
//	Circuit*	-	pointer to the Circuit
//========================
Circuit*			UserInterface::getCircuit() throw (NullPointerException)
{
	if (circuit == NULL)
		throw NullPointerException();
	else
		return circuit;
}

//========================
//User Interface Get Window
//========================
//Parameters:
//	none
//Summary:
//	Returns the Simple Window associated with User Interface
//Returns:
//	SimpleWindow*	-	pointer to the Simple Window
//========================
SimpleWindow*		UserInterface::getWindow() throw (NullPointerException)
{
	if (win == NULL)
		throw NullPointerException();
	else
		return win;
}

//========================
//User Interface Get Pad Display
//========================
//Parameters:
//	int	idx	-	the index of the PadStateDisplay to retrieve
//Summary:
//	Provided idx is a valid index, returns the pointer to the PadStateDisplay
//		at index idx as held in stateDisplays.
//Returns:
//	PadStateDisplay*	-	pointer to a PadStateDisplay
//========================
PadStateDisplay*	UserInterface::getPadDisplay(int idx) throw (IndexOutOfBounds)
{
	if (idx < 0)
		throw IndexOutOfBounds(idx, 0);
	else if (idx > stateDisplays->size())
		throw IndexOutOfBounds(idx, stateDisplays->size());
	else
		return stateDisplays->at(idx);
}

//========================
//User Interface Draw (all)
//========================
//Parameters:
//	none
//Summary:
//	Wrapper for the timedDraw(int) function, in such a way that it calls
//		timedDraw to display all elements.
//Returns:
//	int	-	the return of timedDraw(int)
//========================
int					UserInterface::draw() // refreshcallback function.
{										// wrapper for "final timed" timedDraw function.
	return timedDraw(maxTime);
}

//========================
//User Interface Time based Draw
//========================
//Parameters:
//	int	time	-	The time index up to which all information should be
//						displayed
//Summary:
//	Sends the draw command to all child objects to display their information
//		up to time value time.
//Returns:
//	int	-	1 for success, 0 for failure.
//========================
int UserInterface::timedDraw(int time)
{
	if (win == NULL)
		throw NullPointerException();

	if (circuit == NULL)
		throw NullPointerException();

	float winwidth = win->GetWidth();
	float winheight = win->GetHeight();
	float elemLeft = 0;
	float elemWidth = winwidth;

	win->RenderText(Position(0,0), Position(winwidth, winheight * 0.1), string("Circuit: ").append(circuit->getID()).append(", Vector: ").append(circuit->getVector()->getID()), Black, White);

	int tSize = stateDisplays->size();

	float elemheight = (winheight * 0.86);

	if (tSize > -1)
		elemheight = elemheight / (tSize);

	Position tPos = Position(elemLeft, (winheight * 0.14) + (elemheight / 2));

	int lineSpace = 0; // Code picks an appropriate number of lines to display.

	if (maxTime > 300)		lineSpace = (int) (maxTime / 20);
	else if (maxTime > 200) lineSpace = 15;
	else if (maxTime > 50)	lineSpace = 10;
	else if (maxTime > 10)	lineSpace = 5;
	else					lineSpace = 3;

	float timingLine = (elemWidth * 0.933) / (maxTime + 1);
	timingLine = timingLine * lineSpace;

	Position sLine = Position((elemWidth * 0.066), (winheight * 0.14)); // start at 0 time.
	Position eLine = Position((elemWidth * 0.066), winheight);
	Position lLoc =  Position((elemWidth * 0.066), (winheight * 0.12));

	Position lMorph = Position(timingLine, 0);

	for (int doLine = 0; doLine < timeLines->size(); doLine++)		// Draws a time index
	{
		timeLabels->at(doLine)->SetPosition(lLoc);
		timeLines->at(doLine)->SetPoints(sLine, eLine);
		timeLabels->at(doLine)->Draw();
		timeLines->at(doLine)->Draw();

		sLine = sLine + lMorph;
		eLine = eLine + lMorph;
		lLoc  = lLoc  + lMorph;
	}

	for (int k = 0; k < stateDisplays->size(); k++)			// Draws the PadStateDisplays
	{
		stateDisplays->at(k)->setTop(tPos.GetYDistance());
		stateDisplays->at(k)->setWidth(elemWidth);
		stateDisplays->at(k)->draw(time);
		tPos = tPos + Position(0, elemheight);
	}

	return 1;
}

//========================
//User Interface Increment Time
//========================
//Parameters:
//	none
//Summary:
//	Increments the current time as held by the circuit forward one unit.
//		Ceases to increment when current time reaches maximum time.
//Returns:
//	n/a
//========================
void UserInterface::incTime()
{
	curTime++;
	if (curTime > maxTime)
	{
		curTime = maxTime;
	}
}

//========================
//User Interface Reset Time
//========================
//Parameters:
//	none
//Summary:
//	Resets the current time to 0.
//Returns:
//	n/a
//========================
void UserInterface::resetTime()
{
	curTime = 0;
}

//========================
//User Interface Get Time
//========================
//Parameters:
//	none
//Summary:
//	Returns the current time.
//Returns:
//	int	-	Current time.
//========================
int	UserInterface::getTime()
{
	return curTime;
}

//========================
//User Interface Get Max Time
//========================
//Parameters:
//	none
//Summary:
//	Returns the Maximum time.
//Returns:
//	int	-	Maximum Time
//========================
int	UserInterface::getMaxTime()
{
	return maxTime;
}