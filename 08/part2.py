from sys import argv


def get_antennas(lines: list[str]) -> dict[str, set[tuple[int, int]]]:
    antennas = dict()
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


def add_tuples(a, b):
    return tuple(map(lambda x: x[0] + x[1], zip(a, b)))


def scale_tuple(a, f):
    return tuple(map(lambda x: x * f, a))


def within_board(pos: tuple[int, int], rows: int, cols: int) -> bool:
    return pos[0] >= 0 and pos[1] >= 0 and pos[0] < rows and pos[1] < cols


def get_antinodes(antennas: set[tuple[int, int]], rows: int, cols: int) -> set[tuple[int, int]]:
    antinodes = set()
    antennas = list(antennas)
    for i in range(len(antennas)):
        for j in range(i + 1, len(antennas)):
            diff = add_tuples(antennas[j], scale_tuple(antennas[i], -1))
            a = antennas[j]
            while within_board(a, rows, cols):
                antinodes.add(a)
                a = add_tuples(a, diff)
            diff = scale_tuple(diff, -1)
            b = antennas[i]
            while within_board(b, rows, cols):
                antinodes.add(b)
                b = add_tuples(b, diff)
    return antinodes


def solution(lines: list[str]):
    rows = len(lines)
    cols = len(lines[0])
    antinodes = set()
    antennas = get_antennas(lines)
    for positions in antennas.values():
        new = get_antinodes(positions, rows, cols)
        antinodes = antinodes.union(new)

    print(f"Number of unique antinodes: {len(antinodes)}")


def main():
    filepath = argv[1] if len(argv) > 1 else "./in.txt"
    with open(filepath, "r") as f:
        solution(list(map(str.strip, f.readlines())))


if __name__ == "__main__":
    main()
