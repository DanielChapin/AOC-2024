#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <set>

typedef std::pair<int, int> vec2;

template <typename A, typename B>
std::pair<A, B> addPair(std::pair<A, B> &l, std::pair<A, B> &r)
{
    return std::make_pair(l.first + r.first, l.second + r.second);
}

vec2 rotate(vec2 &vec)
{
    return std::make_pair(vec.second, -vec.first);
}

bool vecWithin(vec2 &pos, vec2 &dim)
{
    return pos.first >= 0 && pos.second >= 0 && pos.first < dim.first && pos.second < dim.second;
}

struct Board
{
    vec2 dimensions;
    std::set<vec2> obstacles;
    vec2 guardPos;
    vec2 guardDir;

    std::string visualize()
    {
        std::string result;
        for (int row = 0; row < this->dimensions.first; row++)
        {
            for (int col = 0; col < this->dimensions.second; col++)
            {
                vec2 pos = std::make_pair(row, col);

                if (this->obstacles.find(pos) != this->obstacles.end())
                {
                    result += '#';
                }
                else if (pos == this->guardPos)
                {
                    result += 'o';
                }
                else
                {
                    result += '.';
                }
            }
            result += "\n";
        }

        return result;
    }
};

vec2 guardDir(char guard)
{
    switch (guard)
    {
    case '^':
        return std::make_pair(-1, 0);
    case 'v':
        return std::make_pair(1, 0);
    case '<':
        return std::make_pair(0, -1);
    case '>':
        return std::make_pair(0, 1);

    default:
        exit(-1);
    }
}

std::optional<Board> input(std::string &filepath)
{
    std::ifstream fs(filepath);
    if (!fs.is_open())
    {
        std::cerr
            << "[FATAL] Couldn't open file '"
            << filepath
            << "'\n";
        return std::nullopt;
    }

    Board result;
    int row = 0, col = 0;
    std::string buf;
    for (; std::getline(fs, buf);)
    {
        col = 0;

        for (char c : buf)
        {
            vec2 pos = std::make_pair(row, col);
            switch (c)
            {
            case '.':
                break;
            case '#':
                result.obstacles.emplace(pos);
                break;
            case '^':
            case 'v':
            case '<':
            case '>':
                result.guardPos = pos;
                result.guardDir = guardDir(c);
                break;

            default:
                std::cerr << "[WARN] Invalid character '" << c << "'\n";
            }

            ++col;
        }

        ++row;
    }

    result.dimensions = std::make_pair(row, buf.size());

    return result;
}

int uniquePositions(Board &board)
{
    std::set<vec2> positions;

    while (vecWithin(board.guardPos, board.dimensions))
    {
        positions.emplace(board.guardPos);

        vec2 next = addPair(board.guardPos, board.guardDir);
        if (board.obstacles.find(next) != board.obstacles.end())
        {
            board.guardDir = rotate(board.guardDir);
            continue;
        }

        board.guardPos = next;
    }

    return positions.size();
}

int main(int argc, char *argv[])
{
    std::string filepath = "./in.txt";
    if (argc == 2)
    {
        filepath = std::string(argv[1]);
    }

    auto in = input(filepath);
    if (in.has_value())
    {
        Board board = std::move(in.value());
        std::cout << board.visualize() << "\n";
        auto result = uniquePositions(board);
        std::cout
            << "Guard steps on "
            << result
            << " distinct positions.\n";
    }
}