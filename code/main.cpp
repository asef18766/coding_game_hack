#include <iostream>
#include "mapAdapter.h"
#include "maze.h"
#include "robot.h"

int main()
{
    int x, y;
    uint64_t step;
    std::cin >> x >> y;
    std::cin >> step;

    auto maze = Maze::GetInstance(x, y);
    CDGMapAdapter maze_parser;
    maze_parser.Init(maze);

    char **map = new char *[y];
    for (auto i = 0; i != y; ++i)
        map[i] = new char[x];

    for (auto i = 0; i != y; ++i)
        std::cin >> map[i];
    if (!maze_parser.Consume(map, x, y))
        throw "Consume failure";
    CDGDPRobot robot;
    auto stp = maze_parser.StartPoint();
    robot.Init(stp.first, stp.second, &step, maze);
    while (step > 0)
        robot.Walk();

    auto pos = robot.GetPos();
    std::cout << pos.first << ' ' << pos.second << '\n';

    for (auto i = 0; i != y; ++i)
        delete[] map[i];
    delete[] map;

    return 0;
}