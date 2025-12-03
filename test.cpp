#include <ac_int.h>
#include <ac_channel.h>

#pragma hls_design
void block1 (
            ac_channel<bool> &last_s,
            ac_channel<bool> &last_o,
            ac_channel<ac_int<8,false > > &inChan,
            ac_channel<ac_int<10,false > > &outChan
            )
{
    bool last = false ;
    while(!last) {
        last = last_s.read() ;
        last_o.write(last) ;
        outChan.write(inChan.read () * 5);
    }
}

#pragma hls_design
void block2 (
            ac_channel<bool> &last_o,
            ac_channel<ac_int<10,false > > &inChan,
            ac_channel<ac_int<8,false > > &incChan,
            ac_channel<ac_int<16,false > > &outChan)
{
    bool last = false ;
    while(!last){
        last = last_o.read() ;
        outChan.write (inChan.read () * incChan.read ());
    }
}

#pragma hls_design top
void test (
           ac_channel<bool> &last_s,
           ac_channel<ac_int<8,false > > &i,
           ac_channel<ac_int<8,false > > &inc,
           ac_channel<ac_int<16,false > > &o)
{
  #pragma hls_dataflow
  static ac_channel<ac_int<10,false > > pipe;
  static ac_channel<bool > last_o;
  
    block1 (last_s,last_o,i,pipe);
    block2 (last_o,pipe,inc,o);
}
