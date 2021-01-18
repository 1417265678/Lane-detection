all: main 

main:main.cpp /home/zhb/Desktop/qqw/process_image.cpp
	g++ -o main main.cpp /home/zhb/Desktop/qqw/process_image.cpp `pkg-config --cflags --libs opencv`


clean: 
	rm main
