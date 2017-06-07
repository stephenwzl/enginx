{
  "targets": [
    {
      "target_name": "enginxNode",
      "sources": ['../http_parser/http_parser.c', '../implements/creator.c', '../implements/enginx.c', '../Lexer/enginx_parse.c', '../Lexer/lex.enginx.c', '../memory/memory.c', '../memory/storage.c','enginxNode.cpp'],
      "cflags": ["-std=c++11", "-fexceptions"],
      "cflags_cc": ["-std=c++11", "-fexceptions"],
      'cflags!': ['-fno-exceptions'], 
      'cflags!_cc':['-fno-exceptions'],
      'include_dirs': ['../http_parser', '../includes'],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CFLAGS': [
              "-fexceptions"
            ],
          }
        }]
      ]
    }
  ]
}