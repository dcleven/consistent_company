require 'helper'
require 'consistent_company'
require 'pry'

class TestConsistentCompany < Test::Unit::TestCase
  # should "probably rename this file and start testing for real" do
  #   flunk "hey buddy, you should probably rename this file and start testing for real"
  # end
  #include ConsistentCompany
  
  def setup
    
    @base = ConsistentCompany::CompanyNamer.new
  end

  def test_make
    # remove leading and trailing space
    assert_equal('TEST', @base.make(" test "))
    # remove embedded space
    assert_equal('TEST', @base.make(" te st "))
    # remove Company
    assert_equal("MYTEST", @base.make("My Test Company"))
    # remove leading The
    assert_equal("AAA", @base.make("The AAA Company"))
    # remove punctuation
    assert_equal("TESTERS", @base.make(%q{The tester's company}))
  end

  def ruby_base_string (value)
    value = value.gsub(/Company/, '')
    value.gsub(/ /, '')
  end

#  def test_benchmark
#     looptimes = 1000000
#     
#     puts "[BaseString]"
#     puts Benchmark::CAPTION
#     puts Benchmark.measure {
#       base = BaseString.new
#       looptimes.times { |n|
#         base.make " My Test Company Name (A)"
#       }
#     }
#   end
#   
#   def test_ruby_benchmark
#      looptimes = 1000000
# 
#      puts "[RubyBaseString]"
#      puts Benchmark::CAPTION
#      puts Benchmark.measure {
# #       base = RubyBaseString.new
#        looptimes.times { |n|
#          ruby_base_string(" My Test Company Name (A)")
#        }
#      }
#    end
end
