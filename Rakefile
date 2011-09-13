# encoding: utf-8

require 'rubygems'
require 'bundler'

begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts "Run `bundle install` to install missing gems"
  exit e.status_code
end
require 'rake'

require "rake/extensiontask"
Rake::ExtensionTask.new("consistent_company") do |extension|
  extension.lib_dir = "lib/consistent_company"
end

task :build => [:clean, :compile]

require 'jeweler'
require './lib/consistent_company/version.rb'
Jeweler::Tasks.new do |gem|
  # gem is a Gem::Specification... see http://docs.rubygems.org/read/chapter/20 for more options
  gem.name = "consistent_company"
  gem.homepage = "http://github.com/dcleven/consistent_company"
  gem.license = "MIT"
  gem.summary = %Q{Normalize a company name for consistent matching}
  gem.description = %Q{Ruby C Extension to normalize a company name. Useful when company names come from various sources.}
  gem.email = "dcleven@marketron.com"
  gem.authors = ["Doug Cleven"]
  gem.version = ConsistentCompany::Version::STRING
  gem.files.include('lib/consistent_company/*') # add native stuff
  # dependencies defined in Gemfile
end
Jeweler::RubygemsDotOrgTasks.new

require 'rake/testtask'
Rake::TestTask.new(:test) do |test|
  test.libs << 'lib' << 'test'
  test.pattern = 'test/**/test_*.rb'
  test.verbose = true
end

task :default => :test
