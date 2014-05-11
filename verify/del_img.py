#!/usr/bin/env python

import os
import sys
import random


def del_img(*args):
    if len(args) != 2:
        print "usage: del(<path>, n_delete)"
        sys.exit(1)
    n_del = args[1]
    assert isinstance(n_del, int), "plz enter a interger"
    assert n_del > 0 and n_del < 10, "Each folder only has 10 .pgm files"
    print "We will delete %i images in each folder" % n_del
    thedir = os.path.abspath(args[0])
    folder_name = [os.path.join(thedir, name) for name in os.listdir(thedir)
                   if os.path.isdir(os.path.join(thedir, name))]
    for _folder in folder_name:
        files = os.listdir(_folder)
        random.shuffle(files)
        files = files[1:n_del+1]
        for _file in files:
            assert _file.endswith('.pgm')
            os.remove(os.path.join(_folder, _file))


def main(argv):
    """main function if adopted in shell"""
    del_img(argv[1], int(argv[2]))


if __name__ == '__main__':
    main(sys.argv)
