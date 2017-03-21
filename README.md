enginX  
---
**client side reverse proxy** 

## Getting Start  

* install for iOS  

```
pod 'enginxiOS', :git => 'https://gitlab.com/zhilong.wang/enginx'
```	  

* install for Android  

```
Please waiting for Android Library :-)
```  

* for other platform supports C/C++  
	* include the header file `enginx.h`
	* add support library for llvm C++ stl;

## Enable enginX  
* for iOS  

```  
NSString *config = ...;
[enginxCocoaTouch loadConfig:config];

//then use it  
NSString *URLString = [enginxCocoaTouch rewriteURLString:absolute_url];
```  

* for Android  

```
Please wait :-)
```  

* use C++ directly  

```
//load config

const char* config_string = "...";
enginx::EnginxError error;
enginx::Enginx::load(configString.UTF8String, error);

//then handle the error  
//use it  

std::string rewrited_url;
const char* absolute_url = "some_url_string";
enginx::Enginx::transfer(absolute_url, rewrited_url);

//use the rewrited_url
```

## Config file  

the config file is a science of researching the usage of reverse proxy, please read the rich [document](docs/index.md) for it.