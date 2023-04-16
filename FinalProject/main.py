# Universidade Federal do Rio Grande do Sul
# Intituto de informatica
# Author: Juliane da Rocha Alves
# This work aims to replicate the paper Image Analogies (Aaron Hertzmann, Charles E. Jacobs, Nuria Oliver,
# Brian Curless, David H. Salesin)

from image_analogies import ImageAnalogies
import cv2
import timeit
import numpy as np


if __name__ == "__main__":
    start = timeit.default_timer()
    image_a_source = cv2.imread("./images/monalisa.jpg")
    image_a_target = cv2.convertScaleAbs(image_a_source, alpha=1, beta=150)
    # image_a_target = cv2.imread("./images/monalisa_starry_night.png")
    image_b_source = cv2.imread("./images/tree.jpg")

    print(f"Image A size: {image_a_source.shape}")
    print(f"Image A' size: {image_a_target.shape}")
    print(f"Image B size: {image_b_source.shape}")

    analogies = ImageAnalogies(image_a_source, image_a_target, image_b_source)

    result = analogies.create_image_analogy()

    stop = timeit.default_timer()
    print("Time: ", stop - start)

    cv2.imshow("Image A", np.uint8(image_a_source))
    cv2.imshow("Image A'", image_a_target)
    cv2.imshow("Image B", image_b_source)
    cv2.imshow("Image B'", result)
    cv2.waitKey(0)


