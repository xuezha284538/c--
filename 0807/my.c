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

    // 预编译stmt替换普通mysql_query 开始 
    const char *sql = "select * from staff";
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    mysql_stmt_prepare(stmt, sql, strlen(sql));
    int ret = mysql_stmt_execute(stmt);
    if (ret)
    {
        fprintf(stderr, "查询：%s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    MYSQL_RES *res = mysql_stmt_result_metadata(stmt);
    printf("row:%ld filed:%d\n", mysql_stmt_num_rows(stmt), mysql_num_fields(res));

    if (!res)
    {
        fprintf(stderr, "获取失败：%s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return EXIT_FAILURE;
    }
    // int n_row = mysql_num_rows(res);
    // int n_filed = mysql_num_fields(res);

    // int cols= mysql_filed_count(conn);
    int cols = mysql_num_fields(res);
    // 行 记录 :字符串数组
    MYSQL_ROW row;
    // 列 字段
    MYSQL_FIELD *field;
    // while((field=mysql_fetch_field(res))!=NULL)
    // {
    //     printf("%s  %d %ld\n",field->name,field->type,field->length);
    // }

    // 获得所有字段构成的数组
    field = mysql_fetch_fields(res);
    for (int i = 0; i < cols; i++)
    {
        printf("%10s\t", field[i].name);
    }
    printf("\n");

    // 绑定结果缓冲区
    char buf[256][128] = {0};
    unsigned long len[256] = {0};
    MYSQL_BIND bind[256] = {0};
    for (int i = 0; i < cols; i++)
    {
        bind[i].buffer_type = MYSQL_TYPE_STRING;
        bind[i].buffer = buf[i];
        bind[i].buffer_length = sizeof(buf[i]);
        bind[i].length = &len[i];
    }
    mysql_stmt_bind_result(stmt, bind);

    // 循环读取行
    while (mysql_stmt_fetch(stmt) == 0)
    {
        // 遍历每一列
        for (int i = 0; i < cols; i++)
        {
            printf("%10s\t", buf[i]);
        }
        printf("\n");
    }
    // ========== 预编译stmt替换普通mysql_query 结束 ==========

    mysql_free_result(res);
    mysql_stmt_close(stmt);
    mysql_close(conn);

    return 0;
}