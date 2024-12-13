Goal:	Check if cell 0 is equal to cell 1
		Store the result as a bool (0 or non zero)
        in cell 2
        Assume cell 2 is zero
		No need to preserve cell 0 and 1 bc
        I have already figured out duplicating

,		Get a value in cell 0
> , <	Get a value in cell 1

Interestingly enough
because 0 is false
we can just implement subtraction and negation

Subtracting cell 0 from cell 1
[
	-
    > - <
]

Put the negation of cell 1 in cell 2
>> +	Set cell 2 to 1
< [		If cell 1 != 0
	>-<	Set cell 2 to 0
    [-]	Zero cell 1 so as to not change cell 2 again
]
