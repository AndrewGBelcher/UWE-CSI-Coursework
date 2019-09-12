force reset 0
force start 0
force clk 0 0, 1 10 -repeat 20
run 20

force reset 1
force start 0
run 20

force start 1
force reset 0
force data_in 44
run 45

force start 0
force data_in 33
run 1000
