#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#define HOST "127.0.0.1"
#define USER "TOM"
#define PASSWORD "tom"
#define DB "hr"
#define PORT 3306

int main()
{
    // 定义数据库
    MYSQL *conn;
    // 初始化数据库连接
    conn = mysql_init(NULL);
    //
    if (!conn)
    {
        perror("初始化失败!");
        return EXIT_FAILURE;
    }
    if (!mysql_real_connect(conn, HOST, USER, PASSWORD, DB, PORT, NULL, 0))
    {
        fprintf(stderr, "连接失败:%d %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        return EXIT_FAILURE;
    }
    printf("已连接\n");

    // sql 注入
    // char *sql = "select * from staff";
    //  char *sql="insert into dept values(NULL,'test','长沙')";
    //  char *city="we";
    //  char *dept="维护";
    //  char sql[127]={0};
    //  my_ulonglong id = mysql_insert_id(conn);
    //  sprintf(sql,"insert into dept values(NULL,'%s','%s')",dept,city);
    //  sprintf(sql,"update dept set name='%s',city='%s' where id=%llu",dept,city,id);
    //  printf("%s",sql);

    //预编译 INSERT 插入 
    const char *sql_insert = "INSERT INTO dept(name,city) VALUES(?,?)";
    MYSQL_STMT *stmt_insert = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt_insert, sql_insert, strlen(sql_insert));

    char name_buf[45] = "运维部";
    char city_buf[45] = "武汉";
    unsigned long len_name = strlen(name_buf);
    unsigned long len_city = strlen(city_buf);

    MYSQL_BIND bind_insert[2];
    memset(bind_insert, 0, sizeof(bind_insert));

    // 绑定第一个? name
    bind_insert[0].buffer_type = MYSQL_TYPE_STRING;
    bind_insert[0].buffer = name_buf;
    bind_insert[0].buffer_length = sizeof(name_buf);
    bind_insert[0].length = &len_name;

    // 绑定第二个? city
    bind_insert[1].buffer_type = MYSQL_TYPE_STRING;
    bind_insert[1].buffer = city_buf;
    bind_insert[1].buffer_length = sizeof(city_buf);
    bind_insert[1].length = &len_city;

    mysql_stmt_bind_param(stmt_insert, bind_insert);
    int ret = mysql_stmt_execute(stmt_insert);
    if (ret != 0)
    {
        fprintf(stderr, "插入失败：%s\n", mysql_stmt_error(stmt_insert));
        mysql_stmt_close(stmt_insert);
        mysql_close(conn);
        return -1;
    }
    printf("插入成功，新增主键ID：%lu\n", mysql_stmt_insert_id(stmt_insert));
    mysql_stmt_close(stmt_insert);
    //====================================================================

    //预编译 UPDATE 更新
    const char *sql_update = "UPDATE dept SET name=?,city=? WHERE id=?";
    MYSQL_STMT *stmt_update = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt_update, sql_update, strlen(sql_update));

    char new_name[45] = "研发中心";
    char new_city[45] = "深圳";
    int target_id = 3;
    unsigned long n1 = strlen(new_name);
    unsigned long n2 = strlen(new_city);

    MYSQL_BIND bind_update[3];
    memset(bind_update, 0, sizeof(bind_update));

    bind_update[0].buffer_type = MYSQL_TYPE_STRING;
    bind_update[0].buffer = new_name;
    bind_update[0].buffer_length = sizeof(new_name);
    bind_update[0].length = &n1;

    bind_update[1].buffer_type = MYSQL_TYPE_STRING;
    bind_update[1].buffer = new_city;
    bind_update[1].buffer_length = sizeof(new_city);
    bind_update[1].length = &n2;

    bind_update[2].buffer_type = MYSQL_TYPE_LONG;
    bind_update[2].buffer = &target_id;

    mysql_stmt_bind_param(stmt_update, bind_update);
    mysql_stmt_execute(stmt_update);
    printf("更新受影响行数：%lu\n", mysql_stmt_affected_rows(stmt_update));
    mysql_stmt_close(stmt_update);
    //====================================================================

    mysql_close(conn);
    return 0;
}