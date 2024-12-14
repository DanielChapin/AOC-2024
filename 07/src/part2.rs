mod part1;

use part1::{input_from_cli, InputErr};

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
            } else if let (true, Some(mut rest)) = (
                value.to_string().ends_with(&n.to_string()),
                find_substitution(
                    (value - n) / i64::pow(10, n.to_string().len().try_into().unwrap()),
                    ns,
                ),
            ) {
                rest.insert(0, '|');
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

    println!("Sum of lhs of eqns with substitutions: {result}");

    Ok(())
}
