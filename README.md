# xorm
基于c++11 标准的orm  

## 特性   
1. 使用简单  
2. stmt全面支持,防止注入  
3. head only,方便用于其他项目  
4. 支持mysql,sqlite3  

## RoadMap  
1. 增加postgre等数据库支持  

# 目录
* [xorm配置](#初始化配置)
* [新增](#新增数据)
* [删除](#删除数据)
* [修改](#修改)  
* [查询](#查询)
* [原生执行](#execute方法)
* [开启事务](#开启事务) 
* [日志监听](#日志监听)
  
# 初始化配置
>在使用之前需要进行数据库配置,通过init_database_config方法注册配置,配置参数如下:  
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
#  新增数据
>通过dao_t<DataBase>::insert 进行数据的添加  
##### db_result<void> insert(T&& t)
* 参数： 通过REFLECTION注册过的表结构对象  
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *affect_rows* 影响行数
 - *unique_id* 唯一ID
 - *error* 是否有错误（error.message取出错误详情）
 
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
	mysql::Double money;
};
REFLECTION(test, id, a, b, time, date, tm, money)
int main(){
	dao_t<mysql> dao;
	test data;
	data.id = 0;
	data.a = i;
	data.b = "t0 你好" + std::to_string(i);
	data.time.format_timestamp(std::time(nullptr));
	data.date = "2019-10-09";
	data.tm.format_timestamp(std::time(nullptr));
	data.money = 12.03;
	auto pr = dao.insert(data);
	if (pr.success == true && pr.affect_rows > 0)
	{
		std::cout << "insert row " << pr.affect_rows << " insert id " << pr.unique_id << std::endl;
	}
	else {
		std::cout << "error " << pr.error << " message " << pr.error.message() << std::endl;
	}
}
````

#   删除数据
>通过dao_t<DataBase>::del 进行数据的删减    
##### db_result<void>  del(std::string const& condition,...)
* 参数1： 删除条件，可以为空字符串 
> "where id=?"  
* 后续参数： 条件中的值  
> mysql::Integer{10}    
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *affect_rows* 影响行数
 - *unique_id* 唯一ID
 - *error* 是否有错误（error.message取出错误详情）
	
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
	mysql::Double money;
};
REFLECTION(test, id, a, b, time, date, tm, money)  
int main(){
  dao_t<mysql> dao;
  bool r = dao.del<test>("where id = ?",mysql::Integer{10});
}
````

#   修改数据  
>支持两种方式   
###### db_result<void> update(T&& v)   
* 参数： 数据表结构对象  
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *affect_rows* 影响行数
 - *unique_id* 唯一ID
 - *error* 是否有错误（error.message取出错误详情）


######  db_result<void> update(std::string const& condition,...)  
* 参数: update语句  
>"update test set a=? where id=?"  
* 后续参数: 条件中的值  
>如 mysql::Integer{10}  
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *affect_rows* 影响行数
 - *unique_id* 唯一ID
 - *error* 是否有错误（error.message取出错误详情）
 
````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
	mysql::Double money;
};
REFLECTION(test, id, a, b, time, date, tm, money)  
int main(){
  test data;
  data.id = 1;
  data.a = 1024;
  dao_t<mysql> dao;
  bool r = t.update(data);
  bool r1 = t.update("update test set a=? where id=?",mysql::Integer{10},mysql::Integer{1});
}
````

#   查询数据
###### db_result<T> query(std::string const& condition, ...)
* 模板参数: 数据表结构类型  	
* 参数： 查询条件 (可以为空字符串)
>"where id=?"
* 后续参数: 条件中的值  
>如 mysql::Integer{1}
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *results* 返回数据结果vector
 - *error* 是否有错误（error.message取出错误详情）
 
###### 使用方式二   
* 模板参数: 自定义std::tuple\<T...\> 如 std::tuple\<mysql::Integer,std::string\> 代表查询的数据集的field类型
* 参数： 完整的查询语句
>"where id=?"
* 后续参数: 条件中的值  
>如 mysql::Integer{1}
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *results* 返回数据结果vector
 - *error* 是否有错误（error.message取出错误详情）

````
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
struct test {
	mysql::Integer id;
	mysql::Integer a;
	std::string b;
	mysql::MysqlDateTime time;
	mysql::MysqlDate date;
	mysql::MysqlTime tm;
	mysql::Double money;
};
REFLECTION(test, id, a, b, time, date, tm, money)  
int main(){
   dao_t<mysql> dao;
   auto r = dao.query<test>("where id=?",mysql::Integer{1});
   auto r0 = dao.query<std::tuple<mysql::Integer,std::string>>("select a,b from test where id=?",mysql::Integer{1});
}
````
# execute方法  
>支持原生数据库的命令执行  
##### db_result<void> execute(std::string const& sql)
* 参数: 完整的sql语句
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *affect_rows* 影响行数
 - *unique_id* 唯一ID
 - *error* 是否有错误（error.message取出错误详情）

##### db_result<void> execute(std::string const& sql,...)
* 参数: 完整的sql语句
>如果执行的sql语句可以返回结果集合  
* 参数2: 执行结果集的回调方法,回调参数为数据结果集的指针（可能是nullptr）  
* 返回数据： 
 - *success* SQL语句无错误返回true 
 - *affect_rows* 影响行数
 - *unique_id* 唯一ID
 - *error* 是否有错误（error.message取出错误详情）

````cpp
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
  dao_t<mysql> dao;
  auto r = dao.execute("commit");
  std::function<void(MYSQL_RES*)> get_reuslt = [](MYSQL_RES*){}
  auto r1 = dao.execute("select * from test",get_reuslt);
}
````
# 开启事务  
>提供了两种开启事务的方式
* 自动事务开启  
###### start_transaction()
>在声明的dao_t对象声明周期结束后会自动提交当前事务
* 手动事务开启    
  ###### begin()
  ###### rollback() 
  ###### commit()  
````cpp
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
   {
      dao_t<mysql> dao;
      dao.start_transaction();
      //////
   }
    dao_t<mysql> dao;
    dao.begin();
    ///balabala
    if(///balabala){
      dao.rollback();
    }
    dao.commit();
}
````
# 日志监听
#### 可以通过设置回调方法获取数据库操作日志  
>如语句执行错误等  
````cpp
#include <iostream>
#include "mysql.hpp"
#include "dao.hpp"
using namespace xorm;
int main(){
	dao_message::get().set_error_callback([](std::string const& msg) {
	   std::cout << msg << "\n";
	});
}
````
