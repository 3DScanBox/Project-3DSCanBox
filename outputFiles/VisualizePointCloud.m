clc;

xyzPoints = dlmread('PointCloud.txt');
pcshow(xyzPoints, [1 0 0])