#ifndef TB_AES
#define TB_AES
#include "define.h"

SC_MODULE(tb_aes)
{
	sc_in<bool> clk;
	sc_in<bool> reset;
	sc_in<sc_uint<8> > data_out[SIZE];

	sc_out<sc_uint<8> > data_in[SIZE];
	sc_out<sc_uint<8> > key_in[SIZE];

	FILE * data_in_file, *key_in_file,  *golden_out_file, *data_out_file;
	FILE  *out_aes_cipher_file, *diff_file;
	sc_uint<8> key_input[SIZE];
	void compare_results();
	void send();
	void recv();

	SC_CTOR(tb_aes)
	{
		SC_CTHREAD(send,clk.pos());
		reset_signal_is(reset,false);
		SC_CTHREAD(recv,clk.pos());
		reset_signal_is(reset,false);
	}
	~tb_aes(){}
};
#endif