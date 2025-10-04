/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    const int context = 10;
    int curr_time = 0;
    std::string address;
    std::string duration;


    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            execution +=  std::to_string(curr_time) + ", " + std::to_string(duration_intr) + ", CPU burst\n";
            curr_time += duration_intr;
        }

        else if (activity == "SYSCALL") {
            auto[execution_hist, time_curr]= intr_boilerplate(curr_time, duration_intr, context, vectors);
            execution += execution_hist;
            curr_time = time_curr;
        }
        
        else if (activity == "END_IO") {
            
            execution +=  std::to_string(curr_time) + ", " + std::to_string(duration_intr) + ", IRET\n";
            curr_time += duration_intr;
            execution +=  std::to_string(curr_time) + ", " + std::to_string(1) + ", END IO\n";
            curr_time += 1;
  

        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
