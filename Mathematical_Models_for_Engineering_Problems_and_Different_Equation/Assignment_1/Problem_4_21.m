clear all
format short e

a_a = [[0 0.9231 0 0 0 0 0 0]; 
    [-1 -0.3846 0 0 0 0 0 0]; 
    [0 0 0 0 1 0 0.8575 0]; 
    [1 0 -0.7809 0 0 0 0 0];
    [0 -0.3846 -0.7809 0 -1 0.3846 0 0]; 
    [0 0.9231 0.6247 0 0 -0.9231 0 0];
    [0 0 0.6247 -1 0 0 0 0]; 
    [0 0 0 1 0 0 -0.5145 -1]];
a_b = [1690; 3625; 0; 0; 0; 0; 0; 0];

b_a = [[0, 3, 8, -5, -1, 6]; 
    [3, 12, -4, 8, 5, -2]; 
    [8, 0, 0, 10, -3, 7];
    [3, 1, 0, 0, 0, 4];
    [0, 0, 4, -6, 0, 2]; 
    [3, 0, 5, 0, 0, -6]];
b_b = [34; 20; 45; 36; 60; 28];

SolutionA = GaussPivotLarge(a_a, a_b)
SolutionB = GaussPivotLarge(b_a, b_b)