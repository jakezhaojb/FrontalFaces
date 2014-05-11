#!/usr/bin/env python

import sys
import os.path

# This is a tiny script to help you creating a CSV file from a face
# database with a similar hierarchie:
#
#  philipp@mango:~/facerec/data/at$ tree
#  .
#  |-- README
#  |-- s1
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#  |-- s2
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#  ...
#  |-- s40
#  |   |-- 1.pgm
#  |   |-- ...
#  |   |-- 10.pgm
#


def create_csv(*args):
    if len(args) != 2:
        print "usage: create_csv(<base_path>, <path_to_csv>)"
        sys.exit(1)

    BASE_PATH = os.path.abspath(args[0])
    SEPARATOR = ";"
    FILE_NAME = args[1]

    label = 0
    fid = open(FILE_NAME, 'w')
    for dirname, dirnames, filenames in os.walk(BASE_PATH):
        for subdirname in dirnames:
            subject_path = os.path.join(dirname, subdirname)
            for filename in os.listdir(subject_path):
                abs_path = "%s/%s" % (subject_path, filename)
                #print "%s%s%d" % (abs_path, SEPARATOR, label)
                fid.write("%s%s%d\n" % (abs_path, SEPARATOR, label))
            label = label + 1
    fid.close()


def main(argv):
    """main function if adopted in shell"""
    create_csv(argv[1], argv[2])


if __name__ == '__main__':
    main(sys.argv)
