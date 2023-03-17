This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

Each function operates only in a certain ROI. There's a need to pass ROI's coordinate locations 
as parameter to these functions.

1. Histogram stretching: histo
Histogram stretch on a single-channel image.

2. Two-valued thresholding + histogram stretching: combohisto
Perform two-valued thresholding (T1, T2 with T2 > T1) to segment a single-channel image 
into three subsets (dark, medium, bright), then apply histogram stretching on each subset.

3. Histogram stretch on a color image: perchastretch
Apply function histo to R,G,B components independently.

4. Histogram stretch on a color image: rgbstretch
Apply function histo to R,G,B components independently.

5. Pseudo coloring (two-valued threshold + stretch): colorize
Perform two-valued thresholding (T1, T2 with T2 > T1) to color each channel, 
then apply histogram stretching on each channel.

6.  Pseudo coloring (9-valued threshold): multithreshold
Implement 9-valued equal thresholding by dividing image intensity range into ten equal 
intervals between Imin and Imax; then colorize using ten different hard-coded colors.

*** PARAMETERS FILE ***

[input image] [output image] [#roi] [xl] [yl] [Sx] [Sy] [function name] [p1...pX] ...

The following steps can be followed to construct the parameters:
1. the input file name;
2. the output file name;
3. number of ROIs
4. xl - inital row;
5. yl - intial column;
6. Sx - total number of pixels in the x axis
7. Sy - total number of pixels in the y axis
4. the name of the filter. Use "histo", "combohisto", "perchastretch", "rgbstretch", "colorize", "multithreshold" for your filters;
4. choose one of the following option:
	a. "histo" or "multithreshold": no parameters required.
	b. "rgbstretch": 
		- requires (A,B) 
		- A and B are the mini and maxi intensity values possible.
	c. "perchastretch": 
		- requires (channel_name,A,B) 
		- A and B are the mini and maxi intensity values possible.
	d. "combohisto" or "colorize": 
		- requires (A,B,T1,T2) 
		- A and B are the mini and maxi intensity values possible.
		- T1 & T2 are the two threshold values and T1 < T2.

*** Run the program: 
$ cd project
$ make
$ cd bin
$ ./iptool parameters.txt