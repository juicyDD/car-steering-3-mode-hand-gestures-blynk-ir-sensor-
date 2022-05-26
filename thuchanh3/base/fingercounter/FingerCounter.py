import cv2
import os
import time
import base.fingercounter.HandTrackingModule as htm
import numpy as np
# gét gô
def nhidangdemngontay(lmList):
    temp = 0
    a = np.array((lmList[8][1],lmList[8][2]))
    b = np.array((lmList[6][1],lmList[6][2]))
    c = np.array((lmList[0][1],lmList[0][2]))
    dist1 = np.linalg.norm(a-c) #norm 2, khoảng cách euclide của 2 landmark
    dist2 = np.linalg.norm(b-c)
    if dist1 >= dist2:
        temp+=1
        
    #ngón cái
    a = np.array((lmList[4][1],lmList[4][2]))
    b = np.array((lmList[3][1],lmList[3][2]))
    
    d = np.array((lmList[17][1],lmList[17][2]))
    dist1 = np.linalg.norm(a-c) 
    dist2 = np.linalg.norm(b-c) 
    
    dist3 = np.linalg.norm(a-d)
    dist4 = np.linalg.norm(b-d)
    if dist1 >= dist2 and dist3 >= dist4:
        temp+=1
        
    # ngón giữa
    a = np.array((lmList[12][1],lmList[12][2]))
    b = np.array((lmList[10][1],lmList[10][2]))
    dist1 = np.linalg.norm(a-c)
    dist2 = np.linalg.norm(b-c)
    if dist1 >= dist2:
        temp+=1
    
    #ngón áp út
    a = np.array((lmList[16][1],lmList[16][2]))
    b = np.array((lmList[14][1],lmList[14][2]))
    dist1 = np.linalg.norm(a-c)
    dist2 = np.linalg.norm(b-c)
    if dist1 >= dist2:
        temp+=1
        
    #ngón út
    a = np.array((lmList[20][1],lmList[20][2]))
    b = np.array((lmList[18][1],lmList[18][2]))
    dist1 = np.linalg.norm(a-c)
    dist2 = np.linalg.norm(b-c)
    if dist1 >= dist2:
        temp+=1
        
    return temp
    
def fingercounter(frame):
    detector = htm.handDetector(detectionCon=0.75)
    frame = detector.findHands(frame,draw=False) #find the hand in the image capped
    
    
    #create a list of the landmarks của cái tay which is already detected
    lmList = detector.findPosition(frame,draw=False) #vẽ landmarks ở câu trên rùi giờ ko vẽ nữa, đc chưa
    # print(lmList)
    fingercount = 0
    if len(lmList)==0:
        return -1
    elif len(lmList) !=0:
        fingercount = nhidangdemngontay(lmList)
        return fingercount
