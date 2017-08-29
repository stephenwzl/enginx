enginX  
---
**client side <del>reverse proxy</del> url rewrite enginx** 

## Getting Start  

* install for iOS  

```ruby
pod 'enginx', '~> 2.0'
```	  

* install for Android  

```gradle
compile 'cc.stephenw.enginxandroid:enginxandroid:2.0.1'
```  

* for other platform supports C/C++  
	* include the header file `enginx.h` (`extern C` for C++)

## Enable enginX  
* for iOS and pure C/C++ 

```objective-c  
char* config = ...;
enginx_load_config_string(config);

enginx_compile_error* error = enginx_get_current_error();
if (error != NULL) {
//handle error
}

//rewrite
char* url = ...;
char* rewrited_url_string = enginx_rewrite_url(url);

//manage memory
enginx_compile_error_release(&error);
free(rewrited_url_string);
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


## Config file  

please see `sample.enginx` in file tree temporarily.