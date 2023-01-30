function [t, y] = solvesystem_espino98 (x1, x2, t0, tN, x0, h)
% This function solves a system of two differential equations

s = round(((tN-t0)/h),0); % This will allow us to create our solution vector, rounded to nearest integer
t = t0:h:tN; % Step size
y = zeros(2,s); % Solutions matrix size 2xnumber of steps
y(1,1) = x0(1); % Alocating initial conditions
y(2,1) = x0(2);
    
    for i = 1:s-1 % 1 to 100

        next_x1 = y(1,i) + h * (x1(t(i), y(1,i), y(2,i)));
        
        next_x2 = y(2,i) + h * (x2(t(i), y(1,i), y(2,i)));

        y(1,i+1) = y(1,i) + (x1(t(i), y(1,i), y(2,i)) + x1((t(i)+h), next_x1, next_x2))*(h/2);
       
        y(2,i+1) = y(2,i) + (x2(t(i), y(1,i), y(2,i)) + x2((t(i)+h), next_x1, next_x2))*(h/2);

    end
