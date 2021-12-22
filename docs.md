# Bookstore 开发文档

项目： Bookstore-2021

文档作者：刘祎禹

## 程序功能概述 Overview of Program Functions

### 基础功能 Basic Functions

- 程序首次运行时即创建用户名为 `root`，密码为 `sjtu`，权限为 `{7}` 的超级管理员账户

- 支持使用命令行（Windows CMD 或 Linux Terminal）进行操作，输入数据以换行符或回车符为分隔为若干指令。

  - 指令合法字符集为标准 Ascii 字符

  - 单个指令最大长度为 `1024` 字节

  - 允许的空白符仅为空格，单个指令被空白符分割为多个部分

    - 多个连续空白符效果与一个空白符等价
    - 行首行末允许出现多余空白符
    - 如无特殊说明禁止省略或另增空白符

  - 指令中第一个部分必须为指令关键词，指令中关键词部分必须与指令格式完全匹配

  - `[x]` 表示一串有特定限制的用户自定义字符串

  - `(a|b|c)` 表示此处仅能出现 `a`, `b`, `c` 中其一

  - `(x)?` 表示此处可以出现零次或一次 `x`

  - `(x)+` 表示此处可以出现一次或多次 `x`

  - ```css
    # 基础指令
    quit
    exit
    
    # 账户系统指令
    su [User-ID] ([Password])?
    logout
    register [User-ID] [Password] [User-Name]
    passwd [User-ID] ([Old-Password])? [New-Password]
    useradd [User-ID] [Password] [Priority] [User-Name]
    delete [User-ID]
    
    # 图书系统指令
    show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]")?
    buy [ISBN] [Quantity]
    select [ISBN]
    modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
    import [Quantity] [Total-Cost]
    
    # 日志系统指令
    report myself
    show finance ([Time])?
    report finance
    report employee
    log
    ```

- 在用户输入一条指令后，如果合法则执行对应操作，如果有则输出操作结果；如果指令非法或操作失败则输出 `Invalid\n`

  - 仅有空白符的指令合法且无输出内容
  - 除非有特殊说明，如果输入指令对应的输出内容非空，则结尾有 `\n` 字符；输出内容为空则不输出任何字符

- `quit` 和 `exit` 指令功能为正常退出系统

- 书店管理系统服务于 **店主（超级管理员）**，**员工（管理员）以及顾客（用户）**。为满足其不同需求，故需账户系统管理账户及权限数据，并提供相应功能

- **权限说明**

  - 本文档中以 `{x}` 表示权限等级，数字越大则权限越高
  - 账户的权限等级有 `{7}`, `{3}` 和 `{1}`
  - 未登录任何账户的状态下当前账户权限视为 `{0}`
  - 低权限账户能够访问的功能严格是高权限账户能够访问的功能的**子集**
  - 指令的权限等级表示能够执行该指令的最低当前账户权限需求

- **各权限可访问功能说明**

  - **店主 `{7}`**：使用 `root` 账户，可以访问书店系统所有功能，包括日志和账户管理功能
  - **员工 `{3}`**：可以访问修改图书数据相关功能等
  - **顾客 `{1}`**：可以注册账户，查询 / 购买图书
  - **游客 `{0}`**：可以注册账户
  - 本系统中有且仅有以上四种权限等级，其余权限等级非法

- **登录栈**

  - 系统支持嵌套登录，即允许多个账户同时处于登录状态

- 输入的指令视为最后登录的账户操作

- 退出系统视为登出所有已登录账户

### 附加功能 Advanced Functions

暂无


## 主体逻辑说明 Description of the Main Part

*注：在处理数据中使用 `std:string`，在储存时使用 `char[]` *

### 运行 Operating

- 首先调用 `void init()` 先检查文件是否存在，如不存在，则建立文件。

- 构造 `AccountManagement` 以及 `BookManagement` 类

- 程序为一个循环的 try-catch 块

- 每次读入一行，输入 token scanner，分析输入内容，调用函数



### 储存 Storing

#### 账户储存

账户储存由2个文件组成：

- 储存所有信息的数据表（按时间先后次序）

- 由用户 ID 为依据的指向数据的索引表（按字典序排序的块状链表）

