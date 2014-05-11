#! /usr/bin/env python
import os
import sys
sys.path.append('./verify')
from del_img import del_img
from create_csv import create_csv


def main():
    '''a python wrapper demo'''
    os.system('./test')
    print 'Capturing finished'
    create_csv('./verify/faces_single', './verify/orl.csv')
    os.system('./verify/build/facerec ./verify/orl.csv')
    print 'Verification finished'


if __name__ == '__main__':
    main()
