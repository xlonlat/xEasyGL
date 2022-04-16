# xEasyGL

English content is translated by Google (including this sentence).

## Purpose | 目的

    This project is for people to develop OpenGL programs faster and easier, and avoid too many choices, such as glut, glfw, glew, glad, glu, win32 window, mfc window, etc., and also hope to quickly build a project development environment without requiring too much configuration.

    这个项目是为了人们能够更快更容易的开发OpenGL程序，避免进行过多的选择，例如glut、glfw、glew、glad、glu、win32窗口、mfc窗口等等，也希望能够快速的搭建项目开发环境而不需要过多的配置。

---  
    The solution of the project is currently based on vs2010, which is already 12 years old software (now 1:49 am on April 17, 2022), I think it is old enough, the advantage of doing this is if You are using a more advanced version of VS, you only need to click the mouse to upgrade these projects while opening xEasyGL.sln, I am tired of compiling for different development environments on different computers. Various open source libraries! What I want to do is to install a VisualStudio (version greater than or equal to 2010) on any computer without any other configuration, and you can develop OpenGL immediately (of course, you must have a graphics card driver).

    项目的解决方案目前是基于vs2010搭建的，这已经是12年前的软件了(现在是2022年4月17号凌晨1点49分)，我认为已经够旧了吧，这样做的好处是如果你用的是vs更高级的版本，你只需要在打开下xEasyGL.sln的同时点下鼠标把这些项目升级了就可以，我已经受够了在不同电脑上针对不同的开发环境不停的编译各种开源库了！我想做的就是任何电脑上只要安装一个VisualStudio（版本大于等于2010）不需要做其他任何的配置，就可以立马开发OpenGL（当然得有显卡驱动）。

---
## Dependencies | 项目依赖

### 1.GLEW

    The difference between glew and glad is that glad can provide a more concise list of api interfaces (modern OpenGL). Their essence is nothing more than defining a series of function pointers and initializing these functions from the graphics card driver. But xEasyGL is not a project for "ModernOpenGL", it just provides a lighter OpenGL development environment for some beginners, and it can also quickly develop some interesting demos. Of course, I don't want to write so many Shaders in the demo stage. .

    对于glew和glad的区别，主要是glad能够提供更为简洁的api接口列表（现代OpenGL），它们的本质无非都是定义一系列函数指针，并且从显卡驱动初始化这些函数。但是xEasyGL并不是一个面向于“ModernOpenGL”的项目，它只是为一些初学者提供更轻便的OpenGL开发环境，而且也能够快速的开发一些有意思的Demo，当然在Demo阶段我也不想写那么多的Shader。

---
### 2.GLFW

    GLFW and GLUT are almost the same in function. They are both cross-platform and very lightweight, which is an important reason for me to choose it. xEasyGL only cares about rendering related content so no GUI is needed, and I don't want to be on win32 or xlib. Do a whole bunch of system window settings.

    GLFW与GLUT功能上几乎是一样，他们都是跨平台的，而且非常轻量化，这也是我选择它的重要理由，xEasyGL只关心渲染相关的内容所以不需要GUI，我也不想在win32或者xlib上进行一大堆的系统窗口设置。

---
### 3.glm

    A math library is definitely essential for developing graphics programs, and glm is definitely the most used library in OpenGL development. (not yet added)

    开发图形程序一个数学库肯定是必不可少了，在OpenGL开发中glm肯定是人们使用最多的库了。（目前还没有加）

---
### 4.stb

    It is very easy to use stb to load images. This is probably the easiest interface I have ever used to decode images. With it, there is no need to use libraries such as libjpeg and libpng.

    使用stb加载图片是非常容易的，这可能是我用过解码图片最简单的接口了，有了它就不需要使用libjpeg和libpng这些库了。

---
### x.etc...

    ...

    。。。

---

## Future | 未来

    The next development route should be:

    -->First build a basic rendering architecture based on "Window+View+Layer";

    --> Then the event system (including mouse, keyboard);

    --> Then the camera system (a variety of cameras may be developed, including ArcBall, God's perspective, etc.);

    --> Then the package shader (should only have vertices and fragments);

    --> Then encapsulate a useful texture object;

    --> Then encapsulate a useful model object, maybe use Assimp;

    --> Then encapsulate some commonly used math libraries, many methods in glm are not very comfortable to use;

    --> Hmm. . . I haven't thought about the rest yet, and I will add it slowly. Since the name of this project is "easy", I won't add too many things. After all, the original intention of this project is to build an OpenGL environment.

---
    接下来的开发路线应该是：

    -->先搭建一套基于“Window+View+Layer”的基础渲染架构；

    -->然后是事件系统（包括鼠标、键盘）；

    -->然后是相机系统（可能会开发多种相机，包括ArcBall、上帝视角等）；

    -->然后是封装着色器（应该只有顶点和片段）；

    -->然后封装一个好用的纹理对象；

    -->然后封装一个好用的模型对象，可能会用Assimp吧；

    -->然后封装一些常用的数学库，glm里面很多方法用着不是很舒服；

    -->嗯。。。剩下的还没想好，慢慢补充，由于这个项目的名字就是“easy”，所以不会加太多东西，毕竟这个项目的初衷只是搭建OpenGL环境而已。

---

<p align="right">2022.04.17 02:09 in Hangzhou.</p>



