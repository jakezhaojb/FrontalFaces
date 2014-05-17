#! /usr/bin/env python

import os
import sys
import numpy as np
import matplotlib.image as mimg
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import Image

BASE_PATH = os.path.dirname(sys.argv[0])


def rgb2gray(img):
    return np.dot(img[...,: 3], [0.299, 0.587, 0.144])


def main():
    src_path = os.path.join(BASE_PATH, 'facesdb_src')
    assert os.path.isdir(src_path), "move facesdv_src in this dir."
    # TODO
    for dirname, dirnames, filenames in os.walk(src_path):
        idx = 0
        for subdirname in dirnames:
            subject_path = os.path.join(dirname, subdirname)
            img_idx = 0
            if os.path.isdir(os.path.join(BASE_PATH, 'person' + str(idx))):
                continue
                print '%s was done before.' % os.path.join(BASE_PATH, 'person'
                                                + str(idx))
            else:
                os.mkdir(os.path.join(BASE_PATH, 'person' + str(idx)))
                for img_name in os.listdir(subject_path):
                    img_path = os.path.join(subject_path, img_name)
                    img = Image.open(img_path).convert('LA')
                    img = img.resize((112, 92))
                    img_dst_path = os.path.join(BASE_PATH, 'person'
                                        + str(idx), str(img_idx)) + '.png'
                    img.save(img_dst_path)
                    img_idx += 1
                print '%s was done.' % os.path.join(BASE_PATH, 'person' + str(idx))
            idx += 1
        break


if __name__ == '__main__':
    main()
