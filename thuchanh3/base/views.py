from django.shortcuts import render
from django.http import HttpResponse, StreamingHttpResponse
import cv2, time
import paho.mqtt.client as mqtt
import math
import socket
from base.fingercounter.FingerCounter import fingercounter
import base.fingercounter.HandTrackingModule as htm
# import statistics
import numpy as np
fingers=0
action ={"-1":"",
         "0":"switch steering mode",
         "1":"Go straight forward",
         "2":"Go backwards",
         "3":"Turn left",
         "4":"Turn right",
         "5":"Stop"}
# Create your views here.
def home(request):
    
   
    # print("hiiiiiiii")   
    context = {'fingers':fingers}
    return render(request, 'base/esp.html',context)
def stream():
    global fingers
    cap = cv2.VideoCapture(0) 
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 300)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 400)
    client = mqtt.Client() #create an instance use
    #Before you can publish messages or subscribe to topics you need to establish a connection to a broker hê hê hê
    ipcuanhi=socket.gethostbyname(socket.gethostname())
    client.connect(ipcuanhi, 1883, 60)
    # client.connect("localhost", 1883, 60)
    pTime = 0
    cTime = 0
    mycounter = 0
    p_fingers = -2
    countArray = []
    countArray = np.array(countArray)
    while True:
        ret, frame = cap.read()

        if not ret:
            print("Error: failed to capture image")
            break
        
        cTime = time.time()
        fps = 1 / (cTime - pTime)
        mycounter+= cTime - pTime
        pTime = cTime
        cv2.putText(frame, f'FPS: {fps:.2f}', (10, 70), cv2.FONT_HERSHEY_PLAIN, 3,
                    (255, 0, 255), 3)
        if mycounter>=0.5: #mỗi 0.3 giây sẽ đem ảnh đi đếm ngón tay một lần, xong gùi reset lại counter = 0 vì opencv delay qué sos
            if fingers >=0:
                p_fingers = fingers
            
            fingers = fingercounter(frame)
            mycounter = 0
        
        # console.log(fingers)
        # countArray.append(fingers)
            if fingers>=0 and p_fingers!=fingers: #and fingers!=0:
                client.publish("test/topic", str(fingers)) #publisher gửi tin đến cho subcriber của topic 'test/topic' ở server mqtt
        # np.append(countArray, fingers)
        # if countArray.shape[0] >= 20:
        #     steeringValue = round(np.mean(countArray))
        #     client.publish("test/topic", str(steeringValue))
        #     countArray=np.array([])
            
        cv2.imwrite('demo.jpg', frame)
        # client.publish("test/topic","1")
        
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + open('demo.jpg', 'rb').read() + b'\r\n')
        

def video_feed(request):
    return StreamingHttpResponse(stream(), content_type='multipart/x-mixed-replace; boundary=frame')


def refreshcard(request):
    context = {'fingers':action[str(fingers)]}
    return render(request,'base/steering_card.html', context)