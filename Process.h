/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: evan
 *
 * Created on January 16, 2019, 5:25 PM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iomanip>

class Process {
public:
    Process(std::string fileName);
    
    Process(const Process & orig) = delete;
    Process(Process && orig) = delete;
    Process operator=(const Process & orig) = delete;
    Process operator=(Process && orig) = delete;
    
    virtual ~Process();
    
    void Exec();
private:
    
    enum Command {
        MEMSIZE, CMP, SET, FILL, DUP, PRINT, //COMMENT
    };
    
    std::string handleCommand(Command cmd, uint32_t address, std::vector<std::string> & args);
    uint32_t convertAddress(std::string arg);
    uint8_t getOddAddress(uint32_t addr);
    uint8_t getEvenAddress(uint32_t addr);
    void cmpHelp(uint32_t addr1, uint32_t addr2, int count);
    void setHelp(uint32_t addr, int val);
    std::vector<uint8_t> dupHelp(uint32_t srcAddr, int count);
    std::string printHelp(uint32_t addr);
    
    std::string fileName;
    std::vector<uint8_t> memory;
    std::ifstream fileStream;
    std::istringstream currentLineStream;
    bool fileOpen;
    int lineCount;
    
    Command currentCommand;
    uint32_t currentAddress;
    

};

#endif /* PROCESS_H */

