import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
import random
from A02_MF import MF


def image_load(img_name):
    # Use a breakpoint in the code line below to debug your script.
    pic = Image.open(img_name)
    pix_mat = np.array(pic)
    return pix_mat


def print_img(pixel_mat):
    plt.figure()
    plt.imshow(pixel_mat)
    plt.colorbar()
    plt.grid(False)
    plt.show()


def print_img_list(original, corrupted, reconsted, c_rate):
    plt.figure(figsize=(10, 3))

    plt.subplot(1, 3, 1)
    plt.xticks([])
    plt.yticks([])
    plt.grid(False)
    plt.imshow(original, cmap=plt.cm.binary)
    plt.xlabel("original")

    plt.subplot(1, 3, 2)
    plt.xticks([])
    plt.yticks([])
    plt.grid(False)
    plt.imshow(corrupted, cmap=plt.cm.binary)
    label_format = "corrupted (rate: {c_rate:.2f})"
    label_str = label_format.format(c_rate=c_rate)
    plt.xlabel(label_str)

    plt.subplot(1, 3, 3)
    plt.xticks([])
    plt.yticks([])
    plt.grid(False)
    plt.imshow(reconsted, cmap=plt.cm.binary)
    plt.xlabel("reconsted")

    plt.show()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    random.seed(0)
    original_pixels = image_load("./Lena_00c.png")

    corruption_rate = 00.0
    corrupted_pixels = image_load("./Lena_00c.png")
    corrupted_pixels = corrupted_pixels / 255 ### 0~1사이 값 가지도록 정규화

    zero_pixels = 0
    minus_pixels = 0
    plus_pixels = 0

    pixels_shape = original_pixels.shape
    for i in range(pixels_shape[0]):
        for j in range(pixels_shape[1]):
            value = corrupted_pixels[i][j]
            if value == 0:
                zero_pixels += 1
            elif value < 0:
                minus_pixels += 1
            else:
                plus_pixels += 1

    print("zero: %d, minus:%d, plus: %d" % (zero_pixels, minus_pixels, plus_pixels))

    mf = MF()
    print("train_start")
    mf.train(R=corrupted_pixels, k=64, reg_lambda=0.001, lr_alpha=0.1, max_iterations=100, epsilon=0.001)
    print("train_end")
    reconst_pixels = mf.P_MultipliedBy_Q_Transpose()
    
    if reconst_pixels is None:
        print("MF is not yet implemented.")
    else:
        print_img_list(original_pixels, corrupted_pixels, reconst_pixels, corruption_rate)

