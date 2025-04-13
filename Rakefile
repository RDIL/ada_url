# frozen_string_literal: true

require "bundler/gem_tasks"
require "rspec/core/rake_task"
require "uri"

RSpec::Core::RakeTask.new(:spec)

require "rake/extensiontask"

task build: :compile

GEMSPEC = Gem::Specification.load("ada_url.gemspec")

Rake::ExtensionTask.new("ada_url", GEMSPEC) do |ext|
  ext.lib_dir = "lib/ada_url"
end

task default: %i[clobber compile spec]
