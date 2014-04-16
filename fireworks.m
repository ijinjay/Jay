function firework
% 烟花烟花满天飞
% CopyRight：xiezhh(谢中华)
% 2011.6.25

OldHandle = findobj( 'Type', 'figure', 'Tag', 'FireWork' ) ;
if ishandle(OldHandle)
    close(OldHandle) ;
end
% 图形窗口初始化
fig = figure('units','normalized','position',[0.1 0.1 0.8 0.8],...
    'menubar','none','name','烟花满天飞（谢中华制作）',...
    'numbertitle','off','color',[0 0 0],'tag','FireWork');
% 烟花炸开前的初始位置
h0 = line(0,0,0,'marker','.',...
    'markersize',18,...
    'MarkerEdgeColor',[1 1 1],...
    'MarkerFaceColor',[1 1 1],...
    'EraseMode' , 'xor',...
    'color',[0 0 0]);
% 设置坐标系显示属性
axis equal
axis([-50 50 -50 50 0 100])
axis off
view(-42,22)

% 设置参数
rate = 1:-0.01:0;  % 颜色衰减率
v0 = 45;  % 烟花头初始速度
g = -9.8;   % 重力加速度
usedtime = -v0/g;  % 烟花头爆炸前所经历时间
zs = v0*usedtime+0.5*g*usedtime^2;  %烟花头爆炸前达到的最高高度

theta = rand(250,1)*2*pi;  % 各粒子速度的方位角
phi =  rand(250,1)*2*pi-pi;  % 各粒子速度的仰角
age = 20;  % 粒子生存期
% 常用颜色矩阵
colormat = [1.0  0.5  0.5
    1.0  0.75  0.5
    1.0  1.0  0.5
    0.75  1.0  0.5
    0.5  1.0  0.5
    0.5  1.0  0.75
    0.5  1.0  1.0
    0.5  0.75  1.0
    0.5  0.5  1.0
    0.75  0.5  1.0
    1.0  0.5  1.0
    1.0  0.5  0.75];
% 随机产生各粒子对应的颜色序号
colorid = randsample(12,250,true);
% 粒子对应的颜色矩阵
colormat = colormat(colorid,:);
% 粒子颜色与背景色（夜色）的距离
colordist = sqrt(sum(colormat.^2,2));
v1 = 20;  % 粒子的初始速度
k = 1;   % 颜色衰减率初始序号

timerA = timer('TimerFcn',@TimerFcnA,...
    'executionmode','fixedspacing','Period',0.001);
start(timerA);
h = getappdata(gcf,'HandleParticle');
timerB = timer('TimerFcn',{@TimerFcnB},...
    'executionmode','fixedspacing','period',0.001);

%--------------------------------------------------------------------------
%  定时器回调函数（烟花头）
%--------------------------------------------------------------------------
function TimerFcnA(timerA,event)
ta = get(timerA,'TasksExecuted')*0.1;
if ta <= usedtime
    z = v0*ta+0.5*g*ta^2;
    set(h0,'zdata',z,...
        'MarkerEdgeColor',[1 1 1],...
        'MarkerFaceColor',[1 1 1])
    drawnow
    %pause(0.01)
else
    delete(h0)
    stop(timerA);
    x0 = zeros(2,250);
    y0 = zeros(2,250);
    z0 = zs*ones(2,250);
    h = line(x0,y0,z0,'marker','h',...
        'markersize',12,...
        'MarkerEdgeColor',[1 1 1],...
        'MarkerFaceColor',[1 1 1],...
        'EraseMode' , 'xor',...
        'color',[0 0 0]);
    setappdata(gcf,'HandleParticle',h);
    start(timerB);
end

end
%--------------------------------------------------------------------------
%  定时器回调函数（粒子）
%--------------------------------------------------------------------------
function TimerFcnB(timerB,event)
tb = get(timerB,'TasksExecuted')*0.15;
if age>0 && any(colordist>=0.05)
    colormat = colormat*rate(k);
    colordist = sqrt(sum(colormat.^2,2));
    for i = 1:250
        xi = v1*cos(phi(i))*cos(theta(i))*tb;
        yi = v1*cos(phi(i))*sin(theta(i))*tb;
        zi = zs+v1*sin(phi(i))*tb+0.5*g*tb^2;
        set(h(i),'XData',xi,'YData',yi,'ZData',zi,...
            'MarkerEdgeColor',colormat(i,:),...
            'MarkerFaceColor',colormat(i,:))
    end
    drawnow
    age = age-0.1;
    k = k+1;
else
    stop(timerB);
    delete(h)
end

end
%-------------------
end