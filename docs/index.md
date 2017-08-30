enginx Document
---
**version**: 2.0  
**config file format**: enginx DSL  
**platform**: Android, iOS, Node.js   

> if you want 1.0 version document, please checkout branch "1.0".  

## installation  

### iOS via Cocoapods 
```
pod 'enginx', '~> 2.0'
```  
### Android 
```
compile 'cc.stephenw.enginxandroid:enginxandroid:2.0.1'
```

### Node.js  
```
npm install -s enginx-nodejs@2.0.0
``` 

## Getting Start 

### write a config file  
for example, you want to rewrite `https://google.com/user/9527` to `google://user?id=9527`, here is the config file(string):  

```
server {
  domain: "google.com";
  scheme: "https";
  port: "443";
  location "/user" {
    match $path "^/user/(.*)$";
    //$1 is the variable of "9527"
    return "google://user?id=$1";
  }
}
```
### load the config & do rewrite 

* for iOS

```
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

```
String config = ...; //load config from network, file, etc..
String errorMessage = EnginxAndroid.loadConfig(config);
if (!errorMessage.isEmpty()) {
	//handle error
}
//then use it
String url = EnginxAndroid.rewriteURL(...);
```

* for Node.js (server side)

```
const enginx = require('enginx-nodejs')

//you can catch config error here
enginx.loadConfig('...')

//then use it
enginx.rewriteURLString('...')

```

# enginx concepts
* [server](#server)
* [location](#location)
* [instruction](#instruction)
	* encode/decode
	* match
	* parse
	* var
	* return
	* greater/smaller/equal
* [variable](#variable)
* [string template](#string-template)

## server
server stands for a host that can response the url  

**fields**:  

* domain: required, url domain name
* scheme: required, url request protocol
* port: required, 80 for http, 443 for https

## location
location is a scope contains instructions for matched path.  
**matching rules**: regex string.  

## instruction

* encode/decode  
`encode/decode [variable]`
do url encode/decode with global or interal variables.

* match   
`match [variable] [regex string]`
use regex string to extract variables from variables, `$0` is the full match, match groups are stored with name `$n`, `n` stands for the n-th one.

* parse  
`parse [variable]`
parse variable in query param format, such as the `$fragment` is string foo=bar, parse `$fragment` will store `$#foo` as key, `bar` as the value in temp variable. 

* var  
`var [variable name] [string template]`
`var` defines variable from string template, then you can access this variable with `$variable_name`. 

## variable
enginx has some pre-defined variables:  

* `$scheme`
* `$host`
* `$request_uri`
* `$path`
* `$fragment`
* `$query_string` 
* `query parameters`: use as `$arg_key` (substitue ‘key’ yourself)

alse you can define variable with specific name, then use it as `$variable_name`

## string template
string template is very important in enginx, which generates variable content and represents final result. Basically, you can use variables in string template, for example:  

```
//sample url
http://sample.com?userid=9527#token=qwerty

//instructions for this url 
parse $fragment;
var timestamp 2017-4-1;

//string template
https://google.com/user/$arg_userid?token=$#token&timpstamp=$timestamp
``` 
this string template uses variable from **query arguments**, **parsed variables**, and **defined variables**.
