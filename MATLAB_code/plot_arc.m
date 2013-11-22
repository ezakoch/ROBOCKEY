function P = plot_arc(a,b,h,k,r)
% Plot a circular arc.
% a is start of arc in radians, 
% b is end of arc in radians, 
% (h,k) is the center of the circle.
% r is the radius of the circle

t = linspace(a,b);
x = r*cos(t) + h;
y = r*sin(t) + k;
plot(x,y);

end