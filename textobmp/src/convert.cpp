/*
    This file is part of textobmp.

    textobmp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    textobmp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with textobmp.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright (C) 2026 Peter Wright
// Author: Peter "apemax" Wright
// TEXtoBMP

#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include "convert.h"

int convert_to_bmp(std::string file_name)
{
    std::string tex_file_name = file_name;
    std::string pal_file_name = file_name;
    std::string bmp_file_name = file_name;
    std::string bmp_out_file_name = file_name;

    bmp_out_file_name.insert(0, "out_");

    pal_file_name.replace(pal_file_name.end() - 3, pal_file_name.end(), "pal");
    bmp_file_name.replace(bmp_file_name.end() - 3, bmp_file_name.end(), "bmp");
    bmp_out_file_name.replace(bmp_out_file_name.end() - 3, bmp_out_file_name.end(), "bmp");

    std::filesystem::path tex_file_path{tex_file_name};
    std::filesystem::path pal_file_path{pal_file_name};
    std::filesystem::path bmp_file_path{bmp_file_name};

    if(!std::filesystem::exists(tex_file_path))
    {
        std::cout << "Unable to find specified .tex file, Please check file name and try again." << std::endl;

        return 1;
    }

    if(!std::filesystem::exists(pal_file_path))
    {
        std::cout << "Unable to find specified .pal file, Please check file name and try again." << std::endl;

        return 1;
    }

    if(!std::filesystem::exists(bmp_file_path))
    {
        std::cout << "Unable to find specified .bmp file, Please check file name and try again." << std::endl;

        return 1;
    }

    uint32_t tex_length = std::filesystem::file_size(tex_file_path);
    uint32_t pal_length = std::filesystem::file_size(pal_file_path);
    uint32_t bmp_length = std::filesystem::file_size(bmp_file_path);

    std::vector<std::byte> tex_file_contents(tex_length);
    std::vector<std::byte> pal_file_contents(pal_length);
    std::vector<std::byte> bmp_file_contents(bmp_length);

    std::ifstream tex_file(tex_file_name, std::ios_base::binary);
    std::ifstream pal_file(pal_file_name, std::ios_base::binary);
    std::ifstream bmp_file(bmp_file_name, std::ios_base::binary);

    tex_file.read(reinterpret_cast<char*>(tex_file_contents.data()), tex_length);
    pal_file.read(reinterpret_cast<char*>(pal_file_contents.data()), pal_length);
    bmp_file.read(reinterpret_cast<char*>(bmp_file_contents.data()), bmp_length);

    //Add fourth null byte to each colour in palette

    for(uint32_t n = 7; n < pal_length; n += 4)
    {
        pal_file_contents.insert(pal_file_contents.begin() + n, std::byte{0x00});

        pal_length++;
    }

    // Create BMP header

    std::vector<std::byte> bmp_out_file_contents(54);

    // Add BM signature

    bmp_out_file_contents[0] = std::byte{0x42};
    bmp_out_file_contents[1] = std::byte{0x4D};

    // Add file size

    uint32_t bmp_file_length = (pal_length - 4) + (bmp_length - 12);

    bmp_out_file_contents[2] = std::byte{(uint8_t)(bmp_file_length >> 0)};
    bmp_out_file_contents[3] = std::byte{(uint8_t)(bmp_file_length >> 8)};
    bmp_out_file_contents[4] = std::byte{(uint8_t)(bmp_file_length >> 16)};
    bmp_out_file_contents[5] = std::byte{(uint8_t)(bmp_file_length >> 24)};

    // Add offset to the beginning of the bitmap data

    uint32_t offset_to_bitmap = pal_length + 50;

    bmp_out_file_contents[10] = std::byte{(uint8_t)(offset_to_bitmap >> 0)};
    bmp_out_file_contents[11] = std::byte{(uint8_t)(offset_to_bitmap >> 8)};
    bmp_out_file_contents[12] = std::byte{(uint8_t)(offset_to_bitmap >> 16)};
    bmp_out_file_contents[13] = std::byte{(uint8_t)(offset_to_bitmap >> 24)};

    // Add Size of info header

    bmp_out_file_contents[14] = std::byte{0x28};

    // Add width of bitmap

    bmp_out_file_contents[18] = tex_file_contents[0];
    bmp_out_file_contents[19] = tex_file_contents[1];

    // Add height of bitmap

    bmp_out_file_contents[22] = tex_file_contents[2];
    bmp_out_file_contents[23] = tex_file_contents[3];

    // Add number of planes

    bmp_out_file_contents[26] = std::byte{0x01};

    // Add bits per pixel

    bmp_out_file_contents[28] = std::byte{0x08};

    // Add PAL colour palette to header

    bmp_out_file_contents.insert(bmp_out_file_contents.end(), pal_file_contents.begin() + 4, pal_file_contents.end());

    // Add BMP bitmap data to header

    bmp_out_file_contents.insert(bmp_out_file_contents.end(), bmp_file_contents.begin() + 12, bmp_file_contents.end());

    // Output .bmp file

    std::ofstream bmp_out_file(bmp_out_file_name, std::ios_base::binary);

    bmp_out_file.write(reinterpret_cast<char*>(bmp_out_file_contents.data()), bmp_out_file_contents.size());

    tex_file.close();

    return 0;
}