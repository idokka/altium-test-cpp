#include <iostream>
#include "../include/logger.hpp"
#include "content-scaner.hpp"
#include "tclap/CmdLine.h"

using namespace std;
using namespace altium;
using namespace TCLAP;

int main(int argc, char const *argv[])
{
	// set defaults
	string log_path = "content-scaner.log";
	string scan_path = "/home/dokka/video/tvshow/*.mkv"

	// try parse command line arguments
	try 
	{
		CmdLine cmd("content scaner for altium", ' ', "0.1");
		ValueArg<string> log_path_arg("l", "log", "Path to log file", false, log_path, "string", cmd);
		ValueArg<string> scan_path_arg("p", "path", "Content scan path", false, scan_path, "string", cmd);
		//SwitchArg use_fork_arg("f","use-fork", "Use fork for connections execute", cmd, false);
		cmd.parse(argc, argv);
		// extract parsed values
		log_path = log_path_arg.getValue();
		scan_path = scan_path_arg.getValue();
	}
	catch (ArgException &e) 
	{ 
		// catch any parsing exceptions, write to std err
		cerr << "error: " << e.error() << " for arg " << e.argId() << endl; 
	}

	// init logger
	logger().set_log_path(log_path);
	logger().init();

	// create scaner ans scan specified path
	ContentScaner scaner(scan_path);
	scaner.Scan();

	// write out duplicates groups of files
	for (auto group : scaner.duplicates())
	{
		cout << string('=', 5) << endl;
		for (string file_name : group)
			cout << file_name << endl;
	}
	return 0;
}
