#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "array.hpp"

class Loader {

    struct LabeledData {
        Array<double> data;
        Array<int> label;
    };

    public:

    static LabeledData loadMNIST(int count = -1)
    {
        return loadMNIST("data/mnist_train.csv", count);
    }

    static LabeledData loadMNIST(std::string fileName, int count=-1)
    {
        std::cout << std::filesystem::current_path() << std::endl;
        std::ifstream file(fileName);
        if (!file.is_open())
            throw std::invalid_argument("File could not be read.");
        std::string line;

        std::vector<Data<double>> lineData;
        std::vector<Data<int>> lineLabels;

        while (!file.eof())
        {
            if (count-- == 0)
                break;

            std::getline(file, line);
            if (line.empty())
                continue;

            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(line);
            while (std::getline(tokenStream, token, ','))
            {
                tokens.push_back(token);
            }

            Data<int> label({std::stoi(tokens[0])});

            Data<double> data(tokens.size() - 1);
            for (size_t i = 1; i < tokens.size(); i++)
            {
                data[i] = std::stod(tokens[i]);
            }

            lineData.push_back(data);
            lineLabels.push_back(label);
        }

        return LabeledData{Array<double>::fromFlatLines(lineData), Array<int>::fromFlatLines(lineLabels)};
    }
};

#endif