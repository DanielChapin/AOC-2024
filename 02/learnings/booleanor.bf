Goal:	Given cell 0 and 1 are booleans
		Put *0 or *1 into *2

Inputs
+	0
><	1

If *0 is true then set *2 to true and zero *0 and *1
[
	[-]
	>>+
    <[-]
    <
]

If *1 is true then set *2 to true and zero *1
>
[
	[-]
    >+<
]

End on *2
>