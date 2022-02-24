# Generated by jeweler
# DO NOT EDIT THIS FILE DIRECTLY
# Instead, edit Jeweler::Tasks in Rakefile, and run 'rake gemspec'
# -*- encoding: utf-8 -*-
# stub: consistent_company 0.0.9 ruby lib
# stub: ext/consistent_company/extconf.rb

Gem::Specification.new do |s|
  s.name = "consistent_company".freeze
  s.version = "0.0.9"

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze]
  s.authors = ["Doug Cleven".freeze]
  s.date = "2022-02-24"
  s.description = "Ruby C Extension to normalize a company name. Useful when company names come from various sources.".freeze
  s.email = "dcleven@marketron.com".freeze
  s.extensions = ["ext/consistent_company/extconf.rb".freeze]
  s.extra_rdoc_files = [
    "LICENSE.txt",
    "README.md"
  ]
  s.files = [
    "ext/consistent_company/consistent_company.c",
    "lib/consistent_company.rb",
    "lib/consistent_company/version.rb"
  ]
  s.homepage = "http://github.com/dcleven/consistent_company".freeze
  s.licenses = ["MIT".freeze]
  s.rubygems_version = "3.1.4".freeze
  s.summary = "Normalize a company name for consistent matching".freeze

  if s.respond_to? :specification_version then
    s.specification_version = 4
  end

  if s.respond_to? :add_runtime_dependency then
    s.add_development_dependency(%q<jeweler>.freeze, [">= 1.6.4"])
    s.add_development_dependency(%q<rake-compiler>.freeze, [">= 0.7.6"])
  else
    s.add_dependency(%q<jeweler>.freeze, [">= 1.6.4"])
    s.add_dependency(%q<rake-compiler>.freeze, [">= 0.7.6"])
  end
end

