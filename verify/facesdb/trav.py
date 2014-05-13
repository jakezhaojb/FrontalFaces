#!/usr/bin/env python

import os
import sys
sys.path.append('/home/junbo/UndergradThesis/FrontalFace/verify')
from create_csv import create_csv
import matplotlib.pyplot as plt
import matplotlib.image as mimg
import matplotlib.cm as cm


def main():
    # traverse the data
    if os.path.isfile('tmp.csv'):
        os.remove('tmp.csv')
    create_csv('.', 'tmp.csv')
    fn = open('tmp.csv')
    plt.figure()
    while 1:
        try:
            line = fn.readline()
            line = line[:line.find(';')]
            if len(line) is 0:
                break
            img = mimg.imread(line) # numpy.ndarray
            plt.imshow(img, cmap=cm.gray) # gray scale
            plt.draw()
            plt.pause(0.2)
            plt.clf()
        except:
            pass
    fn.close()
    os.remove('tmp.csv')

if __name__ == '__main__':
    main()
