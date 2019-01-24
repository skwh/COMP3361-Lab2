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
            address = this->stringToAddress(inputAddress);
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
        
        std::cout << std::dec << this->lineCount << ":" << line << std::endl;
        
        this->handleCommand(this->currentCommand, this->currentAddress, arguments);
    }
    
    if (this->fileStream.bad()) {
        throw std::runtime_error("Error while reading file " + this->fileName);
    }
}

address Process::stringToAddress(std::string val) const {
    return std::stoul(val, nullptr, 16);
}

byte Process::stringToByte(std::string val) const {
    return std::stoul(val, nullptr, 16);
}

void Process::handleCommand(Process::Command cmd, 
                                    address addr, 
                                    std::vector<std::string> & arguments) {
    switch (cmd) {
        case Process::Command::MEMSIZE: {
            this->memsize(addr);
            break; 
        }
        case Process::Command::CMP: {
            if (arguments.size() != 2) {
                std::cerr << "cmp did not have the correct number of args" << std::endl;
                return;
            }
            address addr2 = this->stringToAddress(arguments.at(0));
            int count = this->stringToAddress(arguments.at(1));
            
            this->cmp(addr, addr2, count);
            break; 
        }
        case Process::Command::SET: {
            std::vector<byte> byteValues = std::vector<byte>(arguments.size());
            for (int i = 0; i < arguments.size(); i++) {
                byteValues[i] = this->stringToByte(arguments.at(i));
            }
            this->set(addr, byteValues);
            break; 
        }
        case Process::Command::FILL: {
            if (arguments.size() != 2) {
                std::cerr << "fill did not have the correct number of args" << std::endl;
                return;
            }
            byte val = this->stringToByte(arguments.at(0));
            int count = this->stringToAddress(arguments.at(1));
            this->fill(addr, val, count);
            break; 
        }
        case Process::Command::DUP: {
            if (arguments.size() != 2) {
                std::cerr << "dup did not have the correct number of args" << std::endl;
                return;
            }
            address addr2 = this->stringToAddress(arguments.at(0));
            int count = this->stringToAddress(arguments.at(1));
            this->dup(addr, addr2, count);
            break; 
        }
        case Process::Command::PRINT: {
            if (arguments.size() != 1) {
                std::cerr << "print did not have the correct number of args" << std::endl;
            }
            int count = this->stringToAddress(arguments.at(0));
            this->print(addr, count);
            break; 
        }
        default:
            return;
    }
}

void Process::memsize(address memsize) {
    if (memsize > 4000000) {
        memsize = 4000000;
    }
    this->memory = std::vector<byte>(memsize);
}

void Process::cmp(address addr1, address addr2, int count) const {
    for (int i = 0; i < count; i++) {
        if (this->memory[addr1+i] != this->memory[addr2+i]) {
            std::cerr << "cmp error, addr1 = " << std::setfill('0') << std::setw(7) << std::hex << addr1+i 
                    << ", value = " << std::setfill('0') << std::setw(2) << static_cast<int>(this->memory[addr1+i]) 
                    << ", addr2 = " << std::setfill('0') << std::setw(7) << addr2+i 
                    << ", value = " << std::setfill('0') << std::setw(2) << static_cast<int>(this->memory[addr2+i]) 
                    << std::endl;
        }
    }
}

void Process::set(address addr, std::vector<byte> vals) {
    for (std::vector<byte>::iterator it = vals.begin() ; it != vals.end(); ++it) {
        this->memory[addr] = *it;
        addr++;
    }
}

void Process::fill(address addr, byte val, int count) {
    for (int i = 0; i < count; i++ ) {
        this->memory[addr+i] = val;
    }
}

void Process::dup(address srcAddr, address destAddr, int count) {
    for (int i = 0; i < count; i++ ) {
        this->memory[destAddr+i] = this->memory[srcAddr+i];
    }
}

void Process::print(address addr, int count) const {
    int byteCounter = 0;
    for (int i = 0; i < count; i++) {
        if (i % 16 == 0) {
            std::cout << std::setfill('0') << std::setw(7) << std::hex << addr+i << ": "; 
        }
        std::cout << std::setfill('0') << std::setw(2) << std::hex 
                << static_cast<int>(this->memory[addr+i]) << " ";
        byteCounter++;
        if (byteCounter == 16) {
            std::cout << std::dec << std::endl;
            byteCounter = 0;
        }
    }
    if (byteCounter != 0) {
        std::cout << std::dec << std::endl;
    }
    
}