// wave1d.cpp - Simulates a one-dimensional damped wave equation
//
// Ramses van Zon - 2015-2023
//

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cmath>
#include <chrono>

#include "parameters.h"
#include "initialize.h"
#include "output.h"
#include "evolve.h"
#include "simulation.h"
#include "timing_output.h"

//using namespace 

int main(int argc, char* argv[])
{
    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    if (argc != 3) {
        std::cerr << "Error: wave1d needs two parameters: file argument and the number of cores.\n";
        return 1;
    }
    int core_number = std::atoi(argv[2]);

    // Read the values from the parameter file specified on the command line
    // but exit with error code if something went wrong
    Parameters param;
    int errorcode = readParametersFromFile(argv[1], param);
    if (errorcode > 0) 
        return errorcode;
    
    // Derived parameters 
    param = set_derived_simulation_parameters(param);
    
    // Open output file
    ascii_file_handle  ascf{};
    netcdf_file_handle ncdf{nullptr};
    if (param.netcdfoutput)
        ncdf = netcdf_output_open(param.outfilename);
    else
        ascf = ascii_output_open(param.outfilename);

    output_parameters(ascf, param);
    output_parameters(ncdf, param);
    
    // Define state
    WaveState wave;
    create_simulation_system(param, wave);
    initialize(param, wave);

    // Output initial wave to file
    output_wave(ascf, 0.0, wave);
    output_wave(ncdf, 0.0, wave);
    double time_parallel = 0.0;

    // Take timesteps
    for (size_t s = 0; s < param.nsteps; s++) {
        
	// Do the time calculations
	auto start_parallel = std::chrono::high_resolution_clock::now();
        // Evolve in time
        int int_param = one_time_step(param, wave);
	//std::cout << core_number << std::endl;
	auto end_parallel = std::chrono::high_resolution_clock::now();
	auto duration_parallel = std::chrono::duration_cast<std::chrono::seconds>(end_parallel - start_parallel);
	time_parallel += duration_parallel.count();
        
        // Output wave to file
        if ((s+1)%param.nper == 0) {
            double t = static_cast<double>(s+1)*param.dt;
            output_wave(ascf, t, wave);
            output_wave(ncdf, t, wave);
        }
    }

    // Close file
    output_close(ascf);
    output_close(ncdf);
    std::cout << "Results written to '"<< param.outfilename << "'.\n";
    // End the timer
    auto end = std::chrono::high_resolution_clock::now();
    // Compute the duration and print it in seconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto time = duration.count();
    std::cout << "Program took " << time << " seconds to complete" << std::endl;

    write_timing(core_number,time,time_parallel);

    return 0;
}

