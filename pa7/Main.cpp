#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>

int main(int argc, const char* argv[])
{
    std::string inputFile = argv[1];
    int popSize = atoi(argv[2]);
    int generations = atoi(argv[3]);
    int mutationchance = atoi(argv[4]);
    int seed = atoi(argv[5]);
    std::mt19937 randGen(seed);
    std::vector<Location> locations = GetLocations(inputFile);
    Population pop = GeneratePop(popSize, static_cast<int>(locations.size()), randGen);
    std::ofstream output("log.txt");
    output << "INITIAL POPULATION:\n";
    std::for_each(pop.mMembers.begin(), pop.mMembers.end(), [&output](const std::vector<int>& member)
                  {
                      std::for_each(member.begin(), member.end() - 1, [&output](int num)
                                    {
                                        output << num << ",";
                                    });
                      output << member.back() << std::endl;
                  });
    std::vector<std::pair<int, double>> fitness = GetFitness(pop, locations);
    output << "FITNESS:\n";
    std::for_each(fitness.begin(), fitness.end(), [&output](const std::pair<int, double>& member)
                  {
                      output << member.first << ":" << member.second << std::endl;
                  });
    std::vector<std::pair<int, int>> selected;
    for(int i = 0; i < generations; i++)
    {
        selected = Selection(fitness, pop, randGen);
        output << "SELECTED PAIRS:\n";
        std::for_each(selected.begin(), selected.end(), [&output](const std::pair<int, int>& member)
                      {
                          output << "(" << member.first << "," << member.second << ")\n";
                      });
        pop = Crossover(selected, randGen, pop, mutationchance);
        output << "GENERATION: " << i + 1 << std::endl;
        std::for_each(pop.mMembers.begin(), pop.mMembers.end(), [&output](const std::vector<int>& member)
                      {
                          std::for_each(member.begin(), member.end() - 1, [&output](int num)
                                        {
                                            output << num << ",";
                                        });
                          output << member.back() << std::endl;
                      });
        fitness = GetFitness(pop, locations);
        output << "FITNESS:\n";
        std::for_each(fitness.begin(), fitness.end(), [&output](const std::pair<int, double>& member)
                      {
                          output << member.first << ":" << member.second << std::endl;
                      });
    }
    output << "SOLUTION:\n";
    std::sort(fitness.begin(), fitness.end(), [](std::pair<int, double> left, std::pair<int, double> right)
              {
                  return (left.second < right.second);
              });
    int maxFit = fitness.front().first;
    double minDistance = fitness.front().second;
    std::vector<int> bestPath = pop.mMembers[maxFit];
    std::for_each(bestPath.begin(), bestPath.end(), [&output, locations](int locIndex)
                  {
                      output << locations[locIndex].mName << std::endl;
                  });
    output << locations[0].mName << "\nDISTANCE: " << minDistance << " miles";
    return 0;
}
