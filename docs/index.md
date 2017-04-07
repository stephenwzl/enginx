enginX config guide  
---  
# Hello World  
the format is normal json string like this:  

```
[{
	server_name: “google.com”,
	scheme: “https”,
	action: [
		“decode $query_string”
	],
	location: {
		“^~ /api/”: [
			"rewrite ^/api/(.*)$ /$1",
			"proxy_pass https://localhost:3000"
		],
		"/": [
			"return http://localhost:9000$request_uri"
		]
	}
}]
```  
outside is an array of "servers", inner the server, the json object describes a host.      
**input**:  
`https://google.com/api/auth`  
**output**:  
`https://localhost:3000/auth`

# enginX concepts
* [server](#server)  
* [action](#action)
* [location](#location)
* [instruction](#instruction)
* [internal variable](#internal-variable)
* [query arguments](#query-arguments)
* [temp variable](#temp-variable)
* [string template](#string-template)

## server    
server stands for a host that can response the url  
**fields**:  

* server_name: required, url's domain name  
* scheme: optional, url's request protocol  
* listen: optinal, url's port, default value for http is 80, 443 for https  

valid values:  

* server_name: `*`, `example.com`, if use `*`, this server may not be selected for the first choice  
* scheme: any string  
* listen: any integer value  

## action  
action stands for a series operation for internal variable  
**supported [instruction](#instruction)**  

* encode
* decode
* return  

## location  
location is an object contains keys for path matching and location instructions  
**matching rules**  

* `=`: accurate matching for a path
* `^~`: a path with such prefix
* `~*`: case sensitive regex
* `~`: no case sensitive regex
* `/`: all path, but the lowest priority

**supported [instruction](#instruction)**  
all instruction  

## internal variable  

* $request_uri: path + query + fragment  
* $host
* $scheme
* $query_string
* $fragment
* $path  

## query arguments
when the url is handled, its query params will be stored at seprated scope, one is the server, another is the location. just use it with `$arg_querykey`, such as `https://google.com?name=stephenw`, use it with `$arg_name`, its value is `stephenw`      

## temp variable
when you use `rewrite`/`parse`/`match`/`var` such instructions, they can generate temp variables.  
`rewrite`/`parse`/`match` generated variables will be overwrriten at each their operation.  
`var` defined variables can persist at location scope.

## instruction  
### encode  
`encode [variable]`  
urlencode some thing, variable can be [internal variable](#internal-variable), [query arguments](#query-arguments) and [temp variable](#temp-variable).  

### decode  
`decode [variable]`  
urldecode some thing, **variable** can be [internal variable](#internal-variable), [query arguments](#query-arguments) and [temp variable](#temp-variable).  

### rewrite  
`rewrite [regex_str] [str_template]`  
rewrite `$path` to another string, **regex** is the mode you want to regex match, **str_template** can be compiled and stored into `$path`  
rewrite only generate one *full match* and serveral *match groups*, *full match* stored in temp variable, use `$0` to access. *match groups* stored at index 0 to n. use `$1` to access the first one, `$n` to access the n-th one.  

### proxy_pass  
`proxy_pass [str_template]`  
this instruction can terminate location operation. returns the **str_template** content appended with `$request_uri`    

### return  
`return [str_template]`  
this instruction can terminate location or server operation. returns the **str_template** content.  

### match  
`match [internal variable] [regex_str]`  
use **regex_str** to extract variables from **internal variable**, `$0` is the *full_match**, *match groups* stored at index 0 to n.use `$1` to access the first one, `$n` to access the n-th one. this operation will overwritte the temp variable for `rewrite` and `parse`    

### parse  
`parse [internal variable]`  
parse **internal variable** in *query* param format, such as the `$fragment` is string `foo=bar`, `parse $fragment` will store `$#foo` as key, `bar` as the value in temp variable. this operation will overwritte the temp variable for `rewrite` and `match`   

### var  
`var [variable name] [str_template]`     
`var` defines variable for **str_template**, the key is `$var_[variable name]` the value is compiled **str_template**, `var` defined variables can work in single location scope.

## string template  
string template is very important in enginx, which generates variable content and represents final result.  
basically, you can use variables in string template, for example:  

```  
//sample url
http://sample.com?userid=9527#token=qwerty

//instructions for this url 
parse $fragment
var timestamp 2017-4-1

//string template
https://google.com/user/$arg_userid?token=$#token&timpstamp=$var_timestamp
```  
this string template uses variable from **query arguments**, **parse**d variables, and defined **var**iables

### optional template  
sometimes we want render some string template like `foo=$bar`, but `$bar` does not exist at runtime, so we got a raw string , it's ugly.  
we can use optional template like `{{str_template}?}`, continue the example:  

```
//the timestamp may not exist
https://google.com/{{?timpstamp=$var_timestamp}?}
```  
if the *$var_timestamp* exits, the optional string template will be compiled. the result will be: `https://google.com?timpstamp=2017-4-1`  
if the *$var_timestamp* missed, the optional string template will not be compiled, the result just will be :`https://google.com/`