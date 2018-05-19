set num_instances 6
for {set i 0} {$i < $num_instances} {incr i} {
   startgroup
   create_bd_cell -type ip -vlnv xilinx.com:user:PiEstimator:1.0 PiEstimator_$i
   connect_bd_net [get_bd_pins PiEstimator_0/enableFlag] [get_bd_pins PiEstimator_$i/enable]
   connect_bd_net [get_bd_pins PiEstimator_0/userResetFlag] [get_bd_pins PiEstimator_$i/userReset]
   endgroup
}

