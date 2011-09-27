require 'helper'
require 'consistent_company'
require 'pry'

class TestConsistentCompany < Test::Unit::TestCase

  def test_company_namer
    # don't change the calling string
    str = ' my test '
    company = str.company_namer
    assert_equal(' my test ', str) 
    # remove leading and trailing space
    assert_equal('TEST', " test ".company_namer)
    # remove embedded space
    assert_equal('TEST', " te st ".company_namer)
    # remove Company
    assert_equal("MYTEST", "My Test Company".company_namer)
    assert_equal("MYCOMPANYTEST", ("MY COMPANY TEST").company_namer)
    assert_equal("MYTEST", ("MY TEST COMPANY COMP").company_namer)
    # remove leading The
    assert_equal("AAA", "The AAA Company".company_namer)
    # remove punctuation
    assert_equal("TESTER", %q{The, ?%^* tester's company!}.company_namer)
    # empty name
    assert_equal("", "".company_namer)
    # a very long name
    assert_equal("A"*1000+"NAMEISHERE", (" A"*1000 + 'NAMES IS HERE ').company_namer)
    # parenthesis matching
    assert_equal("BBEE", ("BB(xx)EE").company_namer)
    assert_equal("BE", ("B(xx)E").company_namer)
    assert_equal("XX", ("(xx)").company_namer)
    assert_equal("BB", ("BB(xx").company_namer)
    assert_equal("XX", ("(xx").company_namer)
    assert_equal("BBMMEE", ("BB(xx)MM(xx)EE").company_namer)
    assert_equal("BBEE", ("BB(xx(xx)xx)EE").company_namer)
    assert_equal("BBMM", ("BB(xx)MM(xx").company_namer)
    assert_equal("BB", ("BB(xx(xx)xx").company_namer)
    assert_equal("BB", ("BB(xx(xx").company_namer)
    
    # handle and &
    assert_equal("PRE&POST", ("pre and post").company_namer)
    assert_equal("PRE&POST", ("pre & post").company_namer)
    assert_equal("PRE&POST", ("&pre and post&").company_namer)
    assert_equal("PRE&POST", ("& pre and post &").company_namer)
    assert_equal("ANDPRE&POSTAND", ("and pre and post and").company_namer)
    
    # leading A
    assert_equal("ABTEST", ("A B TEST").company_namer)
    assert_equal("BTEST", ("A BTEST").company_namer)
    assert_equal("APLUSTEST", ("A PLUS TEST").company_namer)
    assert_equal("APLUSTEST", ("A + TEST").company_namer)
    assert_equal("APLUSTEST", ("A+ TEST").company_namer)
    
    # common name shortening
    assert_equal("TESTCTRCTRCTR", ("Test Center Center Center").company_namer)
    
    #singularize
    assert_equal("TESTNAMEHERE", (" Test Names here").company_namer)
    
    assert_equal("My Test Advertising Co".company_namer, "MY TEST ADV COMPANY".company_namer)
  end

    
  # def test_benchmark
  #   looptimes = 1000000
  # 
  #   puts "[BaseString]"
  #   puts Benchmark::CAPTION
  #   puts Benchmark.measure {
  #     base = BaseString.new
  #     looptimes.times { |n|
  #       base.make " My Test Company Name (A)"
  #     }
  #   }
  # end
end
