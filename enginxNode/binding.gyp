{
  "targets": [
    {
      "target_name": "enginxNode",
      "sources": [ "enginxNode.cpp", "../enginx/core/enginxDefinitions.cpp", "../enginx/core/enginxInstance.cpp", "../enginx/core/enginxInstance.cpp", "../enginx/core/enginxServerProcessor.cpp", "../enginx/core/enginxWorker.cpp", "../enginx/core/vendor/http_parser/http_parser.cpp" ],
      "cflags": ["-std=c++11", "-fexceptions"],
      "cflags_cc": ["-std=c++11", "-fexceptions"],
      'cflags!': ['-fno-exceptions'], 
      'cflags!_cc':['-fno-exceptions'],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CFLAGS': [
              "-stdlib=libc++",
              "-fexceptions"
            ],
          }
        }]
      ]
    }
  ]
}