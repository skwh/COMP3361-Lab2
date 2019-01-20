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
        MEMSIZE, CMP, SET, FILL, DUP, PRINT, COMMENT
    };
    
    std::string handleCommand(Command cmd, uint32_t address, std::vector<std::string> & args);
    int convertAddress(std::string arg);
    uint8_t getOddAddress(int addr);
    uint8_t getEvenAddress(int addr);
    void cmpHelp(int addr1, int addr2, int count);
    
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

