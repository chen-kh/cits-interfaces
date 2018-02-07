# 接口文档说明

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
- [遗留的工作与问题](#遗留的工作与问题)
    - [Todo Work](#todo-work)
    - [遗留问题](#遗留问题)
- [关于asn.1与asn1c](#关于asn1与asn1c)

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
3. 根据解析结果创建标准数据格式
4. 测试标准数据格式的编解码

### 测试文件运行
第一步：在interfaces文件夹下执行`make -f Makefile.am.test`，若编译正常，得到test-sample.exe文件，执行test-sample.exe，等待upd广播出现。   
第二步：编译并运行test-server.c文件，该文件会广播一个固定的消息。  
第三步：查看test-sample.exe输出结果是否有误。  

为了更好的查看debug信息，需要在编译测试时，添加`-DEMIT_CITS_DEBUG`, 也可以在Makefile.am.test文件中添加一行`CFLAGS += -DEMIT_CITS_DEBUG`。for_debug.h中定义了CITS_DEBUG函数专门用于输出debug信息。详见[for_debug.h](interfaces/for_debug.h)。

## 遗留的工作与问题
### Todo Work
原设计数据类型中包含BSM, RSM, RSI, SPAT, MAP五大类。由于目前没有掌握除SPAT外的其他四类消息的内容，目前只完成SPAT一部分。其余部分的接口需要按照设计进行补充。
### 遗留问题
- 如何标示不同的数据类型 ？
- der/ber 编码解码函数参数的问题？  
asn1c 源码：
```c
/*
 * The DER encoder of any type. May be invoked by the application.
 * The ber_decode() function (ber_decoder.h) is an opposite of der_encode().
 */
asn_enc_rval_t der_encode(struct asn_TYPE_descriptor_s *type_descriptor,
		void *struct_ptr,	/* Structure to be encoded */
		asn_app_consume_bytes_f *consume_bytes_cb,
		void *app_key		/* Arbitrary callback argument */
	);

/* A variant of der_encode() which encodes data into the pre-allocated buffer */
asn_enc_rval_t der_encode_to_buffer(
		struct asn_TYPE_descriptor_s *type_descriptor,
		void *struct_ptr,	/* Structure to be encoded */
		void *buffer,		/* Pre-allocated buffer */
		size_t buffer_size	/* Initial buffer size (maximum) */
	);
```
第一个函数需要定制回调函数，converter-sample.c文件汇总定制为将编码结果输出到控制台（当然也可以是文件）。针对应用场景，编码后的结果需要发送，因此第二个函数更适合。但是第二个函数调用时两个关键参数`*buffer`和`buffer_size`的注释不是很清楚，buffer需要预先申请多大的空间以及buffer_size需要设置为多大不可预知。因为SPAT编码后的数据长度肯定是变的，因此两个参数设置为固定值不是很好（一定会出问题）。buffer_size不够的话，编码直接错误；buffer申请的内存不够的话可以正常编码，但是编码内容有丢失，在解码时会出错。  
需要更细致的调查这两个参数如何设置。
- another problem
## 关于asn.1与asn1c
待补充

