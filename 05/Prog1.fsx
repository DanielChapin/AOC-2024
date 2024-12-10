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

let isPrintOrderValid(pagePairs: (int * int) list, order: int list): bool =
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
    // A page is valid if none of it's predecessors come after it and none of its postdecessors come before
    let lookupContains(lookup: Map<'K, Set<'V>>, key: 'K, value: 'V): bool =
        match Map.tryFind key lookup with
        | Some values -> values.Contains(value)
        | _ -> false

    let rec isValid(page: int, previous: int list, upcoming: int list): bool =
        let upcomingValid = List.forall (fun p -> not <| lookupContains(predecessorLookup, page, p)) upcoming
        let previousValid = List.forall (fun p -> not <| lookupContains(postdecessorLookup, page, p)) previous
        upcomingValid && previousValid &&
            match upcoming with
            | [] -> true
            | u :: us -> isValid(u, page :: previous, us)
    
    match order with
    | [] -> true
    | p :: ps -> isValid(p, [], ps)

[<EntryPoint>]
let main(args) = 
    let filepath = 
        match args with
        | [||] -> "./in.txt"
        | [|filepath|] -> filepath
        | _ -> raise (ArgException "Expected either 1 or 0 command line args.")
    let (pagePairs, printOrders) = input filepath
    let pagePairs = Array.toList pagePairs

    let middle(vals: 'V []): 'V =
        let l = vals.Length
        vals.[l / 2]

    Array.filter (fun pages -> isPrintOrderValid(pagePairs, Array.toList pages)) printOrders 
    |> Array.map middle
    |> Array.sum
    |> printfn "Sum of middle pages: %d"

    0