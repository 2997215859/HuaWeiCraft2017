# 2017华为软件精英挑战赛初赛解决方案

## 前言

2017年参加该比赛，参赛结果不好，18年初恰逢须要交人工智障课的寒假作业，于是决定重新解决一下该比赛的初赛试题。

上次做的结果不好，主要是因为大方向没把握好，到快截止的最后几天才转向启发式算法，导致没有更多时间进行优化，另外就是去年用JAVA没用C++，这点在每次算法费用流的时候会很吃亏，使得启发式算法迭代次数比没有用C++的要少很多。于是，这次用C++实现了各部分代码。

## 算法思路

模拟退火遗传算法 + 最小费用最大流（SPFA算法），详见课程论文[PDF](https://github.com/2997215859/HuaWeiCraft2017/blob/master/Paper/main.pdf)和代码

## 补充说明

1. 主办方提供的代码环境是在linux下，本仓库是作了稍许修改以在window下开发和调试

## 仓库结构
```
├─HuaWeiCraft2017          % 代码目录，具体实现代码都在该目录下
   ├─...                   % 各种代码
   └─case_example          % 测试用例
├─Paper                    % 论文
   ├─main.pdf              % 论文
   └─main.tex              % 论文tex源文件
├─HuaWeiCraft2017.sln      % window解决方案文件，克隆仓库之后vs直接点击打开这个就行了
├─README                   % 你正在看的文件 -_-" 这不用介绍了吧
```

## 参考文献和博客

1. [2017华为软件精英挑战赛参赛心得](http://www.netcan666.com/2017/03/23/2017%E5%8D%8E%E4%B8%BA%E8%BD%AF%E4%BB%B6%E7%B2%BE%E8%8B%B1%E6%8C%91%E6%88%98%E8%B5%9B%E5%8F%82%E8%B5%9B%E5%BF%83%E5%BE%97/)
2. [单源最短路径（3）：SPFA 算法](https://www.61mon.com/index.php/archives/196/)
3. [2017华为软件精英挑战赛--总结](http://blog.csdn.net/zc20122690/article/details/69926375)
4. [2017华为软件精英挑战赛初赛体验及最终代码](https://www.cnblogs.com/jhmu0613/p/6844133.html)
5. [2017 华为软件精英挑战赛](http://blog.csdn.net/firenet1/article/details/63686904)
6. [poj_2315 最小费用最大流](http://www.cnblogs.com/gtarcoder/p/4890746.html)
