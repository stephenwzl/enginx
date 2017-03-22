enginX config guide  
---  
## format  
the format is normal json string like this:  

```
[{
	server_name: “google.com”,
	scheme: “http”,
	listen: 80,
	action: [
		“decode $query_string”,
		“encode $fragment”
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

## enginX concepts
* server  
* action
* location
* instruction
* internal variable

### server    
server stands for a host that can response the url  
**fields**:  

* server_name: required, url's domain name  
* scheme: optional, url's request protocol  
* listen: optinal, url's port, default value for http is 80, 443 for https  

valid values:  

* server_name: `*`, `example.com`, if use `*`, this server may not be selected for the first choice  
* scheme: any string  
* listen: any integer value  

### action  
action stands for a series operation for internal variable  
**supported instruction**  

* encode
* decode
* return  

### location  
location is an object contains keys for path matching and location instructions  
**matching rules**  

* `=`: accurate matching for a path
* `^~`: a path with such prefix
* `~*`: case sensitive regex
* `~`: no case sensitive regex
* `/`: all path, but the lowest priority

**supported instruction**

* encode
* decode
* rewrite
* return
* proxy_pass  

### internal variable  

* $request_uri: path + query + fragment  
* $host
* $scheme
* $query_string
* $fragment
* $path

but when you rewriting a path, there are some other internal variable in the scope, you can use it with `$` and number like `$0`  

### query params
when the url is handled, its query params will be stored at seprated scope, one is the server, another is the location. just use it with `$arg_querykey`, such as `https://google.com?name=stephenw`, use it with `$arg_name`, its value is `stephenw`    
### value scope
when you encode or decode the variables in location scope, the value will recover to the server retained ones. but when you change the value at server scope, locations scope values also changed