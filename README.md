# xorm
基于c++11 标准的orm  

## 特性   
1. 使用简单  
2. head only,方便用于其他项目  

## RoadMap  
1. 增加sqlite,postgre等数据库支持  


###  使用方式    
#### 初始化配置
>在使用之前需要进行数据库配置,配置参数如下
* host 用来指定连接的地址
* user 数据库用户名
* password 数据库密码
* dbname 数据库名
* conn_number 连接池数量  
##### 非必填参数    
* character_encoding 数据库编码方式 
* port  数据库端口号
* reconnect_number 尝试重连次数
* timeout 超时时间
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;

int main(){
        dataBaseConfig config;
	config.character_encoding = "utf8";
	config.conn_number = 2;
	config.dbname = "xorm";
	config.host = "127.0.0.1";
	config.password = "root";
	config.user = "root";
        init_database_config(config); //全局初始化配置
}
````
####  新增数据
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
	dao<mysql> t;
	auto pr = t.insert(data);
	std::cout<<"insert row "<<pr.first<<" insert key "<<pr.second<<std::endl;
}
````

####   删除数据
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
  dao<mysql> t;
  bool r = t.del<test>("where id = 1");
}
````

####   修改数据
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
  test data;
  data.id = 1;
  data.a = 1024;
  dao<mysql> t;
  bool r = t.update(data);
  bool r1 = t.update(data,"where id = 1");
}
````

####   查询数据
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
   dao<mysql> t;
   auto pr = t.query<test>("where id = 1");
   if(pr.first){
     std::vector<test>& vec = pr.second;
     std::cout<< vec.size()<<std::endl;
     if(!vec.empty()){
       bool a_isnull =  vec[0].a.is_null();
     }
   }
   
   auto pr1 = t.query<std::tuple<mysql::Integer,mysql::Integer,std::string,mysql::MysqlDateTime,mysql::MysqlDate,mysql::MysqlTime>>("select * from test");
   if(pr1.first){
      std::cout<< pr1.second.size()<<std::endl;
   }
}
````
