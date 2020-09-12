# Color Routing for Double Patterning

## Problem Description 

This problem is to find the optimal solution to pin connections in double patterning techniques. People have to find out the way to connect either two-pin wire or multi-pin wire without short/open violation and color conflict. The metals that the wires are set have their preferred direction, and the color distribution should
be as uniform as possible. The input files consist of three smaller files: pin file, net file and block file. The net file denoted the combination of pins for each net, either for two pins or for more pins. 

Detailed report can be found [here](https://drive.google.com/file/d/1CWDREImN59Q_eaBa4h6AJu1to5-KOGTM/view?usp=sharing)

## File tree 
 * Directory:
 ````
  doc/		reports
  src/ 		source C++ codes
  ````
 * How to compile:
 ````
	cd final/
	make
````
* HOW TO RUN, NOTICE THAT BINARY FILE IS NOT IN THE bin DIRECTORY
````
  ./color_route case1_pin.in case1_net.in case1_blockage.in case1_result.out
````
