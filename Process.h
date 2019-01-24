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

using address = uint32_t;
using byte = uint8_t;

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
        MEMSIZE, CMP, SET, FILL, DUP, PRINT, UNKNOWN // include catch-all
    };
    
    void handleCommand(Command cmd, address addr, std::vector<std::string> & args);
    address stringToAddress(std::string val) const;
    byte stringToByte(std::string val) const;
    
    void memsize(address memsize);
    void cmp(address addr1, address addr2, int count) const;
    void set(address addr, std::vector<byte> vals);
    void fill(address addr, byte val, int count);
    void dup(address srcAddr, address destAddr, int count);
    void print(address addr, int count) const;
    
    std::string fileName;
    std::ifstream fileStream;
    std::istringstream currentLineStream;
    
    std::vector<byte> memory;
    bool fileOpen;
    int lineCount;
    
    Command currentCommand;
    address currentAddress;
    

};

#endif /* PROCESS_H */

