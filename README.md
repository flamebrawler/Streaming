# Streaming
This is a project that I made in grade 12 computer engineering.

The purpose of this project is to stream video from one computer to any number of other client computers.

This project uses socket programming with the winsock2.h library(or socket.h on linux)(implementation of linux support was dicontinued due to how I couldn't use linux for video output)
The client also displays images using the window from the windows.h library which is a very low level video output library.

The server has two threads running at the same time to accept new clients and to send each of the clients a new image from the video. 

The client also has two threads running at the same time where one thread is to receive images and to put them into a buffer and the other is for dispaying the images on the screen.


