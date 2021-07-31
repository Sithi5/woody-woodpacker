import argparse
import subprocess
import platform
import pathlib
import re

ACCEPTED_SYSTEM = ["Linux"]
ACCEPTED_ARCHITECTURE = ["64bit"]
ACCEPTED_FILE_EXTENSION = ["asm", "s"]
HEXA_CHAR = "abcdef0123456789"


def error(error_message: str = None):
    print(error_message)
    exit()


def convert_line_to_shellcode(line: str) -> str:
    shell_code = []
    while len(line) > 1:
        if line[0] == " " and line[1] != " ":
            line = line[1:]
        elif line[0] in HEXA_CHAR and line[1] in HEXA_CHAR:
            shell_code.append("\\x" + line[0] + line[1])
            line = line[2:]
        else:
            break
    return "".join(shell_code)


def check_system():
    if platform.system() not in "".join(ACCEPTED_SYSTEM) or platform.architecture()[
        0
    ] not in "".join(ACCEPTED_ARCHITECTURE):
        error_message = (
            "This script need to be run with a "
            + "/".join(ACCEPTED_SYSTEM)
            + " in a "
            + "/".join(ACCEPTED_ARCHITECTURE)
            + " architecture to work properly."
        )
        raise SyntaxError(error_message)


def check_file_extension(file_extension: str):
    if file_extension not in ACCEPTED_FILE_EXTENSION:
        error_message = (
            "Only " + "/".join(ACCEPTED_FILE_EXTENSION) + " files extensions are accepted."
        )
        raise SyntaxError(error_message)


def main(argv=None):
    try:
        check_system()
        parser = argparse.ArgumentParser()
        parser.add_argument("asm_file_name")
        args = parser.parse_args(argv)
        file_name = args.asm_file_name
        file_extension = pathlib.Path(file_name).suffix
        file_base_name = file_name[: -len(file_extension)]
        # Removing the dot for file_extension.
        file_extension = file_extension[1:]
        check_file_extension(file_extension=file_extension)
        output = subprocess.getoutput("nasm -f elf64 " + file_name)
        if len(output) > 0:
            error_message = "An error occured with nasm: " + output
            raise SyntaxError(error_message)
        output = subprocess.getoutput("objdump -d " + file_base_name + ".o")
        print(output)
        print()
        output = output.split("\n")
        shell_code = []
        pattern_match_code_line = r"[\da-f]+:"
        for line in output:
            if re.search(pattern_match_code_line, line):
                # Remove left part before ':' char
                line = line.split(":")[1]
                # Removing left part spaces
                line = re.sub(r"^[ \t]+", "", line)
                shell_code.append(convert_line_to_shellcode(line=line))
        shell_code_inline = "".join(shell_code)
        shell_code_len = len(shell_code_inline)
        print("shell_code : \n", shell_code_inline)
        print("\nshell_code_len : \n", shell_code_len / 4)
    except SyntaxError as e:
        print("An error occured : ", str(e.msg))


if __name__ == "__main__":
    main()
