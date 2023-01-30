%Used and coded in mathlab in order to create a function that solves an ODE through Eulers Method. 

function [IEM] = ODEsolver(f, t0, tN, y0, h) %calling the function
% t0 and tN are the start and end points of the interval on which to solve the ODE 
% y0 is the initial condition of the ODE
% h is the stepsize
        
    t = t0:h:tN;
    y = zeros(size(t)); %The solution's matrix
    y(1) = y0;
    %n = (tN-t0)/h

    for i = 1:length(t)-1 % 1 to 100

        %f(t(i), y(i)) = (y(t+h)-y(t))/h; %This is my derivative

        next = y(i) + h * (f(t(i), y(i)));

        y(i+1) = next;
      
        %y(i) + (f(t(i), y(i)) + f(t(i+1),next))*(h/2);

    end

    [IEM] = struct('t', t, 'y', y);
