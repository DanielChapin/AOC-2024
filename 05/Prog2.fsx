open System.IO
open System.Text.RegularExpressions

exception ArgException of string

let input(filepath): ((int * int) []) * (int [] []) = 
    // Get lines from file and split them at the separation of pairs and lists
    let lines = File.ReadAllLines filepath
    let (pairLines, listLines) = 
        Array.splitAt 
        <| Array.findIndexBack (fun line -> System.String.IsNullOrEmpty(line)) lines 
        <| lines
        |> (fun (pl, ll) -> (pl, Array.tail ll))

    // Parsing pairs
    let pairs =
        Array.map (fun (line: string) -> line.Split "|" |> Array.map System.Int32.Parse) pairLines
        |> Array.map (fun [|l; r|] -> (l, r))

    // Using splits for lists
    let lists = Array.map (fun (line: string) -> Array.map System.Int32.Parse <| line.Split ",") listLines

    (pairs, lists)

let lookupContains(key: 'K, value: 'V, lookup:  Map<'K, Set<'V>>): bool = 
    match Map.tryFind key lookup with
    | Some values -> Set.contains value values
    | None -> false

let rec insertPage(predecessors: Map<int, Set<int>>, postdecessors: Map<int, Set<int>>, order: int list, page: int): (int list) option =
    match order with
    | [] -> Some [page]
    | first :: rest -> 
        if lookupContains(first, page, predecessors)
        then Some(page :: order)
        else insertPage(predecessors, postdecessors, rest, page) 
             |> Option.map (fun rest' -> first :: rest')

let rec reorderPages'(predecessors: Map<int, Set<int>>, postdecessors: Map<int, Set<int>>, current: int list, remaining: int list): (int list) option =
    match remaining with
    | [] -> Some current
    | next :: rest ->
        // I would much rather do this with a monadic bind, but I can't figure out how
        match insertPage(predecessors, postdecessors, current, next) with
        | None -> None
        | Some order' -> reorderPages'(predecessors, postdecessors, order', rest)

let reorderPages(pagePairs: (int * int) list, order: int list): (int list) option =
    let nums = Set.ofList order
    let relevantPairs = List.filter (fun (l, r) -> nums.Contains(l) && nums.Contains(r)) pagePairs
    let predecessorLookup = 
        List.groupBy (fun (_, r) -> r) relevantPairs 
        |> List.map (fun (k, pairs) -> (k, List.map (fun (l, _) -> l) pairs |> Set.ofList))
        |> Map.ofList
    let postdecessorLookup = 
        List.groupBy (fun (l, _) -> l) relevantPairs 
        |> List.map (fun (k, pairs) -> (k, List.map (fun (_, r) -> r) pairs |> Set.ofList))
        |> Map.ofList
    
    match reorderPages'(predecessorLookup, postdecessorLookup, [], order) with
    | None -> None
    | Some order' ->
        if order = order'
        then None
        else Some order'

[<EntryPoint>]
let main(args) = 
    let filepath = 
        match args with
        | [||] -> "./in.txt"
        | [|filepath|] -> filepath
        | _ -> raise (ArgException "Expected either 1 or 0 command line args.")
    let (pagePairs, printOrders) = input filepath
    let pagePairs = Array.toList pagePairs

    let middle(vals: 'V list): 'V =
        let l = vals.Length
        vals.[l / 2]

    Array.choose (fun pages -> reorderPages(pagePairs, Array.toList pages)) printOrders
    |> (fun arr -> printfn "%A" arr; Array.map middle arr)
    |> Array.sum
    |> printfn "Sum of reordered middle elements: %d"

    0