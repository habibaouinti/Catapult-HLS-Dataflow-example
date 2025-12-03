#include <iostream>
#include <mc_scverify.h>

#include <ac_int.h>
#include <ac_channel.h>

// DUT declaration
void test (
    ac_channel<bool>              &last_s,
    ac_channel<ac_int<8,false>>   &i,
    ac_channel<ac_int<8,false>>   &inc,
    ac_channel<ac_int<16,false>>  &o
);

// -----------------------------
// Testbench
// -----------------------------
CCS_MAIN(int argc, char *argv[])
{
    std::cout << "Starting test() testbench..." << std::endl;

    // --------------------------------
    // Declare channels for the DUT
    // --------------------------------
    ac_channel<bool>              last_s_chan;
    ac_channel<ac_int<8,false>>   i_chan;
    ac_channel<ac_int<8,false>>   inc_chan;
    ac_channel<ac_int<16,false>>  o_chan;

    // --------------------------------
    // Prepare stimulus
    // --------------------------------
    const int NUM_TESTS = 8;

    ac_int<8,false> in_vals[NUM_TESTS]  = {1, 2, 3, 10, 15, 7, 9, 20};
    ac_int<8,false> inc_vals[NUM_TESTS] = {3, 4, 2,  5,  1, 8, 6,  2};

    std::cout << "Sending stimulus..." << std::endl;

    // For your loop condition: while(!last)
    // we send last_s = false for N-1 cycles, then true at the end
    for (int k = 0; k < NUM_TESTS; k++) {
        last_s_chan.write(k == (NUM_TESTS-1));     // keep processing active
        i_chan.write(   in_vals[k] );
        inc_chan.write( inc_vals[k] );
    }

    // --------------------------------
    // Call the DUT
    // --------------------------------
    std::cout << "Calling the accelerator..." << std::endl;

    // Run DUT NUM_TESTS+1 times (matching inputs)

    CCS_DESIGN(test)(last_s_chan, i_chan, inc_chan, o_chan);

    // --------------------------------
    // Read outputs
    // --------------------------------
    std::cout << "Reading results..." << std::endl;

    int out_idx = 0;
    while (!o_chan.empty()) {
        ac_int<16,false> result = o_chan.read();
        std::cout << "Output[" << out_idx << "] = "
                  << result.to_int() << std::endl;
        out_idx++;
    }

    std::cout << "test() testbench finished." << std::endl;
    CCS_RETURN(0);
}
