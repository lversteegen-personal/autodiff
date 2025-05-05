#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "array/array_library.hpp"

using namespace ArrayLibrary;

template <DataType T>
class Loader
{

    struct LabeledData
    {
        Array<T> data;
        Array<int> label;
        long getLength() const { return data.refShape()[0]; }
    };

public:
    static LabeledData loadMNIST(int count = -1)
    {
        return loadMNIST("data/mnist_train.csv", count);
    }

    static LabeledData loadMNIST(std::string fileName, int count = -1)
    {
        std::cout << std::filesystem::current_path() << std::endl;
        std::ifstream file(fileName);
        if (!file.is_open())
            throw std::invalid_argument("File could not be read.");
        std::string line;

        std::vector<Data<T>> lineData;
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

            Data<T> data(tokens.size() - 1);
            for (size_t i = 0; i < tokens.size() - 1; i++)
            {
                if (std::is_same_v < T, double>)
                    data[i] = std::stof(tokens[i + 1]);
                else if (std::is_same_v < T,float>)
                    data[i] = std::stof(tokens[i + 1]);
                else
                    throw std::invalid_argument("Template argument for data loader has to be float or double");
            }

            lineData.push_back(data);
            lineLabels.push_back(label);
        }

        return LabeledData{Array<T>::fromFlatLines(lineData), Array<int>::fromFlatLines(lineLabels).reshape({-1})};
    }
};

#endif