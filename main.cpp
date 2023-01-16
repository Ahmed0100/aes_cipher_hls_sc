#include "aes.h"
#include "tb_aes.h"
#define WAVE_DUMP
int sc_main(int argc, char** argv)
{
	sc_clock clk("clk",25,SC_NS,0.5, 12.5, SC_NS, true);
	sc_signal<bool> reset;
	sc_signal<sc_uint<8> > data_in[SIZE];
	sc_signal<sc_uint<8> > data_out[SIZE];
	sc_signal<sc_uint<8> > key_in[SIZE];
	int x;
	char name[10];
	aes aes_inst("aes_inst");
	tb_aes tb_aes_inst("tb_aes_inst");

	aes_inst.clk(clk);
	aes_inst.reset(reset);
	for(int i=0;i<SIZE;i++)
	{
		aes_inst.data_in[i](data_in[x]);
		aes_inst.data_out[i](data_out[x]);
		aes_inst.key_in[i](key_in[x]);
	}
	tb_aes_inst.clk(clk);
	tb_aes_inst.reset(reset);
	for(int i=0;i<SIZE;i++)
	{
		tb_aes_inst.data_in[i](data_in[x]);
		tb_aes_inst.data_out[i](data_out[x]);
		tb_aes_inst.key_in[i](key_in[x]);
	}
#ifdef WAVE_DUMP
	sc_trace_file * trace_file = sc_create_vcd_trace_file("trace_file");
	sc_trace(trace_file, clk, "clk");
	sc_trace(trace_file, reset, "reset");
	for(int i=0;i<SIZE;i++)
	{
		sprintf(name,"data_in%d",i);
		sc_trace(trace_file,data_in[x],name);
		sprintf(name,"data_out%d",i);
		sc_trace(trace_file,data_out[x],name);
		sprintf(name,"key_in%d",i);
		sc_trace(trace_file,key_in[x],name);
	}
#endif
	sc_start(25,SC_NS);
	reset.write(0);
	sc_start(25,SC_NS);
	reset.write(1);
	sc_start(1000,SC_NS);
	sc_start();
#ifdef WAVE_DUMP
		sc_close_vcd_trace_file(trace_file);
#endif
	return 0;
}