{
  "targets": [
    {
      "target_name": "enginxNode",
      "sources": ["enginxNode.cpp", "core/enginxDefinitions.cpp", "core/enginxInstance.cpp", "core/enginxInstance.cpp", "core/enginxServerProcessor.cpp", "core/enginxWorker.cpp", "core/vendor/http_parser/http_parser.cpp", "core/enginxLocationProcessor.cpp"],
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