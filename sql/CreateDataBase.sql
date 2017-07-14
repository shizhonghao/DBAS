create database DBSA
on Primary --主文件及主文件组
( NAME = DBAS_data1,
  FILENAME = 'E:\DB\DBAS\Primary\MainFile.mdf',
  SIZE = 10MB,
  MAXSIZE = 200MB,
  FILEGROWTH = 10%),
( NAME = DBAS_data2,
  FILENAME = 'E:\DB\DBAS\Primary\SecFile1.ndf',
  SIZE = 10MB,
  MAXSIZE = 200MB,
  FILEGROWTH = 10%),
FILEGROUP TLEGroup1	--辅文件组
( NAME = DBAS_dat3,
  FILENAME = 'D:\DB\DBAS\TLEGroup1\SecFile2.ndf',
  SIZE = 10MB,
  MAXSIZE = 200MB,
  FILEGROWTH = 10%),
( NAME = DBAS_dat4,
  FILENAME = 'D:\DB\DBAS\TLEGroup1\SecFile3.ndf',
  SIZE = 10MB,
  MAXSIZE = 200MB,
  FILEGROWTH = 10%),
FILEGROUP TLEGroup2	--辅文件组
( NAME = DBAS_dat5,
  FILENAME = 'F:\DB\DBAS\TLEGroup2\SecFile4.ndf',
  SIZE = 10MB,
  MAXSIZE = 200MB,
  FILEGROWTH = 10%),
( NAME = DBAS_dat6,
  FILENAME = 'F:\DB\DBAS\TLEGroup2\SecFile5.ndf',
  SIZE = 10MB,
  MAXSIZE = 200MB,
  FILEGROWTH = 10%)
 LOG ON 
( NAME = N'DBAS_log',
  FILENAME = N'E:\DB\DBAS\Log\forDBAS_log.ldf' , 
  SIZE = 1024KB , FILEGROWTH = 10%)