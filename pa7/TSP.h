#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

std::vector<Location> GetLocations(const std::string& fileName);

Population GeneratePop(int numIndividuals, int numLocations, std::mt19937& randGen);

std::vector<std::pair<int, double>> GetFitness(const Population& pop, const std::vector<Location>& locations);

std::vector<std::pair<int, int>> Selection(const std::vector<std::pair<int, double>>& fitness, const Population& pop, std::mt19937& randGen);

int SelectParent(std::mt19937& randGen, const Population& pop, const std::vector<double>& probAdjusted, std::uniform_real_distribution<double>& dist);

Population Crossover(std::vector<std::pair<int, int>>& selected, std::mt19937& randGen, const Population& pop, int mutationChance);
