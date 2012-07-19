// 这里应该 include具体项目的App类的头文件
#include "3DBookApplication.h"

extern "C" 
{

// 替换C3DBookApplication为你的项目的App类
// 替换下列接口内的实现,到你的App对应的实现接口上.
C3DBookApplication* __attribute__((used, noinline)) _CreateC3HTMLApplication(const char* pszResRootDir)
{
    // 实现创建App类实例
	return new C3DBookApplication(pszResRootDir);
}

bool __attribute__((used, noinline)) emscripten_bind_C3HTMLApp_Setup(C3DBookApplication* self, const char* pszBookCnfgFileName)
{
   // 实现App的初始化
  return self->Setup(pszBookCnfgFileName);
}

void __attribute__((used, noinline)) emscripten_bind_C3HTMLApp_Destroy(C3DBookApplication* self)
{
  // 实现App的销毁
  self->Destroy();
}

void __attribute__((used, noinline)) emscripten_bind_C3HTMLApp_DoIdle(C3DBookApplication* self)
{
  // 实现App的帧循环和逻辑处理
  self->DoIdle();
}

void __attribute__((used, noinline)) emscripten_bind_C3HTMLApp_OnBeginTouch(C3DBookApplication* self, C3_POS pos)
{
   // 实现鼠标点击
   self->OnBeginTouch(pos);
}

void __attribute__((used, noinline)) emscripten_bind_C3HTMLApp_OnEndTouch(C3DBookApplication* self, C3_POS pos)
{
	// 实现鼠标结束点击
    self->OnEndTouch(pos);
}

void __attribute__((used, noinline)) emscripten_bind_C3HTMLApp_OnTouchMove(C3DBookApplication* self, C3_POS posMoved, C3_POS ptMovedFromBegin)
{
	//  实现鼠标移动
    self->OnTouchMove(posMoved, ptMovedFromBegin);
}

}
