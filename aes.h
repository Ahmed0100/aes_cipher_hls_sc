#ifndef AES_H
#define AES_H

#include "define.h"

SC_MODULE (aes)
{
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_in<sc_uint<8> > data_in[SIZE];
    sc_in<sc_uint<8> > key_in[SIZE];

    sc_out<sc_uint<8> > data_out[SIZE];

    void add_round_key(U8 state[SIZE], U8 key[SIZE]);
    void sub_bytes(U8 state[SIZE]);
    void shift_rows(U8 state[SIZE]);
    void mix_cols(U8 state[SIZE]);
    void key_expansion(U8 current_key[SIZE], U8 rcon, U8 next_key[SIZE]);
    void assign_key(U8 key_in[SIZE], U8 key_out[SIZE]);
    U8 xtime_2(U8 data);
    U8 xtime_3(U8 data);
    U8 xadd(U8 in1, U8 in2);
    int clip(int n);

    void run();

    SC_CTOR(aes)
    {
        SC_CTHREAD(run, clk.pos());
        reset_signal_is(rst, false);
    }

    ~aes() {}
};

#endif // AES_H