/*
    This file is part of imgtobmp.

    imgtobmp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    imgtobmp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with imgtobmp.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright (C) 2026 Peter Wright
// Author: Peter "apemax" Wright
// imgtobmp

#include "convert.h"

int main(int argc, char const *argv[])
{
    const std::string version = "0.1.0";

    if(argc <= 1)
    {
        std::cout << "No options specified, Type -h for usage information." << std::endl;
    }
    else
    {
        std::string first_option = argv[1];
        if(first_option == "-h")
        {
        std::cout << "Usage:" << std::endl;
        std::cout << "imgtobmp filename" << std::endl;
        std::cout << "imgtobmp [-hv]" << std::endl;
        std::cout << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "-v   Display the version number of imgtobmp." << std::endl;
        std::cout << "-h   Display this help." << std::endl;
        std::cout << std::endl;
        std::cout << "Example:" << std::endl;
        std::cout << "Convert .img file to .bmp file:" << std::endl;
        std::cout << "imgtobmp filename.img" << std::endl;
        std::cout << "Output the version of imgtobmp:" << std::endl;
        std::cout << "imgtobmp -v" << std::endl;
        }
        else if(first_option == "-v")
        {
        std::cout << "IMGtoBMP " << version << std::endl;
        }
        else
        {
        convert_to_bmp(argv[1]);
        }
    }
    
    return 0;
}