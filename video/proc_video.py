#! /usr/bin/env python

import os
import Image


def main():
    # using ffmpeg to decode video to images
    if not os.path.isdir('ipart_head_video'):
        flag = True
    else:
        if os.listdir('ipart_head_video') is []:
            flag = True
        else:
            flag = False
    if flag:
        os.system('ffmpeg -i ipart_head.avi -f image2 ipart_head_video/img%04d.png')
    print "video is decoded to images"

    # process the images
    for fn in os.listdir('ipart_head_video'):
        print 'process image %s' % fn
        fn_ = os.path.join('ipart_head_video', fn)
        img = Image.open(fn_)
        img_ = img.resize((img.size[0]/2, img.size[1]/2))
        img_.save(fn_)
    print 'video process done.'

if __name__ == '__main__':
    main()
