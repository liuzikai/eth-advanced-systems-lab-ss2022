Triangle Listing
================

# Input File Format
```
<node count>
<adjacent node count of node 0> <node> <node> ...
<adjacent node count of node 1> <node> <node> ...
...
```

# How to run:
```
./benchmark -num_warmups $WARMUP -num_runs $RUN  -num_phases $PHASE -o result.csv -algorithm $ALGOS -graph $INPUTDIR/$graph.txt
```
IMPORTANT: Adding the flags -no_pre_cut -no_pre_sort should only be done when alogrithm selected is compiled with sorting / cutting code in place (only needed for EI and F) othewise it might segfault since it will expect cutted input, this can be done by defining SORTING (see common.h). Also adding the flags -no_pre_cut -no_pre_sort for EU and F should only be done when SORTING is defined.


# Counting vs Listing
Elements can be counted instead of being listed by commenting "#define COLLECT_TRIANGLES" in main.cpp. Counting increase the performance and runtime of some versions significantly.
 
# Instrumentation
 
## Disable & Enable
Instrumentation can be removed by uncommenting "#define NO_INSTRUMENTATION" in common.h, otherwise the every version will also be run in instrumented mode which might be way slower than the actual version.
 
## AVX Instrumentation
AVX instructions are instrumented by adding them to "instrumented_intrinsics.json", this allows to specify the number of operations a specific intrinsic does. The "code_generator.py" will then generate a mocked intrinsic header "instrumented_immintrin.h" which uses the instrumented intrinsics only if "INSTRUMENTED" is defined.


# Compiler Flags
-fargument-noalias-anything -fno-strict-aliasing
works for ei_u4
does not work for forward


# Versions
## Edge Iterator
+ ei_base: pre sort, no pre cut, has s smaller t everywhere
+ ei_va: pre sort, pre cut, remove all s smaller t, all other versions are based on this
+ ei_v1
+ ei_v2: exponential search + set intersection compare with (1 set, 2 elements stride) at a time = unroll the set intersection loop.
+ ei_u4: unroll t-loop by 4, set intersection compare with (4 sets, 1 element stride) at a time.\
    Unrolling by 3 is a little better\
    Compiling with -fno-strict-aliasing is better.
+ ei_u5: u4 + unfold the loop by hand + store commonly accessed items in variables.\
    This optimization should be done by the compiler, but somehow only done by compiler when using -fno-strict-aliasing.
+ ei_vec4: apply forward_v4 to ei
+ ei_vec5: apply forward_v5 to ei\
    Both vec4 and vec5 has a slightly different gt/lt/ge/le comparison chunk than forward_v4/v5, no significant speedup.

## Forward
+ f_base
+ f_va
+ f_v1: Removes all branches
+ f_v2: Unroll by and vectroize by factor 4
+ f_v3: Unroll by and vectroize by factor 8
+ f_v4: vectorized version of u4. Unroll by and vectroize by factor 2x8=16
+ f_v5: Unroll by and vectroize by factor 4x8=32\
    Compiling v4 and v5 with -fno-strict-aliasing is somehow worse.
+ f_v6: f_v5 + scalar replacment and gather instead of set optimization as explained in report.
+ f_u4: apply ei_u4 to forward. Same performance characteristics.

## Forward Hashed
+ fh_base
+ fh_va
+ fh_v1: put the first hash item in the bucket, reduce one indirection
+ fh_v2:
+ fh_v3: vectorized one item hash table lookup
+ fh_v4: vectorized one item hash table lookup

# Other optimizations
+ put adjacency.count into adjacency.neighbors, similar for forward helper.
