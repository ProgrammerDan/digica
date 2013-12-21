digica
======

Digital Circuit Analyzer
by Daniel Boston (ProgrammerDan)
April 19, 2005

For cs1210 (C++ Programming class)
Clearly this isn't my first coding rodeo, and I wanted to prove as much to 
the prof so I could skip any other intro classes.

Introduction
------------

This is a rather older project that allows circuit simulation.

It's almost-but-not-quite ready to Make -- be sure to modify the makefile
to point to your local installation of EzWindows and X11 -- or modify to use
a Windows build of EzWindows if preferred. If you do this, please fork my code
and send me the results!

The most complete reference to EzWindows (the Devon Lockwood version) I could
find while preparing this archive is:

[http://www.cs.virginia.edu/c++programdesign/software/]
(http://www.cs.virginia.edu/c++programdesign/software/)

Major kudos to [Dr. Keith Shomper]
(http://www.cedarville.edu/Academics/Engineering-and-Computer-Science/Faculty/Shomper-Keith.aspx)
for his excellent lessons on C and C++ and for putting up with me during
cs1210.

Instructions
------------

1. Modify makefile to fit your system (see Introduction)
2. Run Make
3. Execute digica
4. Follow program interactive instructions

Creating a Circuit
------------------

So, I must confess, it's been the better part of a decade since I've even
looked at this code. However, the format for the input file is remarkably
straightforward, so I'll do my best to describe it here.

### Circuit File

Naming: `<filename>`.txt

_Key Concepts:_
* Circuits are larger constructs that deal with signal propagation through
logic gates. Circuits are composed of pads (input and output), logic gates,
and wires. To help visualize, pads are always at the beginning and end of a
circuit (although outputs can feed back into the circuit), gates sit inbetween
the sets of pads, and wires connect every element. Both the "input" and "output"
wire of a pad or gate is referenced by an integer, and whether to use the 
"input" or "output" side of the wire is determined contextually (I think!).
The circuit file is exclusively concerned with setting up the circuit -- it
allows specification of propogation delays, complex gate logic with feedback,
and more, but it does not allow the specification of input sets. That will be
in the next section dealing with the Vector file.

_File Elements:_
* CIRCUIT `<label>`  --  Signals the beginning of a new circuit. Every 
circuit file should start this this. Label must not be skipped.
* INPUT `<pad_id>` `<wire_id>`  --  Generates a new input pad. Pad_id is 
required (for display) and wire_id must be an integer and unique within 
the file.
* OUTPUT `<pad_id>` `<wire_id>`  --  Generates a new output pad. Pad_id is 
required (for display) and wire_id must be an integer and unique within 
the file.
* `<1-gate>` `<delay>` `<in-wire>` `<out-wire>` -- Single-input gate. Delay 
specifies signal propagation delay - the length of time it takes the input 
signal to reach the output, in nanoseconds (must be greater than 0 - if 
negative or zero, gets set to 1ns). In-wire specifies which wire to tap for 
input, out-wire specifies which wire to fill with output. Valid values 
of `<1-gate>`:
** NOT -- Logical NOT gate: if the input is true, the output becomes false
** INV -- Same.
** INVERTER -- Same.
* `<2-gate>` `<delay>` `<in-wire-1>` `<in-wire-2>` `<out-wire>` -- Two-input
gate. Delay specifies signal propogation delay. In-wire-1 and In-wire-2 specify the inputs to the gate, and out-wire specifies which wire to fill with output.
Valid values of `<2-gate>`:
** OR -- Logical OR gate: if either input is true, the output becomes true
(after delay).
** AND -- Logical AND gate: if both inputs are true, the output becomes true
** NAND -- Logical NOT-AND gate: if any input is false, the output becomes true
** NOR -- Logical NOT-OR gate: if none of the inputs are true, the output 
becomes true
** XOR -- Logical Exclusive-OR gate: if exclusively one input is true, the 
output becomes true
** XNOR -- Logical Exclusive-NOR gate: if both inputs are the same value, the 
output becomes true

_Example:_
CIRCUIT Circuit1
INPUT   A    1
INPUT   B    3
INPUT   C    4
OUTPUT  D    5
OUTPUT  E    6
NOT     2ns  1   2
AND     3ns  2   3   5
OR      3ns  4   5   6

### Vector File

Naming: `<filename>`_v.txt

_Key Concepts:_
The vector file is basically a playbook for sending signals into the Circuit.
It specifies a "program" to execute against the inputs, which will naturally
cause some kind of activity inside the circuit. All the gates described above
support ternary logic (just for fun) values which are -- low (0), high (1), and
indeterminate (2, X or x). Thus, inputs can be set to any of these values, and
as the circuit "executes" the inputs will cycle through whatever values
are specified in the vector file.

_File Elements:_
* VECTOR `<label>`  --  Signals the beginning of a new vector. Every 
vector file should start this this. Label must not be skipped.
* INPUT `<pad_id>` `<trans_time>` `<state>` -- Indicates a new scheduled state
transition at an input pad. All inputs by default start as indeterminate, so
it's a good idea to explicitly specify all inputs at trans_time 0. Of course,
pad_id must be defined in the Circuit file, trans_time must be 0 or greater,
and state must be one of:
** 0  --  Low (false)
** 1  --  High (true)
** 2  --  Indeterminate (???)
** X  --  Indeterminate (???)
** x  --  Indeterminate (???)

_Example:_
VECTOR Circuit1
INPUT A  0  0
INPUT B  0  1
INPUT C  0  0
INPUT C  4  1
INPUT A  6  1
INPUT B  9  0

Conclusion
----------

Turns out you can create some rather complex circuits with this relatively
simple application. Check out the circuits/ folder for a number of examples,
and some counter-examples, demonstrating what this code can do, including
flip-flops, muxers, adders, and more.

I hope you enjoy! Contact me at ProgrammerDan@gmail.com or [ProgrammerDan]
(https://twitter.com/ProgrammerDan).
