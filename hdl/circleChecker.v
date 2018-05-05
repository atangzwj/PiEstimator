`timescale 1ns / 1ps

module circleChecker(
   output reg         inside,
   input  wire [31:0] x,
   input  wire [31:0] y
);

   parameter CIRCLE_RADIUS_SQUARED = 65'h0_FFFF_FFFE_0000_0001;

   reg [63:0] xSquared, ySquared;
   reg [64:0] squaredSum;

   always @ (*) begin
      xSquared = x * x;
      ySquared = y * y;
      squaredSum = xSquared + ySquared;
      inside = squaredSum <= CIRCLE_RADIUS_SQUARED;
   end
endmodule

module circleChecker_testbench ();
   wire        inside;
   reg  [31:0] x;
   reg  [31:0] y;
   
   circleChecker dut (.inside(inside), .x(x), .y(y));
   
   initial begin
      x <= 32'h0000_0000; y <= 32'h0000_0000; #10;
      x <= 32'h8000_0000;                     #10;
                          y <= 32'h8000_0000; #10;
      x <= 32'hB504_F333;                     #10;
                          y <= 32'hB504_F333; #10;
      x <= 32'hCC00_0000;                     #10;
      $stop;
   end
endmodule