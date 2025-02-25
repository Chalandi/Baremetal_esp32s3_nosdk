#####################################################################################
# 
# Filename    : CompilerErrorFormater.py
# 
# Author      : Chalandi Amine
#
# Owner       : Chalandi Amine
# 
# Date        : 28.04.2021
# 
# Description : Format the compiler error output into the Visual studio style
# 
#####################################################################################
import sys
import re
import os

os.environ['TERM'] = 'xterm-256color'  # Ensure ANSI colors work properly

# ANSI color code definition
class Color:
    BLACK     = '\033[30m'
    RED       = '\033[31m'
    GREEN     = '\033[32m'
    YELLOW    = '\033[33m'
    BLUE      = '\033[34m'
    MAGENTA   = '\033[35m'
    CYAN      = '\033[36m'
    WHITE     = '\033[37m'
    UNDERLINE = '\033[4m'
    RESET     = '\033[0m'

# Command-line syntax: python <script.py> <InputFile> <{-COLOR|-NOCOLOR}>
if len(sys.argv) != 3:
    print(Color.RED + "Usage: python <script.py> <InputFile> <{-COLOR|-NOCOLOR}>" + Color.RESET)
    sys.exit(1)

_, InputFile, ColorFlag = sys.argv

# Patterns to match
patterns = {
    ": warning:": Color.YELLOW,
    ": error:": Color.RED,
    ": fatal error:": Color.RED,
    ": note:": Color.BLUE,
    ": Warning:": Color.YELLOW,
    ": Error:": Color.RED,
    ": Note:": Color.BLUE,
}

def vs_format_msg(text, pattern, color):
    compiler_msg = text[text.find(pattern) + len(pattern):]
    file_path_match = re.search(r"\D.*\.\w", text)
    file_path = file_path_match.group(0) if file_path_match else ""
    line_number_match = re.search(r":(\d+):", text)
    line_number = f"({line_number_match.group(1)})" if line_number_match else ""
    
    vs_msg = f"{file_path}{line_number}{pattern}{compiler_msg}".strip()
    print((color + vs_msg + Color.RESET) if ColorFlag == "-COLOR" else vs_msg)

if not os.path.exists(InputFile):
    print(Color.RED + f"Error: The input file '{InputFile}' does not exist!" + Color.RESET)
    sys.exit(1)

with open(InputFile, 'r', encoding='utf-8') as err_file:
    for line in err_file:
        for pattern, color in patterns.items():
            if pattern in line:
                vs_format_msg(line, pattern, color)
