#####################################################################################
# 
# Filename    : OsSize.py
# 
# Author      : Chalandi Amine
#
# Owner       : Chalandi Amine
# 
# Date        : 14.10.2024
# 
# Description : OSEK code and data memory consumption calculation tool
# 
#####################################################################################

import re
import argparse

def format_size(size_in_bytes):
    """Convert bytes to a human-readable format (KB, MB, GB)."""
    if size_in_bytes >= 2**30:  # GB
        return f"{size_in_bytes / (2**30):.2f} GB"
    elif size_in_bytes >= 2**20:  # MB
        return f"{size_in_bytes / (2**20):.2f} MB"
    elif size_in_bytes >= 2**10:  # KB
        return f"{size_in_bytes / (2**10):.2f} KB"
    else:  # Bytes
        return f"{size_in_bytes} bytes"

def get_section_sizes(map_file_path, section_names):
    # Regex pattern to match the map file format
    section_pattern = re.compile(r'^\s*(?P<section>\.\w+)\s+(?P<address>0x[0-9a-fA-F]+)\s+(?P<size>0x[0-9a-fA-F]+)\s+(?P<file>.+)')
    obj_file_pattern = re.compile(r'Os.*\.o$')
    section_sizes = {section: 0 for section in section_names}

    with open(map_file_path, 'r') as file:
        for line in file:
            line = line.strip()  # Remove leading/trailing whitespace
            match = section_pattern.match(line)

            if match:
                section = match.group('section')
                size = int(match.group('size'), 16)
                obj_file = match.group('file').strip()  # Strip any leading/trailing whitespace
                
                # Check if the section and object file match
                if section in section_names and obj_file_pattern.search(obj_file):
                    section_sizes[section] += size

    return section_sizes

def main():
    parser = argparse.ArgumentParser(description='Parse a map file and calculate the sum of section sizes for specific object files.')
    parser.add_argument('map_file_path', type=str, help='Path to the map file')
    parser.add_argument('--sections', nargs='+', default=['.text', '.data', '.bss'], help='List of sections to sum (default: .text, .data, .bss)')
    args = parser.parse_args()

    sizes = get_section_sizes(args.map_file_path, args.sections)

    # Prepare the final output string
    final_output = "OSEK memory consumption (including generated OS objects): "
    size_strings = [f"{format_size(sizes[section])} ({section})" for section in args.sections]
    final_output += ", ".join(size_strings)

    print(final_output)

if __name__ == '__main__':
    main()
