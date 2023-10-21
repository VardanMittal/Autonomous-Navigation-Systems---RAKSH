import numpy as np
import cv2

# Define the number of corners on the checkerboard (inner corners).
# You should change these values based on your checkerboard.
num_corners_x = 9  # Number of corners along the x-axis
num_corners_y = 6  # Number of corners along the y-axis

# Define the size of each square on the checkerboard (in meters).
# You should measure the size of the squares on your checkerboard.
square_size = 0.02  # 2 cm in this example

# Prepare object points, e.g., (0,0,0), (1,0,0), (2,0,0), ...,(8,5,0).
objp = np.zeros((num_corners_x * num_corners_y, 3), np.float32)
objp[:, :2] = np.mgrid[0:num_corners_x, 0:num_corners_y].T.reshape(-1, 2) * square_size

# Arrays to store object points and image points from all calibration images.
obj_points = []  # 3D points in real-world space
img_points_left = []  # 2D points in left image
img_points_right = []  # 2D points in right image

# Load calibration images and find chessboard corners.
# Replace 'left*.jpg' and 'right*.jpg' with your image file paths.
for i in range(1, num_images + 1):
    left_image = cv2.imread(f'left{i}.jpg')
    right_image = cv2.imread(f'right{i}.jpg')
    
    gray_left = cv2.cvtColor(left_image, cv2.COLOR_BGR2GRAY)
    gray_right = cv2.cvtColor(right_image, cv2.COLOR_BGR2GRAY)

    # Find chessboard corners
    ret_left, corners_left = cv2.findChessboardCorners(gray_left, (num_corners_x, num_corners_y), None)
    ret_right, corners_right = cv2.findChessboardCorners(gray_right, (num_corners_x, num_corners_y), None)

    if ret_left and ret_right:
        obj_points.append(objp)
        img_points_left.append(corners_left)
        img_points_right.append(corners_right)

# Calibrate the stereo camera
ret, K_left, dist_left, K_right, dist_right, R, T, E, F = cv2.stereoCalibrate(
    obj_points, img_points_left, img_points_right,
    imageSize=(left_image.shape[1], left_image.shape[0])
)

# Print calibration results
print("Stereo Calibration Results:")
print("Left Camera Matrix (K):")
print(K_left)
print("Left Distortion Coefficients:")
print(dist_left)
print("Right Camera Matrix (K):")
print(K_right)
print("Right Distortion Coefficients:")
print(dist_right)
print("Rotation Matrix (R):")
print(R)
print("Translation Vector (T):")
print(T)

# Save calibration results to separate files
np.savez('stereo_camera_calibration.npz', K_left=K_left, K_right=K_right, R=R, T=T)
np.savez('stereo_camera_distortion.npz', dist_left=dist_left, dist_right=dist_right)
