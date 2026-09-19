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

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <filesystem>
#include "extract.h"

std::byte modify_last_byte(std::byte byte_to_modify);

void extract_rdt(std::string file_name)
{
    std::string rdt_file_name = file_name;
    std::string rdi_file_name = "res.rdi";
    std::filesystem::path rdt_file_path{rdt_file_name};
    std::filesystem::path rdi_file_path{rdi_file_name};

    auto rdt_length = std::filesystem::file_size(rdt_file_path);
    auto rdi_length = std::filesystem::file_size(rdi_file_path);

    if (rdt_length == 0) {
        std::cout << "File size 0." << std::endl;
    }
    if (rdi_length == 0) {
        std::cout << "File size 0." << std::endl;
    }

    std::vector<std::byte> rdt_file_contents(rdt_length);
    std::vector<std::byte> rdi_file_contents(rdi_length);
    std::vector<std::byte> rda_string = {std::byte{0x52}, std::byte{0x44}, std::byte{0x41}, std::byte{0x32}};

    std::ifstream rdt_file(rdt_file_name, std::ios_base::binary);
    std::ifstream rdi_file(rdi_file_name, std::ios_base::binary);

    rdt_file.read(reinterpret_cast<char*>(rdt_file_contents.data()), rdt_length);
    rdi_file.read(reinterpret_cast<char*>(rdi_file_contents.data()), rdi_length);

    for(long unsigned int rdt_file_position = 0; rdt_file_position < rdt_length; rdt_file_position++)
    {
        if(rdt_file_contents[rdt_file_position] == rda_string[0])
        {
            if(rdt_file_contents[rdt_file_position + 1] == rda_string[1])
            {
                if(rdt_file_contents[rdt_file_position + 2] == rda_string[2])
                {
                    if(rdt_file_contents[rdt_file_position + 3] == rda_string[3])
                    {
                        long unsigned int rda_size = 0;
                        std::string out_file_name;

                        rda_size = (rda_size << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position - 13]);
                        rda_size = (rda_size << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position - 14]);
                        rda_size = (rda_size << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position - 15]);
                        rda_size = (rda_size << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position - 16]);

                        for(long unsigned int file_offset = 0; file_offset < rda_size - 16; file_offset += 4)
                        {
                            long unsigned int next_file_offset = 0;

                            next_file_offset = (next_file_offset << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position + 19 + file_offset]);
                            next_file_offset = (next_file_offset << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position + 18 + file_offset]);
                            next_file_offset = (next_file_offset << 8) + std::to_integer<int>(rdt_file_contents[rdt_file_position + 17 + file_offset]);
                            next_file_offset = (next_file_offset << 4) + std::to_integer<int>(modify_last_byte(rdt_file_contents[rdt_file_position + 16 + file_offset]));

                            if(next_file_offset != 1)
                            {
                                long unsigned int file_size = 0;
                                long unsigned int file_type_id = 0;
                                long unsigned int file_name_id_offset = 0;

                                file_size = (file_size << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 7]);
                                file_size = (file_size << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 6]);
                                file_size = (file_size << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 5]);
                                file_size = (file_size << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 4]);

                                if(file_size == 16)
                                {
                                    file_name_id_offset = (file_name_id_offset << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 23]);
                                    file_name_id_offset = (file_name_id_offset << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 22]);
                                    file_name_id_offset = (file_name_id_offset << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 21]);
                                    file_name_id_offset = (file_name_id_offset << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 20]);

                                    for(long unsigned int rdi_file_position = 69; rdi_file_position < 693959; rdi_file_position += 49)
                                    {
                                        if(rdi_file_contents[rdi_file_position + 1] == rdt_file_contents[next_file_offset + 21])
                                        {
                                            if(rdi_file_contents[rdi_file_position] == rdt_file_contents[next_file_offset + 20])
                                            {
                                                std::vector<std::byte> file_name_bytes(20);
                                                long unsigned int file_name_char_count = 0;

                                                for(long unsigned int file_name_counter = 0; file_name_counter < 20; file_name_counter++)
                                                {
                                                    file_name_bytes[file_name_counter] = rdi_file_contents[rdi_file_position + 8 + file_name_counter];

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
                                }

                                if(file_size > 16)
                                {
                                    file_type_id = (file_type_id << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 15]);
                                    file_type_id = (file_type_id << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 14]);
                                    file_type_id = (file_type_id << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 13]);
                                    file_type_id = (file_type_id << 8) + std::to_integer<int>(rdt_file_contents[next_file_offset + 12]);

                                    if(rdt_file_name == "res.rdt")
                                    {
                                        out_file_name.insert(0, "res_rdt/file_");
                                        std::filesystem::path extract_directory("res_rdt");

                                        if(std::filesystem::exists(extract_directory) == false)
                                        {
                                            std::filesystem::create_directory("res_rdt");
                                        }
                                    }
                                    else if(rdt_file_name == "disk1.rdt")
                                    {
                                        out_file_name = "disk1_rdt/file_";
                                        std::filesystem::path extract_directory("disk1_rdt");

                                        if(std::filesystem::exists(extract_directory) == false)
                                        {
                                            std::filesystem::create_directory("disk1_rdt");
                                        }
                                    }
                                    else if(rdt_file_name == "disk2.rdt")
                                    {
                                        out_file_name = "disk2_rdt/file_";
                                        std::filesystem::path extract_directory("disk2_rdt");

                                        if(std::filesystem::exists(extract_directory) == false)
                                        {
                                            std::filesystem::create_directory("disk2_rdt");
                                        }
                                    }
                                    else
                                    {
                                        std::string out_file_name = "res_rdt/file_";
                                        std::filesystem::path extract_directory("res_rdt");

                                        if(std::filesystem::exists(extract_directory) == false)
                                        {
                                            std::filesystem::create_directory("res_rdt");
                                        }
                                    }

                                    std::string file_name_dot = ".";
                                    std::string file_name_underscore = "_";
                                    std::string file_name_number;
                                    std::string file_extension;
                                    std::vector<std::byte> file_extract(file_size);

                                    for(long unsigned int file_extract_counter = 0; file_extract_counter < file_size; file_extract_counter++)
                                    {
                                        file_extract[file_extract_counter] = rdt_file_contents[next_file_offset + 20 + file_extract_counter];
                                    }

                                    for(long unsigned int rdi_file_position = 69; rdi_file_position < 693959; rdi_file_position += 49)
                                    {
                                        if(rdi_file_contents[rdi_file_position] == rdt_file_contents[next_file_offset + 14])
                                        {
                                            if(rdi_file_contents[rdi_file_position + 1] == rdt_file_contents[next_file_offset + 15])
                                            {
                                                std::vector<std::byte> file_extension_bytes(4);

                                                for(long unsigned int rdi_counter = 0; rdi_counter < 3; rdi_counter++)
                                                {
                                                    file_extension_bytes[rdi_counter] = rdi_file_contents[rdi_file_position + 4 + rdi_counter];
                                                }

                                                std::string file_extension_temp(file_extension_bytes.begin(), file_extension_bytes.end());

                                                file_extension = file_extension_temp;
                                            }
                                        }
                                    }

                                    out_file_name.append(file_name_underscore);

                                    file_name_number = std::to_string(file_offset);
                                    out_file_name.append(file_name_number);
                                    out_file_name.append(file_name_dot);
                                    out_file_name.append(file_extension);

                                    std::ofstream out_file(out_file_name, std::ios_base::binary);

                                    out_file.write(reinterpret_cast<char*>(file_extract.data()), file_extract.size());

                                    out_file_name.clear();
                                }
                            }
                        }

                        rda_size = 0;
                    }
                }
            }
        }
    }

    rdt_file.close();
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