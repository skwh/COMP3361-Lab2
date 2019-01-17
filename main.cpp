/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: evan
 *
 * Created on January 16, 2019, 5:24 PM
 */

#include <cstdlib>
#include <iostream>
#include "Process.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    if (argc < 2 || argc >= 3) {
        std::cout << "Usage: " << argv[0] << " + Trace File Name" << std::endl;
        return 1;
    }
    std::string fileName(argv[1]);
    Process p(fileName);
    
    p.Exec();
    
    return 1;
}

