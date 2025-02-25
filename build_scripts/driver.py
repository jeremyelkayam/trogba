"""
Copyright (c) 2025 Jeremy Elkayam

based on python-script.py example from Butano game engine
"""

import os
import argparse
import achievements

def write_file(include_folder_path, include_filename, body, header=""):
    output_header_path = os.path.join(include_folder_path, include_filename)
    print('Output header file path: ' + output_header_path)



    with open(output_header_path, 'w') as output_header:
        output_header.write('#pragma once' + '\n')
        output_header.write('\n')
        output_header.write(header)
        output_header.write('namespace ext' + '\n')
        output_header.write('{' + '\n')

        for str in body.split('\n'):
            output_header.write('    ' + str + '\n')
            
        output_header.write('}' + '\n')

def process(build_folder_path):
    print('Build folder path: ' + build_folder_path)

    include_folder_path = os.path.join(build_folder_path, 'include')
    print('Include folder path: ' + include_folder_path)

    src_folder_path = os.path.join(build_folder_path, 'src')
    print('Source folder path: ' + src_folder_path)

    if not os.path.exists(include_folder_path):
        os.makedirs(include_folder_path)

    if not os.path.exists(src_folder_path):
        os.makedirs(src_folder_path)

    if(achievements.need_to_remake_achievement_data_files()):
        write_file(include_folder_path, 'achievement_rom_data.h', achievements.cpp())
    else:
        print("No updates to .csv files, continuing...")







if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='External tooling for TROGBA.')
    parser.add_argument('--build', required=True, help='build folder path')
    args = parser.parse_args()
    process(args.build)
