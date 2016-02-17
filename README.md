# Valid-Ticket
Create a batch of examination tickets with reference to a data file.

This utility creates examination tickets for a group of students
whose data is flagged in a data file. This program is not useful outside
of the MMHS. It is used by the Financial Office for creating Valid tickets
en masse instead of printing each one manually.


1. This program was developed on a Linux computer and cross-compiled
   on a Windows machine. In the MMHS it is mainly used on Windows.

2. You will need the following to successfully compile this package.

   a) Qt 4

   b) A version of the Gnu C++ Compiler. I suggest the MinGW compiler for Windows.

   c) Gnu Make.

To build this package do the following:

   $ qmake Valids.pro
   
   $ make


