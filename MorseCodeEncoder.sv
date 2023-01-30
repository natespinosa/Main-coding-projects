// This project taskes as an input to the circuit, one out of eight letters, that can be represented as morse code. It decodes the letter and 
// gives as an output a series of longs (1.5s) and short (.5s) pulses through an LED. Given a clock frequency, the code arranges the outcome 
// to fullfill the previous need mentioned.

output the appropriate Morse code for that letter to the output DotDashOut using short and
long pulses. Short (0.5s) pulses represent dots and long (1.5s) pulses represent dashes. The
time between pulses is 0.5 seconds. Similar to Part 2, you must determine how many cycles
to count for 0.5 seconds, for a given CLOCK FREQUENCY.
module part3 #(parameter CLOCK_FREQUENCY=500)(ClockIn, Reset, Start, Letter, DotDashOut, NewBitOut); 
	input logic ClockIn;
	input logic Reset;
	input logic Start;
	input logic [2:0] Letter;
	
	logic Enable;
	logic [11:0] Flash; 
	logic [11:0] Shit_Out;
  
	output logic DotDashOut;
	output logic NewBitOut;    
	
	mux12to1 decoder(.Letter(Letter), .Q(Flash));
	shiftreg shift(.ClockIn(ClockIn), .Reset(Reset), .Enable(Enable), .Start(Start), .Flash(Flash), .q(Shit_Out)); 
	counter count(.ClockIn(ClockIn), .Reset(Reset), .Start(Start), .Enable(Enable));

	always_ff @(posedge ClockIn)   
	begin  
		if (Enable) 
			DotDashOut <= Shit_Out[11];
		NewBitOut <= Enable;
	end
endmodule

module mux12to1(Letter, Q);
	input logic [2:0]Letter;
	output logic [11:0]Q;
	
	always_comb 
	begin	
		case(Letter) 
		0: Q = 12'b101110000000;
		1: Q = 12'b111010101000;
		2: Q = 12'b111010111010;
		3: Q = 12'b111010100000;
		4: Q = 12'b100000000000;
		5: Q = 12'b010101110100;
		6: Q = 12'b111011101000;
		7: Q = 12'b101010100000;
		endcase 
	end
endmodule
 
module shiftreg (ClockIn, Reset, Enable, Start, Flash, q); 
	input logic ClockIn;
	input logic Reset;
	input logic Start;  
	input logic Enable;
	input logic [11:0]Flash;  

	output logic [11:0]q;

	always_ff @(posedge ClockIn, posedge Reset)
	begin
			if (Reset)
				q <= 12'b0; 
			else if (Start)
				q <= Flash;
			else 
				if (Enable)
					q <= q << 1;
				else
					q <= q;
		end
endmodule 

module counter #(parameter CLOCK_FREQUENCY=500)(ClockIn, Reset, Start, Enable);
	input logic ClockIn;
	input logic Reset; 
	input logic Start;
	output logic Enable;
	
	logic [$clog2(CLOCK_FREQUENCY):0]q;
	logic [$clog2(CLOCK_FREQUENCY):0]cycles;
	
	assign cycles = CLOCK_FREQUENCY/2;
	
	always_ff @(posedge ClockIn, posedge Reset) 
	begin 
		if (Reset || Start)
			q <= cycles;
		else if (q == 0) 
			q <= (cycles - 1);
		else 
			q <=  q - 1;
	end	
	
	assign Enable = (q == 'b0)?'1:'0;

endmodule  
