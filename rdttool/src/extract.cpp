/*
    This file is part of rdttool.

    rdttool is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    rdttool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with rdttool.  If not, see <http://www.gnu.org/licenses/>.
*/
// Copyright (C) 2026 Peter Wright
// Author: Peter "apemax" Wright
// rdttool

#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <filesystem>
#include "extract.h"

std::byte modify_last_byte(std::byte byte_to_modify);
std::string load_rdt_file_name(std::vector<std::byte> rdr_file_contents_input, long unsigned int rdt_file_name_offset);

void extract_rdt(std::string file_name)
{
    std::string rda_file_name = "res.rda";
    std::string rdr_file_name = file_name;
    std::string rdi_file_name = "res.rdi";
    std::string rdx_file_name = "res.rdx";
    std::filesystem::path rda_file_path{rda_file_name};
    std::filesystem::path rdr_file_path{rdr_file_name};
    std::filesystem::path rdi_file_path{rdi_file_name};
    std::filesystem::path rdx_file_path{rdx_file_name};

    auto rda_length = std::filesystem::file_size(rda_file_path);
    auto rdr_length = std::filesystem::file_size(rdr_file_path);
    auto rdi_length = std::filesystem::file_size(rdi_file_path);
    auto rdx_length = std::filesystem::file_size(rdx_file_path);

    if (rda_length == 0) {
        std::cout << "File size 0." << std::endl;
    }
    if (rdr_length == 0) {
        std::cout << "File size 0." << std::endl;
    }
    if (rdi_length == 0) {
        std::cout << "File size 0." << std::endl;
    }
    if (rdx_length == 0) {
        std::cout << "File size 0." << std::endl;
    }


    std::vector<std::byte> rda_file_contents(rda_length);
    std::vector<std::byte> rdr_file_contents(rdr_length);
    std::vector<std::byte> rdi_file_contents(rdi_length);
    std::vector<std::byte> rdx_file_contents(rdx_length);
    std::vector<std::byte> rda_string = {std::byte{0x52}, std::byte{0x44}, std::byte{0x41}, std::byte{0x32}};

    std::ifstream rda_file(rda_file_name, std::ios_base::binary);
    std::ifstream rdr_file(rdr_file_name, std::ios_base::binary);
    std::ifstream rdi_file(rdi_file_name, std::ios_base::binary);
    std::ifstream rdx_file(rdx_file_name, std::ios_base::binary);

    rda_file.read(reinterpret_cast<char*>(rda_file_contents.data()), rda_length);
    rdr_file.read(reinterpret_cast<char*>(rdr_file_contents.data()), rdr_length);
    rdi_file.read(reinterpret_cast<char*>(rdi_file_contents.data()), rdi_length);
    rdx_file.read(reinterpret_cast<char*>(rdx_file_contents.data()), rdx_length);

    long unsigned int rdr_file_name_count = 0;

    rdr_file_name_count = (rdr_file_name_count << 8) + std::to_integer<int>(rdr_file_contents[11]);
    rdr_file_name_count = (rdr_file_name_count << 8) + std::to_integer<int>(rdr_file_contents[10]);
    rdr_file_name_count = (rdr_file_name_count << 8) + std::to_integer<int>(rdr_file_contents[9]);
    rdr_file_name_count = (rdr_file_name_count << 8) + std::to_integer<int>(rdr_file_contents[8]);

    std::cout << "Archive count: " << std::dec << rdr_file_name_count << std::endl;

    //res.rdt
    std::string res_rdt_file_name = load_rdt_file_name(rdr_file_contents, 12);
    std::filesystem::path res_rdt_file_path{res_rdt_file_name};
    auto res_rdt_length = std::filesystem::file_size(res_rdt_file_path);

    if (res_rdt_length == 0) {
        std::cout << "File size 0." << std::endl;
    }

    std::vector<std::byte> res_rdt_file_contents(res_rdt_length);
    std::ifstream res_rdt_file(res_rdt_file_name, std::ios_base::binary);
    res_rdt_file.read(reinterpret_cast<char*>(res_rdt_file_contents.data()), res_rdt_length);

    //disk1.rdt
    std::string disk_one_rdt_file_name = load_rdt_file_name(rdr_file_contents, 76);
    std::filesystem::path disk_one_rdt_file_path{disk_one_rdt_file_name};
    auto disk_one_rdt_length = std::filesystem::file_size(disk_one_rdt_file_path);

    if (disk_one_rdt_length == 0) {
        std::cout << "File size 0." << std::endl;
    }

    std::vector<std::byte> disk_one_rdt_file_contents(disk_one_rdt_length);
    std::ifstream disk_one_rdt_file(disk_one_rdt_file_name, std::ios_base::binary);
    disk_one_rdt_file.read(reinterpret_cast<char*>(disk_one_rdt_file_contents.data()), disk_one_rdt_length);

    //disk2.rdt
    std::string disk_two_rdt_file_name = load_rdt_file_name(rdr_file_contents, 140);
    std::filesystem::path disk_two_rdt_file_path{disk_two_rdt_file_name};
    auto disk_two_rdt_length = std::filesystem::file_size(disk_two_rdt_file_path);

    if (disk_two_rdt_length == 0) {
        std::cout << "File size 0." << std::endl;
    }

    std::vector<std::byte> disk_two_rdt_file_contents(disk_two_rdt_length);
    std::ifstream disk_two_rdt_file(disk_two_rdt_file_name, std::ios_base::binary);
    disk_two_rdt_file.read(reinterpret_cast<char*>(disk_two_rdt_file_contents.data()), disk_two_rdt_length);

    std::string archive_name;

    long unsigned int rdi_file_name_counter = 0;
    long unsigned int rdx_file_position = 24;
    long unsigned int rda_file_position = 20;
    long unsigned int file_count = 0;
    long unsigned int rda_file_offset = 0;
    long unsigned int rdt_rda_section_offset = 0;
    //long unsigned int rda_size = 0;

    for(; rdx_file_position < rdx_length; rdx_file_position += 8)
    {
        long unsigned int rdt_section_offset_no_mod = 0;

        if(rdx_file_contents[rdx_file_position] == std::byte{0x03})
        {
            rdi_file_name_counter += 1;
            rda_file_offset += 4;
        }

        if(rdx_file_contents[rdx_file_position] == std::byte{0x11})
        {
            rdt_rda_section_offset = 0;
            rdt_rda_section_offset = (rdt_rda_section_offset << 8) + std::to_integer<int>(rda_file_contents[rda_file_position + 3]);
            rdt_rda_section_offset = (rdt_rda_section_offset << 8) + std::to_integer<int>(rda_file_contents[rda_file_position + 2]);
            rdt_rda_section_offset = (rdt_rda_section_offset << 8) + std::to_integer<int>(rda_file_contents[rda_file_position + 1]);
            rdt_rda_section_offset = (rdt_rda_section_offset << 4) + std::to_integer<int>(modify_last_byte(rda_file_contents[rda_file_position]));

            rdt_section_offset_no_mod = (rdt_section_offset_no_mod << 8) + std::to_integer<int>(rda_file_contents[rda_file_position + 3]);
            rdt_section_offset_no_mod = (rdt_section_offset_no_mod << 8) + std::to_integer<int>(rda_file_contents[rda_file_position + 2]);
            rdt_section_offset_no_mod = (rdt_section_offset_no_mod << 8) + std::to_integer<int>(rda_file_contents[rda_file_position + 1]);
            rdt_section_offset_no_mod = (rdt_section_offset_no_mod << 8) + std::to_integer<int>(rda_file_contents[rda_file_position]);

            rdi_file_name_counter += 1;

            if(rdt_section_offset_no_mod == 5)
            {
                archive_name = res_rdt_file_name;

                std::cout << "Extracting files from " << res_rdt_file_name << std::endl;
            }
            if(rdt_section_offset_no_mod == 9)
            {
                archive_name = disk_one_rdt_file_name;

                std::cout << "Extracting files from " << disk_one_rdt_file_name << std::endl;
            }
            if(rdt_section_offset_no_mod == 13)
            {
                archive_name = disk_two_rdt_file_name;

                std::cout << "Extracting files from " << disk_two_rdt_file_name << std::endl;
            }

            rda_file_offset += 4;
        }

        if(rdx_file_contents[rdx_file_position] == std::byte{0x08})
        {
            if(archive_name == "res.rdt")
            {
                std::string out_file_name;

                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 7]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 6]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 5]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 4]);
                    
                long unsigned int next_file_offset = 0;

                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 43 + rda_file_offset]);
                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 42 + rda_file_offset]);
                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 41 + rda_file_offset]);
                next_file_offset = (next_file_offset << 4) + std::to_integer<int>(modify_last_byte(res_rdt_file_contents[rdt_rda_section_offset + 40 + rda_file_offset]));

                if(next_file_offset != 1)
                {
                    long unsigned int file_size = 0;

                    rdi_file_name_counter += 1;

                    file_size = (file_size << 8) + std::to_integer<int>(res_rdt_file_contents[next_file_offset + 7]);
                    file_size = (file_size << 8) + std::to_integer<int>(res_rdt_file_contents[next_file_offset + 6]);
                    file_size = (file_size << 8) + std::to_integer<int>(res_rdt_file_contents[next_file_offset + 5]);
                    file_size = (file_size << 8) + std::to_integer<int>(res_rdt_file_contents[next_file_offset + 4]);

                    out_file_name.insert(0, "res_rdt/file_");
                    std::filesystem::path extract_directory("res_rdt");

                    if(std::filesystem::exists(extract_directory) == false)
                    {
                        std::filesystem::create_directory("res_rdt");
                    }

                    std::string file_name_dot = ".";
                    std::string file_name_underscore = "_";
                    std::string file_name_number;
                    std::string file_extension;
                    std::vector<std::byte> file_extract(file_size);

                    for(long unsigned int file_extract_counter = 0; file_extract_counter < file_size; file_extract_counter++)
                    {
                        file_extract[file_extract_counter] = res_rdt_file_contents[next_file_offset + 20 + file_extract_counter];
                    }

                    file_name_number = std::to_string(rda_file_offset);
                    out_file_name.append(file_name_number);
                    out_file_name.append(file_name_underscore);

                    for(long unsigned int rdi_file_name_position = 69; rdi_file_name_position < 693959; rdi_file_name_position += 49)
                    {
                        long unsigned int rdi_file_name_id = 0;

                        rdi_file_name_id = (rdi_file_name_id << 8) + std::to_integer<int>(rdi_file_contents[rdi_file_name_position + 1]);
                        rdi_file_name_id = (rdi_file_name_id << 8) + std::to_integer<int>(rdi_file_contents[rdi_file_name_position]);

                        if(rdi_file_name_id == rdi_file_name_counter)
                        {
                            if(rdi_file_contents[rdi_file_name_position + 8] != std::byte{0x00})
                            {
                                std::vector<std::byte> file_name_bytes(20);
                                long unsigned int file_name_char_count = 0;

                                for(long unsigned int file_name_counter = 0; file_name_counter < 20; file_name_counter++)
                                {
                                    file_name_bytes[file_name_counter] = rdi_file_contents[rdi_file_name_position + 8 + file_name_counter];

                                    if(file_name_bytes[file_name_counter] != std::byte{0x00})
                                    {
                                        file_name_char_count++;
                                    }
                                }

                                file_name_bytes.resize(file_name_char_count);

                                std::string out_file_name_temp(file_name_bytes.begin(), file_name_bytes.end());

                                out_file_name.append(out_file_name_temp);
                            }
                        }
                    }

                    for(long unsigned int rdi_file_ext_position = 69; rdi_file_ext_position < 693959; rdi_file_ext_position += 49)
                    {
                        if(rdi_file_contents[rdi_file_ext_position] == res_rdt_file_contents[next_file_offset + 14])
                        {
                             if(rdi_file_contents[rdi_file_ext_position + 1] == res_rdt_file_contents[next_file_offset + 15])
                            {
                                std::vector<std::byte> file_extension_bytes(4);

                                for(long unsigned int rdi_counter = 0; rdi_counter < 3; rdi_counter++)
                                {
                                    file_extension_bytes[rdi_counter] = rdi_file_contents[rdi_file_ext_position + 4 + rdi_counter];
                                }

                                std::string file_extension_temp(file_extension_bytes.begin(), file_extension_bytes.end());

                                file_extension = file_extension_temp;
                            }
                        }
                    }

                    out_file_name.append(file_name_dot);
                    out_file_name.append(file_extension);

                    std::ofstream out_file(out_file_name, std::ios_base::binary);

                    out_file.write(reinterpret_cast<char*>(file_extract.data()), file_extract.size());

                    out_file_name.clear();

                    file_count++;
                }
                //rda_size = 0;
            }
            if(archive_name == "disk1.rdt")
            {
                std::string out_file_name;

                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 7]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 6]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 5]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 4]);
                    
                long unsigned int next_file_offset = 0;

                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(disk_one_rdt_file_contents[rdt_rda_section_offset + 43 + rda_file_offset]);
                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(disk_one_rdt_file_contents[rdt_rda_section_offset + 42 + rda_file_offset]);
                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(disk_one_rdt_file_contents[rdt_rda_section_offset + 41 + rda_file_offset]);
                next_file_offset = (next_file_offset << 4) + std::to_integer<int>(modify_last_byte(disk_one_rdt_file_contents[rdt_rda_section_offset + 40 + rda_file_offset]));

                if(next_file_offset != 1)
                {
                    long unsigned int file_size = 0;

                    rdi_file_name_counter += 1;

                    file_size = (file_size << 8) + std::to_integer<int>(disk_one_rdt_file_contents[next_file_offset + 7]);
                    file_size = (file_size << 8) + std::to_integer<int>(disk_one_rdt_file_contents[next_file_offset + 6]);
                    file_size = (file_size << 8) + std::to_integer<int>(disk_one_rdt_file_contents[next_file_offset + 5]);
                    file_size = (file_size << 8) + std::to_integer<int>(disk_one_rdt_file_contents[next_file_offset + 4]);

                    out_file_name = "disk1_rdt/file_";
                    std::filesystem::path extract_directory("disk1_rdt");

                    if(std::filesystem::exists(extract_directory) == false)
                    {
                        std::filesystem::create_directory("disk1_rdt");
                    }

                    std::string file_name_dot = ".";
                    std::string file_name_underscore = "_";
                    std::string file_name_number;
                    std::string file_extension;
                    std::vector<std::byte> file_extract(file_size);

                    for(long unsigned int file_extract_counter = 0; file_extract_counter < file_size; file_extract_counter++)
                    {
                        file_extract[file_extract_counter] = disk_one_rdt_file_contents[next_file_offset + 20 + file_extract_counter];
                    }

                    file_name_number = std::to_string(rda_file_offset);
                    out_file_name.append(file_name_number);
                    out_file_name.append(file_name_underscore);

                    for(long unsigned int rdi_file_name_position = 69; rdi_file_name_position < 693959; rdi_file_name_position += 49)
                    {
                        long unsigned int rdi_file_name_id = 0;

                        rdi_file_name_id = (rdi_file_name_id << 8) + std::to_integer<int>(rdi_file_contents[rdi_file_name_position + 1]);
                        rdi_file_name_id = (rdi_file_name_id << 8) + std::to_integer<int>(rdi_file_contents[rdi_file_name_position]);

                        if(rdi_file_name_id == rdi_file_name_counter)
                        {
                            if(rdi_file_contents[rdi_file_name_position + 8] != std::byte{0x00})
                            {
                                std::vector<std::byte> file_name_bytes(20);
                                long unsigned int file_name_char_count = 0;

                                for(long unsigned int file_name_counter = 0; file_name_counter < 20; file_name_counter++)
                                {
                                    file_name_bytes[file_name_counter] = rdi_file_contents[rdi_file_name_position + 8 + file_name_counter];

                                    if(file_name_bytes[file_name_counter] != std::byte{0x00})
                                    {
                                        file_name_char_count++;
                                    }
                                }

                                file_name_bytes.resize(file_name_char_count);

                                std::string out_file_name_temp(file_name_bytes.begin(), file_name_bytes.end());

                                out_file_name.append(out_file_name_temp);
                            }
                        }
                    }

                    for(long unsigned int rdi_file_ext_position = 69; rdi_file_ext_position < 693959; rdi_file_ext_position += 49)
                    {
                        if(rdi_file_contents[rdi_file_ext_position] == disk_one_rdt_file_contents[next_file_offset + 14])
                        {
                             if(rdi_file_contents[rdi_file_ext_position + 1] == disk_one_rdt_file_contents[next_file_offset + 15])
                            {
                                std::vector<std::byte> file_extension_bytes(4);

                                for(long unsigned int rdi_counter = 0; rdi_counter < 3; rdi_counter++)
                                {
                                    file_extension_bytes[rdi_counter] = rdi_file_contents[rdi_file_ext_position + 4 + rdi_counter];
                                }

                                std::string file_extension_temp(file_extension_bytes.begin(), file_extension_bytes.end());

                                file_extension = file_extension_temp;
                            }
                        }
                    }

                    out_file_name.append(file_name_dot);
                    out_file_name.append(file_extension);

                    std::ofstream out_file(out_file_name, std::ios_base::binary);

                    out_file.write(reinterpret_cast<char*>(file_extract.data()), file_extract.size());

                    out_file_name.clear();

                    file_count++;
                }
                //rda_size = 0;
            }
            if(archive_name == "disk2.rdt")
            {
                std::string out_file_name;

                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 7]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 6]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 5]);
                //rda_size = (rda_size << 8) + std::to_integer<int>(res_rdt_file_contents[rdt_rda_section_offset + 4]);
                    
                long unsigned int next_file_offset = 0;

                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(disk_two_rdt_file_contents[rdt_rda_section_offset + 43 + rda_file_offset]);
                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(disk_two_rdt_file_contents[rdt_rda_section_offset + 42 + rda_file_offset]);
                next_file_offset = (next_file_offset << 8) + std::to_integer<int>(disk_two_rdt_file_contents[rdt_rda_section_offset + 41 + rda_file_offset]);
                next_file_offset = (next_file_offset << 4) + std::to_integer<int>(modify_last_byte(disk_two_rdt_file_contents[rdt_rda_section_offset + 40 + rda_file_offset]));

                if(next_file_offset != 1)
                {
                    long unsigned int file_size = 0;

                    rdi_file_name_counter += 1;

                    file_size = (file_size << 8) + std::to_integer<int>(disk_two_rdt_file_contents[next_file_offset + 7]);
                    file_size = (file_size << 8) + std::to_integer<int>(disk_two_rdt_file_contents[next_file_offset + 6]);
                    file_size = (file_size << 8) + std::to_integer<int>(disk_two_rdt_file_contents[next_file_offset + 5]);
                    file_size = (file_size << 8) + std::to_integer<int>(disk_two_rdt_file_contents[next_file_offset + 4]);

                    out_file_name = "disk2_rdt/file_";
                    std::filesystem::path extract_directory("disk2_rdt");

                    if(std::filesystem::exists(extract_directory) == false)
                    {
                        std::filesystem::create_directory("disk2_rdt");
                    }

                    std::string file_name_dot = ".";
                    std::string file_name_underscore = "_";
                    std::string file_name_number;
                    std::string file_extension;
                    std::vector<std::byte> file_extract(file_size);

                    for(long unsigned int file_extract_counter = 0; file_extract_counter < file_size; file_extract_counter++)
                    {
                        file_extract[file_extract_counter] = disk_two_rdt_file_contents[next_file_offset + 20 + file_extract_counter];
                    }

                    file_name_number = std::to_string(rda_file_offset);
                    out_file_name.append(file_name_number);
                    out_file_name.append(file_name_underscore);

                    for(long unsigned int rdi_file_name_position = 69; rdi_file_name_position < 693959; rdi_file_name_position += 49)
                    {
                        long unsigned int rdi_file_name_id = 0;

                        rdi_file_name_id = (rdi_file_name_id << 8) + std::to_integer<int>(rdi_file_contents[rdi_file_name_position + 1]);
                        rdi_file_name_id = (rdi_file_name_id << 8) + std::to_integer<int>(rdi_file_contents[rdi_file_name_position]);

                        if(rdi_file_name_id == rdi_file_name_counter)
                        {
                            if(rdi_file_contents[rdi_file_name_position + 8] != std::byte{0x00})
                            {
                                std::vector<std::byte> file_name_bytes(20);
                                long unsigned int file_name_char_count = 0;

                                for(long unsigned int file_name_counter = 0; file_name_counter < 20; file_name_counter++)
                                {
                                    file_name_bytes[file_name_counter] = rdi_file_contents[rdi_file_name_position + 8 + file_name_counter];

                                    if(file_name_bytes[file_name_counter] != std::byte{0x00})
                                    {
                                        file_name_char_count++;
                                    }
                                }

                                file_name_bytes.resize(file_name_char_count);

                                std::string out_file_name_temp(file_name_bytes.begin(), file_name_bytes.end());

                                out_file_name.append(out_file_name_temp);
                            }
                        }
                    }

                    for(long unsigned int rdi_file_ext_position = 69; rdi_file_ext_position < 693959; rdi_file_ext_position += 49)
                    {
                        if(rdi_file_contents[rdi_file_ext_position] == disk_two_rdt_file_contents[next_file_offset + 14])
                        {
                             if(rdi_file_contents[rdi_file_ext_position + 1] == disk_two_rdt_file_contents[next_file_offset + 15])
                            {
                                std::vector<std::byte> file_extension_bytes(4);

                                for(long unsigned int rdi_counter = 0; rdi_counter < 3; rdi_counter++)
                                {
                                    file_extension_bytes[rdi_counter] = rdi_file_contents[rdi_file_ext_position + 4 + rdi_counter];
                                }

                                std::string file_extension_temp(file_extension_bytes.begin(), file_extension_bytes.end());

                                file_extension = file_extension_temp;
                            }
                        }
                    }

                    out_file_name.append(file_name_dot);
                    out_file_name.append(file_extension);

                    std::ofstream out_file(out_file_name, std::ios_base::binary);

                    out_file.write(reinterpret_cast<char*>(file_extract.data()), file_extract.size());

                    out_file_name.clear();

                    file_count++;
                }
                //rda_size = 0;
            }

            rda_file_offset += 4;
        }

        rda_file_position += 4;
    }

    std::cout << "Number of files: " << std::dec << file_count << std::endl;

    rda_file.close();
    rdr_file.close();
    rdi_file.close();

    res_rdt_file.close();
    disk_one_rdt_file.close();
    disk_two_rdt_file.close();
}

