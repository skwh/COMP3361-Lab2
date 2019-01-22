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
#include <iomanip>

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
    this->fileStream.close();
    this->fileOpen = false;
}

void Process::Exec() {
    std::string line;
    while(std::getline(this->fileStream, line)) {
        this->lineCount += 1;
        this->currentLineStream = std::istringstream(line);
        
        std::string inputAddress;
        this->currentLineStream >> inputAddress;
        uint32_t address = 0;
        try {
            address = this->convertAddress(inputAddress);
        } catch (const std::invalid_argument & e) {
            // the address couldn't be converted, probably because the input
            // is a star denoting a comment, so skip command processing
            // and just print out the line
            std::cout << this->lineCount << ":" << line << std::endl;
            continue;
        }
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
            this->currentCommand = Process::Command::UNKNOWN;
            std::cerr << "Couldn't process command in line: " << line << std::endl;
            continue;
        }
        
        std::vector<std::string> arguments;
        std::string argument;
        while((this->currentLineStream >> argument)) {
            arguments.push_back(argument);
        }
        
        std::cout << this->lineCount << ":" << line << std::endl;
        
        this->handleCommand(this->currentCommand, this->currentAddress, arguments);
    }
    
    if (this->fileStream.bad()) {
        throw std::runtime_error("Error while reading file " + this->fileName);
    }
}

uint32_t Process::convertAddress(std::string arg) const {
    return std::stoul(arg, nullptr, 16);
}

uint8_t Process::getEvenAddress(uint32_t addr) const {
    uint8_t byte = this->memory[addr];
    return (byte & 240);
}

uint8_t Process::getOddAddress(uint32_t addr) const {
    uint8_t byte = this->memory[addr];
    return (byte & 15);
}

void Process::cmpHelp(uint32_t addr1, uint32_t addr2, int count) const {
    int addr1CountVal = 0;
    int addr2CountVal = 0;
    for (int i = addr1, j = addr2;
            i <= (addr1 + count), j <= (addr2 + count);    // Might need to do count-1 here, testing will tell
            i++, j++) {
        uint8_t addr1CurrVal = 0;
        uint8_t addr2CurrVal = 0;
        if ((i%2) == 0) {
            addr1CurrVal = this->getEvenAddress(i);
            addr1CountVal += addr1CurrVal;
        } else {
            addr1CurrVal = this->getOddAddress(i);
            addr1CountVal += addr1CurrVal;
        }
        if ((i%2) == 0) {
            addr2CurrVal = this->getEvenAddress(j);
            addr2CountVal += addr2CurrVal;
        } else {
            addr2CurrVal = this->getOddAddress(j);
            addr2CountVal += addr2CurrVal;
        }

        if ((addr1CurrVal & addr2CurrVal) != addr1CurrVal) {
            std::cerr << "cmp error, addr1 = " << std::hex << i << ", value = " 
                    << addr1CurrVal << ", addr2 = " << j << ", value = " 
                    << addr2CurrVal << std::endl;
        }
    }
}

void Process::setHelp(uint32_t addr, int val) {
    uint8_t holder = this->memory[addr];
    if ((addr % 2) == 0) {
        holder = (holder & 15);
        holder = (val | holder);
    } else {
        holder = (holder & 240);
        holder = (val | holder);
    }
    this->memory[addr] = holder;
}

std::vector<uint8_t> Process::dupHelp(uint32_t srcAddr, int count) const {
    std::vector<uint8_t> vals;
    
    for (int i = srcAddr; i <= (srcAddr + count); i++) {
        if ((i % 2) == 0) {
            vals.push_back(this->getEvenAddress(i));
        } else {
            vals.push_back(this->getOddAddress(i));
        }
    }
    return vals;
}

std::string Process::printHelp(uint32_t addr) const {
    if ((addr % 2) == 0) {
        return std::to_string(this->getEvenAddress(addr));
    } else {
        return std::to_string(this->getOddAddress(addr));
    }
}

void Process::handleCommand(Process::Command cmd, 
                                    uint32_t address, 
                                    std::vector<std::string> & arguments) {
    switch (cmd) {
        case Process::Command::MEMSIZE: {
            if (address > 4000000) {
                address = 4000000;
            }
            this->memory = std::vector<uint8_t>(address-1);
            break; 
        }
        case Process::Command::CMP: {
            uint32_t addr2 = this->convertAddress(arguments.at(0));
            int count = this->convertAddress(arguments.at(1));
            
            this->cmpHelp(address, addr2, count);
            break; 
        }
        case Process::Command::SET: {
            int count = 0;
            while (!arguments.empty()) {
                uint8_t val = this->convertAddress(arguments.at(0));
                arguments.erase(arguments.begin()); //.erase takes an iterator,
                                                    // not an index integer
                this->setHelp((address + count), val);
                count++;
            }
            break; 
        }
        case Process::Command::FILL: {
            uint8_t val = this->convertAddress(arguments.at(0));
            uint32_t count = this->convertAddress(arguments.at(1));
            
            for (int i = address; i <= address + count; i++) {
                this->setHelp(i, val);
            }
            break; 
        }
        case Process::Command::DUP: {
            uint32_t destAddr = this->convertAddress(arguments.at(0));
            uint32_t count = this->convertAddress(arguments.at(1));
            
            std::vector<uint8_t> vals = this->dupHelp(address, count);
            
            for (int i = destAddr; i <= (destAddr + count); i++) {
                this->setHelp(i, vals.front());
                vals.erase(vals.begin());
            }
            break; 
        }
        case Process::Command::PRINT: {
            uint32_t count = this->convertAddress(arguments.at(0));
            
            for (int i = address; i <= address + count; i += 16) {
                std::ostringstream line;
                line << std::setfill('0') << std::setw(7) << std::hex << i << ": ";
                
                for (int j = i; j <= ((i + 15) || (address + count)); j++) {
                    line << std::setfill('0') << std::setw(2) << std::hex << this->printHelp(j) << " ";
                }
                std::cout << line.str() << std::endl;
            }
            break; 
        }
        default:
            return;
    }
}