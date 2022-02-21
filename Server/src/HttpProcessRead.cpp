#include"HttpProcessRead.h"
#include"LogThread.h"
#include"EpollTools.h"
#include"MysqlConn.h"

#include"stdio.h"
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>


const char* docRoot = "/home/web/MyminiWebServer/Html";

HttpProcessRead::HttpProcessRead(char readbuffer[],int readIndex)
                :checkstate_(CHECK_REQUESTLINE),
                readIndex_(readIndex),
                linger_(false)
{
    memset(readBuffer,'\0',READBUFFERSIZE);
    memset(filePath_,'\0',200); 
    memcpy(readBuffer, readbuffer,(size_t)READBUFFERSIZE); 
    
    
    
}

HttpProcessRead::~HttpProcessRead(){
    
}

HttpProcessRead::HTTPCODE HttpProcessRead::processRead()
{
    HTTPCODE ret = NO_REQUEST;
    LINESTATUS lineStatus = LINE_OK;
    char* text = 0 ;
    

    while((lineStatus == LINE_OK && checkstate_ == CHECK_CONTENT)
            || (lineStatus = parseLine()) == LINE_OK)
    {
        text =  getLine();
        processPosition = checkIndex_;
        //打印获取的http请求信息
        //printf("get 1 http request: %s. \n",text);
        
        if(checkstate_ == CHECK_REQUESTLINE)
        {
            ret = parseRequest(text);
            if(ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
        }
        else if(checkstate_ == CHECK_HEAD)
        {
            ret = parseHead(text);
            if(ret == BAD_REQUEST)
            {
                return BAD_REQUEST;
            }
            else if( ret == GET_REQUEST)
            {
                return doRequset();
            }
        }
        else if(checkstate_ == CHECK_CONTENT)
        {
            ret = parseContent(text);
            if(ret == GET_REQUEST)
            {
                return doRequset();
            }
            lineStatus = LINE_HALFBAKED;
        }
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseContent(char* text)
{
    if(readIndex_ >= (contentLength_ + checkIndex_))
    {
        text[contentLength_] = '\0';
        postdata = text;
        //printf("parseContent: %s\n",text);
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseHead(char* text)
{
    if( text[0] == '\0')
    {
        if(contentLength_ != 0)
        {
            checkstate_ = CHECK_CONTENT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }   

    //处理connection头部
    else if( strncasecmp(text,"Connection:",11) == 0)
    {
        text += 11;
        text += strspn(text," \t");
        if(strcasecmp(text,"keep-alive") == 0)
        {
            linger_ = true;
        }
    }
    else if(strncasecmp(text,"Content-Length:",15) == 0)
    {
        text += 15;
        text += strspn(text," \t");
        contentLength_ = atol(text); 
    }
    else if(strncasecmp( text, "Host:",5) == 0)
    {
        text += 5;
        text += strspn(text," \t");
        host_ = text;
    }
    else
    {
        LOG_FATAL << "unknow header" << text << log::end;
        //printf("unknow header %s\n",text);
    }
    return NO_REQUEST;
}

HttpProcessRead::HTTPCODE HttpProcessRead::parseRequest(char* text)
{
    url_ = strpbrk(text, " \t");
    if(!url_)
    {
        return BAD_REQUEST;
    }
    *url_++ = '\0';
    
    char* mtext = text;
    if(strcasecmp(mtext,"GET") == 0)
    {
        method_ = GET;
        
    }
    else if(strcasecmp(mtext,"POST") == 0)
    {
        method_ = POST;        
    }
    else
    {
        return BAD_REQUEST;
    }

    url_ += strspn(url_," \t");
    version_ = strpbrk(url_," \t");
    if(!version_)
    {
        return BAD_REQUEST;
    }
    *version_++ = '\0';
    version_ += strspn(version_," \t");
    if(strcasecmp(version_,"HTTP/1.1"))
    {
        return BAD_REQUEST;
    }
    if(strncasecmp(url_,"http://",7) == 0)
    {
        url_ += 7;
        url_ = strchr(url_,'/');
    }
    if(strncasecmp(url_,"https://",8) == 0)
    {
        url_ += 8;
        url_ = strchr(url_,'/');
    }
    if(strlen(url_) == 1)
    {
        if(method_ == GET)
        {
            strcpy(url_,"/home.html"); 
        }       
        else if(method_ = POST)
        {
            strcpy(url_ ,"/Signin.html");
        }
    }
    
    if( !url_ || url_[0] != '/')
    {
        return BAD_REQUEST;
    }
    checkstate_ = CHECK_HEAD;
    return GET_REQUEST;  
}

HttpProcessRead::HTTPCODE HttpProcessRead::doRequset()
{
    strcpy(filePath_,docRoot);
    int len = strlen(docRoot);

    //处理指针，看是注册还是登录
    const char* p = strrchr(url_,'/');
    //0代表登录，1代表注册，4代表注册之后登录
    if(method_ == POST && (*(p+1) == '0' || *(p+1) == '1' || *(p+1) == '4'))
    {
        if(*(p+1) == '4')
        {
            strcpy(url_,"/home.html");
        }   
        else
        {     
            //链接数据库，读取用户名和密码
            char name[100];
            char password[100];
            int i = 5;
            for(; postdata[i] != '&';++i)
            {
                name[i-5] = postdata[i];
            }
            name[i-5] = '\0';

            int j  = 0;
            for(i = i+10; postdata[i] != '\0'; ++i,++j)
            {
                password[j] = postdata[i];
            }
            password[j] = '\0';
            if(*(p+1) == '0')
            {
                //根据登录数据，查询是否在数据库中存在
                if( strlen(name) && strlen(password))
                {
                    //查询数据：名字，密码
                    //printf("HttpProcessRead::doRequset:: username = %s, password = %s \n",name,password);
                    //
                    if(userData[name] == password)
                    {
                        strcpy(url_,"/login.html");
                    }
                    else{
                        strcpy(url_,"/logfail.html");
                    }
                }
            }
            //如果是1，说明是注册请求
            else if(*(p+1) == '1')
            {
                //打印获取的用户名、密码
                // printf("get your signin requsetion!\n name is %s,password is %s\n",
                //         name,password);

                char* sqlInsert = (char*)malloc(sizeof(char) * 200);
                strcpy(sqlInsert, "INSERT INTO userdata VALUES (");
                strcat(sqlInsert,"'");
                strcat(sqlInsert, name);
                strcat(sqlInsert, "', ");
                strcat(sqlInsert,"'");
                strcat(sqlInsert, password);
                strcat(sqlInsert,"'");
                strcat(sqlInsert, ")");

                if(userData.find(name) == userData.end())
                {
                    lock.lock();
                    int ret = mysql_query(mysql_,sqlInsert);
                    userData[name] = password;
                    lock.unlock();

                    if(!ret)
                    {
                        strcpy(url_,"/signSuccess.html");
                    }
                    else
                    {
                        strcpy(url_,"/signFailed.html");
                    }
                }
                else{
                    strcpy(url_,"/signFailedUserExist.html");
                }
            }
        }
    }
    strncpy(filePath_+len, url_, 200-len-1);
    if(stat(filePath_,&filestat_) < 0)
    {
        //如果文件处于打开状态，提示
        LOG_FATAL << filePath_ << "is using by other" << log::end;
        //printf("%s is using by other \n",filePath_);
        return BAD_REQUEST;
    }
    if(filestat_.st_mode & S_IFDIR)
    {
        //如果路径是文件夹，提示
        //printf("%s is dir",filePath_);
        LOG_FATAL << filePath_ << "is dir" << log::end;
        return BAD_REQUEST;
    }
    int fd = open(filePath_,O_RDONLY);
    
    fileAddr = (char*)mmap(0,filestat_.st_size,PROT_READ,
                                MAP_SHARED,fd,0);    
    close(fd);
    return FILE_REQUEST;    
}

HttpProcessRead::LINESTATUS HttpProcessRead::parseLine()
{
    char tmp;
    for(;checkIndex_ < readIndex_; checkIndex_++)
    {
        tmp = readBuffer[checkIndex_];
        if(tmp == '\r')
        {
            if(checkIndex_ + 1 == readIndex_)
            {
                return LINE_HALFBAKED;
            }
            else if(readBuffer[checkIndex_+1] == '\n')
            {
                readBuffer[checkIndex_++] = '\0';
                readBuffer[checkIndex_++] = '\0';
                return LINE_OK;
            }
        }
        else if(tmp == '\n')
        {
            return LINE_WRONG; 
        }
    }
    return LINE_HALFBAKED;
}


void HttpProcessRead::reset()
{
    processPosition = 0;
    checkIndex_ = 0;
    readIndex_ = 0;
    checkstate_ = CHECK_REQUESTLINE;

    url_ = 0;
    method_ = GET;
    version_ = 0;
    contentLength_ = 0;
    linger_ = false;
    host_ = 0;

    memset(filePath_,'\0',1024);
    memset(readBuffer,'\0',READBUFFERSIZE);

}



//MySQL数据处理

void HttpProcessRead::initMysqlData(SqlPool* sqlPool)
{
    //从连接池获取链接
    mysql_ = nullptr;
    ConnectMysql mysqlConn(&mysql_,sqlPool);

    //在表中检索username，password，并输出到userData中
    if(mysql_query(mysql_,"SELECT name,password FROM userdata"))
    {
        //如果查询失败，打印并记录
        //printf("SELECT error:%s \n",mysql_error(mysql_));
        LOG_FATAL << "SELECT error "<< mysql_error(mysql_) <<log::end;
    }

    //从表中检索结果集
    MYSQL_RES* ret = mysql_store_result(mysql_);
    //从结果集中检索列数
    int colNum = mysql_num_fields(ret);
    //获取所有字段的数组
    MYSQL_FIELD* fields = mysql_fetch_field(ret);

    //从结果集中提取数据
    while(MYSQL_ROW row = mysql_fetch_row(ret))
    {
        std::string tmpname = row[0];
        std::string tmppassword = row[1];
        userData[tmpname] = tmppassword;
    }
}