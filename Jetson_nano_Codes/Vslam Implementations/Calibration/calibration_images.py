import cv2 as cv

cam1 = cv.VideoCapture(0)
cam2 = cv.VideoCapture(2)

num = 0

while(cam1.isOpened()):
    success1, img1 = cam1.read()
    success2, img2 = cam2.read()

    k = cv.waitKey(5)
    if k == 27:
        break
    elif k == ord('s'):
        cv.imwrite('images/StereoLeft/imageL'+str(num)+'.png', img1)
        cv.imwrite('images/StereoRight/imageR'+str(num)+'.png', img2)
        print("Images saved!!")
        num += 1

    cv.imshow("img1", img1)
    cv.imshow("img2", img2)

cam1.release()
cam2.release()
cv.destroyAllWindows()