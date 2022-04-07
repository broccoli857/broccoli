# broccoli
## 开发环境
Centos 7; VSCode + remote

## 项目路径

## 日志系统
 - logger : 定义日志类别
 - |
 - |	--- Formatter : 日志格式
 - |
 - Appender : 日志输出地方

## 配置系统

- Config : 使用yaml文件进行配置

- yamp-cpp : github

- 自定义类型，需要实现broccoli::lexicalCast的偏特化
实现后，就可以支持Config解析自定义类型，自定义类型可以和常规stl容器疫情使用

- 配置的事件机制
当一个配置发生修改时，可以反向通知对应的代码，回调

## 协程库封装

## socket函数库
## http协议开发
## 分布协议
## 推荐系统