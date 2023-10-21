import numpy as np
import cv2
import glob
## Variables based on camera setup
fillSize = 20
chessBoardSize = (9,6)
frameSize = (640,480)
winsize = (11,11)
zzone = (-1,-1)

criteria = (cv2.TermCriteria_EPS + cv2.TermCriteria_MAX_ITER, 30, 0.001)


###################################logic###################################
objp = np.zeros((chessBoardSize[0] * chessBoardSize[1],3),np.float32)
objp[:, :2] = np.mgrid[0:chessBoardSize[0], 0:chessBoardSize[1]].T.reshape(-1,2)

objp = objp * fillSize

print(objp)

objpoints = []
imgpointsL = []
imgpointsR = []

imagesLeft = glob.glob('images/StereoLeft/*.png')
imagesRight = glob.glob('images/StereoRight/*.png')

for imgLeft, imgRight in zip(imagesLeft,imagesRight):
    imgL = cv2.imread(imgLeft)
    imgR = cv2.imread(imgRight)

    grayL = cv2.cvtColor(imgL, cv2.COLOR_BGR2GRAY)
    grayR = cv2.cvtColor(imgR, cv2.COLOR_BGR2GRAY)
    
    ## estimating the chess board coordinates
    retL, cornersL = cv2.findChessboardCorners(grayL, chessBoardSize, None)
    retR, cornersR = cv2.findChessboardCorners(grayR, chessBoardSize, None)

    if retL and retR == True:

        objpoints.append(objp)

        cornersL = cv2.cornerSubPix(grayL,cornersL, winsize,zzone, criteria)
        imagesLeft.append(cornersL)
        cornersR = cv2.cornerSubPix(grayR,cornersR, winsize,zzone, criteria)
        imagesRight .append(cornersR)

        cv2.drawChessboardCorners(imgL,chessBoardSize, cornersL,retL)
        cv2.imshow("img Left", imgL)
        cv2.drawChessboardCorners(imgR,chessBoardSize, cornersR,retR)
        cv2.imshow("img Right", imgR)
        cv2.waitKey(1000)
    
cv2.destroyAllWindows()

###################################Camera Calibration#################################
retL, cameraMatrixL, distL, rvectL, tvecL = cv2.calibrateCamera(objpoints,imgpointsL,frameSize,None,None)
heightL, widthL, channelsL = imgL.shape
IMAGESIZEL = (widthL,heightL)
newCameraMatrixL, roi_L = cv2.getOptimalNewCameraMatrix(cameraMatrixL,distL, IMAGESIZEL, 1, IMAGESIZEL)
retR, cameraMatrixR, distR, rvectR, tvecR = cv2.calibrateCamera(objpoints,imgpointsR,frameSize,None,None)
heightR, widthR, channelsR = imgR.shape
IMAGESIZER = (widthR,heightR)
newCameraMatrixR, roi_R = cv2.getOptimalNewCameraMatrix(cameraMatrixR,distR, IMAGESIZER, 1, IMAGESIZER)

#########################################Stereo Calibration################################
flags = 0
flags |= cv2.CALIB_FIX_INTRINSIC

STEREOCRITERIA = (cv2.TermCriteria_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

retStereo, newCameraMatrixL, distL, newCameraMatrixR, distR,rot,trans,essentialMatrix,fundamentalMatrix =cv2.stereoCalibrate(objpoints,imgpointsL,imgpointsR,newCameraMatrixL,distL,newCameraMatrixR, distR,grayL.shape[::-1],STEREOCRITERIA,flags)


########## Stereo Rectification #################################################

rectifyScale= 1
rectL, rectR, projMatrixL, projMatrixR, Q, roi_L, roi_R= cv2.stereoRectify(newCameraMatrixL, distL, newCameraMatrixR, distR, grayL.shape[::-1], rot, trans, rectifyScale,(0,0))

stereoMapL = cv2.initUndistortRectifyMap(newCameraMatrixL, distL, rectL, projMatrixL, grayL.shape[::-1], cv2.CV_16SC2)
stereoMapR = cv2.initUndistortRectifyMap(newCameraMatrixR, distR, rectR, projMatrixR, grayR.shape[::-1], cv2.CV_16SC2)

print("Saving parameters!")
cv_file = cv2.FileStorage('stereoMap.xml', cv2.FILE_STORAGE_WRITE)

cv_file.write('stereoMapL_x',stereoMapL[0])
cv_file.write('stereoMapL_y',stereoMapL[1])
cv_file.write('stereoMapR_x',stereoMapR[0])
cv_file.write('stereoMapR_y',stereoMapR[1])

cv_file.release()