每次查找都先在索引表中进行两次二分查找，然后访问数据表



#### 书目储存

书目储存由5个文件组成：

- 储存图书信息的数据表

- 由 ISBN 为依据的指向数据的索引表

- 由书名为依据的索引表

- 由作者为依据的索引表

- 由关键词为依据的索引表



#### 日志储存

- 储存日志文件



## 储存结构 Storage Structure

### 概览 Overview

1. 用户信息 `user_data`
2. 用户 ID 对应映射表 `user_id_map`
3. 图书信息表 `book_data`
4. ISBN-Book 映射表 `isbn_map`
5. 书名-图书索引表 `book_name_index`
6. 作者-图书索引表 `author_map_index`
7. 关键词-图书索引表 `keyword_index`
8. 日志表 `log`



### 详细结构 Detailed Structure

*注：每个文件以二进制方式储存，最小储存单元为一个 class，每次写入或读入一个 class，其长度可用 `sizeof()` 获得。注意：字符类型的情况下需用 `char[<length> + 1]` 而非 `std::string` 完成，否则无法正常读写。*

1. **用户信息** （1-Based， 0表示空指针) 
     - `User-ID`：用户 ID
       - 合法字符集：数字，字母，下划线
       - 最大长度：30
     - `Password`：用户密码
       - 合法字符集：数字，字母，下划线
       - 最大长度：30
     - `UserName`：用户名
       - 合法字符集：除不可见字符以外 Ascii 字符
       - 最大长度：30
     - `Priority`：用户权限
       - 合法字符集：数字 `1, 3, 7`
       - 最大长度：1

2. **用户 ID 对应映射表**（以 `User-ID` 字典序排序的块状链表）
   - `User-ID`
   - `Point-to-User-Data`

3. **图书信息表**
   - `ISBN`：Internal Strange Book Number
     - 合法字符集：除不可见字符以外 Ascii 字符
     - 最大长度：20
     - 特殊说明：本系统中任何两本图书不应有相同 `ISBN` 信息
   - `Book-Name`：图书名字
     - 合法字符集：除不可见字符和英文双引号以外 Ascii 字符
     - 最大长度：60
   - `next_book`
   - `last_book`
   - `Author`：图书作者名字  
     - 合法字符集：除不可见字符和英文双引号以外 Ascii 字符
     - 最大长度：60
   - `next_author`
   - `last_author`
   - `Keyword`：图书关键词
     - 合法字符集：除不可见字符和英文双引号以外 Ascii 字符
     - 最大长度：60
     - 特殊说明：`keyword` 内容以 `|` 为分隔可以出现多段信息。如 `math|magic|quantum` 表示该图书具有三个无顺序的关键词 `math`, `magic` 和 `quantum`
   - `Quantity`：购买数量
     - 合法字符集：数字
     - 最大长度：10
     - 特殊说明：数值不超过 `2'147'483'647`
   - `Price`：图书单价
     - 合法字符集：数字和 `.`
     - 最大长度：13
     - 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位
   - `Total-Cost`：交易总额
     - 合法字符集：数字和 `.`
     - 最大长度：13
     - 特殊说明：本系统中浮点数输入输出精度固定为小数点后两位

4. **ISBN-Book 映射表**（以 `ISBN` 进行字典序排序的块状链表）
   - `ISBN`
   - `Pointer-to-Book-Data`

5. **书名-图书索引表**（以 `Book-Name` 进行字典序排序的块状链表）
   - `Book-Name`
   - `ISBN`
   - `Pointer-to-Book-Data`

6. **作者-图书索引表**（以 `Author` 进行字典序排序的块状链表）
   - `Author`
   - `ISBN`
   - `Pointer-to-Book-Data`

7. **关键词-图书索引表** （以 `Keyword` 进行字典序排序的块状链表）
   - `Keyword`
   - `ISBN`
   - `Pionter-to-Book-Data`

8. **日志表**
   - `User`
   - `Behavior`
   - `in-or-out`
   - `Amount`



## 代码结构及说明 Structures and Descriptions of Code

*如果开发人员代码风格和文档中的代码风格不同，请自行更改为自己的风格*

