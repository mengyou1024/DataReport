### 0. 克隆本仓库

``` powershell
git clone <git-repository>
```

### 1. 安装Chocolatey(可选)

> `Chocolatey` 用于安装`git`和`InnoSetup`
>
> 项目使用`git describe`作为版本号
>
> `InnoSetup`用于打包生成exe安装程序

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

**需要管理员权限**

### 2. 安装依赖包(可选)

``` powershell
choco install packages.config
```

### 3. 添加构建和清除步骤（可选）

> **如果不需要打包生成exe安装程序，可不添加**

##### 3.1 打开项目

![image-20230721154217776](./assets/image-20230721154217776.png)

![image-20230721154337719](./assets/image-20230721154337719.png)

##### 3.2 配置项目

![image-20230721154514095](./assets/image-20230721154514095.png)

![image-20230721153836158](./assets/image-20230721153836158.png)

### 4. 添加测试（可选）

> qml加载前执行`Morose::test()`

![image-20230721154005128](./assets/image-20230721154005128.png)

## 编译输出

**需要安装InnoSetup并添加环境变量**

```powershell
output
├── install
│   └── DataReportInstaller.exe (程序安装包)
└── pack-installer.iss
```



## 程序传入参数

> -h

```powershell
Usage: <可执行程序路径> [options]
DataReport

Options:
  -?, -h, --help                     Displays help on commandline options.
  --help-all                         Displays help including Qt specific options.
  -v, --version                      Displays version information.
  --discovery <discoveryFile>        发现缺陷记录表的文件路径	(默认值为: discoveryRecord.bin).
  --inspection <inspectionFile>      探伤记录表的文件路径	(默认值为: inspectionRecord.bin)).
  --daily <dailyFile>                日常性能检查记录表的文件路径	(默认值为: dailyRecord.bin).
  --quarterlyRecord <quarterlyFile>  季度性能检查记录表的文件路径	(默认值为: quarterlyRecord.bin)
```

