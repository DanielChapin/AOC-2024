#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <optional>
#include <set>

typedef std::pair<int, int> vec2;

template <typename A, typename B>
std::pair<A, B> addPair(const std::pair<A, B> &l, const std::pair<A, B> &r)
{
    return std::make_pair(l.first + r.first, l.second + r.second);
}

vec2 rotate(const vec2 &vec)
{
    return std::make_pair(vec.second, -vec.first);
}

bool vecWithin(const vec2 &pos, const vec2 &dim)
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

            case '\r':
                --col;
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

std::pair<std::set<vec2>, bool> uniquePositions(const Board &board)
{
    // We can very simply detect a loop by checking if the guard has ever been
    // in the exact current position and direction
    std::set<std::pair<vec2, vec2>> seenStates;
    std::set<vec2> positions;
    bool loop = false;

    vec2 pos = board.guardPos, dir = board.guardDir;

    while (vecWithin(pos, board.dimensions))
    {
        std::pair<vec2, vec2> state(pos, dir);
        if (seenStates.find(state) != seenStates.end())
        {
            loop = true;
            break;
        }

        seenStates.emplace(state);
        positions.emplace(pos);

        vec2 next = addPair(pos, dir);
        if (board.obstacles.find(next) != board.obstacles.end())
        {
            dir = rotate(dir);
            continue;
        }

        pos = next;
    }

    return std::make_pair(positions, loop);
}

std::set<vec2> solution(Board &board)
{
    // Note here that we're assuming the guard won't start in a loop.
    std::set<vec2> viablePlacements = uniquePositions(board).first;
    viablePlacements.erase(board.guardPos);

    std::set<vec2> goodPlacements;
    for (vec2 placement : viablePlacements)
    {
        // Modifying the existing board to avoid cloning the whole thing
        board.obstacles.emplace(placement);

        bool isLoop = uniquePositions(board).second;
        if (isLoop)
        {
            goodPlacements.emplace(placement);
        }

        board.obstacles.erase(placement);
    }

    return goodPlacements;
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
        auto result = solution(board);
        std::cout << "The following positions put the guard in a loop:\n";
        for (vec2 placement : result)
        {
            std::cout << placement.first << ", " << placement.second << "\n";
        }
        std::cout << "Total count: " << result.size() << "\n";
    }
}