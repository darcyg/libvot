#libvot - A C++11 multi-thread library for image retrieval
##Introduction

*libvot* is an implementation of vocabulary tree, which is an algorithm widely used in image retrieval and computer vision. It usually comprises three components to build a image retrieval system using vocabulary tree: build a k-means tree using sift descriptors from images, register images into the database, query images against the database. In this library, we use C++11 standard multi-thread library to accelerate the computation, which achieves fast and accurate image retrieval result. This project is inspired by Snavely's [VocabTree2](https://github.com/snavely/VocabTree2) project. Currently this library is under active development for research purpose. 

##Installation

The build system of libvot is based on [CMake](http://cmake.org). To take full advantages of the new features in C++11, we require the version of CMake to be 3.1 or above. Current we have tested our program under Linux (Ubuntu 14.04, CentOS 7) and MacOS (10.10). The common steps to build the library is:

1. Extract source files.
2. Create build directory and change to it.
3. Run CMake to configure the build tree.
4. Build the software using selected build tool.
5. Run unit_test in the 'test' folder
5. See src/example for the use of this library.

On Unix-like systems with GNU Make as build tool, the following sequence of commands
can be used to compile the source code.

    $ cd libvot
    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ cd test && ./unit_test

##First try
libvot now supports two types of feature formats, one feature format we use internally and the other one generated by [openMVG](https://github.com/openMVG/openMVG/). 
The most convenient way to run libvot for now is to first generate descriptor files using openMVG, then run *image_search* in src/example. 
The usage is simply “Usage: ./image_search <sift_list> <output_dir> [depth] [branch_num] [sift_type] [num_matches] [thread_num]”. 
We also add a small image dataset [fountain-P11]( http://cvlabwww.epfl.ch/data/multiview/denseMVS.html) to illustrate this process. 
*test_data* folder only contains the desc files generated by openMVG, while the original images are not included in order to save space. 
If you use the out-of-source build as shown in the installation section and in the *build* directory, 
the following command should work smoothly and generate several output files in build/src/example/vocab_out directory. 

    $ cd src/example
    $ ./image_search ../../../test_data/list ./vocab_out 6 8 1

Each line in *match.out* contains three numbers “first_index second_index similarity score”. 
Since the library is multi-threaded, the rank is unordered with respect to the first index (they are ordered w.r.t the second index). 
*match_pairs* saves the ordered similarity ranks, from *0*th image to *n-1*th image. 

##License
The BSD 3-Clause License

##Contact
For inquiries and suggestions, please send your emails to 
<tshenaa@ust.hk>

