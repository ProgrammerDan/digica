//================================================================
//Filename:		Digital Circuit Analyzer v. 1.0
//Programmer:	Daniel "ProgrammerDan" Boston
//Date:			April 19, 2005
//Summary:		This program forms the file loading interface and
//					circuit creation routines for all the other
//					classes in this project. Basically, this code
//					pulls it all together and gives life to static
//					class definitions.
//Revisions:
//
//================================================================

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "logicException.h"
#include "logic.h"
#include "gate.h"
#include "circuit.h"
#include "gui.h"
#include "ezwin.h"

using namespace std;

// Global gui user interface, instantiated later.
UserInterface* gui = NULL;

void	coutPadState(string Title, int mCnt, PadState *tP);
int		guiTimeDraw ();
int		guiDraw		();

//========================
//Print PadState
//========================
//Parameters:
//	string		Title	-	a static title to print
//	int			mCnt	-	the number of states to retrieve
//	PadState	*tP		-	the PadState to display
//Summary:
//	This function is a simple Debug function; it takes
//		a PadState and prints the various states as held
//		by the PadState, up to time increment mCnt.
//Returns:
//	n/a
//========================
void coutPadState(string Title, int mCnt, PadState *tP)
{
	if (!(tP == NULL))
	{
		cout << Title << ": [";

		try
		{
			for (int abc = 0; abc < mCnt; abc++)
			{
				cout << tP->getState(abc) << ",";
			}

			cout << tP->getState(mCnt) << "]" << endl;
		}
		catch (StateTableEmpty e)
		{
			cout << "Empty]" << endl;
		}
		catch (BasicException f)
		{
			cout << "Error]" << endl;
		}
	}
}

//========================
//Graphical User Interface Draw
//========================
//Parameters:
//	none
//Summary:
//	Because of the animated nature of the GUI, this calls
//		the gui object's timedDraw function, and displays up to the
//		current time (or the max time if that's the case).
//		NOTE: Used as Refresh Callback Function for ezwin.
//Returns:
//	int	-	status of the function, 1 for Success, 0 for Failure.
//========================
int guiDraw()
{
	try
	{
		gui->timedDraw(gui->getTime());
		return 1;
	}
	catch (...)
	{
		cout << "Error drawing interface" << endl;
		return 0;
	}
}

//========================
//Graphical User Interface Time Draw
//========================
//Parameters:
//	none
//Summary:
//	This manages the animation of PadState results. On each call
//		it increments the gui object's timer, and displays the next
//		animation sequence. Once maxtime is reached, the animation
//		ceases.
//Returns:
//	int	-	status of the function, 1 for Success, 0 for Failure.
//========================
int guiTimeDraw()
{
	if (gui->getTime() == gui->getMaxTime())
		gui->getWindow()->StopTimer();

	gui->timedDraw(gui->getTime());
	gui->incTime();
	return 1;
}