std::string load_rdt_file_name(std::vector<std::byte> rdr_file_contents_input, long unsigned int rdt_file_name_offset)
{
    std::vector<std::byte> rdt_file_name_bytes(20);
    long unsigned int file_name_char_count = 0;

    for(long unsigned int file_name_counter = 0; file_name_counter < 20; file_name_counter++)
    {
        rdt_file_name_bytes[file_name_counter] = rdr_file_contents_input[rdt_file_name_offset + file_name_counter];

        if(rdt_file_name_bytes[file_name_counter] != std::byte{0x00})
        {
            file_name_char_count++;
        }
    }

    rdt_file_name_bytes.resize(file_name_char_count);

    std::string rdt_file_name_temp(rdt_file_name_bytes.begin(), rdt_file_name_bytes.end());

    std::transform(rdt_file_name_temp.begin(), rdt_file_name_temp.end(), rdt_file_name_temp.begin(), ::tolower);

    return rdt_file_name_temp;
}

std::byte modify_last_byte(std::byte byte_to_modify)
{
    if(byte_to_modify == std::byte{0x05} || byte_to_modify == std::byte{0x09} || byte_to_modify == std::byte{0x0d})
    {
        return std::byte{0x0};
    }
    else if(byte_to_modify == std::byte{0x15} || byte_to_modify == std::byte{0x19} || byte_to_modify == std::byte{0x1d})
    {
        return std::byte{0x1};
    }
    else if(byte_to_modify == std::byte{0x25} || byte_to_modify == std::byte{0x29} || byte_to_modify == std::byte{0x2d})
    {
        return std::byte{0x2};
    }
    else if(byte_to_modify == std::byte{0x35} || byte_to_modify == std::byte{0x39} || byte_to_modify == std::byte{0x3d})
    {
        return std::byte{0x3};
    }
    else if(byte_to_modify == std::byte{0x45} || byte_to_modify == std::byte{0x49} || byte_to_modify == std::byte{0x4d})
    {
        return std::byte{0x4};
    }
    else if(byte_to_modify == std::byte{0x55} || byte_to_modify == std::byte{0x59} || byte_to_modify == std::byte{0x5d})
    {
        return std::byte{0x5};
    }
    else if(byte_to_modify == std::byte{0x65} || byte_to_modify == std::byte{0x69} || byte_to_modify == std::byte{0x6d})
    {
        return std::byte{0x6};
    }
    else if(byte_to_modify == std::byte{0x75} || byte_to_modify == std::byte{0x79} || byte_to_modify == std::byte{0x7d})
    {
        return std::byte{0x7};
    }
    else if(byte_to_modify == std::byte{0x85} || byte_to_modify == std::byte{0x89} || byte_to_modify == std::byte{0x8d})
    {
        return std::byte{0x8};
    }
    else if(byte_to_modify == std::byte{0x95} || byte_to_modify == std::byte{0x99} || byte_to_modify == std::byte{0x9d})
    {
        return std::byte{0x9};
    }
    else if(byte_to_modify == std::byte{0xa5} || byte_to_modify == std::byte{0xa9} || byte_to_modify == std::byte{0xad})
    {
        return std::byte{0xa};
    }
    else if(byte_to_modify == std::byte{0xb5} || byte_to_modify == std::byte{0xb9} || byte_to_modify == std::byte{0xbd})
    {
        return std::byte{0xb};
    }
    else if(byte_to_modify == std::byte{0xc5} || byte_to_modify == std::byte{0xc9} || byte_to_modify == std::byte{0xcd})
    {
        return std::byte{0xc};
    }
    else if(byte_to_modify == std::byte{0xd5} || byte_to_modify == std::byte{0xd9} || byte_to_modify == std::byte{0xdd})
    {
        return std::byte{0xd};
    }
    else if(byte_to_modify == std::byte{0xe5} || byte_to_modify == std::byte{0xe9} || byte_to_modify == std::byte{0xed})
    {
        return std::byte{0xe};
    }
    else if(byte_to_modify == std::byte{0xf5} || byte_to_modify == std::byte{0xf9} || byte_to_modify == std::byte{0xfd})
    {
        return std::byte{0xf};
    }
    else
    {
        return std::byte{0x1};
    }
}