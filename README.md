# DDLog
#### 一个日志打印系统 DDLog

1. 支持格式化输出日志的行---函数---文件---内容
1. 支持快速日志嵌入 FUNC_RUN_LOG
1. 支持常用的判断日志 CONDITION_VALUE_IS_NULL
1. 支持日志的分级别打印 日志级别 debug 、info、 warning、 error、 fatal
1. 支持在运行期间修改级别
1. 支持不同模式的打印 日志模式 release、 debug、 develop 默认是develop
1. 支持在运行期间修改模式
1. 支持设置错误断言级别 默认是warning
1. 支持设置是否打开断言 默认是关闭的
1. 支持持久化日志到文件 默认级别warning
1. 支持日志文件夹比较大时自动删除
1. 支持自动化测试
1. 支持跨平台 mac 、win 、linux、iPhone、Android