######################################
Polygon Clipping: readme

- Doug 		
######################################

Quick Key Guide:
*********************
Key				Function
----    		-----------
Left Mouse		- Click to make polygon points
				- Click and drag for clipping rectangle
				
Right Mouse		- Click for final polygon point

C				- Begin clipping process
				- clip previously drawn polygon again 
				
E 				- Erase the screne and begin drawing again


How to Use:
-------------
- the program allows the user to click up to 9 times with the left mouse and then once with the right click.
- each left click will create a vertex of the users desired polygon. 
- the right click will signal to the program "done adding verticies, now I want to see the polygon"
- the program will not process the right click until the user has at least clicked the left button at least twice.
- once the user has right clicked the program will display the polygon based on the points clicked.  
- this process is allowed up to ten times.
- at any point the user can press 'c' and begin the clipping process
- the user can draw a clipping rectangle by clicking the left mouse and then dragging.
- once released, the rectangle stays and shows the user the clipped polygon 
- NOTE: it will take a few seconds before you can see the clipped region.
- If the user want to clip again, they simply press the 'c' button again and repeat the process as many times as they'd like
- If the user would like to start over from scratch, the user simply presses 'e' and the screne is erased and the user can draw again.


How to Run:
-------------
- the executable can be found in the Debug folder of the uploaded content
- otherwise you can run the code in a C/C++ compiling environment
- NOTE: you will need the GLU and openGl packages to run 

Scan Converting analysis:
--------------------------------
- The scan conversion will work for any polygon drawn. It has been tested with all kinds of random shapes

Clipping Analysis:
---------------------------------
- The clipping of all polygons works. Many random shapes were tested and proven to work correctly.

Testing Environment:
-----------------------
- Visual Studios 2012