### main.cpp

- `void Initialization();`
  - 先检查文件是否存在，如不存在，则建立文件。
- `void ProcessLine(AccountManagement& accounts, BookManagement& books);`
  - 调用相应类或者函数。
- `void StringToChar(const std::string& source, char* target, int targetMaxLength);`
  - 将 `source` 转为 `char`
  - **注意：一定要设置最大长度！**
- `int main();`
  - `AccountManagement accounts;` 
  -  `BookManagement books;`
  - `LogManagement logs;`
  - 接下来是一个 `while (ture) {} catch (int) {}` 循环，内部调用 `void processLine();` 函数。

### Token_Scanner.h

- `class TokenScanner;`

  private:
  - `std::string buffer_;` 储存的字符串缓冲区
  - `int current_ = 0;` 所在的位置
  - `char delimiter_ = ' ';` 分隔符

  public:
  - `TokenScanner();` Default Constructor
  - `TokenScanner(char delimiter);`
  - `TokenScanner(const std::string& in, char delimiter = ' ');`
  - `~TokenScanner();` Default destructor
  - `std::string NextToken();`
    - 先跳过此前遗留的一个或多个分隔符
    - 记录第一位不是分隔符的位置（如没有则返回空串）
    - 找到下一个分隔符前一个字符
    - 返回字串
  - `void Clear();` 还原到初始空字符串状态
  - `friend std::istream& operator>>(std::istream& is, TokenScanner& obj);` 输入新 buffer
  - `void SetDelimiter(char newdelimiter);` 设定新分隔符

### Accounts.h 
```CPP
#include <iostream>
#include <fstream>
#include <vector>

#include "token_scanner.h"
#include "unrolled_linked_list.h"
#include "log.h"

struct UserID {
  char[31] value;`

  UserID(std::string userID);

  UserID();

  std::string GetUserID() const;

  bool operator==(const UserID& rhs) const;

  bool operator<(const UserID& rhs) const;
};

class User {
private:
  UserID_;

  char[31] user_name_;

  char[31] password_;

  int priority_;

public:
  User();

  User(const std::string& data);

  User(const std::string& ID, const std::string& name, const std::string& password, int priority);
  
  void ChangePassword(const std::string& newPassword);

  [[nodiscard]] int GetPriority() const;
};

struct LogInAccount {
  User user;
  int selected_book_id;
};

class AccountManagement {
private:
  std::vector<LogInAccount> login_; // 用于储存登录的账户及其选定的书本 id，不可使用 ISBN 作为指定对象，因为 ISBN 可能会被之后其他用户改变

  fstream account_data_("user_data"); // 用于储存所有数据的文件

  UnrolledLinkedList<user_id_map, UserID, int, int> user_id_map_; // 第一个 int 忽略即可，填入时用 0 就行

  // Other private variables ...
  
public:
  AccountManagement(); // 注意检查是否有用户名为 root，密码为 sjtu，权限为 {7} 的超级管理员账户，如没有，则添加该用户

  void SwitchUser(TokenScanner& line); // su command

  void LogOut(TokenScanner& line); // logout command

  void RegisterUser(TokenScanner& line); // register command

  void ChangePassword(TokenScanner& line); // passwd command

  void AddUser(TokenScanner& line, LogManagement& logs); // useradd command

  void RemoveUser(TokenScanner& line, LogManagement& logs); // delete command
  
  void UserSelect(int book_id); // 对于当前用户选中对象

  [[nodiscard]] int GetCurrentPriority() const;
};
```



### Book.h 
*注：可以调用 `AccountManagement::getCurrentPriority()` 来获取权限*

```CPP
#include <iostream>
#include <fstream>
#include <vector>

#include "token_scanner.h"
#include "unrolled_linked_list.h"
#include "log.h"

struct ISBN {
  char[21] value;

  ISBN(const std::string& s);

  bool operator==(const ISBN& rhs) const;

  bool operator<(const ISBN& rhs) const;
};

struct BookName {
  char[61] value;

  BookName(const std::string& s);

  bool operator==(const BookName& rhs) const;

  bool operator<(const BookName& rhs) const;
};

struct Author {
  char[61] value;

  Author(const std::string& s);

