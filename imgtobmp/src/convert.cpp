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

#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include "convert.h"

int convert_to_bmp(std::string file_name)
{
    std::string img_file_name = file_name;
    std::string bmp_file_name = file_name;
    std::filesystem::path img_file_path{img_file_name};

    if(!std::filesystem::exists(img_file_path))
    {
        std::cout << "Unable to find specified file, Please check file name and try again." << std::endl;

        return 1;
    }

    uint32_t img_length = std::filesystem::file_size(img_file_path);

    std::vector<std::byte> img_file_contents(img_length);

    std::ifstream img_file(img_file_name, std::ios_base::binary);

    img_file.read(reinterpret_cast<char*>(img_file_contents.data()), img_length);

    // Create BMP header

    std::vector<std::byte> bmp_file_contents(54);

    // Add BM signature

    bmp_file_contents[0] = std::byte{0x42};
    bmp_file_contents[1] = std::byte{0x4D};

    // Add file size

    bmp_file_contents[2] = std::byte{(uint8_t)(img_length >> 24)};
    bmp_file_contents[3] = std::byte{(uint8_t)(img_length >> 16)};
    bmp_file_contents[4] = std::byte{(uint8_t)(img_length >> 8)};
    bmp_file_contents[5] = std::byte{(uint8_t)(img_length >> 0)};

    // Add offset to the beginning of the bitmap data

    bmp_file_contents[10] = std::byte{0x36};

    // Add Size of info header

    bmp_file_contents[14] = std::byte{0x28};

    // Add width of bitmap

    bmp_file_contents[18] = std::byte{0x80};
    bmp_file_contents[19] = std::byte{0x02};

    // Add height of bitmap

    bmp_file_contents[22] = std::byte{0xE0};
    bmp_file_contents[23] = std::byte{0x01};

    // Add number of planes

    bmp_file_contents[26] = std::byte{0x01};

    // Add bits per pixel

    bmp_file_contents[28] = std::byte{0x10};

    // Flip IMG pixel data

    std::reverse(img_file_contents.begin(), img_file_contents.end());

    for(uint32_t n = 0; n < img_length; n += 1280)
    {
        std::reverse(img_file_contents.begin() + n, img_file_contents.begin() + n + 1280);
    }

    // Add IMG bitmap to header

    bmp_file_contents.insert(bmp_file_contents.end(), img_file_contents.begin(), img_file_contents.end());

    // Output .bmp file

    bmp_file_name.replace(bmp_file_name.end() - 3, bmp_file_name.end(), "bmp");

    std::ofstream bmp_file(bmp_file_name, std::ios_base::binary);

    bmp_file.write(reinterpret_cast<char*>(bmp_file_contents.data()), bmp_file_contents.size());

    img_file.close();

    return 0;
}