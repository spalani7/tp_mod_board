/////////////////////////////////////////////////////////////////////
// tp_mod_board.c
// - This is an application for replacing/appending
// NET names in the ICT board file with it's corresponding TP names
// See usage() below to understand how to use this application
// 
// This source code can be compiled for both linux/Windows on Ubuntu as below:
//
// For windows on Ubuntu:
// 		$> sudo apt-get install mingw-w64
// Use x86_64-w64-mingw32-gc++ for 64-bit and i686-w64-mingw32-g++ for 32-bit
//
// For Linux on Ubuntu:
//		$> sudo apt-get install g++
// Use g++
//
//	$> make win 	-- to compile for Windows
//	$> make linux 	-- to compile for Linux
/////////////////////////////////////////////////////////////////////


#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

static void usage()
{
	cout << "Usage: tp_mod_board.exe <input_path> <output_path> <use_defaults>" << endl;
	cout << "  <input_path>		Input board file path" << endl;
	cout << "  <output_path>	Output board file path" << endl;
	cout << "  <use_defaults>	Use default settings (no user interaction) (any non-empty string)" << endl;
}

string trim(string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r\v\f");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r\v\f");
    return str.substr(first, (last-first+1));
}

int main(int argc, char* argv[])
{
	ifstream infile;
	ofstream outfile;
	string line;
	string infile_path;
	string outfile_path;
	int status = 0;

	vector<string> tp_list;
	vector<string> out;
	unsigned out_ptr = 0;
	unsigned net_ptr = 0;
	unsigned TP_cnt = 0;
	const unsigned max_tp_cnt = 2;
	int mode = 1;
	string prefix_text = "";
	string postfix_text = "_ff";

	if (argv[1] == NULL){
		cout << "Please input board file path!" << endl;
		usage();
		return status;
	}
	else
		infile_path = string(argv[1]);

	if (argv[2] == NULL){
		outfile_path = "board_out";
	}
	else
		outfile_path = string(argv[2]);

	cout << "Input board file path is \"" << infile_path << "\"!" << endl;
	cout << "Output board file path is \"" << outfile_path << "\"!" << endl << endl;

	if (argv[3] != NULL){
		cout << "Default settings mode activated with below values:" << endl;
		cout << "Mode: " << mode << endl;
		cout << "prefix_text: \"" << prefix_text << "\"" << endl;
		cout << "postfix_text: \"" << postfix_text << "\"" << endl << endl;
	}
	else {
		cout << "Enter mode (1: Replace mode (default), 2: Append mode): ";
		getline(cin, line);
	    if (!line.empty()) {
	        mode = stoi(line);
	    }
	    
	    cout << "Mode set to: " << mode << endl << endl;

		cout << "Enter prefix_text (Default: \"\"): ";
		getline (cin, line);
		if (!line.empty())
			prefix_text = line;
		
		cout << "prefix_text set to: \"" << prefix_text << "\"" << endl << endl;

		cout << "Enter postfix_text (Default: \"_ff\"): ";
		getline (cin, line);
		if (!line.empty())
			postfix_text = line;

		cout << "postfix_text set to: \"" << postfix_text << "\"" << endl << endl;
	}

	cout << "Reading board file.."<< endl;
	infile.open(infile_path, ifstream::in);

	if (infile.is_open()) {
		
		// FIND "CONNECTIONS"
		while(getline(infile, line)) {
			out.push_back(line);
			out_ptr++;

	    	if ((status == 0) && (line.find("CONNECTIONS") != string::npos)) {
	    		status = 1; // CONNECTIONS FOUND
	        	break;
	    	}
	    }

	    // Input file is a board file
	    if (status == 1) {
		    while(getline(infile, line)) {
		    	out.push_back(line);
				out_ptr++;

		    	if ((status < 2) && (!line.empty())) {
		    		status = 2;
		    		net_ptr = out_ptr-1; // Save line no of NET NAME string
		    		TP_cnt = 0; // reset TP counter
		    		tp_list.clear();
		    	}

		    	// FIND TP NAMES
		    	if ((status == 2) && ((line.find("TP") != string::npos) || (line.find("MP") != string::npos))) {
		    		TP_cnt++;
	    			if (TP_cnt <= max_tp_cnt) 
	    				tp_list.push_back(line.substr(0,line.find(".")));
		    	}


		    	// marks the end of NET
		    	if ((status == 2) && (line.find(";") != string::npos) ) {

		    		if (tp_list.size() > 0) {
			    		// Replace mode
			    		if (mode == 1)
			    			out.at(net_ptr) = "  " + prefix_text;

			    		if (mode == 2)
			    			out.at(net_ptr) = "  " + prefix_text + trim(out.at(net_ptr)) + "_";

		    			for (std::vector<string>::iterator it = tp_list.begin() ; it != tp_list.end(); ++it) {
		    					out.at(net_ptr) += trim(*it);
		    					if (it < (tp_list.end()-1))
		    						out.at(net_ptr) += "_";
		    			}

		    			if (TP_cnt > max_tp_cnt)
		    				out.at(net_ptr) += postfix_text;
		    		}

		    		status = 1; // revert status
		    	}
		    }
		    infile.close();

		    cout << "Writing output.."<< endl;
		    outfile.open(outfile_path, ofstream::out);
		    if (outfile.is_open()){
		    	for (std::vector<string>::iterator it = out.begin() ; it != out.end(); ++it)
					outfile << *it << endl;
		    }
			outfile.close();
			cout << "DONE." << endl;
		}

		// Wrong input file
		if (status == 0){
			cout << "\"CONNECTIONS\" section NOT FOUND in input board file!!!"<< endl;
			infile.close();
		}
	}
	else
		cout << "Board file " << infile_path << " NOT FOUND"<< endl;
	return 0;
}