import json
import sys
import os


if len(sys.argv) != 2:
    print("Usage: python3 code_generator.py <json_file>")
    sys.exit(1)

# read in json file
with open(os.path.dirname(os.path.realpath(__file__)) +'/instrumented_intrinsics.json') as json_file:
    data = json.load(json_file)


result_string="""
#ifndef __INSTRUMENTED_INTRINSICS_H__
#define __INSTRUMENTED_INTRINSICS_H__


#ifndef INSTRUMENTED
#include <immintrin.h>
#else
#include <instrumented_index.h>

namespace real_intrinsics {
    #include <immintrin.h>
}

namespace mocked_intrinsics {

typedef real_intrinsics::__m256i __m256i;


"""

for type_def in data["type_defs"]:
    result_string += f"typedef real_intrinsics::{type_def} {type_def};\n"

for intrinsic in data["functions"]:
    fname = intrinsic['fname']
    num_args = intrinsic['num_args']
    num_ops = intrinsic['num_ops']
    is_void = intrinsic['is_void']


    vars = [ f"i_{i}" for i in range(num_args) ]
    args_str = [ f"auto {var}" for var in vars]
    args_str = ", ".join(args_str)

    function_header = f"auto static inline {fname}({args_str}) " + "{\n"
    function_body =  "    " + f"AVX2::increment_op_count_by({num_ops});\n"
    function_body += "    " + "return " if not is_void else ""
    function_body += f"real_intrinsics::{fname}({', '.join(vars)});\n"


    function_body += "}\n"
    
    
    result_string += function_header + function_body 

result_string += "\n}\n"

result_string += """

using namespace mocked_intrinsics;
#endif

#endif

"""

with open(sys.argv[1], 'w') as f:
    f.write(result_string)

