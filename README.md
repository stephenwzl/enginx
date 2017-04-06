enginX  
---
**client side reverse proxy** 

## Getting Start  

* install for iOS  

```ruby
pod 'enginxiOS', :git => 'https://gitlab.com/zhilong.wang/enginx'
```	  

* install for Android  

```gradle
compile 'cc.stephenw.enginxandroid:enginxandroid:0.1.0'
```  

* install for Node.js  

```shell
npm install enginx-nodejs
```

* for other platform supports C/C++  
	* include the header file `enginx.h`
	* add support library for llvm C++ stl;

## Enable enginX  
* for iOS  

```objective-c  
NSString *config = ...;
[enginxCocoaTouch loadConfig:config];

//then use it  
NSString *URLString = [enginxCocoaTouch rewriteURLString:absolute_url];
```  

* for Android  

```java
String config = ...; //load config from network, file, etc..
String errorMessage = EnginxAndroid.loadConfig(config);
if (!errorMessage.isEmpty()) {
	//handle error
}
//then use it
String url = EnginxAndroid.rewriteURL(...);
```    

* for Node.js  

```javascript  
const enginx = require('enginx-nodejs')

//you can catch config error here
enginx.loadConfig('...')

//then use it
enginx.rewriteURLString('...')
```

* use C++ directly  

```c++
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