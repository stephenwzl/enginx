#
# Be sure to run `pod lib lint enginxCocoa.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'enginx'
  s.version          = '2.0.0'
  s.summary          = 'powerful cross platform url rewrite tool'

  s.description      = <<-DESC
  enginx is a nginx like url rewrite tool, written in pure C language, can load dynamic config file.
  it's very efficient as you could imagine.
                       DESC

  s.homepage         = 'https://github.com/stephenwzl/enginx'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'stephenw' => 'admin@stephenw.cc' }
  s.source           = { :git => 'https://github.com/stephenwzl/enginx.git', :tag => s.version.to_s }
  s.ios.deployment_target = '8.0'

  s.source_files = ['http_parser/**/*', 'implements/**/*', 'includes/*.h', 'Lexer/*.c', 'Lexer/*.h', 'memory/*.c', 'memory/*.h']
  s.compiler_flags = '-w'
  s.public_header_files = 'includes/enginx.h'
end
