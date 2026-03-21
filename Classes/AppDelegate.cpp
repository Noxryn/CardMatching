/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "CardMatchingScene.h" // 引入我们要运行的第一个场景头文件

 // 定义使用的音频引擎宏
 // #define USE_AUDIO_ENGINE 1
 // #define USE_SIMPLE_AUDIO_ENGINE 1

 // 编译检查：不能同时使用两种音频引擎
#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

// 根据宏定义包含相应的音频引擎头文件和命名空间
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC; // 使用 Cocos2d-x 命名空间

// 定义几种预设的分辨率尺寸
// designResolutionSize: 设计分辨率，逻辑坐标系的基准
static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
// smallResolutionSize: 小屏设备参考分辨率
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
// mediumResolutionSize: 中屏设备参考分辨率 (如 iPad 1/2)
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
// largeResolutionSize: 大屏设备参考分辨率 (如 iPad Retina)
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    // 析构时释放音频引擎资源
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// 如果需要不同的 OpenGL 上下文配置，可以修改 glContextAttrs
// 这会影响所有平台
// 上下文初始化
void AppDelegate::initGLContextAttrs()
{
    // 设置 OpenGL 上下文属性: 
    // 顺序为: red(红), green(绿), blue(蓝), alpha(透明度), depth(深度缓冲), stencil(模板缓冲), multisamplesCount(多重采样数)
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// 如果你想使用包管理器安装更多包，请不要修改或删除此函数
static int register_all_packages()
{
    return 0; // 包管理器的标志位
}

// 应用启动完成后的初始化逻辑（核心部分）
bool AppDelegate::applicationDidFinishLaunching() {
    // 1. 初始化导演类 (Director)，它是整个游戏的控制中枢
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    // 如果还没有创建 OpenGL 视图，则创建它
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        // 桌面端：创建一个指定大小和位置的窗口
        // 窗口标题："CardMatching", 位置(0,0), 大小 1080x2080, 缩放因子 0.5 (为了在屏幕上显示得下)
        glview = GLViewImpl::createWithRect("CardMatching", cocos2d::Rect(0, 0, 1080, 2080), 0.5);
#else
        // 移动端：创建全屏视图
        glview = GLViewImpl::create("CardMatching");
#endif
        director->setOpenGLView(glview);
    }

    // 开启显示 FPS (帧率) 统计信息 (左上角)
    director->setDisplayStats(true);

    // 设置动画间隔，即目标帧率。默认是 1.0/60 (60 FPS)
    director->setAnimationInterval(1.0f / 60);

    // glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    // 设置设计分辨率为 1080x2080 保持宽度固定为 1080，高度根据屏幕比例自动调整。
    glview->setDesignResolutionSize(1080, 2080, ResolutionPolicy::FIXED_WIDTH);

    auto frameSize = glview->getFrameSize();

    // 根据屏幕物理尺寸动态调整内容缩放因子

    // 如果屏幕高度大于中屏参考高度 (768)
    if (frameSize.height > mediumResolutionSize.height)
    {
        // 计算大屏的缩放比例，取宽高比的最小值，防止拉伸
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    // 如果屏幕高度大于小屏参考高度 (320) 但小于等于中屏
    else if (frameSize.height > smallResolutionSize.height)
    {
        // 计算中屏的缩放比例
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    // 如果屏幕很小
    else
    {
        // 计算小屏的缩放比例
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    // 注册所有必要的包
    register_all_packages();

    ////////////////////////////
    
    // 创建并获取第一个场景
    auto scene = CardMatching::createScene();




    //////////////////////////////

    // 运行场景：导演类开始渲染和更新这个场景
    director->runWithScene(scene);

    return true;
}

// 当应用进入后台时调用
void AppDelegate::applicationDidEnterBackground() {
    // 停止动画循环，节省电量和 CPU
    Director::getInstance()->stopAnimation();

    // 暂停音频播放
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// 当应用从后台返回前台时调用
void AppDelegate::applicationWillEnterForeground() {
    // 恢复动画循环
    Director::getInstance()->startAnimation();

    // 恢复音频播放
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}