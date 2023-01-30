function [AEM] = exercise4(f, t0, tN, y0, h)
    
    % Parameters
    t = t0:h:tN;
    tol = 1*10^(-8);
    y = zeros(size(t)); %The solution's matrix
    y(1) = y0;
    n = 1;

    while (t(n) < (tN-h)) % 1 to 100
        % Calling the difference

        [D, Z] = difference(f, t(n), y(n), h);

        if (abs(D) < tol)
            y(n+1) = Z + D;
            t(n+1) = t(n)+h;
        elseif (abs(D)>=tol)
            n = n-1;
            h = 0.9*h*min(max(tol/abs(D),0.3),2);
        end
        n = n+1;
        AEM = struct('t', t, 'y', y);
    end 
end 

% Helper function for error estimate 
function [D,Z] = difference(f, t, y, h)
    
    % Slope given from f
    s = f(t, y);
    
    % one Euler step of size h  
    Y = y + (h*s);
   
    % two successive Euler steps of size h/2
    Z_1 = y + ((h/2)*s);
    Z = Z_1 + (h/2)*f(t+(h/2), Y);

    D = Z - Y;

end 
