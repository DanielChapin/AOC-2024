module Part1 where

import Data.List (sort)
import System.Environment (getArgs)

main :: IO ()
main = do
  (ls, rs) <- getInput
  let dists = zipWith (\l r -> abs $ l - r) (sort ls) (sort rs)
  print $ sum dists

getInput :: IO ([Int], [Int])
getInput = do
  args <- getArgs
  let file = case args of [a] -> a; _ -> "in.txt"
  getInput' file

getInput' :: FilePath -> IO ([Int], [Int])
getInput' file = do
  file <- readFile file
  let vals = map (map read . words) $ lines file :: [[Int]]
  return $ foldr (\[l, r] (ls, rs) -> (l : ls, r : rs)) ([], []) vals