from sys import argv


def get_antennas(lines: list[str]) -> dict[str, set[(int, int)]]:
    antennas: dict[str, set[(int, int)]] = dict()
    row = 0
    col = 0
    for line in lines:
        for ch in line:
            if ch == '.':
                col += 1
                continue
            if ch not in antennas:
                antennas[ch] = set()
            antennas[ch].add((row, col))
            col += 1
        row += 1
        col = 0
    return antennas


def get_antinodes(antennas: set[(int, int)]) -> set[(int, int)]:
    # This worked for my part 1... but I'm a bit lost...
    # Is this not an antinode?
    # My solution implies it isn't...
    # but it's on the line the two antennas make and twice as far
    # from one as it is the other.
    # A . . .
    # . # . .
    # . . . .
    # . . . A
    antinodes = set()
    antennas = list(antennas)
    for i in range(len(antennas)):
        for j in range(i + 1, len(antennas)):
            diff = (antennas[j][0] - antennas[i][0],
                    antennas[j][1] - antennas[i][1])
            a = (antennas[j][0] + diff[0], antennas[j][1] + diff[1])
            b = (antennas[i][0] - diff[0], antennas[i][1] - diff[1])
            antinodes.add(a)
            antinodes.add(b)
    return antinodes


def solution(lines: list[str]):
    antinodes = set()
    antennas = get_antennas(lines)
    for positions in antennas.values():
        new = get_antinodes(positions)
        antinodes = antinodes.union(new)

    rows = len(lines)
    cols = len(lines[0])
    valid_antinodes = set(filter(
        lambda an: an[0] >= 0 and an[1] >= 0 and an[0] < rows and an[1] < cols, antinodes))

    print(f"Number of unique antinodes: {len(antinodes)}")
    print(f"Number of antinodes within the board: {len(valid_antinodes)}")


def main():
    filepath = argv[1] if len(argv) > 1 else "./in.txt"
    with open(filepath, "r") as f:
        solution(list(map(str.strip, f.readlines())))


if __name__ == "__main__":
    main()
