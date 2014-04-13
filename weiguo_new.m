% initial environment
a1 = -0.01;
a4 = 0.01;
epsilon = 0.0001;
a2 = a1 + 0.382*(a4 - a1);

x = [3 7]';
S = [0.7 0.714]';
x2 = x + a2 * S;
temp = x;
x = x2;
f2 = x(1)^5 + x(2)^4 -5*x(1)^4+30*x(2)^3 +25*x(1)^3+10*x(2)^2+x(1)*x(2) - 10*x(1) - 14*x(2) + 30;
x = temp;
a3 = a1 + 0.618*(a4 - a1);
x3 = x + a3 * S;
x = x3;
f3 = x(1)^5 + x(2)^4 -5*x(1)^4+30*x(2)^3 +25*x(1)^3+10*x(2)^2+x(1)*x(2) - 10*x(1) - 14*x(2) + 30;
x = temp;

% global varies
astar = 0;x1 = 0;fstar = 0;
while 1
	if f2 < f3
		a4 = a3;a3 = a2;f3 = f2;
		a2 = a1 + 0.382*(a4 - a1);
		x2 = x + a2*S;
		x = x2;
		f2 = x(1)^5 + x(2)^4 -5*x(1)^4+30*x(2)^3 +25*x(1)^3+10*x(2)^2+x(1)*x(2) - 10*x(1) - 14*x(2) + 30;
		x = temp;
	else
		a1 = a2;a2 = a3;f2 = f3;
		a3 = a1 + 0.618*(a4 - a1);
		x3 = x + a3 * S;
		x = x3;
		f3 = x(1)^5 + x(2)^4 -5*x(1)^4+30*x(2)^3 +25*x(1)^3+10*x(2)^2+x(1)*x(2) - 10*x(1) - 14*x(2) + 30;
		x = temp;
	end
	if abs(a4 - a1) > epsilon
		continue;
	else
		astar = (a1 + a4)/2;
		a1,a4
		x1 = x + astar * S;
		x = x1;
		fstar = x(1)^5 + x(2)^4 -5*x(1)^4+30*x(2)^3 +25*x(1)^3+10*x(2)^2+x(1)*x(2) - 10*x(1) - 14*x(2) + 30;
		x = temp;
		break;
	end
end
astar,fstar


% 
e=0.001;
while 1
	s1 = (f3 - f2) / (a3 - a2);
	s2 = ((f2 - f1) / (a2 - a1) - s1) / (a2 - a3);
	if s2==0
		f1 = f2;
		x1 = x + a2 * s;
		break
	else
		a0 = (a1 + a3 - s1 / s2) / 2;
		if (a0 - a1) * (a3 - a0) < 0
			f1 = f2;
			x1 = x + a2 * s;
			break
		else
			x0 = x + a4 * s;
			f0 = x0(1)^5 + x0(2)^4 -5 * x0(1)^4+30*x0(2)^3 +25*x0(1)^3+10*x0(2)^2+x0(1)*x0(2) - 10*x0(1) - 14*x0(2) + 30;
			if abs(a0 - a2) < e
				if f0<f2
					f1 = f0;
					x1 = x0;
					break
				else
					f1 = f2;
					x1 = x + a2 * s;
					break
			else
				if (a0-a2) < 0
					if f2 < f0
						a1 = a0;
						f1 = f0;
						continue
					else
						a3 = a2; f3 = f2;
						a2 = a0; f2 = f0;
						continue
				else
					if f2 < f0
						a3 = a0;
						f3 = f0;
						continue
					else
						a1 = a2; f1 = f2;
						a2 = a0; f2 = f0;
						continue
					end
				end
			end
		end
	end
end
