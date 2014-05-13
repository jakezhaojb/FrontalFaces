#! /usr/bin/env python
import os
import sys
sys.path.append('./verify')
from del_img import del_img
from create_csv import create_csv, create_csv_no_label
import matplotlib.pyplot as plt
import matplotlib.image as mimg
import time


def main():
    '''a python wrapper demo'''
    assert os.path.isfile('test'), "Can't find the executable file"
    os.system('./test')
    print 'Capturing finished'
    assert os.path.isdir('.verify/facesdb'), "Can't find the faces databases"
    n_tr, path_dict_tr = create_csv('./verify/facesdb', './verify/face_tr.csv')
    n_te, path_te = create_csv_no_label('./FrontFace', './verify/face_te.csv')

    # tell n_tr, n_te
    if os.path.isfile('./verify/result.csv'):
        os.remove('./verify/result.csv')
    os.system('./verify/build/facerec ./verify/face_tr.csv\
               ./verify/face_te.csv ./verify/result.csv')
    print 'Verification finished'
    assert os.path.isfile('./verify/result.csv')
    fn_res = open('./verify/result.csv', 'r')

    # showing the result of verify
    for file_path_te in path_te:
        plt.figure()
        # te_sample shows
        img1 = mimg.imread(file_path_te)
        plt.subplot(121)
        plt.imshow(img1)
        # tr_sample shows
        line = fn_res.readline()
        img2 = mimg.imread(path_dict_tr.get(int(line))[0])
        plt.subplot(122)
        plt.imshow(img2)
        # IO
        plt.show()
        time.sleep(1.5)
        plt.close()
    # QA
    line = fn_res.readline()
    assert len(line) == 0 or line == '\n'

    fn_res.close()

if __name__ == '__main__':
    main()
