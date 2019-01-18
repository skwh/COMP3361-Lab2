/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.cpp
 * Author: evan
 * 
 * Created on January 16, 2019, 5:25 PM
 */

#include "Process.h"

#include <iostream>
#include <stdexcept>

Process::Process(std::string fileName) {
    this->fileName = fileName;
    this->fileStream = std::ifstream(fileName, std::ios::binary);
    if (!this->fileStream.is_open()) {
        throw std::runtime_error("Couldn't open file " + fileName);
    }
    this->fileOpen = true;
    this->lineCount = 0;
}

Process::~Process() {
    this->fileOpen = false;
}

void Process::Exec() {
    std::string line;
    while(std::getline(this->fileStream, line)) {
        this->lineCount += 1;
        this->currentLineStream = std::istringstream(line);
        
        std::string inputAddress;
        this->currentLineStream >> inputAddress;
        uint32_t address = this->convertAddress(inputAddress);
        this->currentAddress = address;
        
        std::string command;
        this->currentLineStream >> command;
        
        if (command == "memsize") {
            this->currentCommand = Process::Command::MEMSIZE;
        } else if (command == "cmp") {
            this->currentCommand = Process::Command::CMP;
        } else if (command == "set") {
            this->currentCommand = Process::Command::SET;
        } else if (command == "fill") {
            this->currentCommand = Process::Command::FILL;
        } else if (command == "dup") {
            this->currentCommand = Process::Command::DUP;
        } else if (command == "print") {
            this->currentCommand = Process::Command::PRINT;
        } else {
            this->currentCommand = Process::Command::COMMENT;
        }
        
        std::vector<std::string> arguments;
        std::string argument;
        while((this->currentLineStream >> argument)) {
            arguments.push_back(argument);
        }
        
        std::cout << this->lineCount << ":" << line << std::endl;
        
        std::string result = this->handleCommand(this->currentCommand,
                                                 this->currentAddress,
                                                            arguments);
        
        if (result != "") {
            std::cout << result << std::endl;
        }
    }
    
    if (this->fileStream.bad()) {
        throw std::runtime_error("Error while reading file " + this->fileName);
    }
}

uint32_t Process::convertAddress(std::string arg) {
    return ((std::dec(std::stoi(arg)))/2);
}

std::string Process::handleCommand(Process::Command cmd, 
                                    uint32_t address, 
                                    std::vector<std::string> & arguments) {
    switch (cmd) {
        case Process::Command::MEMSIZE:
            uint32_t convertedSize = convertAddress(arguments.front());
            arguments.erase(0);
            if (convertedSize > 4000000) {
                convertedSize = 4000000;
            }
            this->memory = std::vector<uint8_t>(convertedSize-1);
            break;
        case Process::Command::CMP:
            uint32_t addr1 = convertAddress(arguments.front());
            arguments.erase(0);
            uint32_t addr2 = convertAddress(arguments.front());
            arguments.erase(0);
            
            //INCOMPLETE LOOKS LIKE BIT MANIPULATION, AND FUCK THAT FOR RIGHT NOW
            
            break;
        case Process::Command::PRINT:
            return "HEY GUYS I AM PRINTING SOMETHING JUST LIKE I WAS TOLD";
        default:
            return "";
    }
}