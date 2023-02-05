import cv2
import queue
import numpy as np


class VideoProcessor:
    """
    The class process a frame to apply some transformations
    """
    def __init__(self):
        self.output_buffer = queue.Queue()

    def put_output_frame(self, frame: np.ndarray) -> None:
        self.output_buffer.put(frame)

    def get_output_frame(self) -> np.ndarray:
        return self.output_buffer.get()

    def apply_gaussian_filter(self, frame: np.ndarray, kernel_size: int) -> np.ndarray:
        out_frame = cv2.GaussianBlur(frame, (kernel_size, kernel_size), cv2.BORDER_DEFAULT)
        return out_frame

    def apply_canny(self, frame: np.ndarray) -> np.ndarray:
        out_frame = cv2.Canny(frame, 100, 200)
        return out_frame

    def apply_sobel(self, frame: np.ndarray, kernel_size: int) -> np.ndarray:
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        x = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=kernel_size, scale=1)
        y = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=kernel_size, scale=1)
        absx = cv2.convertScaleAbs(x)
        absy = cv2.convertScaleAbs(y)
        out_frame = cv2.addWeighted(absx, 0.5, absy, 0.5, 0)
        return out_frame

    def adjust_brightness(self, frame: np.ndarray, beta: int) -> np.ndarray:
        out_frame = cv2.convertScaleAbs(frame, alpha=1, beta=beta)
        return out_frame

    def adjust_contrast(self, frame: np.ndarray, alpha: float) -> np.ndarray:
        out_frame = cv2.convertScaleAbs(frame, alpha=alpha)
        return out_frame

    def apply_negative(self, frame: np.ndarray) -> np.ndarray:
        out_frame = cv2.convertScaleAbs(frame, alpha=-1, beta=255)
        return out_frame

    def convert_to_gray_scale(self, frame: np.ndarray) -> np.ndarray:
        out_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        return out_frame

    def resize_frame(self, frame: np.ndarray) -> np.ndarray:
        scale_percent = 50  # percent of original size
        width = int(frame.shape[1] * scale_percent / 100)
        height = int(frame.shape[0] * scale_percent / 100)
        dim = (width, height)

        out_frame = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

        return out_frame

    def rotate_frame(self, frame: np.ndarray) -> np.ndarray:
        out_frame = cv2.rotate(frame, cv2.ROTATE_90_CLOCKWISE)
        return out_frame

    def flip_frame_vertically(self, frame: np.ndarray) -> np.ndarray:
        out_frame = cv2.flip(frame, 0)
        return out_frame

    def flip_frame_horizontally(self, frame: np.ndarray) -> np.ndarray:
        out_frame = cv2.flip(frame, 1)
        return out_frame
