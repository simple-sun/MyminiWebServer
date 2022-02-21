**# webserver1**

尝试通过看书和查找，自己搭建一个简易的服务器



应用技术：Linux、C++11、Socket网络编程、TCP协议、http协议，MySQL数据库。

项目描述：此项目基于Linux的轻量级多线程web服务器，在应用层实现了一个简单的http服务器，使用线程池技术实现了多并发，将服务器与MySQL数据库关联，实现了用户的注册与登录，并为服务器添加了一个简易的日志系统。

主要工作：

（1）实现**GET**、**POST**两种http请求方法，使用**状态机**解析HTTP报文，采用CGI脚本进行post响应。

（2）利用**线程池技术+非阻塞seocket+epoll（ET模式）+事件处理（模拟Proactor）**实现了多用户并发访问，增加并行服务数量。

（3）利用MySQL数据库，将用户的数据存储在MySQL数据库中，用户在首次访问时可以进行注册，后续可以直接输入用户名、密码进行登录。

（4）使用**异步日志系统**，为web服务器的运行，提供了日志功能，记录服务器运行状态，方便维护查询。





**框架**



![image-20220221120227165](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120227165.png)





**Demo演示**

![image-20220221120546528](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120546528.png)

![image-20220221120653817](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120653817.png)

![image-20220221120821644](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120821644.png)

![image-20220221120835141](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120835141.png)

![image-20220221121026860](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221121026860.png)!(C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120859635.png)

![image-20220221120939771](C:\Users\孙晟祺\AppData\Roaming\Typora\typora-user-images\image-20220221120939771.png)