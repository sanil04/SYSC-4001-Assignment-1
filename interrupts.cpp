/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * 
 * @author Mircea Preotesou
 * @author Sanil Srivastava
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
    const int CONTEXT = 10;
    const int ISR_TIME = 40;
    const int IRET_TIME = 1;
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
            auto[execution_hist, time] = intr_boilerplate(curr_time, duration_intr, CONTEXT, vectors);
            execution += execution_hist;
            curr_time = time;

            execution += std::to_string(curr_time) + ", "  + std::to_string(ISR_TIME)  + ", SYSCAll: calling device driver\n"; 
            curr_time += ISR_TIME;

            execution += std::to_string(curr_time) + ", "  + std::to_string(ISR_TIME)  + ", transfering data\n"; 
            curr_time += ISR_TIME;
            if ((delays[duration_intr] - ISR_TIME*2 - IRET_TIME) >= 0){
                execution += std::to_string(curr_time) + ", "  + std::to_string(delays[duration_intr] - ISR_TIME*2 - IRET_TIME)  + ", checking for errors\n"; 
                curr_time += delays[duration_intr] - ISR_TIME*2 - IRET_TIME;
            }
            execution += std::to_string(curr_time) + ", "  + std::to_string(IRET_TIME)  + ",  IRET \n"; 
            curr_time += IRET_TIME;

        }
        
        else if (activity == "END_IO") {
            
            execution += std::to_string(curr_time) + ", "  + std::to_string(ISR_TIME)  + ", END_IO: calling device driver\n"; 
            curr_time += ISR_TIME;

            execution += std::to_string(curr_time) + ", "  + std::to_string(ISR_TIME)  + ", transfering data\n"; 
            curr_time += ISR_TIME;
            if ((delays[duration_intr] - ISR_TIME*2 - IRET_TIME) >= 0){
                execution += std::to_string(curr_time) + ", "  + std::to_string(delays[duration_intr -1] - ISR_TIME*2 - IRET_TIME)  + ", checking for errors\n"; 
                curr_time += delays[duration_intr] - ISR_TIME*2 - IRET_TIME;
            }
            execution += std::to_string(curr_time) + ", "  + std::to_string(IRET_TIME)  + ",  IRET \n"; 
            curr_time += IRET_TIME;
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
