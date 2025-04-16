import numpy
import os
import sys

def main(argv):
    argc = len(argv)
    if (argc != 3):
        print("USAGE: generate_lookup_table [function to generate] [# of points to generate]")
        exit(1)
    if (not argv[2].isnumeric()):
        print("USAGE: generate_lookup_table [function to generate] [# of points to generate]")
        exit(1)
    if (argv[1] == 'sin'):
        generate_sin_table(int(argv[2]))
    if (argv[1] == 'cos'):
        generate_cos_table(int(argv[2]))


def generate_sin_table(number_points):
    print(f"Generating {number_points} number of points in the range [0, π/2]")
    sin_file = open("sin_table.cpp", 'w')
    sin_file.write(""
    "#include \"sin_table.h\"\n"
    "float sin_lookup_table[SIN_TABLE_VALUE_COUNT+1] = {")
    for i in range(number_points):
        val = numpy.sin(numpy.pi / 2 * i / number_points)
        sin_file.write(str(val))
        sin_file.write(", ")
    

    sin_file.write("};")
    sin_file.close()

    sin_file_h = open("sin_table.h", 'w')
    sin_file_h.write(""
    "#ifndef SIN_TABLE_H\n"
    "#define SIN_TABLE_H\n"
    f"#define SIN_TABLE_VALUE_COUNT {number_points}\n"
    "extern float sin_lookup_table[SIN_TABLE_VALUE_COUNT+1];\n"
    "#endif")
    sin_file_h.close()

def generate_cos_table(number_points):
    print(f"Generating {number_points} number of points in the range [0, π/2]")
    cos_file = open("cos_table.cpp", 'w')
    cos_file.write(""
    "#include \"cos_table.h\"\n"
    "float cos_lookup_table[COS_TABLE_VALUE_COUNT+1] = {")
    for i in range(number_points):
        val = numpy.cos(numpy.pi / 2 * i / number_points)
        cos_file.write(str(val))
        cos_file.write(", ")
    

    cos_file.write("};")
    cos_file.close()

    cos_file_h = open("cos_table.h", 'w')
    cos_file_h.write(""
    "#ifndef COS_TABLE_H\n"
    "#define COS_TABLE_H\n"
    f"#define COS_TABLE_VALUE_COUNT {number_points}\n"
    "extern float cos_lookup_table[COS_TABLE_VALUE_COUNT+1];\n"
    "#endif")
    cos_file_h.close()
if __name__ == '__main__':
    main(sys.argv)