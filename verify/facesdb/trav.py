#!/usr/bin/env python

import os
import sys
sys.path.append('/home/junbo/UndergradThesis/FrontalFace/verify')
from create_csv import create_csv
import matplotlib.pyplot as plt
import matplotlib.image as mimg
import matplotlib.cm as cm

BASE_PATH = os.path.dirname(sys.argv[0])


def main():
    for dirname, dirnames, filenames in os.walk(BASE_PATH):
        for fn in filenames:
            try:
                assert fn.endswith('.png')
                assert not fn.startswith('.')
                img_path = os.path.join(dirname, fn)
                img = mimg.imread(img_path)
                plt.imshow(img)
                plt.draw()
                plt.pause(0.2)
                plt.clf()
            except:
                pass


if __name__ == '__main__':
    main()
