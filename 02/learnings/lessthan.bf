Goal:	Check if cell 0 is less than cell 1

Ideas:
	1) subtract from both until either hits 0
    	then analyze the results
	2) subtract from cell 0 and cell 1 until cell 1 hits zero
    	if at any point cell 0 was 0 then it must be less than or equal to

Inputs
+++
> +++++ <

Layout:
	Inputs: *0 *1
    Result: *4 (*0 lt *1)
    Temp: *0 *1 *2 *3
    Starts on: *0
    Ends on: *0
> [    
	Checking if cell 0 is 0
    <
    1) Cloning cell 0 into cell 2
	[ - >> + > + <<< ]
    >>> [ - <<< + >>> ]
    2) Converting cell 2 into its inverted truthy value
    	in cell 3
    +
    < [ >-< [-] ]
    3) If we reached a zero stop execution
    >
    [ >+< - << [-] + >> ]
    
	<< -	Subtract from cell 1
    <->		Subtract from cell 0
]
Cleanup (zero all temps and return to cell 0)
>> [-] < [-] < [-] < [-]