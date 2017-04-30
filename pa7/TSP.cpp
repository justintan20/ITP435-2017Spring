#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>

std::vector<Location> GetLocations(const std::string& fileName)
{
    std::vector<Location> output;
    std::ifstream file(fileName);
    if(file.is_open())
    {
        while(!file.eof())
        {
            std::string line;
            std::getline(file, line);
            //parse line
            int comma1 = static_cast<int>(line.find(','));
            std::string location = line.substr(0,comma1);
            line = line.substr(comma1 + 1);
            int comma2 = static_cast<int>(line.find(','));
            std::string latitude = line.substr(0,comma2);
            std::string longtitude = line.substr(comma2 + 1);
            //create location and add to vector
            Location addedLocation;
            addedLocation.mName = location;
            addedLocation.mLatitude = std::stod(latitude);
            addedLocation.mLongitude = std::stod(longtitude);
            output.push_back(addedLocation);
        }
    }
    return output;
}

Population GeneratePop(int numIndividuals, int numLocations, std::mt19937& randGen)
{
    Population output;
    output.mMembers.resize(numIndividuals);
    //create population and shuffle using randGen
    std::generate(output.mMembers.begin(), output.mMembers.end(), [numLocations,&randGen]()
                  {
                      std::vector<int> initialVec(numLocations);
                      int i = 0;
                      std::generate(initialVec.begin(), initialVec.end(), [&i]()
                                    {
                                        return i++;
                                    });
                      std::shuffle(initialVec.begin()+1, initialVec.end(), randGen);
                      return initialVec;
                  });
    return output;
}

std::vector<std::pair<int, int>> Selection(const std::vector<std::pair<int, double>>& fitness, const Population& pop, std::mt19937& randGen)
{
    std::vector<std::pair<int, double>> fitnessSort = fitness;
    //sort fitness
    std::sort(fitnessSort.begin(), fitnessSort.end(), [](std::pair<int, double> left, std::pair<int, double> right)
              {
                  return (left.second < right.second);
              });
    int popSize = static_cast<int>(pop.mMembers.size());
    //create probabilities and initialize
    std::vector<double> probabilities(popSize);
    std::generate(probabilities.begin(), probabilities.end(), [popSize]()
                  {
                      return 1.0/static_cast<double>(popSize);
                  });
    //top 2 fitness
    int pop1 = fitnessSort[0].first;
    int pop2 = fitnessSort[1].first;
    probabilities[pop1] *= 6.0;
    probabilities[pop2] *= 6.0;
    //top half fitness
    std::for_each(fitnessSort.begin() + 2, fitnessSort.begin() + fitness.size()/2, [&probabilities](const std::pair<int, double>& member)
                  {
                      probabilities[member.first] *= 3.0;
                  });
    double probSum = std::accumulate(probabilities.begin(), probabilities.end(), 0.000);
    std::vector<double> probAdjusted;
    //normalize probabilities
    std::transform(probabilities.begin(), probabilities.end(), std::back_inserter(probAdjusted), [probSum](double prob)
                   {
                       return prob / probSum;
                   });
    
    std::vector<std::pair<int, int>> output(pop.mMembers.size());
    //selecet two pairs
    std::generate(output.begin(), output.end(), [&randGen,pop,probAdjusted]()
                  {
                      std::uniform_real_distribution<double> dist(0.0,1.0);
                      int parent1 = SelectParent(randGen, pop, probAdjusted, dist);
                      int parent2 = SelectParent(randGen, pop, probAdjusted, dist);
                      std::pair<int, int> selected;
                      selected.first = parent1;
                      selected.second = parent2;
                      return selected;
                  });
    return output;
}

int SelectParent(std::mt19937& randGen, const Population& pop, const std::vector<double>& probAdjusted, std::uniform_real_distribution<double>& dist)
{
    double parent = dist(randGen);
    std::vector<double> probDist;
    double probMax = 0.0;
    //use this vector that has range of probabilities
    std::transform(probAdjusted.begin(), probAdjusted.end(), std::back_inserter(probDist), [&probMax](double prob)
                   {
                       probMax += prob;
                       return probMax;
                   });
    bool selected = false;
    int index = 0;
    //find where the random double sits
    std::for_each(probDist.begin(), probDist.end(), [&selected,parent, &index](double probLimit)
                  {
                      if((parent <= probLimit) && (!selected))
                      {
                          selected = true;
                      }
                      else if (!selected)
                      {
                          index++;
                      }
                  });
    return index;
}

Population Crossover(std::vector<std::pair<int, int>>& selected, std::mt19937& randGen, const Population& pop, int mutationChance)
{
    int size = static_cast<int>(pop.mMembers[0].size());
    double givenChance = static_cast<double>(mutationChance) / 100.0;
    std::vector<std::vector<int>> crossoverMembers;
    std::for_each(selected.begin(), selected.end(), [size, &randGen, &pop, givenChance, &crossoverMembers](const std::pair<int, int>& member)
                  {
                      std::uniform_int_distribution<int> crossoverIndexDist(1, size - 2);
                      int crossoverIndex = crossoverIndexDist(randGen);
                      std::uniform_int_distribution<int> goFirstDist(0,1);
                      int goFirst = goFirstDist(randGen);
                      int firstParent;
                      int secondParent;
                      //see which one goes first
                      if(goFirst == 1)
                      {
                          firstParent = member.first;
                          secondParent = member.second;
                      }
                      else
                      {
                          firstParent = member.second;
                          secondParent = member.first;
                      }
                      //set accordingly
                      std::vector<int> firstLocations = pop.mMembers[firstParent];
                      std::vector<int> secondLocations = pop.mMembers[secondParent];
                      std::vector<int> result;
                      //first copy the first few specified by crossover index
                      std::copy_n(firstLocations.begin(), crossoverIndex + 1, std::back_inserter(result));
                      //then copy everything that has not appeared yet
                      std::copy_if(secondLocations.begin(), secondLocations.end(), std::back_inserter(result), [result](int i)
                                   {
                                       bool doesExist = false;
                                       std::for_each(result.begin(), result.end(), [i,&doesExist](int j)
                                                     {
                                                         if(i == j)
                                                         {
                                                             doesExist = true;
                                                         }
                                                     });
                                       return (!doesExist);
                                   });
                      std::uniform_real_distribution<double> mutationChanceDist(0.0,1.0);
                      double chance = mutationChanceDist(randGen);
                      //see if needs to mutate or not
                      if(chance <= givenChance)
                      {
                          std::uniform_int_distribution<int> randIndexDist(1, size - 1);
                          int index1 = randIndexDist(randGen);
                          int index2 = randIndexDist(randGen);
                          std::swap(result[index1], result[index2]);
                      }
                      crossoverMembers.push_back(result);
                  });
    //create new population
    Population output;
    output.mMembers = crossoverMembers;
    return output;
}