//========================
//Application Main
//========================
//Parameters:
//	none
//Summary:
//	Wrapper for main() and holder of all the circuit instantiation
//		code. Builds circuits like an artist paints... little by little,
//		each piece building on the pieces around it.
//Returns:
//	int	-	status of the function, 0 for Success, 1 for Failure
//========================
int ApiMain()
{
	//Circuit.
	Circuit* crc = NULL;

	//basic interface.
	string	cir = "";
	string	vec = "";

	fstream fcir;
	fstream fvec;

	cout << endl;
	cout << "------======<<<<<<(((((( DigiCA ))))))>>>>>>======------" << endl << endl;

	cout << "  Welcome to Daniel Boston's Digital Circuit Analyzer." << endl;
	cout << endl;
	cout << "________________________________________________________" << endl << endl;

	while (cir == "")
	{
		cout << "Please enter the name of the circuit file you would like" << endl;
		cout << "to load, without extension (.txt assumed): ";
		cin >> cir;
	}

	cout << "Please enter the name of the vector file you would like" << endl;
	cout << "to load, without extension (.txt assumed): ";
	cin >> vec;

	if (vec == "")
	{
		vec = cir;
		vec.append("_v.txt");
	}
	else
		vec.append(".txt");

	cir.append(".txt");

	fcir.open(cir.c_str(), fstream::in);
	fvec.open(vec.c_str(), fstream::in);

	if (fcir.good() == 0)
	{
		cout << "Unable to open source file." << endl;
		Terminate();
		return 1;
	}

	if (fvec.good() == 0)
	{
		cout << "Unable to open vector file." << endl;
		Terminate();
		return 1;
	}

	// Loop through input files.

	//Circuit File first.

	try
	{
		string CMD;

		while (!(fcir.eof()))
		{

			fcir >> CMD;

			if ((CMD == "") || fcir.eof()) // Final Blank line or EOF
				break;

			if (CMD == "CIRCUIT") // Create a new circuit.
			{
				string tname;
				fcir >> tname;

				if (tname == "")
					throw NullIDException();

				crc = new Circuit(tname);
			}
			else if (CMD == "INPUT") // Input Pad
			{
				string tname;
				string wName;
				fcir >> tname >> wName;

				if (crc == NULL)
					throw NullPointerException();

				Wire* t = NULL;
				try
				{
					t = (Wire*) crc->getLogicObj(wName);
				}
				catch (ElementNotFound e) // It's not already in the circuit
				{
					t = new Wire(wName, indet);
					crc->addLogicObj(t);
				}

				if (t == NULL)
					throw NullPointerException();

				iPad* tPad = new iPad(tname, indet);

				tPad->setIO(t);		// Link the Wire and the Pad.
				t->setInput(tPad);

				crc->addiPad(tPad);
			}
			else if (CMD == "OUTPUT") // Output Pad
			{
				string tname;
				string wName;
				fcir >> tname >> wName;

				if (crc == NULL)
					throw NullPointerException();

				Wire* t = NULL;
				try
				{
					t = (Wire*) crc->getLogicObj(wName);
				}
				catch (ElementNotFound e) // It's not already in the circuit
				{
					t = new Wire(wName, indet);
					crc->addLogicObj(t);
				}

				if (t == NULL)
					throw NullPointerException();

				oPad* tPad = new oPad(tname, indet);

				tPad->setIO(t);		// Link the wire and the Pad
				t->addOutput(tPad);

				crc->addoPad(tPad);
			}
			else
			{ // It's a gate
				if (crc == NULL)
					throw NullPointerException();

				// Prepare variables for data.
				Gate*	gt = NULL;

				Wire*	wi1 = NULL; // These are explicitly set to NULL
				Wire*	wi2 = NULL; // because VC++ doesn't do it for me.
				Wire*	wo = NULL;

				string	tdly;
				int		tdly2;
				string	twi1;
				string	twi2;
				string	two;
				string	tID;

				fcir >> tdly;

				if (tdly.substr(tdly.size() - 1, 1) == "s") // has ns
				{
					tdly = tdly.substr(0, tdly.size() - 2);
					tdly2 = atoi(tdly.c_str());
				}
				else // lacks ns
					tdly2 = atoi(tdly.c_str());

				if (tdly2 <= 0) // avoid invalid delay.
					tdly2 = 1;

				tID = CMD;	// ID will be constructed.

				// Single input
				if (CMD == "NOT" || CMD == "INVERTER" || CMD == "INV")
				{
					fcir >> twi1 >> two;

					tID = tID.append(twi1).append(two); // ID is generated by GateType[W][W].

					try // Wire Input
					{
						wi1 = (Wire*) crc->getLogicObj(twi1);
					}
					catch (ElementNotFound e) // It's not already in the circuit
					{
						wi1 = new Wire(twi1, indet);
						crc->addLogicObj(wi1);
					}

					if (wi1 == NULL)
						throw NullPointerException();


					try // Wire Ouput
					{
						wo = (Wire*) crc->getLogicObj(two);
					}
					catch (ElementNotFound e) // It's not already in the circuit
					{
						wo = new Wire(two, indet);
						crc->addLogicObj(wo);
					}

					if (wo == NULL)
						throw NullPointerException();

					gt = new logicNOT(tID, indet, tdly2);

					if (gt == NULL)
						throw NullPointerException();

					crc->addLogicObj(gt);

					gt->setOutput(wo);	// Link the Gate with its wires.
					wo->setInput(gt);
					gt->addInput(wi1);
					wi1->addOutput(gt);
					//Done making Gate
				}
				else
				{
					fcir >> twi1 >> twi2 >> two;

					tID = tID.append(twi1).append(twi2).append(two); // see above.

					try // Wire Input
					{
						wi1 = (Wire*) crc->getLogicObj(twi1);
					}
					catch (ElementNotFound e) // It's not already in the circuit
					{
						wi1 = new Wire(twi1, indet);
						crc->addLogicObj(wi1);
					}

					if (wi1 == NULL)
						throw NullPointerException();

					try // Wire Input
					{
						wi2 = (Wire*) crc->getLogicObj(twi2);
					}
					catch (ElementNotFound e) // It's not already in the circuit
					{
						wi2 = new Wire(twi2, indet);
						crc->addLogicObj(wi2);
					}

					if (wi2 == NULL)
						throw NullPointerException();

					try // Wire Ouput
					{
						wo = (Wire*) crc->getLogicObj(two);
					}
					catch (ElementNotFound e) // It's not already in the circuit
					{
						wo = new Wire(two, indet);
						crc->addLogicObj(wo);
					}

					if (wo == NULL)
						throw NullPointerException();

					//Evaluate for Gate Type and create it.
					if (CMD == "OR")
						gt = new logicOR(tID, indet, tdly2);
					else if (CMD == "AND")
						gt = new logicAND(tID, indet, tdly2);
					else if (CMD == "NAND")
						gt = new logicNAND(tID, indet, tdly2);
					else if (CMD == "NOR")
						gt = new logicNOR(tID, indet, tdly2);
					else if (CMD == "XOR")
						gt = new logicXOR(tID, indet, tdly2);
					else if (CMD == "XNOR")
						gt = new logicXNOR(tID, indet, tdly2);
					else
						throw BasicException("Invalid Gate Type.");

					if (gt == NULL)
						throw NullPointerException();

					crc->addLogicObj(gt);

					gt->setOutput(wo);	//Link the Gate with its wires.
					gt->addInput(wi1);
					gt->addInput(wi2);

					wo->setInput(gt);	//And the wires to the Gate.
					wi1->addOutput(gt);
					wi2->addOutput(gt);
					//Done making Gate
				}
			}
		}

		if (crc == NULL)
			throw NullPointerException();
	}
	catch(BasicException e)
	{
		cout << "Error reading from Circuit Input file:" << e.getMessage() << endl;
		Terminate();
		return 1;
	}

	try // Now for the Vector File
	{
		string CMD;

		while (!(fvec.eof()))
		{

			fvec >> CMD;

			if ((CMD == "") || (fvec.eof()))
				break;

			if (CMD == "VECTOR") // Create a new vector.
			{
				if (crc == NULL)
					throw NullPointerException();

				string tID;

				fvec >> tID;

				if (tID == "")
					throw NullIDException();

				Vector* tVec = NULL;

				tVec = new Vector(tID);

				if (tVec == NULL)
					throw NullPointerException();

				crc->setVector(tVec); // set the current circuit's Vector to the new Vector.

				// loop through and create a PadState for each iPad and oPad in the circuit.

				//iPads
				for (int ipa = 0; ipa < crc->cntiPads(); ipa++)
				{
					iPadState *tpi = new iPadState(crc->getiPad(ipa)->getID(), indet, crc->getiPad(ipa));
					tVec->addPadState(tpi);
				}

				for (int opa = 0; opa < crc->cntoPads(); opa++)
				{
					oPadState *tpo = new oPadState(crc->getoPad(opa)->getID(), indet, crc->getoPad(opa));
					tVec->addPadState(tpo);
				}
			}
			else if (CMD == "INPUT")	// An input change!
			{
				string tID;
				string tdly;
				int    tdly2;
				string State;
				state  State2;

				fvec >> tID >> tdly >> State;

				if (tdly.substr(tdly.size() - 1, 1) == "s") // has ns
				{
					tdly = tdly.substr(0, tdly.size() - 2);
					tdly2 = atoi(tdly.c_str());
				}
				else // lacks ns.
					tdly2 = atoi(tdly.c_str());


				if (tdly2 < 0) // avoid invalid delay.
					tdly2 = 0;

				// Determine state.
				if (State == "0")
					State2 = low;
				else if (State == "1")
					State2 = high;
				else if ((State == "2") || (State == "X") || (State == "x"))
					State2 = indet;
				else
					throw UnknownState();

				if (crc == NULL)	// Circuit Exists
					throw NullPointerException();

				if (crc->getVector() == NULL)	// Circuit has Vector
					throw NullPointerException();

				PadState *tPS = NULL;

				try // There should already be a padstate for this input vector.
				{
					tPS = crc->getVector()->getPadState(tID);
				}
				catch (ElementNotFound e)
				{
					cout << "Vector definitions do not match with circuit definitions." << endl;
					Terminate();
					return 1;
				}

				tPS->addState(State2, tdly2);	// Add a new condition.
			}
			else
			{
				cout << "Unrecognized vector file command." << endl;
				Terminate();
				return 1;
			}
		}
	}
	catch(BasicException g)
	{
		cout << "Error reading Vector File: " << g.getMessage() << endl;
		Terminate();
		return 1;
	}

	if (crc->getVector() == NULL)
	{
		cout << "Vector is undefined. Exiting." << endl;
		Terminate();
		return 1;
	}

	// Find Circuit Runtime.
	int trace = crc->getVector()->trace();

	cout << "Circuit Run Time: " << trace << "ns" << endl;

	cout << "Evaluating..." << endl;

	crc->run(trace);	// This runs the circuit to completion. See Circuit notes.

	cout << "Done evaluating circuit" << endl;

	//=======================
	// Create and display gui.
	//=======================

	Position tWinPos = Position(0.0f, 0.0f);

	float width = ((trace + 5) * .3);	// Dynamic window size.

	if (width < 8)
		width = 8;

	if (width > 25)
		width = 25;

	float height = (float) (crc->getVector()->cntPadStates() + 2);

	if (height < 8)
		height = 8;

	if (height > 15)
		height = 15;

	SimpleWindow *w1 = new SimpleWindow(string("Daniel Boston's DigiCA: ").append(crc->getID()), width, height, tWinPos);

	w1->Open();

	if(w1->GetStatus() != WindowOpen)
	{
		cout << "Unable to open display window." << endl;
		Terminate();
		return 1;
	}

	gui = new UserInterface(crc, w1);	// UserInterface is a wrapper for Circuit

	w1->SetRefreshCallback(guiDraw);	// Resize capable.

	w1->SetTimerCallback(guiTimeDraw);	// Animated.

	w1->StartTimer(20);					// 20 millisecond delay between frames.

	return 0;
}

//========================
//Application End
//========================
//Parameters:
//	none
//Summary:
//	Performs final cleanup before the application ends.
//Returns:
//	int	-	status of the function, 0 is Success, 1 is Failure.
//========================
int ApiEnd()
{
	if (gui != NULL)
	{
		gui->getWindow()->Close();
		delete gui->getCircuit();
		delete gui;
	}

	return 0;
}
