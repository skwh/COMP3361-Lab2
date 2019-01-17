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

class Process {
public:
    Process(std::string fileName);
    Process(const Process& orig);
    virtual ~Process();
    
    void Exec();
private:
    std::string fileName;
    std::vector<uint8_t> memory;

};

#endif /* PROCESS_H */

