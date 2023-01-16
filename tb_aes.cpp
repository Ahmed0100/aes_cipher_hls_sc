#include "tb_aes.h"

void tb_aes::send()
{
	int ret=0;
	unsigned int data_in_read,key_in_read;
	data_in_file = fopen(INFILENAME,"rt");
	if(!data_in_file)
	{
		cout << "Could not open " << INFILENAME << "\n";
		sc_stop();
		exit (-1);
	}
	key_in_file = fopen(INFILENAME_KEY,"rt");
	if(!key_in_file)
	{
		cout << "Could not open " << INFILENAME_KEY << "\n";
		sc_stop();
		exit (-1);
	}
	for(int i=0;i<SIZE;i++)
	{
		data_in[i].write(0);
		fscanf(key_in_file,"%x",&key_in_read);
		key_input[i] = key_in_read;
	}
	wait();
	int i=0;
	while(1)
	{
		while(fscanf(data_in_file,"%x",data_in_read)!=EOF)
		{
			data_in[(i%4)*4+ i/4].write(data_in_read);
			key_in[(i%4)*4+ i/4].write(key_in_read);
			i++;
			if(i==SIZE)
			{
				i = 0;
				wait();
			}
		}
		fclose(data_in_file);
		fclose(key_in_file);
		cout<<end<<"starting comparing the results "<<endl;
		wait();
		compare_results();
		exit(-1);
	}
}

void aes::recv()
{
	sc_uint<8> data_out_write[SIZE];
	int i=0;
	data_out_file = fopen(OUTFILENAME,"wt");
	if(!data_out_file)
	{
		cout << "Could not open " << OUTFILENAME << "\n";
		sc_stop();
		exit (-1);
	}

	wait();
	wait();
	while(true)
	{
		wait();
		for(int i=0;i<SIZE;i++)
		{
			data_out_write[i] = data_out[(i%4)*4+i/4].read();
			fprintf(data_out_file, "%x ", (unsigned int)data_out_write[i]);
		}
		fprintf(data_out_file,"\n");
	}
}

void test_aes::compare_results(){

  	unsigned int outaes, out_golden, line=1, element=1, errors=0;

	fclose(data_out_file);

  	data_out_file = fopen (OUTFILENAME, "rt");

	if(!data_out_file){
		cout << "Could not open " << OUTFILENAME << "\n";
		sc_stop();
	   	exit(-1);
	}


  	golden_out_file = fopen (OUTFILENAME_GOLDEN, "rt");

	if(!golden_out_file){
    	cout << "Could not open " << OUTFILENAME_GOLDEN << "\n";
    	sc_stop();
  		exit(-1);
	}


	diff_file = fopen (DIFFFILENAME, "w");
	if(!diff_file){
		cout << "Could not open " << DIFFFILENAME<< "\n";
	 	sc_stop();
	}

    while(fscanf(golden_out_file, "%x", &out_golden) != EOF)
    {
    	fscanf(data_out_file,"%x", &outaes);
     	cout << endl <<"Cycle["<< line << "][" << element << "]: " << out_golden << " -- "<< outaes;
		if(outaes != out_golden){
			cout << "\nOutput missmatch [line:" << line << "] Golden:" << out_golden << " -- Output:" << outaes;
			fprintf(diff_file,"\nOutput missmatch[line:%d][%d] Golden: %u -- Output: %u",line,element, out_golden, outaes);
	  		errors++;
      	}
	    if(element == SIZE){
			element =0;
			line ++;
	    }
		element ++;
   }

    if(errors == 0)
    	cout << endl << "Finished simulation SUCCESSFULLY" << endl;
    else
    	cout << endl << "Finished simulation " << errors << " MISSMATCHES between Golden and Simulation" << endl;

    fclose(data_out_file);
    fclose(diff_file);
    fclose(golden_out_file);
}
