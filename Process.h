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

/*
 * Process: Class that reads in input from a file with a given name and handles
 * commands read in from said file.
 */
class Process {
public:
    /** 
     * Process: parses fileName to a fileStream object and opens the file
     * 
     * @param fileName: returns the name of the input file
     * @throws runtime_error if file failed to open
     */
    Process(std::string fileName);
    
    /**
     * Copy constructor, assignment constructor, move constructor and 
     * move assignment constructor all deleted as specified.
     */
    Process(const Process & orig) = delete;
    Process(Process && orig) = delete;
    Process operator=(const Process & orig) = delete;
    Process operator=(Process && orig) = delete;
    
    /**
     * ~Process: Closes fileStream and sets fileOpen to false
     */
    virtual ~Process();
    
    /**
     * Parses lines from the file into an istringstream, then breaks lines
     * into addresses, commands and arguments and passes them to the 
     * handleCommand function.
     * Prints each line after parsing.
     */
    void Exec();
private:
    /**
     * Enumeration for the various command types
     */
    enum Command {
        MEMSIZE, CMP, SET, FILL, DUP, PRINT, UNKNOWN // include catch-all
    };
    
    /**
     * handleCommand: depending on command given, computes desired command
     * @param cmd: returns the command type
     * @param addr: returns the address and/or first argument of the command
     * @param args: returns the list of arguments of the command
     */
    void handleCommand(Command cmd, address addr, std::vector<std::string> & args);
    /**
     * stringToAddress: converts a string to a uint32_t address
     * @param val: string value to be converted
     * @return the converted uint32_t address
     */
    address stringToAddress(std::string val) const;
    /**
     * stringToByte: converts a string to a uint8_t byte
     * @param val: string value to be converted
     * @return the converted uint8_t byte
     */
    byte stringToByte(std::string val) const;
    
    /**
     * memsize: sets the size of the memory vector to memsize
     * @param memsize: value to set the size of the memory vector 
     */
    void memsize(address memsize);
    /**
     * cmp: Compares count bytes after addr1 to count bytes after addr2
     * @param addr1: First address to compare
     * @param addr2: Second address to compare
     * @param count: Number of bytes after addresses to compare
     */
    void cmp(address addr1, address addr2, int count) const;
    /**
     * set: sets bytes starting at addr to vals
     * @param addr: address at which to begin setting bytes
     * @param vals: values to set bytes to
     */
    void set(address addr, std::vector<byte> vals);
    /**
     * fill: fills count bytes starting at addr with the value val
     * @param addr: address at which to begin filling bytes
     * @param val: value to set bytes to
     * @param count: number of bytes after address to fill
     */
    void fill(address addr, byte val, int count);
    /**
     * dup: duplicates count bytes from srcAddr and pastes them at destAddr
     * @param srcAddr: source address to duplicate bytes
     * @param destAddr: destination address to paste bytes
     * @param count: number of bytes to duplicate
     */
    void dup(address srcAddr, address destAddr, int count);
    /**
     * print: prints count bytes from addr
     * @param addr: address to print from
     * @param count: number of bytes to print
     */
    void print(address addr, int count) const;
    
    /*
     * fileName: name of input file
     * fileStream: stream through which file lines are parsed
     * currentLineStream: stream containing current input line
     */
    std::string fileName;
    std::ifstream fileStream;
    std::istringstream currentLineStream;
    
    /*
     * memory: vector of uint8_t bytes simulating memory
     * fileOpen: boolean stating open status of file
     * lineCount: integer storing the current line count for output format
     */
    std::vector<byte> memory;
    bool fileOpen;
    int lineCount;
    
    /*
     * currentCommand: stores current command from current line
     * currentAddress: stores current address from current line
     */
    Command currentCommand;
    address currentAddress;
    

};

#endif /* PROCESS_H */

