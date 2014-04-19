f = fopen('danci.txt','r');
data = fread(f);
[m,n] = size(data);
number_l = 0;
number_r = 0;
index_l = 0;
index_r = 0;
% 记录< > 出现的位置
for i = 1:m
	if data(i) == '<'
		number_l = number_l + 1;
		index_l(number_l) = i;
	end

	if data(i) == '>'
		number_r = number_r + 1;
		index_r(number_r) = i;
	end
end
fclose(f);
n = 0;
% 取出去掉<>后的字符串
x = {};y = {};z = {};
while 8*n+2 < number_r
	x{n+1} = char(data(index_r(8*n+2)+1 : index_l(8*n+3)-1))';
	y{n+1} = char(data(index_r(8*n+4)+1 : index_l(8*n+5)-1))';
	z{n+1} = char(data(index_r(8*n+6)+1 : index_l(8*n+7)-1))';
	n = n+1;
end
% 乱序排列 mark中存有随机的顺序
m = n;
mark = zeros(m,1);
rand('state',sum(100*clock));
randnum = round(rand(m,1) * m);
for i = 1:m
	if mark(randnum(i)) == 0
		mark(randnum(i)) = i;
	else
		j = randnum(i);
		j = j+1;
		if j > m
			j = 1;
		end
		while mark(j) ~= 0
			j = j+1;
			if j > m
				j = 1;
			end
		end
		mark(j) = i;
	end
end
% mark

newfile = fopen('new.txt','wb');
for i = 1:m
	fprintf(newfile, '<tr class=''ps0''><td>%s</td><td>%s</td><td>%s</td><tr>', x{mark(i)},y{mark(i)},z{mark(i)});
end
fclose(newfile)