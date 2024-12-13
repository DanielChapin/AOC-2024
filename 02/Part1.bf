The annoying part of this implementation is that the lines of
the input do not have the same number of values

Some notes on the assumptions of the inputs
(because terminals and newlines don't go well together)
The input will all be on one line
numbers will always be capped by spaces and groups will always end with a comma
there will be one dollar sign at the end to denote the end of file

Some notes on the structure of the code
There are comments above all the larger steps of computation
I denote the resulting position of an operation with ~x where x is the cell number

Let's have cell 0 be the result of computation

CONSTANTS
44 = comma ascii code
32 = space ascii code
36 = dollars ascii code
48 = zero ascii code

Main loop: this is where the bulk of the computation happens
First we are going to reserve some cells to hold some values for us
*1 will hold a numeric input
*2 will hold a second numeric input
*3 will be the accumulator for parsing numbers
*4 will be a boolean flag indicating if computation should proceed (have we found $)
*5 will be a boolean flag indicating whether the current iteration should continue
*6 will hold the number of input numbers parsed this group (for checking distance between current and previous)
*7 will be a zero cell for breaking from loops

>>>>+    Initialize *4 to true
Main loop implementation
[
    Continuation bit is reset to 1 each iteration
    >[-]+
    
    >>, Get a character in *7
    Moving *7 to *8 ~ *11 for these steps:
    1) Check for $
    2) Check for a space character
    3) Check for a comma
    4) Resolve a digit
    [ - >+ >+ >+ >+ <<<< ]
    >>>>
    ~11

    1) Dollar sign
    First checking if *11 == '$'
    ------------------------------------ Subtract '$' from *11
    >+<[>-<[+]]>    Convert *11 into its negated truthy value in *12
    ~12

    If it is a $
    [
        -           Zero the equality bit
        <<<<<<< [-] Zero the continuation bit
        < [-]       Zero the computation bit
        >>>>>>>>    Return to the equality bit
    ]
    ~12
    <<<<<<<     Goto continuation bit
    ~5

    TODO This shit is just not correct; having some trouble figuring out how to have branching
         expressions result in the pointer being on the same cell
    2) Space
    [ If continuation bit is set
        Checking for equality
        >>>>>

        --------------------------------
        >+<[>-<[-]]>
        ~11

        If it is a space
        [
            -           Zero the equality bit
            <<<<<< [-]  Zero the continuation bit

            Shifting the number registers
            <<<< [-]        Zeroing first register
            > [ - < + > ]   Moving second register to first
            > [ - < + > ]   Moving number accumulator to second register

            If we have more than one number for this group
            check if the distance is within 2
        ]
        ~11
        <<<<
        ~7
    ]
    <<  Goto continuation bit

    3) Comma

    4) Digit

    Before we need to go on to the next cycle
    <   Goto the computation bit
]