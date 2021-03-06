**MyminiWebserver**

尝试通过看书和查找，自己搭建一个简易的服务器

------------

应用技术：Linux、C++11、Socket网络编程、TCP协议、http协议，MySQL数据库。

项目描述：此项目基于Linux的轻量级多线程web服务器，在应用层实现了一个简单的http服务器，使用线程池技术实现了多并发，将服务器与MySQL数据库关联，实现了用户的注册与登录，并为服务器添加了一个简易的日志系统。

主要工作：

（1）实现**GET**、**POST**两种http请求方法，使用**状态机**解析HTTP报文，采用CGI脚本进行post响应。

（2）利用**线程池技术+非阻塞seocket+epoll（ET模式）+事件处理（模拟Proactor）**实现了多用户并发访问，增加并行服务数量（默认开启两个线程，可以手动更改）。

（3）利用MySQL数据库，将用户的数据存储在MySQL数据库中，用户在首次访问时可以进行注册，后续可以直接输入用户名、密码进行登录。

（4）使用**异步日志系统**，为web服务器的运行，提供了日志功能，记录服务器运行状态，方便维护查询。


------------

**框架**


![image](https://user-images.githubusercontent.com/69040952/154946673-c8a60a4a-62d3-457b-9860-cbcebd41f2c2.png)

------------
**Demo演示**

------------
> * 打开网页
> 
![image-20220221120546528](https://user-images.githubusercontent.com/69040952/154888804-05efd7e2-73bd-45ed-a9b1-38872ffb3782.png)
------------
> * 登录失败
> 
![image-20220221120653817](https://user-images.githubusercontent.com/69040952/154888648-b9efd319-025c-4f62-addf-17ad2f57f1ae.png)
------------
> * 注册账号
> 
![image-20220221120821644](https://user-images.githubusercontent.com/69040952/154888665-a330289d-4ff0-4838-afe7-12ba52552a22.png)
------------
> * 注册成功
> 
![image-20220221120835141](https://user-images.githubusercontent.com/69040952/154888680-f154ea75-9fd4-404c-a9bf-40673ba0073d.png)
------------
> * 返回登录界面
> 
![image-20220221121026860](https://user-images.githubusercontent.com/69040952/154888698-64ebad88-267b-4822-87c5-f49becf0d585.png)
------------
> * 登录成功
> 
![image-20220221120939771](https://user-images.githubusercontent.com/69040952/154888712-2d762826-fb43-4f63-8987-18c26be85194.png)


致谢
------------
Linux高性能服务器编程，游双著.
Linux多线程服务端编程，陈硕著
