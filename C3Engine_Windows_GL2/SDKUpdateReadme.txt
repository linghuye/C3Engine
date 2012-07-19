Note: 从11_11_28版本开始C3不再支持主频小于600的android设备.


====10.13=======================================
1.修正BeginDrawClipMask的Bug

====10.21=======================================
1.加入CGameDataSet::EnableReturnDefaultAni,含义见接口头文件.
2.修正TargetBmp 在屏幕颠倒时的显示Bug

====11.21=======================================
1.加入.so替换wdf功能
2.加入
   CMyBitmap::SetImagePixelData2D
   extern "C" GRAPHIC_API CMyBitmap* MyBitmapCreateEmpty(DWORD dwWidth, DWORD dwHeight);

12.20
1.修正ShowString函数的secondcolor错误问题.

2012.1.6
1.修正挖洞接口的内存溢出Bug.

1.10
1.修正文字透明问题

2012.3.7
1.添加CMyBitmap::DrawEx支持浮点精度的2D渲染,保证计算的误差.

2012.3.8
2.修正纹理动画长时间运行时效果错误问题.

2012.4.25
1.修正CMyBitmap::WorldToScreen的概念,使之符合现在Screen是当前转向下的屏幕概念.