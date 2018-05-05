`timescale 1ns / 1ps

module piEstimator (
   output reg  [31:0] innerPoints,
   output reg  [31:0] totalPoints,
   output reg  [31:0] timer,
   output wire        busy,
   output wire        done,
   input  wire [31:0] seedX,
   input  wire [31:0] seedY,
   input  wire [31:0] iterations,
   input  wire        enable,
   input  wire        clk,
   input  wire        reset
);

   reg [1:0] ps, ns;
   parameter
   init    = 2'b00,
   sim     = 2'b01,
   simDone = 2'b10;

   initial begin
      ps = init;

      timer       = 32'b0;
      innerPoints = 32'b0;
      totalPoints = 32'b0;
   end

   wire        setSeed;
   wire [31:0] randX, randY;
   lfsr32 lfsrX (
      .q(randX),
      .seed(seedX),
      .setSeed(setSeed),
      .clk(clk),
      .reset(reset)
   );
   lfsr32 lfsrY (
      .q(randY),
      .seed(seedY),
      .setSeed(setSeed),
      .clk(clk),
      .reset(reset)
   );

   reg [31:0] randX_reg, randY_reg;
   wire       inside;
   circleChecker cc (.inside(inside), .x(randX_reg), .y(randY_reg));

   assign busy    = ~ps[1] & ps[0];
   assign done    = ps[1] & ~ps[0];
   assign setSeed = ~ps[1] & ~ps[0] & enable;

   always @ (*) begin
      case (ps)
         init:    if (~enable)                      ns = init;
                  else                              ns = sim;
         sim:     if (totalPoints < iterations - 1) ns = sim;
                  else                              ns = simDone;
         simDone:                                   ns = simDone;
         default:                                   ns = simDone;
      endcase
   end

   always @ (posedge clk) begin
      if (reset) begin
         timer       <= 32'b0;
         innerPoints <= 32'b0;
         totalPoints <= 32'b0;
      end else if (~ps[1] & ps[0]) begin
         timer <= timer + 1'b1; // Increment timer in sim state
         if (inside)
            innerPoints <= innerPoints + 1'b1;
         totalPoints <= totalPoints + 1'b1;
      end

      randX_reg <= randX; // Registers for pipeline
      randY_reg <= randY;
   end

   always @ (posedge clk) begin
      if (reset) ps <= init;
      else       ps <= ns;
   end
endmodule

module piEstimator_testbench ();
   wire [31:0] innerPoints, totalPoints, timer;
   wire        busy, done;
   reg  [31:0] seedX, seedY, iterations;
   reg         enable, clk, reset;

   piEstimator dut (
      .innerPoints(innerPoints),
      .totalPoints(totalPoints),
      .timer(timer),
      .busy(busy),
      .done(done),
      .seedX(seedX),
      .seedY(seedY),
      .iterations(iterations),
      .enable(enable),
      .clk(clk),
      .reset(reset)
   );

   parameter CLK_PER = 10;
   initial begin
      clk <= 1;
      forever #(CLK_PER / 2) clk <= ~clk;
   end

   initial begin
      reset <= 0; enable <= 0;
      seedX <= 32'hCC00_AA00;
      seedY <= 32'hAACC_EEAA;
      iterations <= 32'hFFFFF; @(posedge clk);
      reset <= 1; enable <= 1; @(posedge clk);
      reset <= 0;              @(posedge clk);
      repeat(6)                @(posedge clk);
      reset <= 1;              @(posedge clk);
      reset <= 0;              @(posedge clk);
      seedX <= 32'hDEAD_BEEF;
      seedY <= 32'hDECA_FBAD;  @(posedge clk);
      
      repeat(iterations + 6)   @(posedge clk);
      $stop;   
   end
endmodule