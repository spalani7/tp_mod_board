tp_mod_board
========================
This is an application for replacing/appending NET names in the ICT board file with its corresponding TP names.
See `usage` below to understand how to use this application

This source code can be compiled for both linux/Windows on Ubuntu as below:

For windows on Ubuntu:

	$> sudo apt-get install mingw-w64

Use x86_64-w64-mingw32-g++ for 64-bit and i686-w64-mingw32-g++ for 32-bit

For Linux on Ubuntu:

	$> sudo apt-get install g++

Use g++ for both 32 and 64-bit.

**Building application:**

	$> make win 	-- to compile for Windows
	$> make linux 	-- to compile for Linux

usage
-----------------
tp_mod_board.exe <`input_path`> <`output_path`> <`use_defaults`>  
	<`input_path`>		Input board file path  
	<`output_path`>		Output board file path (OPTIONAL) (Default: board_out)  
	<`use_defaults`>	Use default settings (no user interaction) (any non-empty string) (OPTIONAL)  