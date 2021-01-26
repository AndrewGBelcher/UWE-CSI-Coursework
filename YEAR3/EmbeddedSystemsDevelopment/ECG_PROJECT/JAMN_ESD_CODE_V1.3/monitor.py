import serial # import Serial Library
import numpy  # Import numpy
import matplotlib.pyplot as plt #import matplotlib library
import time
from drawnow import *
import sys
 
tempF= []
pressure=[]
arduinoData = serial.Serial('/dev/ttyS9', 230400) #Creating our serial object named arduinoData
plt.ion() #Tell matplotlib you want interactive mode to plot live data
cnt=0
HR = 0
bpm = 0
bpm_detect = False
R1time = 0
R2time = 0
pulsecnt = 0
def makeFig(): #Create a function that makes our desired plot
    plt.ylim(0,5000)                                 #Set y min and max values
    plt.xlim(0,100)                                 #Set y min and max values
    plt.title('Heart rate monitor HR:'+ str(int(bpm)))      #Plot the title
    plt.grid(True)                                  #Turn the grid on
    plt.ylabel('Raw pulse siganl')                            #Set ylabels
    plt.plot(tempF, 'r-', label='ADC values')       #plot the temperature
    plt.legend(loc='upper left')                    #plot the legend

    
tempF.append(0)                     #Build our tempF array by appending temp readings
tempF.append(0)                     #Build our tempF array by appending temp readings
tempF.append(0)                     #Build our tempF array by appending temp readings
tempF.append(0)                     #Build our tempF array by appending temp readings
tempF.append(0)                     #Build our tempF array by appending temp readings
 
while True: # While loop that loops forever
    while (arduinoData.inWaiting()==0): #Wait here until there is data
        pass #do nothing

    arduinoString = arduinoData.read(arduinoData.in_waiting)                    #read the line of text from the serial port
    dataArray = arduinoString.split('\n')   #Split it into an array called dataArray

    while(dataArray[0]=='\n' or dataArray[0]==''):
        arduinoString = arduinoData.read(arduinoData.in_waiting)                    #read the line of text from the serial port
        dataArray = arduinoString.split('\n')   #Split it into an array called dataArray


    print(repr(dataArray[0]))

    temp = int(dataArray[0])            #Convert first element to floating number and put in temp

    if(len(tempF) > 100):
        tempFnew = tempF[100:]
        tempF = tempFnew

    tempF.append(temp)


    if(len(tempF) >5):
        HR = tempF[len(tempF)-2] - tempF[-1]

    if(HR>100):
        bpm_detect = True
    else:
        bpm_detect = False

    if(bpm_detect):
        pulsecnt = pulsecnt + 1
        if((pulsecnt % 2)==0):
            R1time = time.time()
            bpm = (R1time - R2time)# * 60
        else:
            R2time = time.time()
        
            bpm = (R2time - R1time)# * 60

    # else:
        # bpm = bpm


    drawnow(makeFig)                       #Call drawnow to update our live graph
    #plt.pause(.000001)                     #Pause Briefly. Important to keep drawnow from crashing
    cnt=cnt+1
