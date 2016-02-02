# Valid-Ticket
Create a batch of examination tickets with reference to a data file.

This utility creates examination tickets for a group of students
whose data is flagged in a data file. This program is not useful outside
of the MMHS. It is used by the Financial Office for creating Valid tickets
en masse instead of printing each one manually.


1. This program is developed on a Linux computer but can be cross-compiled
   on a Windows machine. In fact, it is used on Windows Machines.

2. You will need the following to successfully compile this package.
   a) Qt 4
   b) Gnu C++ Compiler. Use the MinGW compiler on Windows.
   c) Gnu Make.

To build this package do the following:

   $ qmake Valids.pro
   $ make


