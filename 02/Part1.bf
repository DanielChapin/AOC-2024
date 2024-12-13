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
    >+<[>-<[-]]>    Convert *11 into its negated truthy value in *12
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

    2) Space
    Moving the continuation bit into the branching bit
    [ - >>+<< ]
    >>
    [ If branching bit is set
        [ - <<+>> ] Move the branching bit to the continuation bit

        Checking for equality
        >>>
        --------------------------------
        >+<[>-<[-]]>
        ~11

        If it is a space
        [
            [-]         Zero the equality bit
            <<<<<< [-]  Zero the continuation bit

            Incrementing the input counter
            > + <

            Shifting the number registers
            <<<< [-]        Zeroing first register
            > [ - < + > ]   Moving second register to first
            > [ - < + > ]   Moving number accumulator to second register

            TODO
            At this point we know that we have a full number in the sequence
            We need to check if the input counter is greater than 1
            This determines whether or not we should check for the distance
            1) If it is then we need to check if the distance between the two numbers is within 2
                i)  If it is then this sequence is invalid and we need to skip to the next comma
                ii) If it isn't then we continue
            2) If it isn't then we continue
        ]
        ~11
        <<<<
        ~7
    ]
    <<  Goto continuation bit
    ~5

    3) Comma
    Moving the continuation bit into the branching bit
    [ - >>+<< ]
    >>
    [
        [ - <<+>> ]

        Checking for equality
        >>
        -------------------------------------------- (subtract 44)
        >+<[>-<[-]]>
        ~10

        If it is a comma
        [
            [-]         Zero the equality bit
            <<<< [-]    Zero the input counter
            < [-]       Zero the continuation bit

            Zero all the numeric registers
            << [-]
            <  [-]
            <  [-]
            
            Add one to the result register
            < +
            ~0

            >>>>>>>>>>
            ~10
        ]

        <<<     Goto branching bit
    ]
    <<      Goto continuation bit
    ~5

    4) Assume the char is a digit if the continuation bit is set
    [ - >>+<< ]
    >>
    [
        [ - <<+>> ]
        
        Converting to numeric value
        >
        ------------------------------------------------ (subtract 48)

        Add *3 times 10 to cell
        <<<<<
        [ - >>>>> ++++++++++ <<<<< ]
        >>>>>

        Moving numeric value back to accumulator
        [ - <<<<< + >>>>> ]

        Go back to branching bit
        <
    ]
    <<  Goto continuation bit

    Cleanup
    >>[-]
    >[-]
    >[-]
    >[-]
    >[-]
    <<<<<<

    Before we need to go on to the next cycle
    <   Goto the computation bit
]