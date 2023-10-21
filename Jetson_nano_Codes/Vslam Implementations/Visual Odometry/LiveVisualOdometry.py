import cv2
import numpy as np
import os
from scipy.optimize import least_squares
import time
from tqdm import tqdm
import matplotlib.pyplot as plt

from pytransform3d import transformations as pt, trajectories as ptr, rotations as pr, camera as pc

from cycler import cycle

class VisualOdometry():
    def __init__(self, data_dir):
        self.K_l, self.P_l, self.K_r, self.P_r = self._load_calib("")