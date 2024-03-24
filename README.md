# README  
Project 4: Calibration and Augmented Reality
Spring 2024  
CS 5330 Northeastern  
Professor Bruce Maxwell, PhD
___

### Group Member Names:
* Joseph Nelson Farrell 
* Harshil Bhojwani

___

### Links/Urls:

https://northeastern.zoom.us/rec/play/eVsXL1KlAwmPj87eFdcDAVrTNNczApLvkEq5_EauwajsW12EVJEXK0C7k8fZF5nqvOtwSTZkrQ9wb_xY.Uks3sV8OLcNRJzSi?canPlayFromShare=true&from=my_recording&continueMode=true&componentName=rec-play&originRequestUrl=https%3A%2F%2Fnortheastern.zoom.us%2Frec%2Fshare%2FIyylbOCAS2n55FWxjsnpv-fxS-iKNp7PA-P-VqZRu2UIE7C7gk0ksPA6nXPllPcm.GcPTLRl4esZXtgkP

This is a link the video demo (it is also in the report):


___

### Operating System & IDE:
* MacOS
* Visual Studio Code

___

### Time Travel Days:
* 2

___
## Executing the Program:

This requires your iPhone be connected to your computer.

It also requires a checkerboard pattern to use as the target.

A different target can be used for steps 11 - 12; Harris corner detection.

### Step 1: Run calibrate.cpp

To execute the program that will calibrate the camera the user has to run this executable:
```bash
./ar
```

### Step 2: Save Images with Features Detected

Next, the user has to select frames to use in calibration. To select frames the user must press:

```
s
```
This will add the last frame where all the points were detected in the checkerboard to a vector called ```points_set```. 

### Step 3: Calibrate Camera
To calibrate the camera the user must press:
```
c
```

This requires ```points_set``` contains at least 5 frames. If it does not contain 5 frames the user will see this message:
```
No, no, no...We need at least 5 frames!
We only have: 0
```

Then the user must press any key to continue selecting frames.

If 5 frames are the present, the user will see this message:
```
_________________________________________
 
Distortion Coefficients: Pre Calibration
Empty
_________________________________________
 
Camera Matrix: Pre Calibration 
1 0 960 
0 1 540 
0 0 1 
_________________________________________
 
Distortion Coefficients: Post Calibration
0.36583, 1.50272, -0.0225171, 0.115271, -6.56414, 
_________________________________________
 
Camera Matrix: Post Calibration 
2181.94 0 1217.47 
0 2047.72 594.423 
0 0 1 
_________________________________________
 
Reprojection Error = 1.65882
_________________________________________
```
This indicates the camera has been calibrated and the data stored in the camera matrix and distortion coefficients vector have been saved to a ```calibration_data.csv```.

The user in now free to quit the program and run ```project_points.cpp```, the AR program.


### Step 4: Exit ./ar

To exit press: 
```
q
```
Program Terminates.

### Step 5: Generate AR with project_points.cpp
To create the augmented reality objects the user now needs to execute this executable:
```bash
./pr
```
### Step 6: Display World Axes in 2D
To display the AR projected axes the user must press
```
a
```
This will displays the world axes: x, y, and z, on the target.

### Step 7: Display Diamond
To display an AR floating diamond the user must press:
```
d
```
This will display a diamond hovering over the target.

### Step 8: Display Rectangles
To display AR rectangles the user must press 
```
r
```
This displays rectangles on the target.

### Step 9: Extension - Display Mountain Scene
To display the AR mountian scene the user must press 
```
c
```
The displays an AR generated mountain scene that completely obscures the target.

### Step 10: Exit ./pr
The user can run the program as many times as they wish and switch between displays.  

To exit press: 
```
q
```
Program Terminates.

### Step 11: Display Harris Corners
To display the Harris corners on a target the user must run this executable: 

```
./hr
```

This will display the Harris corners corners found in the source image.

### Step 12: Exit ./hr
  
To exit press: 
```
q
```
Program Terminates.


