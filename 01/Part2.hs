module Part2 where

import Part1 (getInput)

main :: IO ()
main = do
  (ls, rs) <- getInput
  -- Naive O(n^2) implementation - linear search of rs for each l.
  let scores = map (\l -> l * length (filter (== l) rs)) ls
  print $ sum scores