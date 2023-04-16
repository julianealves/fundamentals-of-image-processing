# Universidade Federal do Rio Grande do Sul
# Intituto de informatica
# Author: Juliane da Rocha Alves
# This work aims to replicate the paper Image Analogies (Aaron Hertzmann, Charles E. Jacobs, Nuria Oliver,
# Brian Curless, David H. Salesin)

import cv2
import numpy as np
import math
from typing import Dict
from scipy.spatial import distance


class ImageAnalogies:
    def __init__(self, a_source: np.ndarray, a_target: np.ndarray, b_source: np.ndarray) -> None:
        self.image_a_source = a_source
        self.image_a_target = a_target
        self.image_b_source = b_source
        self.image_b_target = np.zeros((b_source.shape[0], b_source.shape[1], 3))
        self.source_s = np.zeros((b_source.shape[0], b_source.shape[1]), dtype=[("x", "int"), ("y", "int")])
        self.levels_gaussian_pyramids = 2
        self.gaussian_pyramids_image_a_source = []
        self.gaussian_pyramids_image_a_target = []
        self.gaussian_pyramids_image_b_source = []
        self.gaussian_pyramids_image_b_target = []
        self.gaussian_pyramids_source_s = []

        self.neighborhood_size_fine_level = 5
        self.neighborhood_size_coarse_level = 3

    def compute_gaussian_pyramids(self) -> None:
        """
        Computes the Gaussian pyramids

        :return: None
        """
        self.gaussian_pyramids_image_a_source.append(self.image_a_source)
        self.gaussian_pyramids_image_a_target.append(self.image_a_target)
        self.gaussian_pyramids_image_b_source.append(self.image_b_source)
        self.gaussian_pyramids_image_b_target.append(self.image_b_target)
        self.gaussian_pyramids_source_s.append(self.source_s)

        for i in range(self.levels_gaussian_pyramids - 1):
            self.gaussian_pyramids_image_a_source.append(cv2.pyrDown(self.gaussian_pyramids_image_a_source[i]))
            self.gaussian_pyramids_image_a_target.append(cv2.pyrDown(self.gaussian_pyramids_image_a_target[i]))
            self.gaussian_pyramids_image_b_source.append(cv2.pyrDown(self.gaussian_pyramids_image_b_source[i]))

            image_b_target_pyramid = cv2.pyrDown(self.gaussian_pyramids_image_b_target[i])
            self.gaussian_pyramids_image_b_target.append(image_b_target_pyramid)
            source_s_pyramid = np.zeros((image_b_target_pyramid.shape[0], image_b_target_pyramid.shape[1]),
                                        dtype=[("x", "int"), ("y", "int")])
            self.gaussian_pyramids_source_s.append(source_s_pyramid)

        self.gaussian_pyramids_image_a_source.reverse()
        self.gaussian_pyramids_image_a_target.reverse()
        self.gaussian_pyramids_image_b_source.reverse()
        self.gaussian_pyramids_image_b_target.reverse()
        self.gaussian_pyramids_source_s.reverse()

        print("Gaussian pyramids computed!")

    def convert_images_to_gray_scale(self) -> None:
        """
        Converts all images to gray scale applying the conversion from RGB to YIQ and remapping the Y value

        :return: None
        """
        for level in range(self.levels_gaussian_pyramids):
            image_a_source = self.transform_bgr2_yiq(self.gaussian_pyramids_image_a_source[level])
            image_a_target = self.transform_bgr2_yiq(self.gaussian_pyramids_image_a_target[level])
            image_b_source = self.transform_bgr2_yiq(self.gaussian_pyramids_image_b_source[level])

            self.gaussian_pyramids_image_a_source[level] = self.luminance_remapping(image_a_source, image_b_source)
            self.gaussian_pyramids_image_a_target[level] = self.luminance_remapping(image_a_target, image_b_source)
            self.gaussian_pyramids_image_b_source[level] = image_b_source

    def copy_iq_channels(self, source: np.ndarray, target: np.ndarray) -> np.ndarray:
        target[:, :, 1:] = source[:, :, 1:]
        return target

    def transform_bgr2_yiq(self, img_rgb: np.ndarray) -> np.ndarray:
        """
        Converts an RGB image to YIQ color space
        :param img_rgb: An Image in RGB
        :return: A YIQ in image color space
        """
        yiq_from_rgb = np.array([[0.21153661, -0.52273617, 0.31119955],
                                 [0.59590059, -0.27455667, -0.32134392],
                                 [0.299, 0.587, 0.114]])
        orig_shape = img_rgb.shape
        return np.dot(img_rgb.reshape(-1, 3), yiq_from_rgb.transpose()).reshape(orig_shape)

    def transform_yiq2_bgr(self, img_yiq: np.ndarray) -> np.ndarray:
        """
        Converts an YIQ image to RGB color space
        :param img_yiq: An Image in YIQ
        :return: A RGB in image color space
        """
        yiq_from_rgb = np.array([[0.21153661, -0.52273617, 0.31119955],
                                 [0.59590059, -0.27455667, -0.32134392],
                                 [0.299, 0.587, 0.114]])
        orig_shape = img_yiq.shape
        return np.dot(img_yiq.reshape(-1, 3), np.linalg.inv(yiq_from_rgb).transpose()).reshape(orig_shape)

    def luminance_remapping(self, image_a: np.ndarray, image_b: np.ndarray) -> np.ndarray:
        """
        Remaps the luminance according to the following equation: (p) = (std_b/std_a)*(Y(p) - mean_a) + mean_b

        :param image_a: Image A in gray scale
        :param image_b: Image B in gray scale
        :return: Image A with luminance remapped
        """

        height, width, _ = image_a.shape
        for y in range(width):
            for x in range(height):
                image_a[x, y][0] = (np.std(image_b[:, :, 0]) / np.std(image_a[:, :, 0])) * (
                            image_a[x, y][0] - np.mean(image_a[:, :, 0])) + np.mean(image_b[:, :, 0])

        return image_a

    def get_neighbors(self, image_a: np.ndarray, image_b: np.ndarray, pixel_p: tuple[int, int],
                      pixel_q: tuple[int, int], neighborhood_size: int) -> Dict:
        """
        Gets all the neighbors from pixel p and q according to the size of the neighborhood.
        It truncates the vectors with neighbors if the sizes are different. This is done because of the pixels in the
        edges

        It returns a dict where the key the image 'a' or 'b' and the value is the neighbors in a vector
        {'a': [neighbors of pixel p], 'b': [neighbors of pixel q]}

        :param image_a: Image A
        :param image_b: Image B
        :param pixel_p: Pixel p from image A
        :param pixel_q: Pixel q from image B
        :param neighborhood_size: The size of the neighborhood (3x3 if 3, or 5x5 if 5)
        :return: The neighbors for pixels p and q
        """

        border = math.floor(neighborhood_size / 2)

        features_a = []
        features_b = []

        partial_search = False
        number_neighbors = 0

        height_b, width_b, _ = image_b.shape
        if pixel_q[0] <= 4 or pixel_q[1] <= 4 or pixel_q[0] >= width_b - 4 or pixel_q[1] >= height_b - 4:
            partial_search = True

        # searching for neighbors in B
        for y in range(max(0, pixel_q[1] - border), min(pixel_q[1] + border + 1, width_b)):
            for x in range(max(0, pixel_q[0] - border), min(pixel_q[0] + border + 1, height_b)):
                if x == pixel_q[0] and y == pixel_q[1]:
                    continue
                if partial_search:
                    number_neighbors = number_neighbors + 1
                features_b.append(image_b[x, y][0])

        height_a, width_a, _ = image_a.shape
        # searching for neighbors in A
        for y in range(max(0, pixel_p[1] - border), min(pixel_p[1] + border + 1, width_a)):
            for x in range(max(0, pixel_p[0] - border), min(pixel_p[0] + border + 1, height_a)):
                if x == pixel_p[0] and y == pixel_p[1]:
                    continue
                if partial_search and number_neighbors == 0:
                    break
                features_a.append(image_a[x, y][0])

                if partial_search:
                    number_neighbors = number_neighbors - 1

            if partial_search and number_neighbors == 0:
                break

        min_features = min(len(features_a), len(features_b))

        return {"a": features_a[:min_features], "b": features_b[:min_features]}

    def get_neighbors_target(self, pixel_p: tuple[int, int], pixel_q: tuple[int, int], level: int) -> Dict:
        """
        Gets all the neighbors from pixel p and q from A' and B'. This is done separately from the get_neighbors method
        because the target images will not use all the neighbors, once B' has not all pixels synthesized yet.
        It truncates the vectors with neighbors if the sizes are different. This is done because of the pixels in the
        edges.

        It returns a dict where the key the image 'a' or 'b' and the value is the neighbors in a vector
        {'a': [neighbors of pixel p], 'b': [neighbors of pixel q]}

        :param pixel_p: Pixel p from image A'
        :param pixel_q: Pixel q from image B'
        :param level: The level of the Gaussian pyramid
        :return: The neighbors for pixels p and q
        """
        image_a = self.gaussian_pyramids_image_a_target[level]
        image_b = self.gaussian_pyramids_image_b_target[level]

        border = math.floor(self.neighborhood_size_fine_level / 2)

        features_a = []
        features_b = []

        total_neighbors = 0

        stop_searching = False

        height_b, width_b, _ = image_b.shape
        # searching for neighbors in B'
        for y in range(max(0, pixel_q[1] - border), min(pixel_q[1] + border + 1, width_b)):
            for x in range(max(0, pixel_q[0] - border), min(pixel_q[0] + border + 1, height_b)):
                if x == pixel_q[0] and y == pixel_q[1]:
                    stop_searching = True
                    break
                features_b.append(image_b[x, y][0])
                total_neighbors = total_neighbors + 1
            if stop_searching:
                break

        stop_searching = False

        height_a, width_a, _ = image_a.shape
        # searching for neighbors in A'
        for y in range(max(0, pixel_p[1] - border), min(pixel_p[1] + border + 1, width_a)):
            for x in range(max(0, pixel_p[0] - border), min(pixel_p[0] + border + 1, height_a)):
                if x == pixel_p[0] and y == pixel_p[1]:
                    stop_searching = True
                    break
                if total_neighbors == 0:
                    stop_searching = True
                    break
                features_a.append(image_a[x, y][0])
                total_neighbors = total_neighbors - 1
            if stop_searching:
                break

        min_features = min(len(features_a), len(features_b))

        return {"a": features_a[:min_features], "b": features_b[:min_features]}

    def get_distance(self, image_a_source: np.ndarray,
                     image_b_source: np.ndarray,
                     pixel_p: tuple[int, int],
                     pixel_q: tuple[int, int],
                     level: int) -> float:
        """
        It calculates the Euclidean distance from the feature vector from A and B.
        The feature vectors are the neighbors from pixel p and q.

        :param image_a_source: Image A
        :param image_b_source: Image B
        :param pixel_p: Pixel from image A
        :param pixel_q: Pixel from image B
        :param level: The level of the Gaussian Pyramid
        :return: The Euclidean distance between the feature vectors from A and B
        """

        neighbors_fine_level_src = self.get_neighbors(image_a=image_a_source,
                                                      image_b=image_b_source,
                                                      pixel_p=pixel_p,
                                                      pixel_q=pixel_q,
                                                      neighborhood_size=self.neighborhood_size_fine_level)

        neighbors_coarse_level_src = None
        neighbors_coarse_level_target = None
        if level != 0:
            neighbors_coarse_level_src = self.get_neighbors(
                image_a=self.gaussian_pyramids_image_a_source[level - 1],
                image_b=self.gaussian_pyramids_image_b_source[level - 1],
                pixel_p=(math.floor(pixel_p[0] / 2), math.floor(pixel_p[1] / 2)),
                pixel_q=(math.floor(pixel_q[0] / 2), math.floor(pixel_q[1] / 2)),
                neighborhood_size=self.neighborhood_size_coarse_level)

            neighbors_coarse_level_target = self.get_neighbors(
                image_a=self.gaussian_pyramids_image_a_target[level - 1],
                image_b=self.gaussian_pyramids_image_b_target[level - 1],
                pixel_p=(math.floor(pixel_p[0] / 2), math.floor(pixel_p[1] / 2)),
                pixel_q=(math.floor(pixel_q[0] / 2), math.floor(pixel_q[1] / 2)),
                neighborhood_size=self.neighborhood_size_coarse_level)

        neighbors_fine_level_target = self.get_neighbors_target(pixel_p=pixel_p, pixel_q=pixel_q,
                                                                level=level)

        if neighbors_coarse_level_src is not None and neighbors_coarse_level_target is not None:
            features_a = np.concatenate((neighbors_fine_level_src["a"], neighbors_coarse_level_src["a"],
                                         neighbors_fine_level_target["a"], neighbors_coarse_level_target["a"]),
                                        axis=None)

            features_b = np.concatenate((neighbors_fine_level_src["b"], neighbors_coarse_level_src["b"],
                                         neighbors_fine_level_target["b"], neighbors_coarse_level_target["b"]),
                                        axis=None)
        else:
            features_a = np.concatenate((neighbors_fine_level_src["a"], neighbors_fine_level_target["a"]), axis=None)

            features_b = np.concatenate((neighbors_fine_level_src["b"], neighbors_fine_level_target["b"]), axis=None)

        dist = distance.euclidean(features_a / np.linalg.norm(features_a), features_b / np.linalg.norm(features_b))

        return dist

    def best_approximate_match(self, image_a_source: np.ndarray,
                               image_b_source: np.ndarray,
                               level: int,
                               pixel_q: tuple[int, int]) -> tuple[int, int]:
        """
        It returns the coordinate of pixel p from image A that has the minimum euclidean distance from pixel q,
        according to the feature vectors

        :param image_a_source: Image A
        :param image_b_source: Image B
        :param level: The level of the Gaussian Pyramid
        :param pixel_q: Pixel q from image B
        :return: The pixel coordinate from the minimum euclidean distance
        """

        min_x = -1
        min_y = -1
        min_dist = np.inf

        height, width, _ = image_a_source.shape

        for y in range(width):
            for x in range(height):

                pixel_p = (x, y)

                dist = self.get_distance(image_a_source=image_a_source, image_b_source=image_b_source, pixel_p=pixel_p,
                                         pixel_q=pixel_q, level=level)

                if dist < min_dist:
                    min_dist = dist
                    min_x = x
                    min_y = y

        return min_x, min_y

    def best_coherence_match(self, image_a_source: np.ndarray,
                             image_b_source: np.ndarray,
                             source_s: np.ndarray,
                             level: int,
                             pixel_q: tuple[int, int]) -> tuple[int, int]:
        """
        It returns the coordinate of pixel p from image A that has more coherence with pixel q,
        according to the feature vectors.
        It uses the following formulation to obtain the pixel p for image A

        r* = arg min ||Fl(s(r) + (q-r)) - Fl(q)||^2

        :param image_a_source: Image A
        :param image_b_source: Image B
        :param source_s: Source
        :param level: The level of the Gaussian Pyramid
        :param pixel_q: Pixel q from image B
        :return: The pixel p coordinate that has more coherence with pixel q
        """

        r_star_y = -1
        r_star_x = -1
        min_dist = np.inf

        height, width, _ = image_a_source.shape

        border = math.floor(self.neighborhood_size_fine_level / 2)

        # dict_features = {(x,y): ([features AA'],[Features BB'])}
        dict_features = {}

        stop_searching = False

        height_b, width_b, _ = image_b_source.shape
        # r = (x,y)
        for y in range(max(0, pixel_q[1] - border), min(pixel_q[1] + border + 1, width_b)):
            for x in range(max(0, pixel_q[0] - border), min(pixel_q[0] + border + 1, height_b)):
                if x == pixel_q[0] and y == pixel_q[1]:
                    stop_searching = True
                    break

                # source_s contain pixels
                surce_s_x = source_s[x, y][0] + (pixel_q[0] - x)
                surce_s_y = source_s[x, y][1] + (pixel_q[1] - y)

                if surce_s_x >= height or surce_s_x < 0 or surce_s_y >= width or surce_s_y < 0:
                    continue

                neighbors_fine_level_src = self.get_neighbors(image_a=image_a_source,
                                                              image_b=image_b_source,
                                                              pixel_p=(surce_s_x, surce_s_y),
                                                              pixel_q=pixel_q,
                                                              neighborhood_size=self.neighborhood_size_fine_level)

                neighbors_coarse_level_src = None
                neighbors_coarse_level_target = None
                if level != 0:
                    neighbors_coarse_level_src = self.get_neighbors(
                        image_a=self.gaussian_pyramids_image_a_source[level - 1],
                        image_b=self.gaussian_pyramids_image_b_source[level - 1],
                        pixel_p=(math.floor(surce_s_x / 2), math.floor(surce_s_y / 2)),
                        pixel_q=(math.floor(pixel_q[0] / 2), math.floor(pixel_q[1] / 2)),
                        neighborhood_size=self.neighborhood_size_coarse_level)

                    neighbors_coarse_level_target = self.get_neighbors(
                        image_a=self.gaussian_pyramids_image_a_target[level - 1],
                        image_b=self.gaussian_pyramids_image_b_target[level - 1],
                        pixel_p=(math.floor(surce_s_x / 2), math.floor(surce_s_y / 2)),
                        pixel_q=(math.floor(pixel_q[0] / 2), math.floor(pixel_q[1] / 2)),
                        neighborhood_size=self.neighborhood_size_coarse_level)

                neighbors_fine_level_target = self.get_neighbors_target(pixel_p=(surce_s_x, surce_s_y), pixel_q=pixel_q,
                                                                        level=level)

                if neighbors_coarse_level_src is not None and neighbors_coarse_level_target is not None:
                    features_a = np.concatenate((neighbors_fine_level_src["a"], neighbors_coarse_level_src["a"],
                                                 neighbors_fine_level_target["a"], neighbors_coarse_level_target["a"]),
                                                axis=None)

                    features_b = np.concatenate((neighbors_fine_level_src["b"], neighbors_coarse_level_src["b"],
                                                 neighbors_fine_level_target["b"], neighbors_coarse_level_target["b"]),
                                                axis=None)
                else:
                    features_a = np.concatenate((neighbors_fine_level_src["a"], neighbors_fine_level_target["a"]),
                                                axis=None)

                    features_b = np.concatenate((neighbors_fine_level_src["b"], neighbors_fine_level_target["b"]),
                                                axis=None)

                dict_features[(x, y)] = (features_a, features_b)

            if stop_searching:
                break

        for key, value in dict_features.items():

            dist = distance.euclidean(value[0] / np.linalg.norm(value[0]), value[1] / np.linalg.norm(value[1]))

            if dist < min_dist:
                min_dist = dist
                r_star_x = key[0]
                r_star_y = key[1]

        best_coh_match_x = source_s[r_star_y, r_star_x][0] + (pixel_q[0] - r_star_x)
        best_coh_match_y = source_s[r_star_y, r_star_x][1] + (pixel_q[1] - r_star_y)

        return best_coh_match_x, best_coh_match_y

    def best_match(self, image_a_source: np.ndarray,
                   image_b_source: np.ndarray,
                   source_s: np.ndarray,
                   level: int,
                   pixel_q: tuple[int, int]) -> tuple[int, int]:
        """
        It finds the best match between pixel p  from images A and A' and the pixel q that is being synthesized.

        :param image_a_source: Image A
        :param image_b_source: Image B
        :param source_s: Source
        :param level: The level of the Gaussian Pyramid
        :param pixel_q: Pixel q the is being synthesized
        :return: The coordinate of pixel p
        """
        k = 0.5

        p_app_x, p_app_y = self.best_approximate_match(image_a_source, image_b_source, level, pixel_q)

        height, width, _ = image_b_source.shape

        # If q is in one of the edges, return the result of the approximate match function
        if pixel_q[0] <= 4 or pixel_q[1] <= 4 or pixel_q[0] >= width - 4 or pixel_q[1] >= height - 4:
            return p_app_x, p_app_y

        p_coh_x, p_coh_y = self.best_coherence_match(image_a_source, image_b_source, source_s, level, pixel_q)

        height_a, width_a, _ = image_a_source.shape
        if p_coh_x < 0 or p_coh_y < 0 or p_coh_x >= height_a or p_coh_y >= width_a:
            return p_app_x, p_app_y

        d_app = self.get_distance(image_a_source=image_a_source,
                                  image_b_source=image_b_source,
                                  pixel_p=(p_app_x, p_app_y),
                                  pixel_q=pixel_q,
                                  level=level)

        d_coh = self.get_distance(image_a_source=image_a_source,
                                  image_b_source=image_b_source,
                                  pixel_p=(p_coh_x, p_coh_y),
                                  pixel_q=pixel_q,
                                  level=level)

        if d_coh <= d_app * (1 + pow(2, level - (self.levels_gaussian_pyramids - 1)) * k):
            return p_coh_x, p_coh_y

        return p_app_x, p_app_y

    def create_image_analogy(self) -> np.ndarray:
        """
        It learns the analogy between A and A' and returns a synthesized image B'

        :return: The synthesized image
        """
        self.compute_gaussian_pyramids()
        self.convert_images_to_gray_scale()

        for level in range(self.levels_gaussian_pyramids):
            print(f"Computing level: {level}")
            image_b_target = self.gaussian_pyramids_image_b_target[level]
            height, width, _ = image_b_target.shape
            for y in range(width):
                for x in range(height):
                    pixel_p = self.best_match(image_a_source=self.gaussian_pyramids_image_a_source[level],
                                              image_b_source=self.gaussian_pyramids_image_b_source[level],
                                              source_s=self.gaussian_pyramids_source_s[level],
                                              level=level,
                                              pixel_q=(x, y))

                    image_a_target = self.gaussian_pyramids_image_a_target[level]
                    image_b_target[x, y][0] = image_a_target[pixel_p[0], pixel_p[1]][0]
                    self.gaussian_pyramids_image_b_target[level] = image_b_target

                    source_s_level = self.gaussian_pyramids_source_s[level]
                    source_s_level[x, y] = pixel_p
                    self.gaussian_pyramids_source_s[level] = source_s_level

                    print(f"Synthesised pixel ({x},{y}) of the final image at level {level}")

        final_image = self.gaussian_pyramids_image_b_target[-1]
        final_image = self.copy_iq_channels(source=self.gaussian_pyramids_image_b_source[-1],
                                            target=final_image)
        final_image = self.transform_yiq2_bgr(final_image)

        return np.uint8(final_image)
