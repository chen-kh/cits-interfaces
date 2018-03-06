# 接口文档简略说明
**注意**：完整项目已经移至act实验室gitlab上，[详情戳我](http://gitlab.act.buaa.edu.cn/TCSAE/libcits)，该仓库只完成了部分工作。

<!-- TOC -->

- [接口设计原则](#接口设计原则)
    - [模块按照功能进行划分](#模块按照功能进行划分)
    - [各功能模块间相互独立](#各功能模块间相互独立)
    - [下层接口封装](#下层接口封装)
    - [返回值、注释、内存管理](#返回值注释内存管理)
    - [头文件格式](#头文件格式)
    - [接口测试](#接口测试)
- [接口功能划分](#接口功能划分)
- [接口本地测试文件](#接口本地测试文件)
    - [测试文件功能逻辑](#测试文件功能逻辑)
    - [测试文件运行](#测试文件运行)
- [关于asn.1与asn1c](#关于asn1与asn1c)
    - [asn.1和asn1c是什么？](#asn1和asn1c是什么)
    - [asn1c使用小提示](#asn1c使用小提示)

<!-- /TOC -->

接口设计目标是为xx学院提供标准数据转化接口。主要涉及内容为：接收非标准化数据，数据解析，数据标准化，标准化数据编码，编码结果发送，接收编码结果，标准二进制数据解码。

文件夹说明：
- TCSAE文件夹：中国汽车工程协会标准内容文件夹。标准使用asn.1格式的以asn为后缀的文件声明。使用asn1c进行文件编译得到对应的标准c文件。两类文件都在这个文件夹中。其中.c和.h文件为需要创建接口需要使用的文件。
- interfaces文件夹：真正的接口文件夹，提供的.c和.h文件供xx学院调用。另外包含接口文件的测试代码文件。

## 接口设计原则
### 模块按照功能进行划分
主要包含四种功能：
- socket接收和发送：提供原始数据接收和编码后的标准数据发送功能
- 数据接收后的信息解析：提供判断数据类型并提取有用信息功能
- 数据解析后标准数据格式的创建
- 针对标准数据的编解码

### 各功能模块间相互独立
即不存在互相调用函数的情况（不存在模块1中的函数调用模块2中的函数的情况）。但有可能某些模块间定义的数据类型需要在多个模块中使用，数据类型的定义是共享的。开发人员在使用接口时，根据程序运行逻辑调用各功能模块提供的函数。

### 下层接口封装
本接口其实是基于TCSAE文件夹中的.c和.h文件封装而来。用户使用时接口时不应使用任何TCSAE中除SPAT/BSM/RSM/RSI/MAP五中数据类型之外的任何数据类型或功能函数。尽管某些功能十分接近原始的函数，在接口设计时，也要尽量封装成自己的接口。

### 返回值、注释、内存管理
大部分功能函数**返回值为0或-1**，分别表示运行成功还是失败。函数的功能注释写在.h文件中，更细节的函数说明可能出现在对应的.c文件中。另外，所有的功能函数即便功能是创建一个指针，也尽量不以指针作为返回值，而是传入该指针并在函数中进行修改。**变量内存管理尽量遵循谁创建谁管理的原则。**

### 头文件格式
所有.h文件需要使用如下格式编写，便于避免重复引入头文件。
```c
#ifndef _SAMPLE_HEADER_
#define _SAMPLE_HEADER_
// header content
#endif
```
### 接口测试
接口编写人员的本地测试文件不需要上传到仓库中。interfaces文件夹中的test-sample.c和Makefile.am.test分别是为开发人员提供的使用样例代码和编译文件。运行`make -f Makefile.am.test`生成可执行文件test-sample.exe。

## 接口功能划分
- socket接收和发送：citssocket.h citssocket.c
- 数据接收后的信息解析：parser.h parser.c 
- 数据解析后标准数据格式的创建: creator.h creator.c 
- 针对标准数据的编解码: encode_decode.h encode_decode.c

详细函数介绍见头文件或[接口文档]()

## 接口本地测试文件
### 测试文件功能逻辑
1. 从udp广播中接收信号灯信息
2. 解析接收的socket然后判断数据类型，之后根据不同数据类型的信息格式进行数据解析
3. 根据解析结果创建标准数据格式并检查是否符合constraint
4. 测试标准数据格式的编解码

### 测试文件运行
第一步：在interfaces文件夹下执行`make -f Makefile.am.test`，若编译正常，得到test-sample.exe文件，执行test-sample.exe，等待upd广播出现。   
第二步：编译并运行test-server.c文件，该文件会广播一个固定的消息。  
第三步：查看test-sample.exe输出结果是否有误。  

为了更好的查看debug信息，需要在编译测试时，添加`-DEMIT_CITS_DEBUG`, 也可以在Makefile.am.test文件中添加一行`CFLAGS += -DEMIT_CITS_DEBUG`。for_debug.h中定义了CITS_DEBUG函数专门用于输出debug信息。详见[for_debug.h](interfaces/for_debug.h)。

## 关于asn.1与asn1c
### asn.1和asn1c是什么？
维基百科中对asn.1的描述为：

> 在电信和计算机网络领域，ASN.1（Abstract Syntax Notation One) 是一套标准，是描述数据的表示、编码、传输、解码的灵活的记法。它提供了一套正式、无歧义和精确的规则以描述独立于特定计算机硬件的对象结构。  

asn.1是一套标准，这套标准规定了如何表示数据，如何编解码数据等等。使用这套标准可以较轻松地创建出符合标准的数据格式，其中使用到的语法是`asn language`，编解码规则有`ber,der,per,xer`等。简单Rectangle示例如下。更更多基本介绍见[维基百科关于asn.1的介绍](https://zh.wikipedia.org/wiki/ASN.1)

```asn.1
FooProtocol DEFINITIONS ::= BEGIN

    FooQuestion ::= SEQUENCE {
        trackingNumber INTEGER,
        question       IA5String
    }

    FooAnswer ::= SEQUENCE {
        questionNumber INTEGER,
        answer         BOOLEAN
    }

END
```

实际定义数据格式的文件是.asn文件，也就是后缀是asn。在项目开发时，需要根据项目所需语言，将.asn文件编译成可供使用的文件，其中**asn1c**就是将asn文件编译得到.c和.h的标准C语言格式文件。asn1c全称`Open Source ASN.1 Compiler`，[官网戳这里](http://lionet.info/asn1c/compiler.html)。其中的Documentation和Examples都有阅读的必要。

asn1c在ubuntu 14.04中的下载，以及编译命令：
```shell
# install asn1c
sudo apt-get update
sudo apt-get install asn1c
# compile asn files
asn1c *.asn
# asn usage parameters
man asn1c
```

### asn1c使用小提示
- c文件与h文件  
asn1c得到的.c和.h文件，类似于接口文件。数据类型和方法的定义都在.h文件中，代码的注释需要仔细阅读，由于没有标准的代码注释文档，使用时需要多参考代码的注释。比如什么方法是用户可能调用的，什么方法是内部使用的，一个新的数据结构如何构建，如何检查数据的规范性等等。

- converter_sample.c文件  
该文件是一个编码格式转换功能文件，使用Makefile.am.sample进行编译（查看文件内容，编译需要设置-DPDU参数）得到progname程序，可以从文件中读取某种编码格式数据，然后转换成指定编码格式数据。阅读该文件可以基本**掌握各种编码格式的使用方法**，非常有用。

- 关于der编解码的长度问题  
ber是常用的编码规则，在使用时，有两种方式：1. 仿照converter_sample.c文件中的定制回调函数形式，将编码后的结果写入文件（or stdout） 2. 使用der\_encode\_to\_buffer方法，将编码结果写入缓存区。第二种方法在使用过程中的主要难点是**确定编码后二进制数据的长度问题**。下面引用[asn-usgae](https://lionet.info/asn1c/asn1c-usage.html#SECTION02212000000000000000)说明解决办法。
    > As you see, the DER encoder does not write into some sort of buffer or something. It just invokes the custom function (possible, multiple times) which would save the data into appropriate storage. The optional argument app_key is opaque for the DER encoder code and just used by _write_stream() as the pointer to the appropriate output stream to be used.
    If the custom write function is not given (passed as 0), then the DER encoder will essentially do the same thing (i.e., encode the data) but no callbacks will be invoked (so the data goes nowhere). ***It may prove useful to determine the size of the structure's encoding before actually doing the encoding2.2.***

    方法描述为：为了确定编码后的长度，可以先进行一次预编码工作，这次编码工作中，回调函数不进行数据的写入，只记录编码后的长度。





