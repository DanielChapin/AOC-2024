use std::{
    env,
    fs::File,
    io::{self, BufRead, BufReader},
    iter,
    path::Path,
};

use regex::Regex;

#[derive(Debug)]
struct Eqn {
    pub lhs: i64,
    pub rhs: Vec<i64>,
}

#[derive(Debug)]
enum InputErr {
    ParseErr(String),
    IoErr(io::Error),
}

fn input(path: &Path) -> Result<Vec<Eqn>, InputErr> {
    let file = File::open(path).map_err(InputErr::IoErr)?;
    let lines = BufReader::new(file).lines();
    let pattern = Regex::new(r"^(\d+):\s*([\d\s]+)\s*$").unwrap();

    lines
        .map(|line| {
            let line = line.map_err(InputErr::IoErr)?;
            let caps = pattern.captures(&line).ok_or(InputErr::ParseErr(format!(
                "Could not parse Eqn from line '{line}'"
            )))?;

            let lhs = caps[1].parse().map_err(|_| {
                InputErr::ParseErr(format!(
                    "Failed to parse lhs of equation from '{}'",
                    &caps[0]
                ))
            })?;
            let rhs = caps[2]
                .split_whitespace()
                .map(|num| {
                    num.parse().map_err(|_| {
                        InputErr::ParseErr(format!(
                            "Failed to parse rhs element '{}' in eqn '{}'",
                            num, &caps[0]
                        ))
                    })
                })
                // NOTE I reverse here just because I don't like the given associativity :)
                .rev()
                .collect::<Result<_, _>>()?;

            Ok(Eqn { lhs, rhs })
        })
        .collect()
}

fn input_from_cli() -> Result<Vec<Eqn>, InputErr> {
    let filepath = match &env::args().collect::<Vec<_>>()[..] {
        [_, filepath] => filepath.clone(),
        _ => String::from("in.txt"),
    };

    input(Path::new(&filepath))
}

fn find_substitution(value: i64, nums: &[i64]) -> Option<Vec<char>> {
    match nums {
        [] => None,
        [n] => {
            if *n == value {
                Some(vec![])
            } else {
                None
            }
        }
        [n, ns @ ..] => {
            if let Some(mut rest) = find_substitution(value - n, ns) {
                rest.insert(0, '+');
                Some(rest)
            } else if let (0, Some(mut rest)) = (value % n, find_substitution(value / n, ns)) {
                rest.insert(0, '*');
                Some(rest)
            } else {
                None
            }
        }
    }
}

fn main() -> Result<(), InputErr> {
    let eqns = input_from_cli()?;

    let result = eqns
        .iter()
        .map(|eqn| {
            let subst = find_substitution(eqn.lhs, &eqn.rhs);
            println!("{:?}    {:?}", &eqn, &subst);
            subst.map_or(0, |_| eqn.lhs)
        })
        .sum::<i64>();

    println!("Number of eqns with valid substitutions: {result}");

    Ok(())
}