  bool operator==(const Author& rhs) const;

  bool operator<(const Author& rhs) const;
};

struct Keyword {
  char [61] value;

  Keyword(const std::string& s);

  bool operator==(const Keyword& rhs) const;

  bool operator<(const Keyword& rhs) const;
};

class Book {
private:
  ISBN isbn_;
  BookName book_name_;
  Author author_;
  Keyword keyword_;
  int quantity_ = 0;
  double price_ = 0;
  double total_cost_ = 0;

public:
  int book_ID_;

  Book();

  Book(int id, const std::string& isbn, const std::string& bookName, const std::string& author, const std::string& keyword, int quantity, double price, double _total_cost); // 这样方便构造，但注意 keyword 需要以升序重新排列

  Book(int id, const std::string& isbn); // 将除 ISBN 以外的部分全部为空字符串或 0



  friend std::ostream& operator<<(std::ostream&, const Book& book); // 用于输出
};

class BookManagement {
private:
  fstream book_data_("book_data"); // 用于储存所有数据的文件

  UnrolledLinkedList<book_name_index, ISBN, int, int> isbn_map_; // 第一个 int 忽略即可，填入时用 0 就行

  UnrolledLinkedList<book_name_index, BookName, ISBN, int> book_name_index_;

  UnrolledLinkedList<author_map_index, Author, ISBN, int> author_map_index_;

  UnrolledLinkedList<keyword_index, Keyword, ISBN, int> keyword_index_;

  // Other private variables ...

public:
  BookManagement();

  // 下面的指令请调用 accounts::getCurrentPriority() 来获取权限

  void Show(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 先判断是不是 show finance（都是以 show 开头），然后分四种情况讨论，如无参数，则按照 ISBN 输出全部（traverse 函数）

  void Buy(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); 

  void Select(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 检查是否有权限，检查是否有 ISBN，然后选中

  void Modify(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 检查是否有权限

  void ImportBook(TokenScanner& line, AccountManagement& accounts, LogManagement& logs); // 检查是否有权限
};
```



### Unrolled_Linklist.h 
```CPP
template<std::string index_name, typename key_type, typename subkey_type, typename _value_type>
class UnrolledLinkedList {
private:
  fstream index_(index_name);

  // ... other private contents

public:
  UnrolledLinkedList();

  void Insert(const _key_type& key, const _subkey_type& subkey, const _value_type& value);

  void Modify(const _key_type& key, const _subkey_type& subkey, const _value_type& value);

  void Erase(const _key_type& key, const _subkey_type& subkey);

  void Clear();

  bool Exist(const _key_type& key) const;

  bool Exist(const _key_type& key, const _subkey_type& subkey) const;

  std::vector<_value_type>* Traverse(); // 此函数返回一个遍历所有值的有序数组，请用 new 新建 std::vector，并且 std::vector::reserve() 足够多的空间减少浪费，调用该函数需要最后 delete 此指针（使用指针是为了减少不必要的复制）

  std::vector<_value_type>* Traverse(const _key_type& key); // 此函数返回一个遍历所有值的有序数组，请用 new 新建 std::vector，并且 std::vector::reserve() 足够多的空间减少浪费，调用该函数需要最后 delete 此指针（使用指针是为了减少不必要的复制）

  [[nodiscard]] value_type Get(const _key_type& key, const _subkey_type& subkey) const;
};
```

### Log.h

```CPP
#include <ioetream>
#include <fstream>

#include "accounts.h"

enum Behavior {AddUser, Delete, Show, Buy, Select, Modify, Import};
struct Log {
  User user;
  enum behavior;
  char[150] description;
  bool if_earn = false; // 表示是否是收入
  double Amount;
};

class LogManagement {
private:
  fstream log_data_("log");
  int count_ = 0; // 交易笔数

public:
  LogManagement();

  void Report(TokenScanner& line, AccountManagement& accounts);

  void AddLog(log& Log); // 把 log 放进文件的同时还需要检查是否有交易

  void ShowFinance(int Limit = -1); // 若为 -1，则显示全部

  void Log(TokenScanner& line); // log command，检查有无额外的 token
};
```

add:使用Google代码规范
