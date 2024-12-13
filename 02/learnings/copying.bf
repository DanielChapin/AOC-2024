Goal: Copy the current byte into the next two cells
	  assuming that the next two cells are zero

, Getting a byte to copy

[
	-		Subtract one from the current cell
    
    > +		Add one to the first cell
    > +		Add one to the second cell
    		^^^ This can be repeated any number of times
	<<		Return to the original cell
]			Only repeat if the cell is non zero