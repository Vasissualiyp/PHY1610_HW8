#include "timing_output.h"

// This function is used to store the general data about the performance of the runs (total runtime)
void write_timing(int cores_number, int integer2)
{
    // Read file and find line starting with integer
    const char* filename = "timing_output.csv";
    std::ifstream file(filename);
    std::string line;
    bool found_line = false;
    if (file.good()) {  // file exists
	    while (std::getline(file, line)) {
		if (std::isdigit(line[0]) && std::isdigit(line[1])) {
		    int line_cores_number = std::stoi(line.substr(0, 2));
		    if (line_cores_number == cores_number) {
			found_line = true;
			break;
		    }
		}
	    }
	    file.close();
	    
	    // If line found, delete it
	    if (found_line) {
		std::ifstream file2(filename);
		std::ofstream temp("temp.txt");
		while (std::getline(file2, line)) {
		    if (!(std::isdigit(line[0]) && std::isdigit(line[1]) && 
			  std::stoi(line.substr(0, 2)) == cores_number)) {
			temp << line << std::endl;
		    }
		}
		file2.close();
		temp.close();
		std::remove(filename);
		std::rename("temp.txt", filename);
	    }
	    
	    // Append new line to file
	    std::ofstream outfile(filename, std::ios_base::app);
	    outfile << cores_number << "," << integer2 << std::endl;
	    outfile.close();
    } else {  // file does not exist
        std::ofstream outfile(filename);
        outfile << cores_number << "," << integer2 << "\n";
        outfile.close();
    }
}

