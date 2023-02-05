import cv2
from video_processor import VideoProcessor
import time
import threading
import numpy as np

gaussian_filter = False
canny_filter = False
sobel_filter = False
brightness = False
contrast = False
negative = False
gray_scale = False
resize = False
rotate = False
flip_vertical = False
flip_horizontal = False
save = False
clear = True


def print_menu():
    print("****** MENU ******")
    print("Select the option: ")
    print("1 - Apply Gaussian filter")
    print("2 - Apply Canny filter")
    print("3 - Apply Sobel filter")
    print("4 - Adjust brightness")
    print("5 - Adjust contrast")
    print("6 - Apply negative")
    print("7 - Convert to gray scale")
    print("8 - Resize video")
    print("9 - Rotate video")
    print("10 - Flip video vertically")
    print("11 - Flip video horizontally")
    print("12 - Save video")
    print("13 - Clear")
    print("0 - Exit")
    print("******************")


def wait_user_input():
    global gaussian_filter
    global clear
    global canny_filter
    global sobel_filter
    global brightness
    global negative
    global contrast
    global gray_scale
    global resize
    global rotate
    global flip_horizontal
    global flip_vertical
    global save

    while True:
        user_input = int(input())

        if user_input == 1:
            # Gaussian
            gaussian_filter = True
            canny_filter = False
            sobel_filter = False
            clear = False
            brightness = False
            contrast = False
            negative = False
        elif user_input == 2:
            # Canny
            gaussian_filter = False
            sobel_filter = False
            clear = False
            brightness = False
            contrast = False
            negative = False
            gray_scale = False
            canny_filter = True
        elif user_input == 3:
            # Sobel
            gaussian_filter = False
            clear = False
            canny_filter = False
            brightness = False
            contrast = False
            negative = False
            gray_scale = False
            sobel_filter = True
        elif user_input == 4:
            # Brightness
            gaussian_filter = True
            canny_filter = False
            sobel_filter = False
            clear = False
            contrast = False
            negative = False
            brightness = True
        elif user_input == 5:
            # Contrast
            gaussian_filter = True
            canny_filter = False
            sobel_filter = False
            clear = False
            negative = False
            brightness = False
            contrast = True
        elif user_input == 6:
            # Negative
            gaussian_filter = True
            canny_filter = False
            sobel_filter = False
            clear = False
            brightness = False
            contrast = False
            gray_scale = False
            negative = True
        elif user_input == 7:
            # Gray scale
            clear = False
            sobel_filter = False
            canny_filter = False
            negative = False
            gray_scale = True
        elif user_input == 8:
            # Resize video
            clear = False
            resize = True
        elif user_input == 9:
            # Rotate
            clear = False
            rotate = True
        elif user_input == 10:
            # Flip vertically
            clear = False
            flip_horizontal = False
            flip_vertical = True
        elif user_input == 11:
            # Flip horizontally
            clear = False
            flip_vertical = False
            flip_horizontal = True
        elif user_input == 12:
            # Save
            if rotate or resize:
                save = False
                print("***** Not possible to record with rotation or resize on! *****")
            else:
                save = True
                print("Press 's' to stop recording")
        elif user_input == 13:
            # Clear
            gaussian_filter = False
            canny_filter = False
            sobel_filter = False
            brightness = False
            contrast = False
            negative = False
            gray_scale = False
            rotate = False
            flip_horizontal = False
            flip_vertical = False
            clear = True
        else:
            break


def nothing(x):
    pass


if __name__ == "__main__":
    # define a video capture object
    vid = cv2.VideoCapture(0)
    cv2.namedWindow("Output")
    cv2.createTrackbar("Value", "Output", 0, 100, nothing)

    frame_width = int(vid.get(3))
    frame_height = int(vid.get(4))
    size = (frame_width, frame_height)
    result = cv2.VideoWriter("my_video.avi", cv2.VideoWriter_fourcc(*"XVID"), fps=25, frameSize=size)

    processor = VideoProcessor()
    print_menu()
    t = threading.Thread(target=wait_user_input)
    t.start()

    while True:

        # Capture the video frame
        # by frame
        ret, frame = vid.read()

        out_frame = frame

        if gaussian_filter:
            kernel_size = cv2.getTrackbarPos("Value", "Output")
            if kernel_size % 2 == 0:
                kernel_size = kernel_size + 1
            out_frame = processor.apply_gaussian_filter(frame, kernel_size)
        if canny_filter:
            out_frame = processor.apply_canny(frame)
        if sobel_filter:
            out_frame = processor.apply_sobel(frame, 3)
        if brightness:
            beta = cv2.getTrackbarPos("Value", "Output")
            out_frame = processor.adjust_brightness(frame, beta)
        if contrast:
            alpha = cv2.getTrackbarPos("Value", "Output")
            out_frame = processor.adjust_contrast(frame, alpha)
        if negative:
            out_frame = processor.apply_negative(frame)
        if gray_scale:
            out_frame = processor.convert_to_gray_scale(out_frame)
        if resize:
            out_frame = processor.resize_frame(out_frame)
        if rotate:
            out_frame = processor.rotate_frame(out_frame)
        if flip_horizontal:
            out_frame = processor.flip_frame_horizontally(out_frame)
        if flip_vertical:
            out_frame = processor.flip_frame_vertically(out_frame)

        # Display the input frame
        cv2.imshow("Input", frame)

        # Display the output frame
        processor.put_output_frame(out_frame)
        time.sleep(0.0025)
        final_frame = processor.get_output_frame()
        cv2.imshow("Output", final_frame)

        if save:
            result.write(final_frame)
        if cv2.waitKey(1) & 0xFF == ord("s"):
            print("Stop recording")
            save = False

        # the 'q' button is set as the
        # quitting button you may use any
        # desired button of your choice
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

    # After the loop release the cap object
    vid.release()
    result.release()
    # Destroy all the windows
    cv2.destroyAllWindows()
