NOTE: This file is incomplete

Goal:	Parse a number from input
		Assume the number can fit in a single cell
        Stop parsing at the first non digit

Steps:
	1) Get a number from input
    2) Check if that number is a digit
    	a) If it is a digit 
        	then parse the digit (into a num)
            multiply the accumulator by 10
            and finally add the digit to the accumulator
		b) If it is not then we have parsed the num

Initialization:
1) Put 48 in cell 0 (this is ascii 0)
Layout:
	Output: *0 (48)
    Temp: *0 *1
    Ends on: *1
+++++++
[ - >+++++++< ]
> -
[ ------ <++++++> ]

Take an input in cell 1
,

Subtract 48 to convert to maybe num
Layout:
	Input: *0
    Output: *0 *1 (Maybe num)
    Temp: *1
    Ends on:
<[ ---- >---- >++++ << ]>
> [ ---- <<++++>> ] <

Check if the value is a digit (less than 10)
