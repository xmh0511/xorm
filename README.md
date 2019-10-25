# xorm
基于c++14 标准的orm  

## 特性  

1. 使用简单  
2.head only,方便用于其他项目  

### 实例  
#### 初始化配置
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
};
REFLECTION(test, id, a, b, time, date, tm)

int main(){

        init_database_config({ "127.0.0.1","root","root","xorm",3306,2 }); //全局初始化配置
	test data;
	data.id = 0;
	data.a = i;
	data.b = "hello,world";
	data.time = "2019-10-25 18:05:01";
	data.date = "2019-10-09";
	data.tm = "18:19:01";
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
	t.insert(data);
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
  t.del<test>("where id = 1");
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
  t.update(data);
  t.update(data,"where id = 1");
}
````
