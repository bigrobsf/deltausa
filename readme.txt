README.TXT               DELTA DENTAL INSURANCE COMPANY                2001.05.31


DeltaUSA Fee Reporting System for DOS
Version 2.0

by Robert Conner


Table of Contents
-----------------

- Purpose
- Description of this Release
- Installation
- Registry Settings
- Notes, Problems, Limitations
- Support


Purpose
-------

The DeltaUSA Fee Report System was designed to automate the calculation of service fee and withhold amounts due by state to the Delta Dental Plans Association.  It also calculates the amounts by group type needed to create the DeltaUSA Admin journal voucher.


Description of this Release
---------------------------

This application is coded entirely in C and was developed using Microsoft C++ 6.0.


Installation
------------

The program and required text files are contained in a self-extracting zip file.  Running the zip file will extract the program and files into the proper folders.

File and folders:

c:\deltausa\program\deltausa.exe (the application)

c:\deltausa\aso_list.txt (the list of ASO groups)
c:\deltausa\dasohelp.txt (ASO list maintenance instructions)
c:\deltausa\dcdshelp.txt (create mainframe dataset instructions)
c:\deltausa\dddshelp.txt (download dataset instructions)
c:\deltausa\disshelp.txt (import file to spread sheet instructions)
c:\deltausa\dpdfhelp.txt (process data file instructions)

For convenience, a shortcut to the executable should be created to launch the application from the desktop or start menu.



Registry Settings
-----------------

This program is a DOS application and makes no changes to the Windows registry.



Notes, Problems, Limitations
----------------------------

Dependencies:

If the format of the DeltaUSA reports created by Deltanet changes, the program may not function or may produce inaccurate results.  This is because the program expects each line of the report to contain certain information in specific columns.  For example, the group number and state abbreviation are expected to begin in columns 24 and 33, respectively.

If the section headings of the Deltanet reports change, the program may be unable to find the proper position in the file to start processing.  The program looks for the words "GROUP FOR" in the heading (beginning in column 57) and begins processing once those words are found.

If the locations of these words change in the Deltanet report, the header file dusamain.h can be edited accordingly.

This program is intended to run in a DOS window under Windows NT 4.0.  If run under Windows 9x or 2000, the final page of any printed output may have to be manually ejected.

The help facility will not work if the help text files described in Installation (above) are missing.



Support
-------

Send support questions or suggestions to:

rconner@delta.org or call me at x-3506 in San Francisco.

I assume that a Deltanet employee would be able to support, enhance, or revise this program after my separation from Delta Dental.  Since this is the first production application I've written, I have no doubt that there is significant room for improvement.  However, I will continue to support this program if needed beyond my employment with Delta for a nominal fee.  I can be contacted at:

rjconner@pacbell.net or robtconner@yahoo.com