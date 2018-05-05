`timescale 1ns / 1ps

module lfsr32 (
   output wire [31:0] q,
   input  wire [31:0] seed,
   input  wire        setSeed,
   input  wire        clk,
   input  wire        reset
);

   wire [31:0] d, muxIn0;
   xnor xn0 (muxIn0[0], q[0], q[1], q[21], q[31]); // LFSR taps
   assign muxIn0[31:1] = q[30:0];
   genvar i;
   generate
      for (i = 0; i < 32; i = i + 1) begin
         mux2_1 mux (.out(d[i]), .in0(muxIn0[i]), .in1(seed[i]), .sel(setSeed));
         d_ff dff (.q(q[i]), .d(d[i]), .clk(clk), .reset(reset));
      end
   endgenerate
endmodule

module d_ff (
   output reg  q,
   input  wire d,
   input  wire clk,
   input  wire reset
);

   always @ (posedge clk) begin
      if (reset) q <= 0;
      else       q <= d;
   end
endmodule

module mux2_1 (
   output wire out,
   input  wire in0,
   input  wire in1,
   input  wire sel
);

   wire seln;
   not n (seln, sel);

   wire out0, out1;
   and a0 (out0, in0, seln);
   and a1 (out1, in1, sel);

   or o (out, out0, out1);
endmodule

module lfsr32_testbench ();
   wire [31:0] q;
   reg  [31:0] seed;
   reg         setSeed, clk, reset;

   lfsr32 dut (
      .q(q),
      .seed(seed),
      .setSeed(setSeed),
      .clk(clk),
      .reset(reset)
   );

   parameter CLK_PER = 10;
   initial begin
      clk <= 1;
      forever #(CLK_PER / 2) clk <= ~clk;
   end

   initial begin
      seed <= 32'hCCAA_8668;
      reset <= 0; setSeed <= 0; @(posedge clk);
      reset <= 1;               @(posedge clk);
      reset <= 0; setSeed <= 1; @(posedge clk);
                  setSeed <= 0; @(posedge clk);
      repeat(24)                @(posedge clk);
      $stop;
   end
endmodule