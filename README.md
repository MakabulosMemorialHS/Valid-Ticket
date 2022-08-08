# Valid-Ticket
Create a batch of examination tickets with reference to a data file.

This utility creates examination tickets for a group of students
whose data is flagged in a data file. This program is not useful outside
of the MMHS. It is used by the Financial Office for creating Valid tickets
en masse instead of printing each one manually.


1. This program was developed on a Linux computer and cross-compiled
   on a Windows machine. In the MMHS it is mainly used on Windows.

2. You will need the following to successfully compile this package.

   a) Qt 4.7.4 (minimum)

   b) A version of the Gnu C++ Compiler. I suggest the MinGW-4.4.0 compiler for Windows.

   c) Gnu Make.

To build this package do the following:

   $ qmake Valids.pro
   
   $ make
   
If building under Linux with multiple versions of Qt, then give the following command
before anything else:

   $ export QT_SELECT=qt4
   

GitHub
------------------------------------------------------------
This package is version managed by github.com.

Before using git, set the following necessary global variables.

   $ git config --global user.name "Robert Pascual"
   $ git config --global user.email "rtonypascual@gmail.com"
   $ git config --global core.editor vim

Check what branch you are in with

   $ git branch

To switch to branch master

   $ git checkout master

To determine where the remote repository exists

   $ git remote show origin


Then, before working on this package, synchronize the branch you will be working on
with the remote branch. For example, to synchronize the branch refactor ...

   $ git pull https://github.com/MakabulosMemorialHS/Valid-Ticket.git refactor
   
To push your changes to the remote repository on branch master:

   $ git push https://github.com/MakabulosMemorialHS/Valid-Ticket.git master
   


