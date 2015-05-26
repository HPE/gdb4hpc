#!/usr/bin/ruby
#add the rubygems location to the load path
$:.insert(0, '/cray/css/users/claustin/myrubygems/lib')

require 'rubygems'
require 'chronic'
require 'date'

#at the moment, the monthly release date is the 3rd thursday of every month
day_of_week = 'thursday'
#we want to advance two weeks from the first instance of 'day_of_week'
days_to_advance = 14

first_day_str = Chronic.parse("this month #{day_of_week}").to_s
first_date = Date.parse(first_day_str)
today = Date.today
release_date = (first_date + days_to_advance)

# in case we're packaging in the month before the release
if today > release_date
  first_day_str = Chronic.parse("next month #{day_of_week}").to_s
  first_date = Date.parse(first_day_str)
  release_date = (first_date + days_to_advance)
end

p release_date.strftime("%B %d, %Y")
