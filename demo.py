#! /usr/bin/env python
import os
import sys
sys.path.append('./verify')
from del_img import del_img
from create_csv import create_csv, create_csv_no_label
import matplotlib.pyplot as plt
import matplotlib.image as mimg
import matplotlib.cm as cm
import time
import Image


def main():
    '''a python wrapper demo'''
    assert os.path.isfile('test'), "Can't find the executable file"
    # QA
    while 1:
        print 'FrontFace/ already exists, would you like to overwrite it? [Y/n]'
        key = raw_input()
        if key is 'Y':
            try:
                os.system('./test')
            except:
                pass
            break
        if key is 'n':
            print 'Not overwrite anything.'
            break
        else:
            continue
    print 'Capturing finished'

    print 'Click any key to continue for verification module'
    raw_input()
    assert os.path.isdir('./verify/facesdb'), "Can't find the faces databases"
    n_tr, path_dict_tr = create_csv('./verify/facesdb', './verify/face_tr.csv')

    for img_name in os.listdir('./FrontFace'):
        img_path = os.path.join('./FrontFace', img_name)
        img = Image.open(img_path).convert('LA')
        img = img.resize((112, 92))
        img.save(img_path)
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
        # te_sample shows
        img1 = mimg.imread(file_path_te)
        plt.subplot(121)
        plt.imshow(img1, cmap=cm.gray)
        # tr_sample shows
        line = fn_res.readline()
        img2 = mimg.imread(path_dict_tr.get(int(line))[-1])
        plt.subplot(122)
        plt.imshow(img2, cmap=cm.gray)
        # IO
        plt.draw()
        plt.pause(2)
        plt.clf()
    plt.close()
    # QA
    line = fn_res.readline()
    assert len(line) == 0 or line == '\n'

    fn_res.close()
    os.remove('./verify/face_tr.csv')
    os.remove('./verify/face_te.csv')

    print 'Click any key to continue for traverse the dataset!'
    raw_input()
    os.system('verify/facesdb/trav.py')

if __name__ == '__main__':
    main()
